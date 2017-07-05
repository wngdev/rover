/*
 * CameraController.cpp
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#include "CameraController.h"
#include "CameraSimulator.h"
#include <iostream>

CameraController::CameraController(bool simulator) {
	std::cout << "CameraController() constructor" << endl;

	if (simulator) {
		m_camera.reset(new CameraSimulator());
	}
}

CameraController::~CameraController() {
	std::cout << "CameraController() destructor" << endl;
	if (m_camera) {
		m_camera.reset();
	}
}

void CameraController::start() {
	std::cout << "CameraController - start()" << endl;
	if (m_camera) {
		m_camera->startGrab();
	}
}

void CameraController::stop() {
	std::cout << "CameraController - stop()" << endl;
	if (m_camera) {
		m_camera->stopGrab();
	}
}

void CameraController::addCameraObserver(CameraObserverInterface* observer) {
	m_observers.push_back(observer);
}
