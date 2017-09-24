pipeline {
	agent {
		label 'linux'
	}
	stages {
		stage ('Compile') {
			steps {
				script {
					// JENKINS-33510 prevents using the 'dir' command, so let's use 'cd'
					sh 'cd build && cmake .'
					sh 'cd build && make -j4 2> ../build.log'
				}
			}
		}

		stage ('Static analysis') {
			steps {
				script {
					sh 'cppcheck --enable=all --xml-version=2 src/ 2> report.xml'
				}
			}
		}

		stage ('SonarQube analysis') {
			steps {
				script {
					def scannerHome = tool 'SonarQube Scanner 3.0.3.778';
					withSonarQubeEnv('SonarQube Server 6.2') {
						sh "${scannerHome}/bin/sonar-scanner -Dsonar.branch=$GIT_BRANCH"
					}
				}
			}
		}

		stage ('SonarQube quality gate') {
			steps {
				script {
					timeout time: 30, unit: 'MINUTES' {
						def qg = waitForQualityGate()
						if (qg.status != 'OK') {
							error "Pipeline aborted due to quality gate failure: ${qg.status}"
						}
					}
				}
			}
		}

		stage ('Cleanup') {
			steps {
				script {
					cleanWs()
				}
			}
		}
	}
}
