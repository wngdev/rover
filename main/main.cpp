#include <iostream>

#include "../src/manualcontrol/ManualController.h"
#include "../src/autocontrol/AutoController.h"
#include <boost/thread.hpp>
#include <signal.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define AUTO

#ifndef AUTO
ManualController manualCont;
#else
AutoController autoCont;
#endif

void interruptHandler(int s) {
	cout << "Caught signal " << s << endl;
#ifndef AUTO
	manualCont.stop();
#else
	autoCont.stop();
#endif
	exit(1);
}

int main(int argc, char* argv[]) {
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = interruptHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

#ifndef AUTO
	manualCont.start();
#else
	autoCont.start();
#endif

	while (true) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}

	return 0;
}
