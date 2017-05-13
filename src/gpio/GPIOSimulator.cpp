/*
 * GPIOSimulator.cpp
 *
 *  Created on: May 26, 2016
 *      Author: sebastian
 */

#include "GPIOSimulator.h"
#include <iostream>

GPIOSimulator::GPIOSimulator() {
	cout << "GPIOSimulator() constructor" << endl;
}

GPIOSimulator::~GPIOSimulator() {
	cout << "GPIOSimulator() destructor" << endl;
}

void GPIOSimulator::GPIOpinMode(int pin, int mode) {
	cout << "pinMode(" << pin << ", " << mode << ")" << endl;
}

void GPIOSimulator::GPIOdigitalWrite(int pin, int value) {
	cout << "digitalWrite(" << pin << ", " << value << ")" << endl;
}

void GPIOSimulator::GPIOpwmWrite(int pin, int value) {
	cout << "pwmWrite(" << pin << ", " << value << ")" << endl;
}

int GPIOSimulator::GPIOdigitalRead(int pin) {

	int value = 0;

	if (m_testSequence.count(pin) != 0) {
		if (!m_testSequence.at(pin).empty()) {
			value = m_testSequence.at(pin).front();
			m_testSequence.at(pin).pop();
		}
	}

	cout << "digitalRead(" << pin << ") = " << value << endl;
	return value;
}

int GPIOSimulator::GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange) {
	cout << "softPwmCreate(" << pin << ", " << initialValue << ", " << pwmRange << ")";
	return 0;
}

void GPIOSimulator::pushTestSequence(int pin, int value) {
	m_testSequence[pin].push(value);
}
