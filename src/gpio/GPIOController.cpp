/*
 * GPIOController.cpp
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#include "GPIOController.h"
#include "GPIOSimulator.h"
#include "GPIOHardware.h"

GPIOController::GPIOController(bool simulator) :
	m_gpio() {

	if (simulator) {
		m_gpio.reset(new GPIOSimulator);
	} else {
		m_gpio.reset(new GPIOHardware);
	}
}

GPIOController::~GPIOController() {
	m_gpio.reset();
}

void GPIOController::GPIOpinMode(int pin, int mode) {
	if (m_gpio) {
		m_gpio->GPIOpinMode(pin, mode);
	}
}

void GPIOController::GPIOdigitalWrite(int pin, int value) {
	if (m_gpio) {
		m_gpio->GPIOdigitalWrite(pin, value);
	}
}

void GPIOController::GPIOpwmWrite(int pin, int value) {
	if (m_gpio) {
		m_gpio->GPIOpwmWrite(pin, value);
	}
}

int GPIOController::GPIOdigitalRead(int pin) {
	if (m_gpio) {
		return m_gpio->GPIOdigitalRead(pin);
	}
	return 0;
}

int GPIOController::GPIOcreateSoftPWM(int pin, int initialValue, int pwmRange) {
	if (m_gpio) {
		m_gpio->GPIOcreateSoftPWM(pin, initialValue, pwmRange);
	}
	return 0;
}

void GPIOController::pushTestSequence(int pin, int value) {
	if (m_gpio) {
		m_gpio->pushTestSequence(pin, value);
	}
}
