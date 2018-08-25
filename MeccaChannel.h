
/*
       LEGAL NOTICE:
      Meccanoâ„¢, MeccaBrainâ„¢, MeccanoTechâ„¢ and Meccanoidâ„¢ are trademarks of Meccano.  All rights reserved.
      The information contained in this document is Proprietary to Spin Master Ltd.   
      This information should not be used in the creation and sale of any commercial product.â€�

	  This work is derived from Meccano's MeccaBrain open source library. The library
	  has been reworked to get rid of the features not supported by the Meccano M.A.X.
	  It has also been enhanced to support most of the features of the Meccano M.A.X.
*/
#ifndef MeccaChannel_h
#define MeccaChannel_h

#include "Arduino.h"
#include "MeccaDevice.h"

class MeccaChannel{
	public:
		static const int MAX_DEVICES_PER_CHANNEL;

		MeccaChannel(int pin);
		
		// getters to retrieve info mainly for debugging purposes
		byte getOutputByteInfo(int modNum);
		byte getInputByteInfo();
		byte getInputByteInfo(int modNum);
		byte getCheckSumByteInfo();
		const char* getDeviceName(int modNum);
		byte getModuleNumInfo();
		int getMaxDevices();

		// setters to control operations of the communication channel between
		// the Arduino and the Meccano M.A.X. devices
		void setDevice(int num, MeccaDevice *device);
		void setDeviceChain(MeccaDevice* chain);
		void setSingleByeCommand(int modNum, byte command);
		void setDoubleByteCommand(int modNum, byte command, byte data);
		void setMaxDevices(int num);
		
		// the following methods implement Meccano's Smart Protocol
		void communicate();
		void communicateAll();
		void sendByte(byte data);
		byte receiveByte();
		byte calculateCheckSum(byte Data1, byte Data2, byte Data3, byte Data4);
		
		// MAX specific - controlling display of images on MAX's face
		void sendImageToFace(byte image[], int count);
		void sendByteToFace(byte data);
		static byte calculateImageCheckSum(byte data[], int count);

	protected:
		int commPin;
		int moduleNum = 0;
		int printModNum = 0;
		int bitDelay = 417;
		int period = 420;
		
		byte inputByte;
		byte inputBytes[4];
		byte checkSum;
		byte header = 0xFF;
		byte outputByte[4];
		byte printOutputByte[4];
		int maxDevices;
		
		MeccaDevice *device[4];
		MeccaDevice* deviceChain;
		
};

#endif
