#include "l293d.h"

L293D::L293D(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm, uint16_t battery_voltage, uint8_t ID):
  HBridgeInterface(battery_voltage, ID), _pin_in1(pin_in1), _pin_in2(pin_in2), _pin_pwm(pin_pwm) {
  //do nothing
}

bool L293D::init() {
  pinMode(_pin_in1, OUTPUT);

  // Check if the bridge is 1 or 2 pin controlled
  if(_pin_in2 != 0xFF) pinMode(_pin_in2, OUTPUT);

  return _battery_voltage_sensor->init();
}

void L293D::setOutputs() {
  if(_pwm != 0) {
    digitalWrite(_pin_in1, _polarity);
    if(_pin_in2 != 0xFF) digitalWrite(_pin_in2, !_polarity);
    analogWrite(_pin_pwm, _pwm);
  } else {
    // fast motor stop
    digitalWrite(_pin_in1, false);
    if(_pin_in2 != 0xFF) digitalWrite(_pin_in2, false);
    analogWrite(_pin_pwm, 255);
  }
}
