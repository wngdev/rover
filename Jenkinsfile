node('linux') {
    stage ('Checkout') {
	checkout scm 
    }

    stage ('Static analysis') {
        sh 'cppcheck --enable=all --xml-version=2 src/ 2> report.xml'
    }
    
    stage ('Rover build') {
        dir ('build') {
            sh 'cmake .'
            sh 'make -j4 2> ../build.log'
        }
    }
    
    stage ('Cleanup') {
        cleanWs()
    }
}
