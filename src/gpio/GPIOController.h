/*
 * GPIOController.h
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#ifndef SRC_GPIO_GPIOCONTROLLER_H_
#define SRC_GPIO_GPIOCONTROLLER_H_

#include <boost/shared_ptr.hpp>

class GPIOInterface;

class GPIOController {
public:
	GPIOController(bool simulator);
	virtual ~GPIOController();

	void GPIOpinMode(int pin, int mode);

	void GPIOdigitalWrite(int pin, int value);
	void GPIOpwmWrite(int pin, int value);

	int GPIOdigitalRead(int pin);

	int GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange);

	void pushTestSequence(int pin, int value);
private:
	boost::shared_ptr<GPIOInterface> m_gpio;
};



#endif /* SRC_GPIO_GPIOCONTROLLER_H_ */
