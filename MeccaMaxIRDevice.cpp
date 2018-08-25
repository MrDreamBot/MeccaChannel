#include "MeccaMaxIRDevice.h"

// Class which implements MeccaDevice's handler interface to control MAX's IR sensor

const byte MeccaMaxIRDevice::MY_DEVICE_ID = 4;

MeccaMaxIRDevice::MeccaMaxIRDevice(const char* name): MeccaDevice(name)  {
}

// note: this methods is not working yet. Need more work.
int MeccaMaxIRDevice::getReading(MeccaChannel *channel) {
	channel->setSingleByeCommand(moduleNum, 0xa0);
	// Formula obtained via empirical experiment
	channel->communicateAll();
	return (int) (float(channel->getInputByteInfo(moduleNum)) * -6.75 + 962.);
}

MeccaDevice* MeccaMaxIRDevice::execute(byte id, int moduleNum){
	if ( (this->moduleNum == NOT_CONNECTED) &&(id == MeccaMaxIRDevice::MY_DEVICE_ID))
	{
		this->moduleNum = moduleNum;
		return this;
	}
	return MeccaDevice::execute(id, moduleNum);
}
