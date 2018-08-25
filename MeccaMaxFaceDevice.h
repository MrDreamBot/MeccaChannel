#ifndef MeccaMaxFaceDevice_h
#define MeccaMaxFaceDevice_h

#include "MeccaDevice.h"
#include "MeccaChannel.h"

// Class which implements MeccaDevice's handler interface to control MAX's face 
class MeccaMaxFaceDevice: public MeccaDevice
{
  public:
	static const byte MY_DEVICE_ID;

	MeccaMaxFaceDevice(const char* name);

	int setImage(byte image[], int count, MeccaChannel *channel);
	void setScrollImage(byte image[], int size);
	void scrollImage(MeccaChannel *channel);
	int getScrollPos();

	virtual MeccaDevice* execute(byte id, int moduleNum);

  protected:
	byte tmpImage[18];
	byte *image;
    int scrollPos;
	int size;
  
};

#endif