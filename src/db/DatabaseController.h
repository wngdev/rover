/*
 * DatabaseController.h
 *
 *  Created on: Jun 20, 2016
 *      Author: sebastian
 */

#ifndef SRC_DB_DATABASECONTROLLER_H_
#define SRC_DB_DATABASECONTROLLER_H_

#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>
#include <boost/thread.hpp>

using namespace std;

class DatabaseController {
public:
	DatabaseController();
	virtual ~DatabaseController();

	bool executeQueryWithoutTuples(std::string query);
	bool executeQueryWithTuples(std::string query, std::vector<std::map<std::string, std::string> >& result);
private:
	sqlite3* m_db;
	boost::mutex m_mutex;
};



#endif /* SRC_DB_DATABASECONTROLLER_H_ */
