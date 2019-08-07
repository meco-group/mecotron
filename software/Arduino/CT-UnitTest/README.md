UNIT TEST
=========

This project is used to test the hardware and software.

Hardware checks:

- Distance sensors connected to IRSENS1 (front) and IRSENS2 (side)
- Pendulum connected to ASENS1 @ 5V supply
- Remaining jumpers bridging SIG+GND

Test list:

0. Default status: idle.
1. Motors encorder: the motors start turning back and forth. Channel FloatIn0 = encoderA [rad], FloatIn1 = encoderB [rad]).
2. Motors current: the motors start turning back and forth. Channel FloatIn2 = currentA [A], FloatIn3 = currentB [A]).
3. Distance front: Hold your hand in ~10cm distance from front distance sensor, verify the value reading. Channel FloatIn4 = FrontDistance [m].
4. Distance front: Hold your hand in ~10cm distance from side distance sensor, verify the value reading. Channel FloatIn4 = SideDistance [m].
5. Pendulum: Put the pendulum in the stable and unstable position, verify the value reading. Channel FloatIn5 = PendulumAngle [deg].

Run the software:

1. Flash the project on the Arduino.
2. Open the QRoboticsCenter.
3. Lift the platform and press `Button 1` in QRoboticsCenter to move from idle to test 1.
4. Observe verbose output in the bottom bar to see which test is running.
5. Check the results of the test and press `Button 0` or `Button 1` to move to the previous or next test respectively.
