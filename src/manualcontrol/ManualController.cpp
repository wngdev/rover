/*
 * ManualController.cpp
 *
 *  Created on: May 28, 2016
 *      Author: sebastian
 */
#include "ManualController.h"
#include <curses.h>

ManualController::ManualController() :
	HardwareController(),
	m_paired(false) {
	cout << "ManualController() constructor" << endl;
}

ManualController::~ManualController() {
	cout << "ManualController() destructor" << endl;
	m_keyboardInputThread.interrupt();
	m_keyboardInputThread.join();
	m_wiimoteInputThread.interrupt();
	m_wiimoteInputThread.join();
}

void ManualController::start() {
	cout << "ManualController - start()" << endl;
	HardwareController::start();

//	m_keyboardInputThread = boost::thread(&ManualController::keyboardInputCaptureThread, this);
	m_wiimoteInputThread = boost::thread(&ManualController::wiimoteInputCaptureThread, this);
}

void ManualController::stop() {
	cout << "ManualController - stop()" << endl;
	HardwareController::stop();

	closeWiimoteConnection();

	m_keyboardInputThread.interrupt();
	m_keyboardInputThread.join();
	m_wiimoteInputThread.interrupt();
	m_wiimoteInputThread.join();
}

void ManualController::keyboardInputCaptureThread() {
	try {

		WINDOW* w;
		int c;

		w = initscr();
		if (w == NULL) {
			cerr << "Error initializing curses library." << endl;
			return;
		}
		raw();
		noecho();
		nonl();
		keypad(w, FALSE);

		cout << "Press Q to quit" << endl;

		do {
			c = getch();

//			if (isprint(c)) {
//				cout << "Received character '" << c << "'" << endl;
//			}

			keyboardInputHandler(c);

		} while (c != 'Q');

		endwin();

	} catch (boost::thread_interrupted& ti) {
		cout << "ManualController() - keyboardInputCaptureThread() interrupted" << endl;
	}
}

void ManualController::keyboardInputHandler(char c) {
	switch (c) {
	case 65: {
		// Up
		driveForward(1.0);
		break;
	}
	case 66: {
		// Down
		driveBackward(1.0);
		break;
	}
	case 67: {
		// Right
		turnRight(1.0);
		break;
	}
	case 68: {
		// Left
		turnLeft(1.0);
		break;
	}
	}
}

void ManualController::wiimoteInputCaptureThread() {
	try {
		while (!m_paired) {
			if (boost::this_thread::interruption_requested()) {
				break;
			}
			openWiimoteConnection();
			if (!m_paired) {
				boost::this_thread::sleep(boost::posix_time::seconds(3));
			}
		}

		if (m_paired) {
#ifdef CWIID
			cwiid_set_led(m_wiimote, CWIID_LED1_ON | CWIID_LED2_ON | CWIID_LED3_ON | CWIID_LED4_ON);

			if (cwiid_set_rumble(m_wiimote, 1) != 0) {
				cerr << "Failed to set rumble ON" << endl;
			}

			boost::this_thread::sleep(boost::posix_time::seconds(1));

			if (cwiid_set_rumble(m_wiimote, 0) != 0) {
				cerr << "Failed to set rumble OFF" << endl;
			}

			cwiid_set_rpt_mode(m_wiimote, CWIID_RPT_BTN | CWIID_RPT_STATUS);
#endif
		}

		if (m_paired) {
#ifdef CWIID
			while(true) {
				int mesg_count;
				union cwiid_mesg* mesg;
				struct timespec t;
				if (cwiid_get_mesg(m_wiimote, &mesg_count, &mesg, &t) == 0) {
					for (int i = 0; i < mesg_count; i++) {
						switch (mesg[i].type) {
						case CWIID_MESG_BTN: {
							if (mesg[i].btn_mesg.buttons == 0) {
								cout << "Stop" << endl;
								stopDrive();
							}
							if (mesg[i].btn_mesg.buttons == CWIID_BTN_LEFT) {
								cout << "Backward" << endl;
								driveBackward(0.3);
							}
							if (mesg[i].btn_mesg.buttons == CWIID_BTN_RIGHT) {
								cout << "Forward" << endl;
								driveForward(0.3);
							}
							if (mesg[i].btn_mesg.buttons == CWIID_BTN_DOWN) {
								cout << "Right" << endl;
								turnRight(0.3);
							}
							if (mesg[i].btn_mesg.buttons == CWIID_BTN_UP) {
								cout << "Left" << endl;
								turnLeft(0.3);
							}
							//cout << "Button report: " << mesg[i].btn_mesg.buttons << endl;
							break;
						}
						default: {
							cout << "Unhandled case" << endl;
							break;
						}
						}
					}
					//delete mesg_count;
					//delete[] mesg;
				}

				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			}
#endif
		}
	} catch (boost::thread_interrupted& ti) {
		cout << "ManualController() - wiimoteInputCaptureThread() interrupted" << endl;
	}
}

void ManualController::openWiimoteConnection() {
	if (!m_paired) {
		cout << "Opening Wiimote connection" << endl;
#ifdef CWIID
		struct cwiid_state state;
		bdaddr_t bdaddr = { 0 };
		m_wiimote = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC);

		if (!m_wiimote) {
			cerr << "Failed to set up Wiimote" << endl;
		} else {
			cout << "Successfully set up Wiimote" << endl;
			m_paired = true;
		}
#endif
	}
}

void ManualController::closeWiimoteConnection() {
	if (m_paired) {
		cout << "Closing Wiimote connection" << endl;
#ifdef CWIID
		if (cwiid_close(m_wiimote) != 0) {
			cerr << "Failed to disconnect from Wiimote" << endl;
		} else {
			cout << "Closed" << endl;
			m_paired = false;
		}
#endif
	}
}
