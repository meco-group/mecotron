/*
 * MECOTRON TUTORIAL SOLUTION
 *
 * This is a solution to the tasks stated in the tutorial of the Mecotrons. The
 * implementation is using SI units, taking care of required conversions when
 * reading sensor values or setting the motor voltage.
 * It is highly recommended to use this solution as a starting point for further
 * implementations.
 *
 */

#include "robot.h"

bool Robot::init() {
  MECOtron::init(); // initialize the MECOtron

  // initializing the robot's specifics
  resetPendulum();
  i = 0;
  _state = IDLE;
  message(state2text());

  return true;
}

void Robot::controllerHook() {

  for(int k=0; k<7; k++) out[k] = 0.0;
  for(int k=0; k<2; k++) MV[k] = 0.0;

  switch(_state) {

    case MOTORS_ENCODERS : {
      if(i<200){
        MV[0] = 8.0; MV[1] = 8.0;
        i++;
      }
      if(i>=200 && i<400){
        MV[0] = -8.0; MV[1] = -8.0;
        i++;
      }
      if(i==400)
        i = 0;
      out[0] = _encoderA->readCalibratedValue();
      out[1] = _encoderB->readCalibratedValue();
      break;
    }

    case MOTORS_CURRENT : {
      if(i<100){
        MV[0] = 8.0; MV[1] = 8.0;
        i++;
      }
      if(i>=100 && i<200){
        MV[0] = -8.0; MV[1] = -8.0;
        i++;
      }
      if(i==200)
        i=0;
      out[2] = _currentA->readCalibratedValue();
      out[3] = _currentB->readCalibratedValue();
      break;
    }

    case DISTANCE_FRONT : {
      out[4] = 100.0*(_distance1->readCalibratedValue());
      break;
    }

    case DISTANCE_SIDE : {
      out[5] = 100.0*(_distance2->readCalibratedValue());
      break;
    }

    case PENDULUM : {
      out[6] = (180.0/M_PI)*(_pendulum->readCalibratedValue());
      break;
    }

    default : {
      // including case IDLE
    }

  }

  for(int k=0; k<7; k++) writeFloat(k, out[k]);

  setVoltageMotorA(MV[0]);
  setVoltageMotorB(MV[1]);

}

void Robot::resetPendulum() {
  _pendulum->setOffset(_pendulum->readRawValue());
}

void Robot::button0callback() {
  // go to the previous test
  toPrevTest();
  message(state2text());
}

void Robot::button1callback() {
  // go to the next test
  toNextTest();
  message(state2text());
}

void Robot::toNextTest() {
  _state = _state+1;
  if(_state>=NUM_OF_TESTS){
    _state = _state-NUM_OF_TESTS;
  }
  if(_state<0){
    _state = _state+NUM_OF_TESTS;
  }
  i=0;
  resetEncoders();
  resetPendulum();
}

void Robot::toPrevTest() {
  _state = _state-1;
  if(_state>=NUM_OF_TESTS){
    _state = _state-NUM_OF_TESTS;
  }
  if(_state<0){
    _state = _state+NUM_OF_TESTS;
  }
  i=0;
  resetEncoders();
  resetPendulum();
}

String Robot::state2text() {
  String str = "IDLE";
  switch(_state) {
    case MOTORS_ENCODERS : {
      str = "Testing motor encoders, plotting in rad.";
      break;
    }
    case MOTORS_CURRENT : {
      str = "Testing motor current sensors, plotting in A.";
      break;
    }
    case DISTANCE_FRONT : {
      str = "Testing infrared sensor on the front, plotting in cm.";
      break;
    }
    case DISTANCE_SIDE : {
      str = "Testing infrared sensor on the side, plotting in cm.";
      break;
    }
    case PENDULUM : {
      str = "Testing pendulum angular sensor, plotting in degrees.";
      break;
    }
    default : {
      // do nothing
    }
  }
  return str;
}
