#include "robot.h"

static Robot robot;

void update() {
  robot.io();
  robot.control();
}

void setup() {
  System.setHAL(&robot);
  System.setCommunicator(robot.getCommunicator());
  System.addThread(ABOVENORMAL, 10000, &update, false);
  System.start(SEQUENTIAL);
}

int main(void) {
  init();
  setup();
  while(true) System.run(RESCHEDULED);
  return 0;
}
