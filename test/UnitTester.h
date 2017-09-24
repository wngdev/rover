#ifndef UNIT_TESTER_H_
#define UNIT_TESTER_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class UnitTester : public ::testing::Test {
public:
	UnitTester();
	virtual ~UnitTester();
protected:
	virtual void SetUp();
	virtual void TearDown();
private:
};

#endif
