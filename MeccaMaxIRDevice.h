#ifndef MeccaMaxUltrasonicDevice_h
#define MeccaMaxIRDevice_h

#include "MeccaDevice.h"
#include "MeccaChannel.h"

// Class which implements MeccaDevice's handler interface to control MAX's IR sensor
class MeccaMaxIRDevice: public MeccaDevice
{
  public:
	static const byte MY_DEVICE_ID;

	MeccaMaxIRDevice(const char* name);

	int getReading(MeccaChannel *channel);

	virtual MeccaDevice* execute(byte id, int moduleNum);

  protected:
	byte toLow;
    byte toHigh;
  
};

#endif