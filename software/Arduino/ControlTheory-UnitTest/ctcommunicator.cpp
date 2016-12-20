#include "ctcommunicator.h"

CTCommunicator::CTCommunicator(Robot *robot, HALBase *hal) :
	MavlinkCommunicator(robot->id(),hal,robot->type()),
	_robot(robot)
{
	//do nothing special
}

void CTCommunicator::handleEvent(uint16_t event)
{
	//MavlinkCommunicator::handleEvent(event);

	switch(event){
	case 1000:
		_robot->button1callback();
		break;
	case 1001:
		_robot->button2callback();
		break;
	case 1002:
		_robot->button3callback();
		break;
	case 1003:
		_robot->button4callback();
		break;
	case 1004:
		_robot->button5callback();
		break;
	case 1005:
		_robot->button6callback();
		break;
	case 1006:
		_robot->button7callback();
		break;
	case 1007:
		_robot->button8callback();
		break;
	}
}
