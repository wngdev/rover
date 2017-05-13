/*
 * AutoController.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: sebastian
 */

#include "AutoController.h"
#include <boost/date_time/posix_time/posix_time.hpp>

#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

AutoController::AutoController() :
	HardwareController(),
	m_distanceReceivedOnce(false),
	m_latestDistance(-1),
	m_timeOfLatestDistance(),
	m_latestOnLineStatus(false),
	m_latestDirection(STOP),
	m_latestSpeed(-1),
	m_db(new DatabaseController()) {
	cout << "AutoController() constructor" << endl;

	setDBController(m_db);

	m_db->executeQueryWithoutTuples(
		"CREATE TABLE autodirections (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, timestamp DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%f','now')) NOT NULL, direction INTEGER NOT NULL, speed FLOAT NOT NULL);"
	);
}

AutoController::~AutoController() {
	cout << "AutoController() destructor" << endl;
	m_thread.interrupt();
	m_thread.join();
}

void AutoController::start() {
	cout << "AutoController - start()" << endl;
	HardwareController::start();

	//spinAround();
	m_thread = boost::thread(&AutoController::go, this);
}

void AutoController::stop() {
	cout << "AutoController - stop()" << endl;
	HardwareController::stop();
	m_thread.interrupt();
	m_thread.join();
}

void AutoController::notifyOnLine(bool onLine) {
	m_latestOnLineStatus = onLine;
}

void AutoController::notifyDistance(float distance) {
	if (!m_distanceReceivedOnce) {
		m_distanceReceivedOnce = true;
	}
	if ((m_latestDistance - distance > 0.03) || (distance - m_latestDistance > 0.03)) {
		m_latestDistance = distance;
		m_timeOfLatestDistance = boost::posix_time::microsec_clock::local_time();
		//cout << "Distance " << m_latestDistance << " m received at " << boost::posix_time::to_iso_string(m_timeOfLatestDistance) << endl;
		//cout << "Distance " << m_latestDistance << " m received" << endl;
	}
}

void AutoController::spinAround() {
	cout << "Starting to spin around" << endl;
	driveInDirection(LEFT, 0.3);
	boost::this_thread::sleep(boost::posix_time::seconds(3));
	driveInDirection(STOP, 1.0);
	cout << "Stopping" << endl;
}

void AutoController::go() {
	cout << "go()" << endl;

	try {
		while (boost::this_thread::interruption_requested() == false) {

			static float latestPrintedDistance = m_latestDistance;
			if (latestPrintedDistance != m_latestDistance) {
				cout << "Acting on distance " << m_latestDistance << endl;
				latestPrintedDistance = m_latestDistance;
			}

			if (m_distanceReceivedOnce && boost::posix_time::microsec_clock::local_time() - m_timeOfLatestDistance >= boost::posix_time::seconds(3)) {
				// Probably stuck, move around a bit
				cout << "I might be stuck. I'll move around a bit" << endl;
				driveInDirection(BACKWARD, 0.3);
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				driveInDirection(LEFT, 0.3);
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			} else if (m_distanceReceivedOnce && m_latestDistance < 0.30) {
				driveInDirection(LEFT, 0.3);
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			} else {

				float speed = 0.4;

				if (m_latestDistance < 1.0) {
					speed = 0.4 * m_latestDistance;

					if (speed < 0.22) {
						speed = 0.21;
					}
				}

				driveInDirection(FORWARD, speed);
			}

			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
	} catch (boost::thread_interrupted& ti) {
		cout << "go() interrupted" << endl;
	}
}

void AutoController::driveInDirection(int direction, float speed) {
	if (m_latestDirection != direction || m_latestSpeed != speed) {
		m_latestDirection = direction;
		m_latestSpeed = speed;
		switch(direction) {
		case STOP: {
			stopDrive();
			break;
		}
		case FORWARD: {
			driveForward(speed);
			break;
		}
		case BACKWARD: {
			driveBackward(speed);
			break;
		}
		case LEFT: {
			turnLeft(speed);
			break;
		}
		case RIGHT: {
			turnRight(speed);
			break;
		}
		}

		logDirection(direction, speed);
	}
}

void AutoController::logDirection(int direction, float speed) {
	stringstream ssDirection;
	ssDirection << direction;
	stringstream ssSpeed;
	ssSpeed << speed;

	string query = "INSERT INTO autodirections (direction, speed) VALUES (";
	query.append(ssDirection.str());
	query.append(", ");
	query.append(ssSpeed.str());
	query.append(");");
	m_db->executeQueryWithoutTuples(query);
}
