The software folder is a snapshot (20/12/2016) of the [MECO-CSI](https://github.com/maartenverbandt/MECO-CSI) 

# MECO-CSI
[MECO](http://www.mech.kuleuven.be/en/pma/research/meco) Control Setup Interface

1. Should you be interested?
============================
Are you looking for one of the following?
* trying to do more advanced things with your setup
* looking for a nice way of interacting with your setup
* tired of cumbersome debugging
* interested in multitasking for your setup

Then keep on reading because the MECO-CSI repo might resolve your problems!

2. What does MECO-CSI offer?
============================
The MECO-CSI project was inspired by the high popularity of Arduino products and its derivatives. The average Arduino project is becoming more and more complex, which makes development cumbersome. The main difficulties in this regard are:
* Multitasking: running different tasks at the same time
* Interacting: getting information in and out

This project tackles both problems at the same time. Multitasking is done by means of the microOS Arduino library. It allows you to easily run tasks in parallel at a fixed rate. For instance, checking the serial bus for incoming data at 100Hz, updating the info on an LCD screen at 20Hz and controlling some motors and LEDs at 50Hz. The interaction is simplified by the combination of the microOS library and a GUI. The microOS library polls the serial busses for incoming data and sends data on these busses. 
~~~~
System.getGPinFloat(0); //get incoming floating point data (channel 1 out of 8)
System.getGPinInt(3); //get incoming integer data (channel 4 out of 4)
System.setGPoutFloat(7,0.1); //Write 0.1 to the last floating point outgoing channel
System.setGPoutInt(0,10); //Write 10 to the last integer outgoing channel
~~~~

The GUI allows the user to visualize incoming data, for instance sensor readings or intermediate results of onboard computations. The outputs can be generated with a control panel on the side, which comes in handy move a servo to some position or activate an LED.

![alt tag](https://raw.githubusercontent.com/maartenverbandt/MECO-CSI/master/Util/screenshot.png)

3. What if I really want to use it?
===================================

3.1. Using it out of the box

Cloning the repo is all you have to do to get things running. In the Arduino folder, you get the libraries and some example projects to get you started. In the Desktop folder, you can find stable builds for Windows, Unix and MAC OS. 

3.2. Building from source

The MECO-CSI repo is composed of two separate repos:
* https://github.com/maartenverbandt/MicroOS
* https://github.com/maartenverbandt/QRoboticsCenter

You can find the instructions over there to build them from source.

