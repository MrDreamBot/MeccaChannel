#ifndef MeccaMaxMotorDevice_h
#define MeccaMaxMotorDevice_h

#include "MeccaDevice.h"
#include "MeccaChannel.h"

// Class which implements MeccaDevice's handler interface to control MAX's motor
class MeccaMaxMotorDevice: public MeccaDevice
{
  public:
	static const byte MY_DEVICE_ID;

	MeccaMaxMotorDevice(const char* name);

	void forward(MeccaChannel *channel);

	void backward(MeccaChannel *channel);

	void stop(MeccaChannel *channel);

	void flipMotor();

	virtual MeccaDevice* execute(byte id, int moduleNum);

  protected:
	  byte forwardCmd;
	  byte backwardCmd;
  
};

#endif
