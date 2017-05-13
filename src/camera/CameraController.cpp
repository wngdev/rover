/*
 * CameraController.cpp
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#include "CameraController.h"
#include "CameraSimulator.h"

CameraController::CameraController(bool simulator) {
	cout << "CameraController() constructor" << endl;

	if (simulator) {
		m_camera.reset(new CameraSimulator());
	}
}

CameraController::~CameraController() {
	cout << "CameraController() destructor" << endl;
	if (m_camera) {
		m_camera.reset();
	}
}

void CameraController::start() {
	cout << "CameraController - start()" << endl;
	if (m_camera) {
		m_camera->startGrab();
	}
}

void CameraController::stop() {
	cout << "CameraController - stop()" << endl;
	if (m_camera) {
		m_camera->stopGrab();
	}
}

void CameraController::addCameraObserver(CameraObserverInterface* observer) {
	m_observers.push_back(observer);
}
