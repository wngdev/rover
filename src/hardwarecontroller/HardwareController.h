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
#include "../imu/IMUController.h"

#include "../linesensor/LineSensorObserverInterface.h"
#include "../distancesensor/DistanceSensorObserverInterface.h"
#include "../motor/MotorObserverInterface.h"
#include "../camera/CameraObserverInterface.h"
#include "../imu/IMUObserverInterface.h"

#include "../db/DatabaseController.h"

using namespace std;

class HardwareController : public LineSensorObserverInterface, DistanceSensorObserverInterface, MotorObserverInterface, CameraObserverInterface, IMUObserverInterface {
public:
	HardwareController();
	virtual ~HardwareController();

	void setDBController(shared_ptr<DatabaseController> db);

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

	void notifyCameraImage(shared_ptr<Mat> image);

	void notifyAccelerometerData(const float& x, const float& y, const float& z);
private:
	shared_ptr<LineSensorController> m_lineSensor;
	shared_ptr<DistanceSensorController> m_distanceSensor;
	shared_ptr<MotorController> m_motor;
	shared_ptr<DatabaseController> m_db;
	shared_ptr<CameraController> m_camera;
	shared_ptr<IMUController> m_imu;
};



#endif /* SRC_HARDWARECONTROLLER_HARDWARECONTROLLER_H_ */
