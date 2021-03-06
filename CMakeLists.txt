cmake_minimum_required (VERSION 2.6)
project (Rover)
set(EXECUTABLE_OUTPUT_PATH "bin")

set(LINK_WIRINGPI OFF)
set(LINK_CWIID OFF)
set(BUILD_UNIT_TESTER ON)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread date_time REQUIRED)
find_package(OpenCV REQUIRED)

if (NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armv7l")
	# Uses simulated HW
	add_definitions(-DTESTMODE)

	# Enable warnings
	add_definitions(-fdiagnostics-show-option)
endif()

include_directories(${Boost_INCLUDE_DIRS})
# Assuming that wiringPi has been cloned into a directory above this one. 
include_directories(${CMAKE_SOURCE_DIR}/../wiringPi/wiringPi)
include_directories(${CMAKE_SOURCE_DIR}/lib/googletest/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/include)

aux_source_directory(src SRC_FILES)
aux_source_directory(src/manualcontrol SRC_FILES)
aux_source_directory(src/autocontrol SRC_FILES)
aux_source_directory(src/gpio SRC_FILES)
aux_source_directory(src/hardwarecontroller SRC_FILES)
aux_source_directory(src/distancesensor SRC_FILES)
aux_source_directory(src/linesensor SRC_FILES)
aux_source_directory(src/motor SRC_FILES)
aux_source_directory(src/db SRC_FILES)
aux_source_directory(src/camera SRC_FILES)
aux_source_directory(src/imu SRC_FILES)

aux_source_directory(test TESTER_FILES)

link_directories(${CMAKE_SOURCE_DIR}/lib/googletest/googlemock)
link_directories(${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/gtest)

# Main binary
add_executable(rover ${SRC_FILES} main/main.cpp)
target_link_libraries(rover 
	${Boost_LIBRARIES}
	dl
	ncurses
	pthread
	sqlite3
	gtest
	gmock)

if (LINK_WIRINGPI)
	target_link_libraries(rover
		wiringPi)
endif()

if (LINK_CWIID)
	target_link_libraries(rover
		cwiid)
endif()

# Tester
add_executable(rover-test ${SRC_FILES} ${TESTER_FILES})
target_link_libraries(rover-test 
	${Boost_LIBRARIES}
	dl
	ncurses
	pthread
	sqlite3
	gtest
	gmock)

if (LINK_WIRINGPI)
	target_link_libraries(rover-test
		wiringPi)
endif()

if (LINK_CWIID)
	target_link_libraries(rover-test
		cwiid)
endif()


install (DIRECTORY DESTINATION bin)
install (TARGETS rover rover-test RUNTIME DESTINATION bin)
