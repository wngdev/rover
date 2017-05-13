/*
 * MotorController.cpp
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#include "../motor/MotorController.h"

#include <boost/chrono.hpp>

#include "../motor/MotorObserverInterface.h"

#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef PWM_OUTPUT
#define PWM_OUTPUT 2
#endif

#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

MotorController::MotorController(bool simulated) :
	m_motorLeftBackwardPin(7),
	m_motorLeftForwardPin(8),
	m_motorRightBackwardPin(9),
	m_motorRightForwardPin(10),
	m_ioService(),
	m_work() {

	cout << "MotorController() constructor" << endl;

	m_gpio.reset(new GPIOController(simulated));

	if (simulated) {
		for (int i = 0; i < 10; i++) {
			//m_gpio->pushTestSequence(m_gpioPin, i % 2);
		}
	}

	if (m_gpio) {
		m_gpio->GPIOpinMode(m_motorLeftBackwardPin, OUTPUT);
		m_gpio->GPIOpinMode(m_motorLeftForwardPin, OUTPUT);
		m_gpio->GPIOpinMode(m_motorRightBackwardPin, OUTPUT);
		m_gpio->GPIOpinMode(m_motorRightForwardPin, OUTPUT);
		m_gpio->GPIOcreateSoftPWM(m_motorLeftBackwardPin, 0, 100);
		m_gpio->GPIOcreateSoftPWM(m_motorLeftForwardPin, 0, 100);
		m_gpio->GPIOcreateSoftPWM(m_motorRightBackwardPin, 0, 100);
		m_gpio->GPIOcreateSoftPWM(m_motorRightForwardPin, 0, 100);
	}
}

MotorController::~MotorController() {
	cout << "MotorController() destructor" << endl;

	m_observers.clear();

	if (m_ioService) {
		m_ioService->stop();
	}
	m_ioThread.interrupt();
	m_ioThread.join();
	m_work.reset();
	m_ioService.reset();
}

void MotorController::start() {
	cout << "MotorController - start()" << endl;

	m_ioService.reset(new boost::asio::io_service);
	m_work.reset(new boost::asio::io_service::work(*m_ioService));
	m_ioThread = boost::thread(&MotorController::workerThread, this);
}

void MotorController::stop() {
	cout << "MotorController - stop()" << endl;
	if (m_ioService) {
		m_ioService->stop();
	}
	m_ioThread.interrupt();
	m_ioThread.join();
	m_work.reset();
	m_ioService.reset();
}

void MotorController::notifyStop() {
	for(vector<MotorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyStop();
	}
}

void MotorController::notifyDriveForward(float speed) {
	for(vector<MotorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyDriveForward(speed);
	}
}

void MotorController::notifyDriveBackward(float speed) {
	for(vector<MotorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyDriveBackward(speed);
	}
}

void MotorController::notifyTurnLeft(float speed) {
	for(vector<MotorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyTurnLeft(speed);
	}
}

void MotorController::notifyTurnRight(float speed) {
	for(vector<MotorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyTurnRight(speed);
	}
}

void MotorController::addMotorObserver(MotorObserverInterface* observer) {
	m_observers.push_back(observer);
}

void MotorController::workerThread() {
	try {
		if (m_ioService) {
			m_ioService->run();
		}
	} catch (boost::thread_interrupted& ti) {
		cout << "MotorController() - workerThread() interrupted";
	} catch (std::exception& e) {
		cout << "MotorController() - workerThread() unhandled exception";
	}
}

void MotorController::stopMotors() {
	if (m_ioService) {
		m_ioService->post(boost::bind(&MotorController::motorControl, this, 0, 1.0));
	}
}

void MotorController::driveForward(float speed) {
	if (m_ioService) {
		m_ioService->post(boost::bind(&MotorController::motorControl, this, 1, speed));
	}
}

void MotorController::driveBackward(float speed) {
	if (m_ioService) {
		m_ioService->post(boost::bind(&MotorController::motorControl, this, 2, speed));
	}
}

void MotorController::turnLeft(float speed) {
	if (m_ioService) {
		m_ioService->post(boost::bind(&MotorController::motorControl, this, 3, speed));
	}
}

void MotorController::turnRight(float speed) {
	if (m_ioService) {
		m_ioService->post(boost::bind(&MotorController::motorControl, this, 4, speed));
	}
}

void MotorController::motorControl(int direction, float speed) {
	// TODO: speed
	switch (direction) {
	case STOP: {
		if (m_gpio) {
			m_gpio->GPIOpwmWrite(m_motorLeftBackwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorLeftForwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorRightBackwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorRightForwardPin, 0);
		}
		notifyStop();
		break;
	}
	case FORWARD: {
		if (m_gpio) {
			m_gpio->GPIOpwmWrite(m_motorLeftBackwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorRightBackwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorLeftForwardPin, 100*0.95*speed);
			m_gpio->GPIOpwmWrite(m_motorRightForwardPin, 100*speed);
		}
		notifyDriveForward(speed);
		break;
	}
	case BACKWARD: {
		if (m_gpio) {
			m_gpio->GPIOpwmWrite(m_motorLeftForwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorRightForwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorLeftBackwardPin, 100*0.95*speed);
			m_gpio->GPIOpwmWrite(m_motorRightBackwardPin, 100*speed);
		}
		notifyDriveBackward(speed);
		break;
	}
	case LEFT: {
		if (m_gpio) {
			m_gpio->GPIOpwmWrite(m_motorLeftForwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorRightBackwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorLeftBackwardPin, 100*0.95*speed);
			m_gpio->GPIOpwmWrite(m_motorRightForwardPin, 100*speed);
		}
		notifyTurnLeft(speed);
		break;
	}
	case RIGHT: {
		if (m_gpio) {
			m_gpio->GPIOpwmWrite(m_motorRightForwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorLeftBackwardPin, 0);
			m_gpio->GPIOpwmWrite(m_motorRightBackwardPin, 100*0.95*speed);
			m_gpio->GPIOpwmWrite(m_motorLeftForwardPin, 100*speed);
		}
		notifyTurnRight(speed);
		break;
	}
	}
}
