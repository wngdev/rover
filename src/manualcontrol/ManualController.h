/*
 * ManualController.h
 *
 *  Created on: May 28, 2016
 *      Author: sebastian
 */

#ifndef SRC_MANUALCONTROL_MANUALCONTROLLER_H_
#define SRC_MANUALCONTROL_MANUALCONTROLLER_H_

#include "../hardwarecontroller/HardwareController.h"
#include <boost/thread.hpp>

#ifdef CWIID
#include <cwiid.h>
#endif


class ManualController : public HardwareController {
public:
	ManualController();
	virtual ~ManualController();

	void start();
	void stop();

private:
	void keyboardInputCaptureThread();
	void keyboardInputHandler(char c);
	void wiimoteInputCaptureThread();
	void openWiimoteConnection();
	void closeWiimoteConnection();
	bool m_paired;
	boost::thread m_keyboardInputThread;
	boost::thread m_wiimoteInputThread;
#ifdef CWIID
	cwiid_wiimote_t* m_wiimote;
#endif
};



#endif /* SRC_MANUALCONTROL_MANUALCONTROLLER_H_ */
