#include "UnitTester.h"

UnitTester::UnitTester() {

}

UnitTester::~UnitTester() {

}

void UnitTester::SetUp() {
}

void UnitTester::TearDown() {

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
