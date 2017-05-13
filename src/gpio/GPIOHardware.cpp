/*
 * GPIOHardware.cpp
 *
 *  Created on: May 28, 2016
 *      Author: sebastian
 */

#include "GPIOHardware.h"
#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <errno.h>

using namespace std;

#ifndef INPUT
#define INPUT 0
#endif

#ifndef OUTPUT
#define OUTPUT 1
#endif

GPIOHardware::GPIOHardware() {
	cout << "GPIOHardware() constructor" << endl;
#ifdef WIRINGPI
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
		wiringPiSetupGpio();
	}
#endif
}

GPIOHardware::~GPIOHardware() {
	cout << "GPIOHardware() destructor" << endl;
	for (map<int, int>::iterator it = m_usedPins.begin(); it != m_usedPins.end(); ++it) {
		if ((*it).second == OUTPUT) {
#ifdef WIRINGPI
			GPIOdigitalWrite((*it).first, 0);
#endif
		}
	}
}

void GPIOHardware::GPIOpinMode(int pin, int mode) {
	cout << "pinMode(" << pin << ", " << mode << ")" << endl;
	m_usedPins[pin] = mode;
#ifdef WIRINGPI
	pinMode(pin, mode);
	GPIOdigitalWrite(pin, 0);
#endif
}

void GPIOHardware::GPIOdigitalWrite(int pin, int value) {
	//cout << "digitalWrite(" << pin << ", " << value << ")" << endl;
#ifdef WIRINGPI
	digitalWrite(pin, value);
#endif
}

void GPIOHardware::GPIOpwmWrite(int pin, int value) {
#ifdef WIRINGPI
	softPwmWrite(pin, value);
#endif
}

int GPIOHardware::GPIOdigitalRead(int pin) {
#ifdef WIRINGPI
	return digitalRead(pin);
#else
	return 0;
#endif
}

int GPIOHardware::GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange) {
#ifdef WIRINGPI
	int result = softPwmCreate(pin, initialValue, pwmRange);
	if (result != 0) {
		cout << "Failed to create software PWM, error code " << errno << endl;
	}
	return result;
#else
	return 0;
#endif
}
