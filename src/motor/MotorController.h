/*
 * MotorController.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_MOTOR_MOTORCONTROLLER_H_
#define SRC_MOTOR_MOTORCONTROLLER_H_

#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "../gpio/GPIOController.h"
#include <vector>

#include "../motor/MotorObserverInterface.h"

using namespace std;

class MotorController {
public:
	MotorController(bool simulated);
	virtual ~MotorController();

	void start();
	void stop();

	void stopMotors();
	void driveForward(float speed);
	void driveBackward(float speed);
	void turnLeft(float speed);
	void turnRight(float speed);

	void addMotorObserver(MotorObserverInterface* observer);
private:

	void workerThread();

	void motorControl(int direction, float speed);

	void notifyStop();
	void notifyDriveForward(float speed);
	void notifyDriveBackward(float speed);
	void notifyTurnLeft(float speed);
	void notifyTurnRight(float speed);

	shared_ptr<GPIOController> m_gpio;
	vector<MotorObserverInterface*> m_observers;
	int m_motorLeftBackwardPin;
	int m_motorLeftForwardPin;
	int m_motorRightBackwardPin;
	int m_motorRightForwardPin;
	shared_ptr<boost::asio::io_service> m_ioService;
	boost::scoped_ptr<boost::asio::io_service::work> m_work;
	boost::thread m_ioThread;
};



#endif /* SRC_MOTOR_MOTORCONTROLLER_H_ */
