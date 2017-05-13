/*
 * HardwareController.cpp
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#include "HardwareController.h"
#include <iostream>

#define LINESENSOR_SAMPLING_INTERVAL	10
#define DISTANCESENSOR_SAMPLING_INTERVAL	30

HardwareController::HardwareController() {
	m_lineSensor.reset(new LineSensorController(false, LINESENSOR_SAMPLING_INTERVAL));
	m_distanceSensor.reset(new DistanceSensorController(false, DISTANCESENSOR_SAMPLING_INTERVAL));
	m_motor.reset(new MotorController(false));
	m_camera.reset(new CameraController(false));

	if (m_lineSensor) {
		m_lineSensor->addLineSensorObserver(this);
	}
	if (m_distanceSensor) {
		m_distanceSensor->addDistanceSensorObserver(this);
	}
	if (m_motor) {
		m_motor->addMotorObserver(this);
	}
	if (m_camera) {
		m_camera->addCameraObserver(this);
	}
}

HardwareController::~HardwareController() {
	m_lineSensor.reset();
	m_distanceSensor.reset();
	m_motor.reset();
	m_camera.reset();
}

void HardwareController::setDBController(boost::shared_ptr<DatabaseController> db) {
	m_db = db;
	m_lineSensor->setDBController(m_db);
	m_distanceSensor->setDBController(m_db);
}

void HardwareController::start() {
	cout << "HardwareController - start()" << endl;
	if (m_lineSensor) {
		m_lineSensor->start();
	}
	if (m_distanceSensor) {
		m_distanceSensor->start();
	}
	if (m_motor) {
		m_motor->start();
	}
	if (m_camera) {
		m_camera->start();
	}
}

void HardwareController::stop() {
	cout << "HardwareController - stop()" << endl;
	if (m_lineSensor) {
		m_lineSensor->stop();
	}
	if (m_distanceSensor) {
		m_distanceSensor->stop();
	}
	if (m_motor) {
		m_motor->stop();
	}
	if (m_camera) {
		m_camera->stop();
	}
}

void HardwareController::notifyOnLine(bool onLine) {
	cout << "Sensor on line: " << ((onLine) ? "true" : "false") << endl;
}

void HardwareController::notifyDistance(float distance) {
	cout << "Distance to object: " << distance << " m" << endl;
}

void HardwareController::notifyStop() {
	cout << "Stopping motors" << endl;
}

void HardwareController::notifyDriveForward(float speed) {
	cout << "Driving forward (speed " << speed << ")" << endl;
}

void HardwareController::notifyDriveBackward(float speed) {
	cout << "Driving backward (speed " << speed << ")" << endl;
}

void HardwareController::notifyTurnLeft(float speed) {
	cout << "Turning left (speed " << speed << ")" << endl;
}

void HardwareController::notifyTurnRight(float speed) {
	cout << "Turning right (speed " << speed << ")" << endl;
}

void HardwareController::stopDrive() {
	if (m_motor) {
		m_motor->stopMotors();
	}
}

void HardwareController::driveForward(float speed) {
	if (m_motor) {
		m_motor->driveForward(speed);
	}
}

void HardwareController::driveBackward(float speed) {
	if (m_motor) {
		m_motor->driveBackward(speed);
	}
}

void HardwareController::turnLeft(float speed) {
	if (m_motor) {
		m_motor->turnLeft(speed);
	}
}

void HardwareController::turnRight(float speed) {
	if (m_motor) {
		m_motor->turnRight(speed);
	}
}

void HardwareController::notifyCameraImage(boost::shared_ptr<Mat> image) {
	cout << "New camera image delivered" << endl;
	// TODO
}
