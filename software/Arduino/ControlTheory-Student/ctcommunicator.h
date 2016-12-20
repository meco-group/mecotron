#ifndef CTCOMMUNICATOR_H
#define CTCOMMUNICATOR_H

#include "mavlink_communicator.h"
#include "robot.h"

class CTCommunicator : public MavlinkCommunicator
{
private:
	Robot *_robot;

public:
	CTCommunicator(Robot *robot, HALBase *hal);
	void handleEvent(uint16_t event);
};

#endif //CTCOMMUNICATOR_H
