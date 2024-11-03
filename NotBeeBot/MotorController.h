#include <AFMotor.h>

class MotorController
{
public:
  MotorController();
  void Initialise(unsigned int motor_speed);
  void Update(unsigned long loop_time);

  void Forward(unsigned int distance);
  void Backward(unsigned int distance);
  void TurnLeft(float angle);
  void TurnRight(float angle);
  void Stop();

private:
  AF_DCMotor motor1;
  AF_DCMotor motor2;

  // How long the motor has been running for in ticks
  unsigned int motorRunTime;
  unsigned int motorTargetRunTime;
  bool motorRunning;
  unsigned int motorDirection;
  unsigned int motorSpeed;
  byte currentSpeed;
};
