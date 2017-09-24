pipeline {
	agent {
		label 'linux'
	}
	stages {
		stage ('Compile Google Test') {
			steps {
				script {
					sh 'cd external/googletest && cmake . && make -j4'
				}
			}
		}

		stage ('Compile Rover') {
			steps {
				script {
					sh 'cmake .'
					sh 'make -j4 2> ../build.log'
				}
			}
		}

		stage ('Static analysis') {
			steps {
				script {
					sh 'cppcheck --enable=all --xml-version=2 --suppress=missingIncludeSystem src/ 2> report.xml'
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
	}
	post {
		always {
			deleteDir()
			emailext(
				body: '$DEFAULT_CONTENT',
				mimeType: 'text/html',
				subject: '$DEFAULT_SUBJECT',
				recipientProviders: [
					[$class: 'DevelopersRecipientProvider'], 
					[$class: 'CulpritsRecipientProvider'], 
					[$class: 'RequesterRecipientProvider']])
		}
	}
}
