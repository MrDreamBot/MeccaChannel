#include "MeccaDevice.h"

// This is the base handler class of GOF's Chain of Responsibility design pattern to 
// loosely-couple potentially new devices

MeccaDevice::MeccaDevice(const char* name)
{
	this->name = name;
	this->successor = NULL;
	this->moduleNum = 255;	//255 means unused
}

void MeccaDevice::setSuccessor(MeccaDevice* successor)
{
	this->successor = successor;
}

const char* MeccaDevice::getName()
{
	return name;
}

int MeccaDevice::getModuleNum()
{
	return moduleNum;
}

MeccaDevice* MeccaDevice::execute(byte id, int moduleNum)
{
	if (successor != NULL)
	{
		return successor->execute(id, moduleNum);
	}
    return NULL;
}
