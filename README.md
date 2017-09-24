# Rover
Raspberry Pi self-driving mini car

###Dependency installation (Debian Jessie):
```
sudo apt build-essential \
	cmake \
	libboost-date-time1.55-dev \
	libboost-system1.55-dev \
	libboost-thread1.55-dev \
	libcwiid-dev \
	libncurses5-dev \
	libopencv-dev \
	libsqlite3-dev
```

###WiringPi installation:
```
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build
```

###To build Rover:
```
cmake .
make
```

###To run Rover:
```
cd bin
./rover
```