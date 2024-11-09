#include <AFMotor.h>

const unsigned int MOTOR_STATUS_STOPPED = 0;
const unsigned int MOTOR_STATUS_FORWARD = 1;
const unsigned int MOTOR_STATUS_BACKWARD = 2;
const unsigned int MOTOR_STATUS_TURN_LEFT = 3;
const unsigned int MOTOR_STATUS_TURN_RIGHT = 4;

class MotorController
{
public:
  MotorController();
  void Initialise(unsigned int motorSpeed);
  void Update(unsigned long microsSinceStart);

  void Forward(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void Backward(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void TurnLeft(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void TurnRight(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void Stop();

  bool IsRunning();

private:
  AF_DCMotor motor1;
  AF_DCMotor motor2;

  // How long the motor has been running for in ticks
  unsigned long motorEndTime;
  unsigned int motorSpeed;
  unsigned int motorStatus;
  byte currentSpeed;

  // This will be set to true if the motor is currently running
  bool motorIsRunning;

  void StartRunning(unsigned long startTime, unsigned int motor1Direction, unsigned int motor2Direction, unsigned long howLongToRun);
};
