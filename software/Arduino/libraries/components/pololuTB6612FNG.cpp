#include "pololuTB6612FNG.h"

PololuTB6612FNG::PololuTB6612FNG(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm, int16_t battery_voltage) :
  HBridgeInterface(battery_voltage, 0), _PIN_IN1(pin_in1), _PIN_IN2(pin_in2), _PIN_PWM(pin_pwm), _PIN_STBY(NULL) {
  // assuming standby is hard-wired off
  init();
}

PololuTB6612FNG::PololuTB6612FNG(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm, uint8_t pin_stby, int16_t battery_voltage) :
  HBridgeInterface(battery_voltage, 0), _PIN_IN1(pin_in1), _PIN_IN2(pin_in2), _PIN_PWM(pin_pwm), _PIN_STBY(pin_stby) {
  // init driver
  this->init();
}

bool PololuTB6612FNG::init() {
  pinMode(_PIN_IN1, OUTPUT);
  pinMode(_PIN_IN2, OUTPUT);
  pinMode(_PIN_STBY, OUTPUT);

  standby_on();

  _battery_voltage_sensor->init();

  return true;
}

void PololuTB6612FNG::setOutputs() {

  if(_pwm != 0) {
    digitalWrite(_PIN_IN1, _polarity);
    digitalWrite(_PIN_IN2, !_polarity);
    analogWrite(_PIN_PWM, _pwm);
  } else {
    // fast motor stop
    digitalWrite(_PIN_IN1, HIGH);
    digitalWrite(_PIN_IN2, HIGH);
    // analogWrite(_PIN_PWM, HIGH);
    // standby_on();
  }
}

void PololuTB6612FNG::standby_on() {
  digitalWrite(_PIN_STBY, LOW);
}

void PololuTB6612FNG::standby_off() {
  digitalWrite(_PIN_STBY, HIGH);
}
