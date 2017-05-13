/*
 * CameraSimulator.cpp
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#include "CameraSimulator.h"
#include <iostream>

CameraSimulator::CameraSimulator() {
	cout << "CameraSimulator() constructor" << endl;
}

CameraSimulator::~CameraSimulator() {
	cout << "CameraSimulator() destructor" << endl;
}

void CameraSimulator::startGrab() {
	cout << "startGrab()" << endl;
}

void CameraSimulator::stopGrab() {
	cout << "stopGrab()" << endl;
}
