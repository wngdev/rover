/*
 * GPIOHardware.h
 *
 *  Created on: May 28, 2016
 *      Author: sebastian
 */

#ifndef SRC_GPIO_GPIOHARDWARE_H_
#define SRC_GPIO_GPIOHARDWARE_H_

#include "GPIOInterface.h"
#include <map>

using namespace std;

class GPIOHardware : public GPIOInterface {
public:
	GPIOHardware();
	virtual ~GPIOHardware();

	virtual void GPIOpinMode(int pin, int mode);

	virtual void GPIOdigitalWrite(int pin, int value);
	virtual void GPIOpwmWrite(int pin, int value);

	virtual int GPIOdigitalRead(int pin);

	virtual int GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange);

	void pushTestSequence(int pin, int value) {}
private:
	map<int, int> m_usedPins; // key = pin, value = mode
};

#endif /* SRC_GPIO_GPIOHARDWARE_H_ */
