/*
 * LineSensorController.cpp
 *
 *  Created on: May 27, 2016
 *      Author: sebastian
 */

#include "LineSensorController.h"
#include "LineSensorObserverInterface.h"
#include <boost/chrono.hpp>

#ifndef INPUT
#define INPUT 0
#endif

LineSensorController::LineSensorController(bool simulated, int samplingInterval) :
	m_gpioPin(25),
	m_samplingInterval(samplingInterval),
	m_onLine(false),
	m_db() {

	cout << "LineSensorController() constructor" << endl;

	m_gpio.reset(new GPIOController(simulated));

	if (simulated) {
		for (int i = 0; i < 10; i++) {
			if (m_gpio) {
				m_gpio->pushTestSequence(m_gpioPin, i % 2);
			}
		}
	}

	if (m_gpio) {
		m_gpio->GPIOpinMode(m_gpioPin, INPUT);
	}
}

LineSensorController::~LineSensorController() {
	cout << "LineSensorController() destructor" << endl;

	m_observers.clear();
	m_thread.interrupt();
	m_thread.join();
}

void LineSensorController::setDBController(boost::shared_ptr<DatabaseController> db) {
	m_db = db;

	m_db->executeQueryWithoutTuples(
		"CREATE TABLE lines (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, timestamp DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%f','now')) NOT NULL, onLine BOOLEAN NOT NULL);"
	);
}

void LineSensorController::start() {
	cout << "LineSensorController - start()" << endl;
	m_thread = boost::thread(&LineSensorController::pollingThread, this);
}

void LineSensorController::stop() {
	cout << "LineSensorController - stop()" << endl;
	m_thread.interrupt();
	m_thread.join();
}

void LineSensorController::pollingThread() {
	try {
		while (true) {
			// read pin
			if (m_gpio) {
				bool state = ((m_gpio->GPIOdigitalRead(m_gpioPin) == 0) ? true : false);
				if (state != m_onLine) {
					m_onLine = state;
					notifyOnLine(m_onLine);
					logOnLine(m_onLine);
				}
			}

			// sleep
			boost::this_thread::sleep(boost::posix_time::milliseconds(m_samplingInterval));
		}
	} catch (boost::thread_interrupted& ti) {
		cout << "LineSensorController - Thread interrupted" << endl;
	}
}

void LineSensorController::notifyOnLine(bool onLine) {
	for(vector<LineSensorObserverInterface* >::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->notifyOnLine(onLine);
	}
}

void LineSensorController::addLineSensorObserver(LineSensorObserverInterface* observer) {
	m_observers.push_back(observer);
}

void LineSensorController::logOnLine(bool onLine) {
	string query = "INSERT INTO lines (onLine) VALUES ('";
	query.append(((onLine) ? "true" : "false"));
	query.append("');");
	if (m_db) {
		m_db->executeQueryWithoutTuples(query);
	}
}
