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
    SOURCE_DIR = 'sources'
    OUTPUT_DIR = 'build'
    BW_OUTPUT_DIR = 'bw-out'
  }
  stages {
    stage('build') {
      agent {
        docker {
          image 'cross-gcc-windows-x64-sonar-build-wrapper:latest'
          label 'docker && linux'
        }
      }
      stages {
        stage('compile') {
          steps {
            sh "mkdir -p ${OUTPUT_DIR} ${BW_OUTPUT_DIR}"
            sh "cmake -B ${OUTPUT_DIR}"
            sh "build-wrapper-linux-x86-64 --out-dir ${BW_OUTPUT_DIR} cmake --build ${OUTPUT_DIR}"
          }
        }
      }
    }
    stage('sonar quality gate') {
      agent {
        docker {
          image 'cross-gcc-windows-x64-sonar-scanner-cli:latest'
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
