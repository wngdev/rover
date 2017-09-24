/*
 * DatabaseController.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: sebastian
 */

#include "DatabaseController.h"
#include <iostream>

DatabaseController::DatabaseController() :
	m_db(0),
	m_mutex() {

	std::string filename = "sqlite.db";

	boost::lock_guard<boost::mutex> lock(m_mutex);
	int retVal = sqlite3_open(filename.c_str(), &m_db);
	if (retVal) {
		cout << "Failed to open database file " << filename << endl;
		sqlite3_close(m_db);
		return;
	}
}

DatabaseController::~DatabaseController() {
	boost::lock_guard<boost::mutex> lock(m_mutex);
	if (m_db != NULL) {
		sqlite3_close(m_db);
	}
	m_db = 0;
}

bool DatabaseController::executeQueryWithoutTuples(const std::string& query) {
	std::vector<std::map<std::string, std::string> > emptyResult;
	return executeQuery(query, emptyResult);
}

bool DatabaseController::executeQueryWithTuples(const std::string& query, std::vector<std::map<std::string, std::string> >& result) {
	return executeQuery(query, result);
}

bool DatabaseController::executeQuery(const string& query, std::vector<std::map<std::string, std::string> >& result) {
	boost::lock_guard<boost::mutex> lock(m_mutex);

	//cout << query << endl;

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, NULL);

	if (rc != SQLITE_OK) {
		cerr << sqlite3_errmsg(m_db) << endl;
		sqlite3_finalize(stmt);
		return false;
	} else {
		rc = sqlite3_step(stmt);

		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			cerr << sqlite3_errmsg(m_db) << endl;
			sqlite3_finalize(stmt);
			return false;
		}
		sqlite3_finalize(stmt);
	}

	if (rc != SQLITE_OK) {
		cerr << sqlite3_errmsg(m_db) << endl;
		sqlite3_finalize(stmt);
		return false;
	}

	while (rc == SQLITE_ROW) {
		std::map<std::string, std::string> tuple;
		int ncols = sqlite3_column_count(stmt);
		for (int i = 0; i < ncols; i++) {
			string colName = sqlite3_column_name(stmt, i);
			stringstream ss;
			ss << sqlite3_column_text(stmt, i);
			tuple[colName] = ss.str();
		}
		result.push_back(tuple);
		rc = sqlite3_step(stmt);
	}

	return true;
}
