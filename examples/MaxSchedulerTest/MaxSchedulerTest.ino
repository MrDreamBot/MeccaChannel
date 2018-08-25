#include <MeccaMaxServoDevice.h>
#include <MeccaMaxFaceDevice.h>
#include <MeccaMaxDrive.h>
#include <Scheduler.h>

// select only one of the #defines below to test one device at a time
#define _SERVOS
#define _FACE
#define _DRIVE

// for use by Face FSM for multitasking purposes
enum FaceStates {
  SCROLLING_TEXT,
  HAPPY_FACE
};

// for use by Drive FSM for multitasking purposes
enum DriveStates {
  FORWARD,
  BACKWARD,
  STOP,
  TURN_LEFT,
  TURN_RIGHT
};
/* Test program to test out a new Meccano Max library
 *  which is still under development
 *  Testing MAX servos and motors
 */
static MeccaChannel *channelFACE;
static MeccaChannel *channelSERVOS;
static MeccaChannel *channelDRIVE;
static MeccaMaxServoDevice *headServo;
static MeccaMaxServoDevice *clawServo;
static MeccaMaxFaceDevice *face;
static MeccaMaxDrive *drive;
static int pos = 0;
static int scrollPos = 0;
static FaceStates faceState = SCROLLING_TEXT;
static DriveStates driveState = FORWARD;
static int drive_update = 200;

static Scheduler mainScheduler = Scheduler();
static Scheduler subScheduler = Scheduler();

static byte happyFace[18] = {
  0xFF, 0xFF, 0xF1, 0xE6, 0xE4, 0xB1, 0x7F, 0x7F,
  0x7F, 0x7F, 0xF1, 0xE6, 0xE4, 0xF1, 0xFF, 0xFF,
  0xF5, 0x2D
  };
static byte sadFace[18] = {
  0xFF, 0xFF, 0xFF, 0xC7, 0x8B, 0x8D, 0xC3, 0xFF, 
  0xFF, 0xC3, 0x8D, 0x8B, 0xC7, 0xFF, 0xFF, 0xFF, 
  0xF5, 0x03 
};

static byte scrollMaxImage[70] = {
0xff, 0xff, 0x01, 0xfb, 0xf7, 0xfb, 0x01, 0xff, 
0xfd, 0xfb, 0x07, 0xfb, 0xfd, 0xff, 0xff, 0xff, 
0xff, 0x01, 0xfb, 0xf7, 0xfb, 0x01, 0xff, 0x03, 
0xed, 0xed, 0x01, 0xff, 0x3d, 0xdb, 0xe7, 0xdb, 
0x3d, 0xff, 0xff, 0xff, 0xff, 0x01, 0xed, 0xed, 
0x13, 0xff, 0x83, 0x7d, 0x7d, 0x83, 0xff, 0x83, 
0x7d, 0x7d, 0x7d, 0xff, 0x01, 0xef, 0xd7, 0x35, 
0xff, 0x73, 0x6d, 0x6d, 0x9d, 0xff, 0xff, 0x41, 
0xff, 0xff, 0x41, 0xff, 0xff, 0x41 
};

static byte tmpImage[18];
static int pinFACE = 2;
static int pinSERVOS = 3;
static int pinDRIVE = 4;

// the following functions are for debugging

// print the image bytes and compare the check sum with that calculated
void printImage(byte image[], int count)
{
  for (int i = 0; i < count; i++)
  {
    Serial.print(image[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("Checksum: ");
  Serial.print(image[17], HEX);
  Serial.print(" vs ");
  Serial.println(MeccaChannel::calculateImageCheckSum(image, 17), HEX);
}

// print device info for debugging
void printDeviceStatus(MeccaDevice *dev1, MeccaDevice *dev2)
{
  if (dev1 == NULL) return;
  Serial.print(dev1->getName());
  Serial.print(": ");
  Serial.print((dev1->getModuleNum() == 255)? "Not Connected: ": "Connected at position: ");
  Serial.println(dev1->getModuleNum());

  if (dev2 == NULL) return;
  Serial.print(dev2->getName());
  Serial.print(": ");
  Serial.print((dev2->getModuleNum() == 255)? "Not Connected: ": "Connected at position: ");
  Serial.println(dev2->getModuleNum());
}

// print device info for a channel
void printDevices(MeccaChannel *channel)
{
  for (int i = 0; i < MeccaChannel :: MAX_DEVICES_PER_CHANNEL; i++) 
  {
    if (channel->getDeviceName(i) == NULL) return;
    Serial.print("Device #");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(channel->getDeviceName(i));
  }
}

// print protocol exchange between channel and devices
void printDebugInfo(MeccaChannel *channel)
{
  for (int i = 0; i < MeccaChannel :: MAX_DEVICES_PER_CHANNEL; i++) 
  {
    Serial.print(channel->getOutputByteInfo(i), HEX);
    Serial.print(" ");
  }
  Serial.print(channel->getModuleNumInfo(), HEX);
  Serial.print(" - ");
  Serial.println(channel->getInputByteInfo(), HEX);
}

// communicate with all devices and print debug info
void poll(MeccaChannel *channel)
{
  for (int i = 0; i < MeccaChannel :: MAX_DEVICES_PER_CHANNEL; i++) 
  {
    channel->communicate();
    printDebugInfo(channel);
  }
}
// end of debugging functions

// setup async port for serial monitor
void setup() 
{
  Serial.begin(57600);
  Serial.println("\n\n");


#ifdef _DRIVE
  channelDRIVE = new MeccaChannel(pinDRIVE);
  drive = new MeccaMaxDrive(channelDRIVE);
    // wait till last device in chain has been discovered
  while (channelDRIVE->getDeviceName(1) == NULL) {
    channelDRIVE->communicate(); // start comm with devices
    printDebugInfo(channelDRIVE);
    delay(100);
  }

  mainScheduler.schedule(testDrive, 200);
#endif

  // setup 2 servos
#ifdef _SERVOS
  channelSERVOS = new MeccaChannel(pinSERVOS);
  headServo = new MeccaMaxServoDevice("HeadServo");
  headServo->mapTo180Degrees(100, 150);
  clawServo = new MeccaMaxServoDevice("ClawServo");
  clawServo->mapTo180Degrees(0x80, 0xe0);
  // using Chain of Responsibility Design Pattern to loosely couple Channel and Devices
  headServo->setSuccessor(clawServo);
  channelSERVOS->setDeviceChain(headServo);
  pos = 0;
  while (channelSERVOS->getDeviceName(1) == NULL) {
    channelSERVOS->communicate(); // start comm with devices
    printDebugInfo(channelSERVOS);
    delay(100);
  }

  mainScheduler.schedule(testServos, 200);
#endif

// set up 1 face device
#ifdef _FACE
  channelFACE = new MeccaChannel(pinFACE);
  face = new MeccaMaxFaceDevice("Face");
  face->setScrollImage(scrollMaxImage, 70);
  channelFACE->setDeviceChain(face);

  printImage(happyFace, 18);
  printImage(sadFace, 18);
  happyFace[17] = channelFACE->calculateImageCheckSum(happyFace, 17);
  sadFace[17] = channelFACE->calculateImageCheckSum(sadFace, 17);
  while (channelFACE->getDeviceName(0) == NULL) {
    channelFACE->communicate(); // start comm with devices
    printDebugInfo(channelFACE);
    delay(100);
  }
  channelFACE->setMaxDevices(1);
  mainScheduler.schedule(testFace, 200);
#endif

  // note that it is possible to chain 2 motors and 2 servos in one channel
  // max. 4 devices per channel
  

  Serial.println("Starting Max Tests...");
  

  Serial.println("Last device has been discovered...");

}

#ifdef _SERVOS
void testServos() 
{
  Serial.print("Servo position: ");
  Serial.println(pos);

  // make the servos move 30 degrees at a time between 0 and 180
  printDeviceStatus(headServo, clawServo);
  printDevices(channelSERVOS);
  headServo->setServoPosition(channelSERVOS, pos);
  clawServo->setServoPosition(channelSERVOS, pos);
  poll(channelSERVOS);
  pos = (pos + 30) % 180;
  mainScheduler.schedule(testServos, 1500);
}
#endif


#ifdef _FACE

void testFace() 
{
    // print debugging info
    printDeviceStatus(face, NULL);
    printDevices(channelFACE);

    // FSM sequencing the display of scrolling text and the happy face
    switch (faceState) {
      case SCROLLING_TEXT:
        face->scrollImage(channelFACE);
        mainScheduler.schedule(testFace, 300);
        if (face->getScrollPos() == 0) {
          // scrolling of text has been completed
          faceState = HAPPY_FACE;
        }
        break;
      case HAPPY_FACE:
        // display happy face
        face->setImage(happyFace, 18, channelFACE);
        mainScheduler.schedule(testFace, 6000);
        faceState = SCROLLING_TEXT;
        break;
    }
}
#endif

#ifdef _DRIVE
void testDrive()
{
  // reset the sub-Scheduler
  subScheduler.clear();

  // FSM governing the behaviour of the drive test
  switch (driveState) {
    case FORWARD:
      drive->forward();
      mainScheduler.schedule(testDrive, 10000);
      subScheduler.schedule(driveForward, drive_update);
      driveState = TURN_LEFT;
      break;
    case TURN_LEFT:
      drive->turnLeft();
      mainScheduler.schedule(testDrive, 10000);
      subScheduler.schedule(driveLeft, drive_update);
      driveState = TURN_RIGHT;
      break;
    case TURN_RIGHT:
    drive->turnRight();
      mainScheduler.schedule(testDrive, 10000);
      subScheduler.schedule(driveRight, drive_update);
      driveState = STOP;
      break;
    case STOP:
      drive->stop();
      mainScheduler.schedule(testDrive, 10000);
      subScheduler.schedule(driveStop, drive_update);
      driveState = BACKWARD;
      break;
    case BACKWARD:
      drive->backward();
      mainScheduler.schedule(testDrive, 10000);
      subScheduler.schedule(driveBackward, drive_update);
      driveState = FORWARD;
      break;
  }

}

// function to be called in the Drive FSM
void driveForward()
{
  drive->forward();
  subScheduler.schedule(driveForward, drive_update);
}
void driveLeft()
{
  drive->turnLeft();
  subScheduler.schedule(driveLeft, drive_update);
}

void driveRight()
{
  drive->turnRight();
  subScheduler.schedule(driveRight, drive_update);
}
void driveBackward()
{
  drive->backward();
  subScheduler.schedule(driveBackward, drive_update);
}

void driveStop()
{
  drive->stop();
  subScheduler.schedule(driveStop, drive_update);
}

#endif

void loop()
{
  // multitasking using Scheduler and FSM
  mainScheduler.update();

  // sub-Scheduler for updating the drive as the motors will stop without
  // receiving a command for extended period of time
  subScheduler.update();
}


