#ifndef ROBOT_H
#define ROBOT_H

/*
 * ROBOT Class
 *
 * Class incorporating the robot. This class is used to define state machines,
 * control algorithms, sensor readings,...
 * It should be interfaced with the communicator to send data to the world.
 *
 */

#include "mecotron.h" // Include MECOTRON header
#include <sharp41S.h> // Include headers for additional sensors

#define NUM_OF_TESTS 6

class Robot : public MECOtron {
  private:
    // Sensors
    Sensor1D* _distance1 = new Sharp41S(DIST1_PIN);
    Sensor1D* _distance2 = new Sharp41S(DIST2_PIN);
    Sensor1D* _pendulum  = new AnalogSensor(PENDULUM_PIN);

    // Variables
    int i;
    bool dir;
    enum state {
      IDLE,
      MOTORS_ENCODERS,
      MOTORS_CURRENT,
      DISTANCE_FRONT,
      DISTANCE_SIDE,
      PENDULUM,
    };
    state _state = IDLE;
    
    float out[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    float MV[2] = {0.0, 0.0};

    String state2text();

  public:
    // Constructor
    Robot() {
      _pendulum->setScale(PEN_TO_RAD);
    }

    void controllerHook();

    // General functions
    bool init();  // Set up the robot
    void resetPendulum(); // Reset pendulum zero position

    void button0callback();
    void button1callback();
    void toNextTest();
    void toPrevTest();

};

#endif // ROBOT_H
