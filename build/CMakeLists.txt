cmake_minimum_required (VERSION 2.6)
project (Rover)
set(EXECUTABLE_OUTPUT_PATH "../bin")

set(LINK_WIRINGPI OFF)
set(LINK_CWIID OFF)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread date_time REQUIRED)
find_package(OpenCV REQUIRED)

if (NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armv7l")
	# Uses simulated HW
	add_definitions(-DTESTMODE)
endif()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(../wiringPi/wiringPi)

aux_source_directory(../src SRC_FILES)
aux_source_directory(../src/manualcontrol SRC_FILES)
aux_source_directory(../src/autocontrol SRC_FILES)
aux_source_directory(../src/gpio SRC_FILES)
aux_source_directory(../src/hardwarecontroller SRC_FILES)
aux_source_directory(../src/distancesensor SRC_FILES)
aux_source_directory(../src/linesensor SRC_FILES)
aux_source_directory(../src/motor SRC_FILES)
aux_source_directory(../src/db SRC_FILES)
aux_source_directory(../src/camera SRC_FILES)
aux_source_directory(../src/imu SRC_FILES)

add_executable(Rover ${SRC_FILES} ${INCLUDE_FILES})

target_link_libraries(Rover 
	${Boost_LIBRARIES}
	dl
	ncurses
	pthread
	sqlite3)

if (LINK_WIRINGPI)
	target_link_libraries(Rover
		wiringPi)
endif()

if (LINK_CWIID)
	target_link_libraries(Rover
		cwiid)
endif()

install (DIRECTORY DESTINATION bin)
install (TARGETS Rover RUNTIME DESTINATION bin)
