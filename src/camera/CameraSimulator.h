/*
 * CameraSimulator.h
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#ifndef SRC_CAMERA_CAMERASIMULATOR_H_
#define SRC_CAMERA_CAMERASIMULATOR_H_

#include "CameraInterface.h"

class CameraSimulator : public CameraInterface {
public:
	CameraSimulator();
	virtual ~CameraSimulator();

	virtual void startGrab();
	virtual void stopGrab();
};




#endif /* SRC_CAMERA_CAMERASIMULATOR_H_ */
