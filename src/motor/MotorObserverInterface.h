/*
 * MotorObserverInterface.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_MOTOR_MOTOROBSERVERINTERFACE_H_
#define SRC_MOTOR_MOTOROBSERVERINTERFACE_H_

class MotorObserverInterface {
public:
	MotorObserverInterface() {};
	virtual ~MotorObserverInterface() {};

	virtual void notifyStop() = 0;
	virtual void notifyDriveForward(float speed) = 0;
	virtual void notifyDriveBackward(float speed) = 0;
	virtual void notifyTurnLeft(float speed) = 0;
	virtual void notifyTurnRight(float speed) = 0;
};



#endif /* SRC_MOTOR_MOTOROBSERVERINTERFACE_H_ */
