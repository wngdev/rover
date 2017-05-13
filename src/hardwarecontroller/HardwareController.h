/*
 * HardwareController.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_HARDWARECONTROLLER_HARDWARECONTROLLER_H_
#define SRC_HARDWARECONTROLLER_HARDWARECONTROLLER_H_

#include "../linesensor/LineSensorController.h"
#include "../distancesensor/DistanceSensorController.h"
#include "../motor/MotorController.h"
#include "../camera/CameraController.h"

#include "../linesensor/LineSensorObserverInterface.h"
#include "../distancesensor/DistanceSensorObserverInterface.h"
#include "../motor/MotorObserverInterface.h"
#include "../camera/CameraObserverInterface.h"

#include "../db/DatabaseController.h"

#include <boost/shared_ptr.hpp>

using namespace std;

class HardwareController : public LineSensorObserverInterface, DistanceSensorObserverInterface, MotorObserverInterface, CameraObserverInterface {
public:
	HardwareController();
	virtual ~HardwareController();

	void setDBController(boost::shared_ptr<DatabaseController> db);

	void start();
	void stop();

	void notifyOnLine(bool onLine);
	void notifyDistance(float distance);
	void notifyStop();
	void notifyDriveForward(float speed);
	void notifyDriveBackward(float speed);
	void notifyTurnLeft(float speed);
	void notifyTurnRight(float speed);

	void stopDrive();
	void driveForward(float speed);
	void driveBackward(float speed);
	void turnLeft(float speed);
	void turnRight(float speed);

	void notifyCameraImage(boost::shared_ptr<Mat> image);
private:
	boost::shared_ptr<LineSensorController> m_lineSensor;
	boost::shared_ptr<DistanceSensorController> m_distanceSensor;
	boost::shared_ptr<MotorController> m_motor;
	boost::shared_ptr<DatabaseController> m_db;
	boost::shared_ptr<CameraController> m_camera;
};



#endif /* SRC_HARDWARECONTROLLER_HARDWARECONTROLLER_H_ */
