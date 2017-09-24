pipeline {
	agent {
		docker 'stretch_amd64-builder:latest'
	}
	stages {
		stage ('Static analysis') {
			sh 'cppcheck --enable=all --xml-version=2 src/ 2> report.xml'
		}

		stage ('Compile') {
			// JENKINS-33510 prevents using the 'dir' command, so let's use 'cd'
			sh 'cd build && cmake .'
			sh 'cd build && make -j4 2> ../build.log'
		}
	}
}
