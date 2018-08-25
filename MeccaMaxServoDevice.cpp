#include "MeccaMaxServoDevice.h"

// Class which implements MeccaDevice's handler interface to control MAX's servo
const byte MeccaMaxServoDevice::MY_DEVICE_ID = 1;

MeccaMaxServoDevice::MeccaMaxServoDevice(const char* name): MeccaDevice(name)  {
	toLow = 0x18;
	toHigh = 0xE8;
}

void MeccaMaxServoDevice::mapTo180Degrees(byte low, byte high) {
	toLow = low;
	toHigh = high;
}

void MeccaMaxServoDevice::setServoPosition(MeccaChannel *channel, byte pos) {
	if (moduleNum != NOT_CONNECTED)
	{
		int servoPos = constrain(pos, 0, 180);
		servoPos = map(pos, 0, 180, toLow, toHigh);

		channel->setSingleByeCommand(moduleNum, servoPos);
	}
}

MeccaDevice* MeccaMaxServoDevice::execute(byte id, int moduleNum){
	if ( (this->moduleNum == NOT_CONNECTED) &&(id == MeccaMaxServoDevice::MY_DEVICE_ID))
	{
		//channel->setModuleType(moduleNum, 'M');
		this->moduleNum = moduleNum;
		return this;
	}
	return MeccaDevice::execute(id, moduleNum);
}
