#ifndef MeccaMaxServoDevice_h
#define MeccaMaxServoDevice_h

#include "MeccaDevice.h"
#include "MeccaChannel.h"

// Class which implements MeccaDevice's handler interface to control MAX's servo
class MeccaMaxServoDevice: public MeccaDevice
{
  public:
	static const byte MY_DEVICE_ID;

	MeccaMaxServoDevice(const char* name);

	void mapTo180Degrees(byte low, byte high);

	void setServoPosition(MeccaChannel *channel, byte pos);

	virtual MeccaDevice* execute(byte id, int moduleNum);

  protected:
	byte toLow;
    byte toHigh;
  
};

#endif