#ifndef MeccaDevice_h
#define MeccaDevice_h

#include "Arduino.h"

// This is the base handler class of GOF's Chain of Responsibility design pattern to 
// loosely-couple potentially new devices
class MeccaDevice
{
  public:
	MeccaDevice(const char* name);
	void setSuccessor(MeccaDevice* successor);
	const char* getName();
	int getModuleNum();
	virtual MeccaDevice* execute(byte id, int moduleNum);


  protected:
	static int const NOT_CONNECTED = 255;
	const char* name;
	MeccaDevice* successor;
	int moduleNum;
    
};
#endif
