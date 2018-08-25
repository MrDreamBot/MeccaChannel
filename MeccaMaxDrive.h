#ifndef MeccaMaxDrive_h
#define MeccaMaxDrive_h

#include "MeccaMaxMotorDevice.h"
#include "MeccaChannel.h"

// This class combines MAX's 2 motor devices to implement a two-whell drive system
class MeccaMaxDrive
{
  public:

	MeccaMaxDrive(MeccaChannel *channel);

	void forward();

	void backward();

	void stop();

	void turnLeft();

	void turnRight();

  protected:
	  MeccaMaxMotorDevice *leftMotor;
	  MeccaMaxMotorDevice *rightMotor;
	  MeccaChannel *channel;
};

#endif
