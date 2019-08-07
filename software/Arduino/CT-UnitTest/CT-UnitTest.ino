#include "robot.h"

static Robot robot;

int updateRobot() {
  robot.controllerHook();
  return 0;
}

void setup() {
  System.setHAL(&robot);
  System.setCommunicator(robot.getCommunicator());
  System.addThread(ABOVENORMAL, 10000, &updateRobot, false);
  System.start(SEQUENTIAL);
}

int main(void) {
  init();
  setup();
  while(true) System.run(RESCHEDULED);
  return 0;
}
