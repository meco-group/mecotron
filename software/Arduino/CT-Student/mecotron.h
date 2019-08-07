#ifndef MECOTRON_H
#define MECOTRON_H

/*
 * MECOtron header
 * This header defines pins, values and conversion factors inherent to the
 * MECOtrons.
 *
 */

#include "math.h"

#define VERSION       3   // Version of the MECOtron
#define MECOTRON_TYPE 20  // Type of the MECOtron
#define MECOTRON_ID   0   // ID of the MECOtron

/* PIN ASSIGNMENTS */
#define MOT_STBY      6
#define MOT_A_IN1     4
#define MOT_A_IN2     5
#define MOT_A_PWM     7
#define MOT_A_ENC_A   2
#define MOT_A_ENC_B   11
#define MOT_A_CURR    A0
#define MOT_B_IN1     9
#define MOT_B_IN2     10
#define MOT_B_PWM     8
#define MOT_B_ENC_A   3
#define MOT_B_ENC_B   12
#define MOT_B_CURR    A1

#define DIST1_PIN     A2
#define DIST2_PIN     A3
#define PENDULUM_PIN  A5
#define LED1_PIN      A6
#define LED2_PIN      A7

/* MECOtron HARDWARE */
#define BATTERY_VOLTAGE                     12.0f  // battery voltage [V]
#define R_WHEEL                            0.0325  // wheel radius [m]
#define WHEELBASE                          0.1660  // wheelbase [m]

/* TIMING INFORMATION */
#define TSAMPLE                             0.010  // sampling time [s]
#define MINIMUM_FD_TIMELAPSE                 1000  // minimum timelapse for updating a finite difference [µs]

/* CONVERSION FACTORS */
#define ENC_TO_RAD       M_PI / 28.0 / 16.0 / 2.0  // transform encoder counts to radians (1792 enc per 2*pi)
#define RAD_TO_ENC               1.0 / ENC_TO_RAD  // transform radians to encoder counts
#define VOLT_TO_AMP   5.0 / 0.082 / 20.0 / 1023.0  // transform shunt voltage to motor current (82 mOhm, amplified 20x)

/* STATUS LED */
#define STATUS_LED    13      // on Arduino MEGA

/* FUNCTIONALITY */
#define USB_SERIAL            // deactivate with #undef
#undef BT_SERIAL              // deactivate with #undef
#define BT_BAUD       115200  // baud rate for BT

/* LOGICAL SHORTCUT */
#define ON            1
#define OFF           0

// Required headers
#include "microOS.h"
#include "mavlink_communicator.h"
#include "hal_base.h"

// Hardware headers
#include <pololuTB6612FNG.h>
#include <encoder_sensor.h>
#include <difference_sensor.h>
#include <analog_sensor.h>
#include <sharp41S.h>
#include <vishaySpectrol157.h>

class Communicator : public MavlinkCommunicator {
  private:
    HALBase* _hal;
    void* _tron;
  public:
    Communicator(HALBase *hal) : MavlinkCommunicator(MECOTRON_ID, MECOTRON_TYPE, hal), _hal(hal), _tron(hal) { }
    void handleEvent(uint16_t event);
};

class MECOtron : public HALBase {
  protected:
    uint8_t _ID = MECOTRON_ID;
    uint8_t _type = MECOTRON_TYPE;

    // Communicator for QRC
    Communicator* _comm;

    // Motors
    HBridgeInterface* _motorA = new PololuTB6612FNG(MOT_A_IN1, MOT_A_IN2, MOT_A_PWM, MOT_STBY, (uint16_t)(1000*BATTERY_VOLTAGE));
    HBridgeInterface* _motorB = new PololuTB6612FNG(MOT_B_IN1, MOT_B_IN2, MOT_B_PWM, MOT_STBY, (uint16_t)(1000*BATTERY_VOLTAGE));

    // Sensors
    Sensor1D* _encoderA = new EncoderSensor(MOT_A_ENC_A, MOT_A_ENC_B);
    Sensor1D* _encoderB = new EncoderSensor(MOT_B_ENC_A, MOT_B_ENC_B);
    Sensor1D* _speedA = new DifferenceSensor(_encoderA, MINIMUM_FD_TIMELAPSE);
    Sensor1D* _speedB = new DifferenceSensor(_encoderB, MINIMUM_FD_TIMELAPSE);
    Sensor1D* _currentA = new AnalogSensor(MOT_A_CURR);
    Sensor1D* _currentB = new AnalogSensor(MOT_B_CURR);
    Sensor1D* _distance1 = new Sharp41S(DIST1_PIN);
    Sensor1D* _distance2 = new Sharp41S(DIST2_PIN);
    Sensor1D* _pendulum  = new VishaySpectrol157(PENDULUM_PIN);

    float _hbridge_buffer[2] = { 0.0, 0.0 };

    // Buttons
    bool _button_states[8] = {false, false, false, false, false, false, false, false};
    bool toggleButton(uint8_t button) {
      _button_states[button] = !_button_states[button];
      return _button_states[button];
    }

  public:
    MECOtron() : HALBase(STATUS_LED, VERSION) {
      // set primary serial to USB if flag is defined
      #ifdef USB_SERIAL
        setPrimarySerial(&Serial);
      #else
        setPrimarySerial(NULL);
      #endif

      // set secondary serial to BT if flag is defined
      #ifdef BT_SERIAL
        setSecondarySerial(&Serial3);
      #else
        setSecondarySerial(NULL);
      #endif

      _comm = new Communicator(this);

      // set up sensors
      _encoderA->setScale(-ENC_TO_RAD);
      _encoderB->setScale(ENC_TO_RAD);
      _speedA->setScale(-ENC_TO_RAD);
      _speedB->setScale(ENC_TO_RAD);
      _currentA->setScale(VOLT_TO_AMP);
      _currentA->setOffset(511.0);
      _currentB->setScale(VOLT_TO_AMP);
      _currentB->setOffset(511.0);
    }

    bool init() {
      // start serials
      #ifdef USB_SERIAL
        Serial.begin(115200);
      #endif
      #ifdef BT_SERIAL
        Serial3.begin(BT_BAUD);
      #endif

      // set up Timer4 for fast PWM @ 62kHz
      TCNT4  = 0;
      TCCR4A = 0 | _BV(COM4A1) | _BV(WGM41) | _BV(WGM40);
      TCCR4B = 0 | _BV(WGM43)  | _BV(WGM42) | _BV(CS40);
      OCR4A  = 255;

      resetEncoders();
      resetPendulum();
      initLEDs();

      return true;
    }

    void io() {
      _motorA->setBridgeVoltage((int16_t)(1000*_hbridge_buffer[0]));
      _motorB->setBridgeVoltage((int16_t)(1000*_hbridge_buffer[1]));
      _encoderA->readCalibratedValue();
      _encoderB->readCalibratedValue();
      _speedA->readCalibratedValue();
      _speedB->readCalibratedValue();
      _currentA->readCalibratedValue();
      _currentB->readCalibratedValue();
      _distance1->readCalibratedValue();
      _distance2->readCalibratedValue();
      _pendulum->readCalibratedValue();
    }

    virtual void control() { }

    void resetEncoders() {
      _encoderA->init();
      _encoderB->init();
    }

    void resetPendulum() {
      _pendulum->setOffset(_pendulum->readRawValue());
    }

    void initLEDs() {
      pinMode(LED1_PIN, OUTPUT);
      pinMode(LED2_PIN, OUTPUT);
    }

    // Shorthands for QRC
    inline void message(String str) { System.println(str); }
    inline void writeInt(const uint8_t nr, const int16_t val) { System.setGPoutInt(nr, val); }
    inline int16_t readInt(const uint8_t nr) { return System.getGPinInt(nr); }
    inline void writeFloat(const uint8_t nr, const float val) { System.setGPoutFloat(nr, val); }
    inline float readFloat(const uint8_t nr) { return System.getGPinFloat(nr); }

    // Shorthands for microOS
    inline void setVoltageMotorA(const float volts) { _hbridge_buffer[0] = volts; }
    inline void setVoltageMotorB(const float volts) { _hbridge_buffer[1] = volts; }
    inline float getVoltageMotorA() { return ((float)_motorA->getBridgeVoltage())/1000.0; }
    inline float getVoltageMotorB() { return ((float)_motorB->getBridgeVoltage())/1000.0; }
    inline float getPositionMotorA() { return _encoderA->peekCalibratedValue(); }
    inline float getPositionMotorB() { return _encoderB->peekCalibratedValue(); }
    inline float getSpeedMotorA() { return _speedA->peekCalibratedValue(); }
    inline float getSpeedMotorB() { return _speedB->peekCalibratedValue(); }
    inline float getCurrentMotorA() { return _currentA->peekCalibratedValue(); }
    inline float getCurrentMotorB() { return _currentB->peekCalibratedValue(); }
    inline float getFrontDistance() { return _distance1->peekCalibratedValue(); }
    inline float getSideDistance() { return _distance2->peekCalibratedValue(); }
    inline float getPendulumAngle() { return _pendulum->peekCalibratedValue(); }

    // Shorthands for LEDs
    inline void LED1(const bool on) { digitalWrite(LED1_PIN, (on ? HIGH : LOW)); }
    inline void LED2(const bool on) { digitalWrite(LED2_PIN, (on ? HIGH : LOW)); }

    // Provided for Mavlink
    uint8_t id() { return _ID; }
    uint8_t type() { return _type; }

    // Button callbacks
    virtual void button0callback() {
      if(toggleButton(0)) message("MECOtron enabled.");
      else message("MECOtron disabled.");
    }
    virtual void button1callback() {
      toggleButton(1);
      init();
      message("Reset.");
    }
    virtual void button2callback() { toggleButton(2); }
    virtual void button3callback() { toggleButton(3); }
    virtual void button4callback() { toggleButton(4); }
    virtual void button5callback() { toggleButton(5); }
    virtual void button6callback() { toggleButton(6); }
    virtual void button7callback() { toggleButton(7); }

    Communicator* getCommunicator() {
      return _comm;
    }

};

// making this inline to avoid linking errors
inline void Communicator::handleEvent(uint16_t event) {
  MECOtron* tron = (MECOtron*) _tron;
  switch(event) {
    case 1000: tron->button0callback(); break;
    case 1001: tron->button1callback(); break;
    case 1002: tron->button2callback(); break;
    case 1003: tron->button3callback(); break;
    case 1004: tron->button4callback(); break;
    case 1005: tron->button5callback(); break;
    case 1006: tron->button6callback(); break;
    case 1007: tron->button7callback(); break;
  }
};

#endif // MECOTRON_H
