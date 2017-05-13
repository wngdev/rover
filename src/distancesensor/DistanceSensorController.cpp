/*
 * DistanceSensorController.cpp
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#include "DistanceSensorController.h"

#include "DistanceSensorObserverInterface.h"
#include <numeric>

#ifndef OUTPUT
#define OUTPUT 1
#endif
#ifndef INPUT
#define INPUT 0
#endif

DistanceSensorController::DistanceSensorController(bool simulated, int samplingInterval) :
	m_db(),
	m_loadedDistances(),
	m_triggerPin(17),
	m_echoPin(18),
	m_samplingInterval(samplingInterval),
	m_lastDistances(),
	m_stopRequested(false),
	m_testMode(false) {

	cout << "DistanceSensorController() constructor" << endl;

	m_gpio.reset(new GPIOController(simulated));
/*
	if (simulated) {
		for (int i = 0; i < 10000; i++) {
			int value = 1;
			if (i == 9999) {
				value = 0;
			}
			if (m_gpio) {
				m_gpio->pushTestSequence(m_echoPin, value);
			}
		}
	}
*/
	if (m_gpio) {
		m_gpio->GPIOpinMode(m_triggerPin, OUTPUT);
		m_gpio->GPIOpinMode(m_echoPin, INPUT);
	}

#ifdef TESTMODE
	m_testMode = true;
#endif
}

DistanceSensorController::~DistanceSensorController() {
	cout << "DistanceSensorController() destructor" << endl;

	m_observers.clear();
	m_thread.interrupt();
	m_thread.join();
}

void DistanceSensorController::setDBController(boost::shared_ptr<DatabaseController> db) {
	m_db = db;

	m_db->executeQueryWithoutTuples(
		"CREATE TABLE distances (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, timestamp DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%f','now')) NOT NULL, distance FLOAT NOT NULL);"
	);

	if (m_testMode) {
		std::vector<std::map<std::string, std::string> > result;
		m_db->executeQueryWithTuples("SELECT timestamp, distance FROM distances ORDER BY id ASC", result);

		for (vector<map<string, string> >::iterator it = result.begin(); it != result.end(); ++it) {
			//cout << (*it)["timestamp"] << ", " << (*it)["distance"] << endl;

			stringstream ss((*it)["distance"]);
			float distance;
			ss >> distance;

			m_loadedDistances[(*it)["timestamp"]] = distance;
		}
	}
}

void DistanceSensorController::start() {
	cout << "DistanceSensorController - start()" << endl;
	if (m_testMode) {
		m_thread = boost::thread(&DistanceSensorController::testingThread, this);
	} else {
		m_thread = boost::thread(&DistanceSensorController::pollingThread, this);
	}
}

void DistanceSensorController::stop() {
	cout << "DistanceSensorController - stop()" << endl;
	m_stopRequested = true;
	m_thread.interrupt();
	m_thread.join();
	m_stopRequested = false;
}

void DistanceSensorController::pollingThread() {
	try {
		while (true) {

			bool skipThisIteration = false;

			if (boost::this_thread::interruption_requested()) {
				cout << "Requested to stop polling" << endl;
			}

			// send 10µs pulse to trigger
			if (m_gpio) {
				m_gpio->GPIOdigitalWrite(m_triggerPin, 1);
			}
			boost::this_thread::sleep(boost::posix_time::microseconds(10));
			if (m_gpio) {
				m_gpio->GPIOdigitalWrite(m_triggerPin, 0);
			}

			boost::posix_time::ptime startTime = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::ptime stopTime;
			boost::posix_time::time_duration elapsedTime;

			boost::posix_time::ptime totalMeasurementStartTime = boost::posix_time::microsec_clock::local_time();

			// Reset start time until echo pin is high
			while (m_gpio && m_gpio->GPIOdigitalRead(m_echoPin) == 0) {

				boost::posix_time::time_duration totalElapsedTime = boost::posix_time::microsec_clock::local_time() - totalMeasurementStartTime;

				if (m_stopRequested || totalElapsedTime.total_milliseconds() > 30) {
					skipThisIteration = true;
					break;
				}
				startTime = boost::posix_time::microsec_clock::local_time();
				boost::this_thread::sleep(boost::posix_time::microseconds(5));
			}

			if (!skipThisIteration) {
				// Stop when echo pin is no longer high
				while (!m_stopRequested && m_gpio && m_gpio->GPIOdigitalRead(m_echoPin) == 1) {
					stopTime = boost::posix_time::microsec_clock::local_time();

					elapsedTime = stopTime - startTime;
					// Maximum range of sensor is 4 m, which comes to 23306 µs
					if (elapsedTime.total_microseconds() >= 23306) {
						cout << "Object too close or too far away" << endl;
						stopTime = startTime;
						break;
					}
					boost::this_thread::sleep(boost::posix_time::microseconds(5));
				}

				// Calculate pulse length
				elapsedTime = stopTime - startTime;

				// Distance that the pulse travelled
				float distance = elapsedTime.total_microseconds() * 0.34326f;

				// Distance to object is half of travelled distance
				distance /= 2;

				// Convert to cm
				distance = round(distance/10);

				int oldDistance = getAvgDistance();
				pushDistanceValue(distance);

				int newDistance = getAvgDistance();

				//cout << distance << endl;
				if (getAvgDistance() != oldDistance && newDistance >= 2 && newDistance <= 400) {
					notifyDistance((float)newDistance/100.f);
					logDistance((float)newDistance/100.f);
				}

				// set trigger to false
				if (m_gpio) {
					m_gpio->GPIOdigitalWrite(m_triggerPin, 0);
				}
			}
			// sleep
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_samplingInterval));
		}
	} catch (boost::thread_interrupted& ti) {
		cout << "DistanceSensorController - Thread interrupted" << endl;
	}
}

void DistanceSensorController::testingThread() {
	try {
		boost::posix_time::ptime ptime;
		for (map<string, float>::iterator it = m_loadedDistances.begin(); it != m_loadedDistances.end(); ++it) {
			boost::posix_time::time_duration td = boost::posix_time::milliseconds(0);

			boost::posix_time::ptime newTime = toPtime((*it).first);
			if (newTime.is_not_a_date_time()) {
				cerr << "Invalid ptime found - skipping" << endl;
				continue;
			}

			if (it != m_loadedDistances.begin()) {
				td = newTime - ptime;
			}

//			cout << "Sleeping " << td.total_milliseconds() << " ms before delivering next sample" << endl;

			boost::this_thread::sleep(td);

//			cout << "Delivering next sample: " << (*it).second << " m" << endl;
			notifyDistance((*it).second);

			ptime = newTime;
		}
	} catch (boost::thread_interrupted& ti) {
		cout << "DistanceSensorController - Thread interrupted" << endl;
	} catch (...) {
		cerr << "Caught unhandled exception" << endl;
	}
}

void DistanceSensorController::notifyDistance(float distance) {
	for(vector<DistanceSensorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyDistance(distance);
	}
}

void DistanceSensorController::addDistanceSensorObserver(DistanceSensorObserverInterface* observer) {
	m_observers.push_back(observer);
}

void DistanceSensorController::pushDistanceValue(float distance) {
	if (m_lastDistances.size() > 9) {
		m_lastDistances.pop_front();
	}
	m_lastDistances.push_back(distance);
}

int DistanceSensorController::getAvgDistance() {
	if (m_lastDistances.empty()) {
		return 0;
	}

	return round(std::accumulate(m_lastDistances.begin(), m_lastDistances.end(), 0.0) / m_lastDistances.size() * 10000) / 10000;
}

void DistanceSensorController::logDistance(float distance) {
	stringstream ss;
	ss << distance;

	string query = "INSERT INTO distances (distance) VALUES ('";
	query.append(ss.str());
	query.append("');");
	if (m_db) {
		m_db->executeQueryWithoutTuples(query);
	}
}

boost::posix_time::ptime DistanceSensorController::toPtime(string input) {
	boost::posix_time::ptime pt;
	try {
		stringstream ss;
		const static locale currlocale(ss.getloc(), new boost::local_time::local_time_input_facet("%Y-%m-%d %H:%M:%S%f"));

		ss.exceptions(ios_base::failbit);
		ss.imbue(currlocale);
		ss << input;
		ss >> pt;
	} catch (...) {
		cerr << "Caught unhandled exception" << endl;
	}

	return pt;
}
