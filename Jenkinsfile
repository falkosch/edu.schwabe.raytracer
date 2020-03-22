pipeline {
  agent any

  options {
    buildDiscarder(logRotator(numToKeepStr: '10'))
    preserveStashes(buildCount: 3)
    skipStagesAfterUnstable()
    skipDefaultCheckout()
    timeout(time: 1, unit: 'HOURS')
  }

  triggers {
    pollSCM('H */15 * * *')
  }

  environment {
    CI = true
    HOME = "${env.WORKSPACE}"
    BW_OUTPUT_DIR = 'bw-out'
  }

  stages {
    stage('build stage') {
      agent {
        docker {
          image 'schwabe/gcc-toolchain:latest'
          label 'docker && linux'
          args '--memory=1g --memory-swap=1g'
        }
      }

      environment {
        // Will be evaluated once the stage runs on the requested
        // "docker && linux" agent, otherwise HOME may have the already
        // evaluated value from the "pipeline" level, which could be a Windows
        // path if the master runs on that OS.
        HOME = "${env.WORKSPACE}"
      }

      stages {
        stage('scm stage') {
          steps {
            checkout([
              $class: 'GitSCM',
              branches: scm.branches,
              doGenerateSubmoduleConfigurations: scm.doGenerateSubmoduleConfigurations,
              extensions: [
                [
                  $class: 'SubmoduleOption',
                  disableSubmodules: false,
                  parentCredentials: true,
                  recursiveSubmodules: true,
                  reference: '',
                  trackingSubmodules: false
                ]
              ],
              submoduleCfg: scm.submoduleCfg,
              userRemoteConfigs: scm.userRemoteConfigs
            ])
          }
        }

        stage("prepare") {
          steps {
            sh "rm -rf ${BW_OUTPUT_DIR} build/*"
            sh "mkdir -p ${BW_OUTPUT_DIR}"
            sh "chmod +x ./*.sh"
            sh "CONAN_ADD_ARGS=\"--profile cmake/conan_profile_linux_to_win64.txt\" ./install-dependencies-with-local-conan.sh"
          }
        }

        stage("build & tests") {
          steps {
            sh "TOOLCHAIN_FILE=cmake/toolchain-mingw-w64-x86_64.cmake build-wrapper-linux-x86-64 --out-dir ${BW_OUTPUT_DIR} ./ci-reports-with-local-cc.sh"
          }
        }
        
        stage('sonar quality gate') {
          steps {
            lock(resource: 'sonarcloud-raytracer') {
              withSonarQubeEnv('sonarqube') {
                sh "sonar-scanner -Dsonar.branch.name=${env.BRANCH_NAME} -Dsonar.cfamily.build-wrapper-output=${BW_OUTPUT_DIR}"
              }

              sleep time: 20, unit: 'SECONDS'

              timeout(time: 1, unit: 'MINUTES') {
                waitForQualityGate abortPipeline: true
              }
            } // sonarcloud-raytracer
          }
        } // sonar quality gate
      }
    } // build stage
  }

  post {
    failure {
      script {
        committerEmail = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%ae\'').trim()
      }

      mail(
        to: "${committerEmail}",
        subject: "Failed Pipeline: ${currentBuild.fullDisplayName}",
        body: "Something is wrong with ${env.BUILD_URL}"
      )
    }
  }
}
