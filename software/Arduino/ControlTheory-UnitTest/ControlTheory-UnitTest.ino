#include <Wire.h>
#include "robot.h"
#include "encoder_sensor.h"
#include "microOS.h"
#include "ctcommunicator.h"

#define USB_SERIAL

static Robot robot;

int updateRobot(void)
{
	robot.controllerHook();
	return 0;
}

int slowHook(void)
{
	System.sendSystemRequest(NEXTTHREADINFO);
	return 0;
}

void setup()
{
	HALBase *CThal = new HALBase(34,78);
	System.setHAL(CThal);

	CTCommunicator *CTcomm = new CTCommunicator(&robot,CThal);
	System.setCommunicator(CTcomm);

	System.addThread(ABOVENORMAL, 10000, &updateRobot, false);
	System.addThread(LOWEST, 2000000, &slowHook, false);

	robot.init();
	System.start(SEQUENTIAL);
}

void loop()
{
	System.run(RESCHEDULED);
}

int main(void) {
  // Mandatory init
  init();
  setup();

  while(true) loop();
  return 0;
}
