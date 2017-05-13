/*
 * CameraInterface.h
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#ifndef SRC_CAMERA_CAMERAINTERFACE_H_
#define SRC_CAMERA_CAMERAINTERFACE_H_

class CameraInterface {
public:
	virtual ~CameraInterface() {}

	virtual void startGrab() = 0;
	virtual void stopGrab() = 0;
};



#endif /* SRC_CAMERA_CAMERAINTERFACE_H_ */
