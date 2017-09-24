/*
 * CameraObserverInterface.h
 *
 *  Created on: Sep 4, 2016
 *      Author: sebastian
 */

#ifndef SRC_CAMERA_CAMERAOBSERVERINTERFACE_H_
#define SRC_CAMERA_CAMERAOBSERVERINTERFACE_H_

#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class CameraObserverInterface {
public:
	CameraObserverInterface() {};
	virtual ~CameraObserverInterface() {};

	virtual void notifyCameraImage(shared_ptr<Mat> image) = 0;
};



#endif /* SRC_CAMERA_CAMERAOBSERVERINTERFACE_H_ */
