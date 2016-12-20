#include "robot.h"
#include <math.h>

#define ENC1_PINA		18
#define ENC1_PINB		32
#define ENC2_PINA		19
#define ENC2_PINB		42
#define DIST1_PIN		A0
#define DIST2_PIN		A1
#define MOT1_PIN_IN1	8u
#define MOT1_PIN_IN2	12u
#define MOT1_PIN_EN		6u
#define MOT2_PIN_IN1	7u
#define MOT2_PIN_IN2	4u
#define MOT2_PIN_EN		5u
#define PENDULUM_PIN	A3
#define LED1_PIN 		34
#define LED2_PIN 		40
#define BATTERY_VOLTAGE	6000


Robot::Robot(uint8_t ID):
	_ID(ID),
	_type(20),
	_encoder1(new EncoderSensor(ENC1_PINA,ENC1_PINB)),
	_encoder2(new EncoderSensor(ENC2_PINA,ENC2_PINB)),
	_distance1(new Sharp41S(DIST1_PIN)),
	_distance2(new Sharp41S(DIST2_PIN)),
	_pendulum(new AnalogSensor(PENDULUM_PIN,12)),
	_motor1(new L293D(MOT1_PIN_IN1,MOT1_PIN_IN2,MOT1_PIN_EN,BATTERY_VOLTAGE)),
	_motor2(new L293D(MOT2_PIN_IN1,MOT2_PIN_IN2,MOT2_PIN_EN,BATTERY_VOLTAGE))
{
	_pendulum->setScale(2.0f*M_PI/1023.0f);
}

void Robot::init(){
	//initialize the robot - sort of starting procedure
	resetEncoders();
	_state = IDLE;
}

void Robot::controllerHook(){
	//do something that is periodic: reading from sensors, setting the motors, updating variables sent to the pc..

	switch(_state){
		case IDLE:{
			_motor1->setBridgeVoltage(0);
			_motor2->setBridgeVoltage(0);
			break;}

		case MOTORS_FORWARD:{
			_timer++; //update timer variable
			_motor1->setBridgeVoltage(_voltage);
			_motor2->setBridgeVoltage(_voltage);
			if(_timer >= 400){
				_state = MOTORS_REVERSE;
				_timer = 0;
				System.print(_encoder1->readRawValue());
				System.print(", ");
				System.println(_encoder2->readRawValue());

				// motors off
				_motor1->setBridgeVoltage(0);
				_motor2->setBridgeVoltage(0);
			}
			break;}

		case MOTORS_REVERSE:{
			_timer++; //update timer variable
			_motor1->setBridgeVoltage(-_voltage);
			_motor2->setBridgeVoltage(-_voltage);
			if(_timer >= 400){
				_state = DISTANCE_FRONT;
				_timer = 0;
				System.print(_encoder1->readRawValue());
				System.print(", ");
				System.println(_encoder2->readRawValue());

				// motors off
				_motor1->setBridgeVoltage(0);
				_motor2->setBridgeVoltage(0);
			}
			break;}

		case DISTANCE_FRONT:{
			if(_button_states[4]){
				toggleButton(4); //untoggle it
				System.print("Front distance: ");
				System.print(_distance1->readCalibratedValue());
				System.println(" m.");

				_state = DISTANCE_SIDE;
			}
			break;}

		case DISTANCE_SIDE:{
			if(_button_states[4]){
				toggleButton(4); //untoggle it
				System.print("Side distance: ");
				System.print(_distance2->readCalibratedValue());
				System.println(" m.");

				_state = PENDULUM_DOWN;
			}
			break;}

		case PENDULUM_DOWN:{
			if(_button_states[4]){
				toggleButton(4); //untoggle it
				resetPendulum();
				System.println(" Pendulum reset.");

				_state = PENDULUM_UP;
			}
			break;}

		case PENDULUM_UP:{
			if(_button_states[4]){
				toggleButton(4); //untoggle it
				System.print("Pendulum top angle: ");
				System.print(_pendulum->readCalibratedValue());
				System.println(" rad.");

				_state = IDLE;
			}
			break;}
	}

	System.setGPoutFloat(0,_distance1->readCalibratedValue());
	System.setGPoutFloat(1,_distance2->readCalibratedValue());
	System.setGPoutFloat(2,_pendulum->readCalibratedValue());
	System.setGPoutFloat(3,_pendulum->readRawValue());

	System.setGPoutInt(0,_encoder1->readRawValue());
	System.setGPoutInt(1,_encoder2->readRawValue());
	System.setGPoutInt(2,_motor1->getBridgeVoltage());
	System.setGPoutInt(3,_motor2->getBridgeVoltage());
}

void Robot::resetEncoders()
{
	_encoder1->init();
	_encoder2->init();
}


void Robot::resetPendulum()
{
	_pendulum->setOffset(_pendulum->readRawValue());
}

uint8_t Robot::id()
{
    return _ID;
}

uint8_t Robot::type()
{
	return _type;
}

bool Robot::toggleButton(uint8_t button)
{
	_button_states[button] = !_button_states[button];
	return _button_states[button];
}

bool Robot::controlEnabled()
{
	return _button_states[0];
}

void Robot::button1callback()
{
	if(toggleButton(0)){
		System.println("Controller enabled.");
		_state = MOTORS_FORWARD;
	} else{
		System.println("Controller disabled.");
		_state = IDLE;
	}
}

void Robot::button2callback()
{
	toggleButton(1);

	resetEncoders();
	resetPendulum();

	System.println("Reset.");
}
void Robot::button3callback()
{
	toggleButton(2);
}

void Robot::button4callback()
{
	toggleButton(3);
}

void Robot::button5callback()
{
	toggleButton(4);
}

void Robot::button6callback()
{
	toggleButton(5);
}

void Robot::button7callback()
{
	toggleButton(6);
}

void Robot::button8callback()
{
	toggleButton(7);
}
