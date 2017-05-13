/*
 * DistanceSensorObserverInterface.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_DISTANCESENSOR_DISTANCESENSOROBSERVERINTERFACE_H_
#define SRC_DISTANCESENSOR_DISTANCESENSOROBSERVERINTERFACE_H_

class DistanceSensorObserverInterface {
public:
	DistanceSensorObserverInterface() {};
	virtual ~DistanceSensorObserverInterface() {};

	virtual void notifyDistance(float distance) = 0;
};

#endif /* SRC_DISTANCESENSOR_DISTANCESENSOROBSERVERINTERFACE_H_ */
