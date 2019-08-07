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

class Robot : public MECOtron {
  private:
    // Class variables
    float x[2];   // We can, for example, remember the last two velocities of wheel A in a vector (float array) x

  public:
    // Constructor
    Robot() { }

    void control();

    // General functions
    bool init();  // Set up the robot

    bool controlEnabled();

    void button0callback();
    void button1callback();

};

#endif // ROBOT_H
