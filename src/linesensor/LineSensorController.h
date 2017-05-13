/*
 * LineSensorController.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_LINESENSOR_LINESENSORCONTROLLER_H_
#define SRC_LINESENSOR_LINESENSORCONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "../gpio/GPIOController.h"
#include "LineSensorObserverInterface.h"
#include "../db/DatabaseController.h"
#include <vector>

using namespace std;

class LineSensorController {
public:
	LineSensorController(bool simulated, int samplingInterval);
	virtual ~LineSensorController();

	void setDBController(boost::shared_ptr<DatabaseController> db);

	void start();
	void stop();

	void addLineSensorObserver(LineSensorObserverInterface* observer);
private:

	void pollingThread();
	void notifyOnLine(bool onLine);
	void logOnLine(bool onLine);

	boost::shared_ptr<GPIOController> m_gpio;
	vector<LineSensorObserverInterface*> m_observers;
	int m_gpioPin;
	int m_samplingInterval;
	bool m_onLine;
	boost::thread m_thread;
	boost::shared_ptr<DatabaseController> m_db;
};



#endif /* SRC_LINESENSOR_LINESENSORCONTROLLER_H_ */
