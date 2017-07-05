node('linux') {
    stage ('Checkout') {
	checkout scm 
    }
    
    stage ('Rover build') {
        dir ('build') {
            sh 'cmake .'
            sh 'make -j4'
        }
    }
    
    stage ('Cleanup') {
        cleanWs()
    }
}
