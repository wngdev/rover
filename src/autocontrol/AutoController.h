/*
 * AutoController.h
 *
 *  Created on: Jun 15, 2016
 *      Author: sebastian
 */

#ifndef SRC_AUTOCONTROL_AUTOCONTROLLER_H_
#define SRC_AUTOCONTROL_AUTOCONTROLLER_H_

#include "../hardwarecontroller/HardwareController.h"
#include "../db/DatabaseController.h"
#include <boost/thread.hpp>

class AutoController : public HardwareController {
public:
	AutoController();
	virtual ~AutoController();

	void start();
	void stop();

private:
	void notifyOnLine(bool onLine);
	void notifyDistance(float distance);
	void spinAround();
	void go();
	void driveInDirection(int direction, float speed);
	void logDirection(int direction, float speed);

	bool m_distanceReceivedOnce;
	float m_latestDistance;
	boost::posix_time::ptime m_timeOfLatestDistance;
	bool m_latestOnLineStatus;
	boost::thread m_thread;
	int m_latestDirection;
	float m_latestSpeed;
	boost::shared_ptr<DatabaseController> m_db;
};



#endif /* SRC_AUTOCONTROL_AUTOCONTROLLER_H_ */
