/*
       LEGAL NOTICE:
      Meccanoâ„¢, MeccaBrainâ„¢, MeccanoTechâ„¢ and Meccanoidâ„¢ are trademarks of Meccano.  All rights reserved.
      The information contained in this document is Proprietary to Spin Master Ltd.   
      This information should not be used in the creation and sale of any commercial product.â€�

	  This work is derived from Meccano's MeccaBrain open source library. The library
	  has been reworked to get rid of the features not supported by the Meccano M.A.X.
	  It has also been enhanced to support most of the features of the Meccano M.A.X.
*/

#include "Arduino.h"
#include "MeccaChannel.h"

const int MeccaChannel :: MAX_DEVICES_PER_CHANNEL = 4;

/******    Initialization of the "MeccaChannel" to a specific I/O pin  *****/
MeccaChannel :: MeccaChannel(int pin){
  pinMode(pin, OUTPUT);
  commPin = pin;
  maxDevices = MAX_DEVICES_PER_CHANNEL;
  
  for (int x = 0; x < maxDevices; x++){
    outputByte[x] = 0xFE;   // initialize output data
    device[x] = NULL;
  }
}
/* end */ 


// getters to retrieve info mainly for debugging purposes

byte MeccaChannel :: getOutputByteInfo(int modNum){
	return printOutputByte[modNum];
}

byte MeccaChannel :: getInputByteInfo(){
	return inputByte;
}

byte MeccaChannel :: getInputByteInfo(int modNum){

		return inputBytes[modNum];
	
}

byte MeccaChannel :: getCheckSumByteInfo(){
	return checkSum;
}

const char* MeccaChannel :: getDeviceName(int modNum) {
	if (device[modNum] == NULL)
	{
		return NULL;
	}
	return device[modNum]->getName(); 
}

byte MeccaChannel :: getModuleNumInfo(){
  return printModNum;
}

int MeccaChannel :: getMaxDevices() {
	return maxDevices;
}

/* end */ 

// setters to control operations of the communication channel between
// the Arduino and the Meccano M.A.X. devices

void MeccaChannel :: setDevice(int modNum, MeccaDevice *device) {
	this->device[modNum] = device;
}

void MeccaChannel :: setDeviceChain(MeccaDevice* chain)
{
	this->deviceChain = chain;
}

void MeccaChannel :: setSingleByeCommand(int modNum, byte command) {
	outputByte[modNum] = command;
}

void MeccaChannel :: setMaxDevices(int num) {
	maxDevices = num;
}

void MeccaChannel :: setDoubleByteCommand(int modNum, byte command, byte data) {
	// not implemented yet
}

/*    communicate()  -  this is the main method that takes care of initializing, sending data to and receiving data from Meccano Smart modules

  The datastream consists of 6 output bytes sent to the Smart modules   and  one return input byte from the Smart modules.   
  Since there can be a maximum of 4 Smart modules in a chain, each module takes turns replying along the single data wire.

  Output bytes:   
      0xFF - the first byte is always a header byte of 0xFF
      Data 1 -  the second byte is the data for the Smart module at the 1st position in the chain
      Data 2 -  the third byte is the data for the Smart module at the 2nd position in the chain
      Data 3 -  the fourth byte is the data for the Smart module at the 3rd position in the chain
      Data 4 -  the fifth byte is the data for the Smart module at the 4th position in the chain 
      Checksum  -  the sixth byte is part checksum, part module ID.  The module ID tells which of the modules in the chain should reply
   end  */
void MeccaChannel :: communicate(){
		    
    sendByte(0xFF);                         // send header
  
    for (int x = 0; x < maxDevices; x++){            // send 4 data bytes
      sendByte(outputByte[x]);
      printOutputByte[x] = outputByte[x];
    }

    checkSum = calculateCheckSum(outputByte[0], 
                      			 outputByte[1],
								 outputByte[2],
								 outputByte[3]);	
                       
    sendByte(checkSum);                    // send checksum
    inputByte = receiveByte();


    if (inputByte == 0xFE){                 // if received back 0xFE, then the module exists so get ID number
      outputByte[moduleNum] = 0xFC;
    }

	if ((outputByte[moduleNum] == 0xFC) && (device[moduleNum] == NULL))
	{
		// use GOF's Chain of Responsibility design pattern to loosely-couple
		// to potentially new devices
		MeccaDevice *device = deviceChain->execute(inputByte, moduleNum);
		if (device != NULL)
		{
			setDevice(moduleNum, device);
		}
	}
	
    printModNum = moduleNum;
	inputBytes[moduleNum] = inputByte;
    moduleNum++;                             // increment to next module ID
    if (moduleNum >= maxDevices) {
      	moduleNum = 0;
    } 
	
	delay(10);

}

void MeccaChannel :: communicateAll() {
	for (int i = 0; i < maxDevices ; i++)
	{
		communicate();
	}
}

void MeccaChannel :: sendByte(byte data){
  pinMode(commPin, OUTPUT);                                     
  digitalWrite(commPin,LOW);
  delayMicroseconds(bitDelay);                  // start bit - 417us LOW

  for (byte mask = B00000001; mask>0; mask <<= 1) {   // iterate through bit mask
    if (data & mask){                           // if bitwise AND resolves to true
      
      digitalWrite(commPin,HIGH);               // send 1
      
    }else{                                      // if bitwise and resolves to false
      
      digitalWrite(commPin,LOW);                // send 0
    }
    delayMicroseconds(bitDelay);                //delay
  }
  
  digitalWrite(commPin,HIGH);
  delayMicroseconds(bitDelay);         // stop bit - 417us HIGH
  
  digitalWrite(commPin,HIGH);
  delayMicroseconds(bitDelay);         // stop bit - 417us HIGH
  
}

byte MeccaChannel :: receiveByte(){
  byte tempByte;
  tempByte = 0;
  
  pinMode(commPin, INPUT);
  
  delay(1.5);
  
  for (byte mask = B00000001; mask>0; mask <<= 1) {   // iterate through bit mask
         
    if (pulseIn(commPin, HIGH, 2500) > 400){
      tempByte = tempByte | mask;
    }
  }
  return tempByte;  
}

byte MeccaChannel :: calculateCheckSum(byte Data1, byte Data2, byte Data3, byte Data4){
  int CS;
  CS =  Data1 + Data2 + Data3 + Data4;  // ignore overflow
  CS = CS + (CS >> 8);                  // right shift 8 places
  CS = CS + (CS << 4);                  // left shift 4 places
  CS = CS & 0xF0;                       // mask off top nibble
  CS = CS | moduleNum;
  return CS;
}

// stop byte and checksum must be in the image array
void MeccaChannel :: sendImageToFace(byte image[], int count) {
	  pinMode(commPin, OUTPUT);

	  // the following sequence constitutes a start bit
	  digitalWrite(commPin,LOW);
	  delayMicroseconds(140);                  // start bit - 136us LOW
	  digitalWrite(commPin,HIGH);
	  delayMicroseconds(550);                  // - 564us HIGH
	  digitalWrite(commPin,LOW);
	  delayMicroseconds(140);                  // - 136us LOW

	  for (int i = 0; i < count; i++) {
		  sendByteToFace(image[i]);
	  }

	  digitalWrite(commPin,HIGH);
	  delayMicroseconds(550);                  // - 564us HIGH
	  digitalWrite(commPin,LOW);
}

void MeccaChannel :: sendByteToFace(byte data) {
	  for (byte mask = B10000000; mask>0; mask >>= 1) {   // iterate through bit mask
	    if (data & mask){                         // if bitwise AND resolves to true
	      digitalWrite(commPin,HIGH);             // send 1
	      delayMicroseconds(280);
	      digitalWrite(commPin,LOW);
	      delayMicroseconds(period - 280);
	    }else{                                    // if bitwise and resolves to false
		  digitalWrite(commPin,HIGH);             // send 0
		  delayMicroseconds(160);
		  digitalWrite(commPin,LOW);
		  delayMicroseconds(period - 160);
	    }
	  }
}

byte MeccaChannel :: calculateImageCheckSum(byte data[], int count)
{
  int cs = 0;
  for (int i = 0; i < count; i++)
  {
    cs = (cs + data[i]) % 255;
  }
  return (byte)(0xFF - cs);
}

/*  END  OF CODE */ 
