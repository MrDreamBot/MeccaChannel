#include "MeccaMaxFaceDevice.h"

// Class which implements MeccaDevice's handler interface to control MAX's face

const byte MeccaMaxFaceDevice::MY_DEVICE_ID = 6;

MeccaMaxFaceDevice::MeccaMaxFaceDevice(const char* name): MeccaDevice(name)  {
	scrollPos = 0;
	size = 0;
}

// display an images which must include the end-byte and check sum
// image array must be 18 bytes long
int MeccaMaxFaceDevice::setImage(byte image[], int count, MeccaChannel *channel) {
	channel->setSingleByeCommand(0, 0);
    channel->setSingleByeCommand(1, 0xff);
    channel->setSingleByeCommand(2, 0);
    channel->setSingleByeCommand(3, 0xff);
    channel->communicateAll();
	channel->sendImageToFace(image, count);
}

// set the scrolling image to display but does not start the display
// image must not contain end byte abd check sum. They will be added automatically.
void MeccaMaxFaceDevice::setScrollImage(byte image[], int size) {
	this->image = image;
	this->size = size;
	scrollPos = 0;
}

// start displying the scrolling image
void MeccaMaxFaceDevice::scrollImage(MeccaChannel *channel) {
	int index = scrollPos;
	for (int i = 0; i< 16; i++)
	{
		tmpImage[i] = image[index++];
		index %= size;
	}
	tmpImage[16] = 0xf5;
	tmpImage[17] = channel->calculateImageCheckSum(tmpImage, 17);
	setImage(tmpImage, 18, channel);
	scrollPos = (scrollPos + 1) % size;
}

int MeccaMaxFaceDevice::getScrollPos() {
	return scrollPos;
}

MeccaDevice* MeccaMaxFaceDevice::execute(byte id, int moduleNum){
	if ( (this->moduleNum == NOT_CONNECTED) &&(id == MeccaMaxFaceDevice::MY_DEVICE_ID))
	{
		this->moduleNum = moduleNum;
		return this;
	}
	return MeccaDevice::execute(id, moduleNum);
}
