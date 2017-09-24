#ifndef SRC_IMU_CONTROLLER_H_
#define SRC_IMU_CONTROLLER_H_

#include <boost/shared_ptr.hpp>
#include "../db/DatabaseController.h"
#include <vector>

using namespace std;

class IMUHardwareInterface;
class IMUObserverInterface;

class IMUController {
public:
	IMUController(const bool& simulated);
	virtual ~IMUController();

	void setDBController(const shared_ptr<DatabaseController>& db);

	void start();
	void stop();

	void addIMUObserver(IMUObserverInterface* observer);

	void notifyAccelerometerData(const float& x, const float& y, const float& z);
	void notifyConnectedState(const bool& connected);
private:
	void logAccelerometer(const float& x, const float& y, const float& z);

	shared_ptr<IMUHardwareInterface> m_imuHw;
	shared_ptr<DatabaseController> m_db;
	vector<IMUObserverInterface*> m_observers;
	bool m_testMode;
};

#endif
