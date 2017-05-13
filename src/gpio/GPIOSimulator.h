/*
 * GPIOSimulator.h
 *
 *  Created on: May 26, 2016
 *      Author: sebastian
 */

#ifndef SRC_GPIO_GPIOSIMULATOR_H_
#define SRC_GPIO_GPIOSIMULATOR_H_

#include "GPIOInterface.h"
#include <map>
#include <queue>

using namespace std;

class GPIOSimulator : public GPIOInterface {
public:
	GPIOSimulator();
	virtual ~GPIOSimulator();

	virtual void GPIOpinMode(int pin, int mode);

	virtual void GPIOdigitalWrite(int pin, int value);
	virtual void GPIOpwmWrite(int pin, int value);

	virtual int GPIOdigitalRead(int pin);

	virtual int GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange);

	void pushTestSequence(int pin, int value);
private:
	map< int, queue<int> > m_testSequence;
};



#endif /* SRC_GPIO_GPIOSIMULATOR_H_ */
