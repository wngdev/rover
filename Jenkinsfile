pipeline {
	agent {
		label 'linux'
	}
	stages {
		stage ('Static analysis') {
			steps {
				script {
					sh 'cppcheck --enable=all --xml-version=2 src/ 2> report.xml'
				}
			}
		}

		stage ('Compile') {
			steps {
				script {
					// JENKINS-33510 prevents using the 'dir' command, so let's use 'cd'
					sh 'cd build && cmake .'
					sh 'cd build && make -j4 2> ../build.log'
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
