#ifndef IMU_HW_IFC_H_
#define IMU_HW_IFC_H_

class IMUController;

class IMUHardwareInterface {
public:
	IMUHardwareInterface(const shared_ptr<IMUController>& imuCtrl);
	virtual ~IMUHardwareInterface() {};

	virtual bool connect() = 0;
	virtual bool disconnect() = 0;
private:
	shared_ptr<IMUController> m_imuCtrl;
};

#endif
