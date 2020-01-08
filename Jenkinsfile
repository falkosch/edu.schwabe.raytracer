pipeline {
  agent none
  options {
    skipStagesAfterUnstable()
    timeout(time: 1, unit: 'HOURS')
  }
  triggers {
    pollSCM('H */15 * * *')
  }
  environment {
    CI = true
    HOME = "${env.WORKSPACE}"
    SOURCE_DIR = 'trunk'
    OUTPUT_DIR = 'trunk/build'
    BW_OUTPUT_DIR = 'trunk/bw-out'
  }
  stages {
    stage('build') {
      agent {
        docker {
          image 'gcc-with-sonar-build-wrapper:latest'
          label 'docker && linux'
        }
      }
      stages {
        stage('compile') {
          steps {
            sh "mkdir -p ${BW_OUTPUT_DIR}"
            sh "build-wrapper-linux-x86-64 --out-dir ${BW_OUTPUT_DIR} make -C ${SOURCE_DIR} clean build"
          }
        }
      }
    }
    stage('sonar quality gate') {
      agent {
        docker {
          image 'gcc-with-sonar-scanner-cli:latest'
          label 'docker && linux'
        }
      }
      steps {
        withSonarQubeEnv('sonarqube') {
          sh "sonar-scanner -Dsonar.branch.name=${env.BRANCH_NAME} -Dsonar.sources=${SOURCE_DIR} -Dsonar.cfamily.build-wrapper-output=${BW_OUTPUT_DIR}"
        }
        sleep 3
        timeout(time: 1, unit: 'MINUTES') {
          waitForQualityGate abortPipeline: true
        }
      }
    }
  }
}
