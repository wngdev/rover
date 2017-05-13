/*
 * GPIOInterface.h
 *
 *  Created on: May 26, 2016
 *      Author: sebastian
 */

#ifndef SRC_GPIO_GPIOINTERFACE_H_
#define SRC_GPIO_GPIOINTERFACE_H_

class GPIOInterface {
public:
	virtual ~GPIOInterface() {}

	virtual void GPIOpinMode(int pin, int mode) = 0;

	virtual void GPIOdigitalWrite(int pin, int value) = 0;
	virtual void GPIOpwmWrite(int pin, int value) = 0;

	virtual int GPIOdigitalRead(int pin) = 0;

	virtual int GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange) = 0;

	virtual void pushTestSequence(int pin, int value) = 0;
};



#endif /* SRC_GPIO_GPIOINTERFACE_H_ */
