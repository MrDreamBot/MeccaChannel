# MeccaChannel
### Arduino Library to control the Mecanno M.A.X. Robot
#### Version 0.9.0: 2018-08-25

### OVERVIEW:
An Arduino library to control the Meccano M.A.X. robot:
1. Uses GOF's Chain of Responisibility Design Pattern the loosely-couple the interaction between the Arduino and Mecanno M.A.X. devices using the Meccano Smart Protocol
2. Supports Meccano M.A.X.'s Face, motors and servos
3. Provides a sketch which uses Finite State machines (FSMs) and a modified version of Scheduler to give the impression of multitasking


**MeccaChannel** was tested on the following platforms:
* Arduino Uno R3
* Arduino Leonardo compatible

---
![MeccaChannel's UML class  diagram](https://github.com/MrDreamBot/MeccaChannel/MaxClassDiagram.png)
---

### INSTALLATION INSTRUCTIONS:
1. Download this repository as a zip file and unzip it into your Arduino's libraries directory.
2. Do the same with the Scheduler.zip file that is in the previous zip file. In other words, unzip it into your Arduino's libraries directory.

### UNSUPPORTED FUNCTIONS:
1. Learned Intelligent Movement (L.I.M.) programming which allows you to create and record animations by physically moving the robot
2. Meccano M.A.X.'s Infra-red Distance Sensor (Class provided but not yet working)

### YOU CAN FOLLOW THIS PROJECT ON [hackaday.io](https://hackaday.io/project/160606-hacking-the-meccano-max).

