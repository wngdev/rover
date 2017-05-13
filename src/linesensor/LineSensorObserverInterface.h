/*
 * LineSensorObserverInterface.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_LINESENSOR_LINESENSOROBSERVERINTERFACE_H_
#define SRC_LINESENSOR_LINESENSOROBSERVERINTERFACE_H_

class LineSensorObserverInterface {
public:
	LineSensorObserverInterface() {};
	virtual ~LineSensorObserverInterface() {};

	virtual void notifyOnLine(bool onLine) = 0;
};



#endif /* SRC_LINESENSOR_LINESENSOROBSERVERINTERFACE_H_ */
