#ifndef SRC_IMU_OBSERVER_INTERFACE_H_
#define SRC_IMU_OBSERVER_INTERFACE_H_

class IMUObserverInterface {
public:
	IMUObserverInterface() {};
	virtual ~IMUObserverInterface() {};

	virtual void notifyAccelerometerData(const float& x, const float& y, const float& z) = 0;
};

#endif
