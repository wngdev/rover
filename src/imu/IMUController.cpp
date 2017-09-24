#include "IMUController.h"
#include "IMUObserverInterface.h"
#include "IMUHardwareInterface.h"

IMUController::IMUController(const bool& simulated) :
	m_db(),
	m_testMode(false) {
	cout << "IMUController() constructor" << endl;
#ifdef TESTMODE
	m_testMode = true;
#endif
}

IMUController::~IMUController() {
	cout << "IMUController() destructor" << endl;

	m_observers.clear();
}

void IMUController::setDBController(const shared_ptr<DatabaseController>& db) {
	m_db = db;

	m_db->executeQueryWithoutTuples(
		"CREATE TABLE accelerometer (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, timestamp DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%f','now')) NOT NULL, x FLOAT NOT NULL, y FLOAT NOT NULL, z FLOAT NOT NULL);"
	);

	if (m_testMode) {
		// TODO
	}
}

void IMUController::start() {
	cout << "IMUController - start()" << endl;
	// TODO
}

void IMUController::stop() {
	cout << "IMUController - stop()" << endl;
	// TODO
}

void IMUController::notifyAccelerometerData(const float& x, const float& y, const float& z) {
	for(vector<IMUObserverInterface*>::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyAccelerometerData(x, y, z);
	}
}

void IMUController::addIMUObserver(IMUObserverInterface* observer) {
	m_observers.push_back(observer);
}

void IMUController::logAccelerometer(const float& x, const float& y, const float&z) {
	stringstream ss;
	ss << x << ", ";
	ss << y << ", ";
	ss << z;

	string query = "INSERT INTO accelerometer (x, y, z) VALUES ('";
	query.append(ss.str());
	query.append("');");
	if (m_db) {
		m_db->executeQueryWithoutTuples(query);
	}
}
