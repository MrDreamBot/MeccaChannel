#include "MeccaMaxMotorDevice.h"

// Class which implements MeccaDevice's handler interface to control MAX's motor

const byte MeccaMaxMotorDevice::MY_DEVICE_ID = 5;

MeccaMaxMotorDevice::MeccaMaxMotorDevice(const char* name): MeccaDevice(name)  {
	forwardCmd = 0x24;
	backwardCmd = 0x34;
}

void MeccaMaxMotorDevice::forward(MeccaChannel *channel) {
	if (moduleNum != NOT_CONNECTED)
	{
		channel->setSingleByeCommand(moduleNum, forwardCmd);
	}
	
}

void MeccaMaxMotorDevice::backward(MeccaChannel *channel) {
	if (moduleNum != NOT_CONNECTED)
	{
		channel->setSingleByeCommand(moduleNum, backwardCmd);
	}
}

void MeccaMaxMotorDevice::stop(MeccaChannel *channel) {
	if (moduleNum != NOT_CONNECTED)
	{
		channel->setSingleByeCommand(moduleNum, 0x30);
	}
}

// virtually flipping the polarity of the motor
// meaning forward becomes backward, etc.
void MeccaMaxMotorDevice::flipMotor() {
	byte tmpCmd;
	tmpCmd = forwardCmd;
	forwardCmd = backwardCmd;
	backwardCmd = tmpCmd;
}

MeccaDevice* MeccaMaxMotorDevice::execute(byte id, int moduleNum){
	if ( (this->moduleNum == NOT_CONNECTED) &&(id == MeccaMaxMotorDevice::MY_DEVICE_ID))
	{
		//channel->setModuleType(moduleNum, 'M');
		this->moduleNum = moduleNum;
		return this;
	}
	return MeccaDevice::execute(id, moduleNum);
}
