#include "MeccaMaxDrive.h"

// This class combines MAX's 2 motor devices to implement a two-whell drive system

MeccaMaxDrive::MeccaMaxDrive(MeccaChannel *channel) {
	// set up MAX's 2 motors
	leftMotor = new MeccaMaxMotorDevice("LeftMotor");
	leftMotor->flipMotor();
	rightMotor = new MeccaMaxMotorDevice("RightMotor");
	// using Chain of Responsibility Design Pattern to 
	// loosely couple Channel and Devices
	rightMotor->setSuccessor(leftMotor);
	channel->setDeviceChain(rightMotor);
	this->channel = channel;
}

// methods to drive the motors
void MeccaMaxDrive::forward() {
	leftMotor->forward(channel);
	rightMotor->forward(channel);
	channel->communicateAll();
}

void MeccaMaxDrive::backward() {
	leftMotor->backward(channel);
	rightMotor->backward(channel);
	channel->communicateAll();
}

void MeccaMaxDrive::stop() {
	leftMotor->stop(channel);
	rightMotor->stop(channel);
	channel->communicateAll();
}

void MeccaMaxDrive::turnLeft() {
	leftMotor->backward(channel);
	rightMotor->forward(channel);
	channel->communicateAll();
}

void MeccaMaxDrive::turnRight() {
	leftMotor->forward(channel);
	rightMotor->backward(channel);
	channel->communicateAll();
}
