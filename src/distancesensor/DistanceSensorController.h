/*
 * DistanceSensorController.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_DISTANCESENSOR_DISTANCESENSORCONTROLLER_H_
#define SRC_DISTANCESENSOR_DISTANCESENSORCONTROLLER_H_

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "../gpio/GPIOController.h"
#include "../db/DatabaseController.h"
#include "DistanceSensorObserverInterface.h"
#include <vector>

using namespace std;

class DistanceSensorController {
public:
	DistanceSensorController(bool simulated, int samplingInterval);
	virtual ~DistanceSensorController();

	void setDBController(shared_ptr<DatabaseController> db);

	void start();
	void stop();

	void addDistanceSensorObserver(DistanceSensorObserverInterface* observer);
private:

	void pollingThread();
	void notifyDistance(float distance);
	void pushDistanceValue(float distance);
	int getAvgDistance();
	void logDistance(float distance);
	void testingThread();
	boost::posix_time::ptime toPtime(const string& input);

	shared_ptr<GPIOController> m_gpio;
	vector<DistanceSensorObserverInterface*> m_observers;
	shared_ptr<DatabaseController> m_db;
	map<string, float> m_loadedDistances;
	int m_triggerPin;
	int m_echoPin;
	int m_samplingInterval;
	deque<int> m_lastDistances;
	boost::thread m_thread;
	bool m_stopRequested;
	bool m_testMode;
};



#endif /* SRC_DISTANCESENSOR_DISTANCESENSORCONTROLLER_H_ */
