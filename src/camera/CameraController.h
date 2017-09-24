/*
 * CameraController.h
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#ifndef SRC_CAMERA_CAMERACONTROLLER_H_
#define SRC_CAMERA_CAMERACONTROLLER_H_

#include "CameraObserverInterface.h"
#include <vector>

using namespace std;

class CameraInterface;

class CameraController {
public:
	CameraController(bool simulator);
	virtual ~CameraController();

	void start();
	void stop();

	void addCameraObserver(CameraObserverInterface* observer);
private:
	shared_ptr<CameraInterface> m_camera;
	vector<CameraObserverInterface*> m_observers;
};



#endif /* SRC_CAMERA_CAMERACONTROLLER_H_ */
