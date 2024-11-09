#include <Arduino.h>
#include "MotorController.h"

MotorController::MotorController()
    : motor1(1), motor2(2)
{
  // Initialise to a stopped state
  Stop();
}

void MotorController::Initialise(unsigned int motor_speed)
{
  // Set initial speed of the motor & stop
  motorSpeed = motor_speed;
  currentSpeed = 0;
  motor1.setSpeed(motor_speed);
  motor1.run(RELEASE);

  motor2.setSpeed((unsigned int)(motor_speed * 1.12));
  motor2.run(RELEASE);
}

void MotorController::Update(unsigned long microsSinceBoot)
{
  if (motorStatus == MOTOR_STATUS_STOPPED || microsSinceBoot > motorEndTime)
  {
    // Stop the motors if we have passed end time
    Stop();
    return;
  }

  // Speed up motor from insitial start
  if (currentSpeed < motorSpeed)
  {
    currentSpeed++;
    motor1.setSpeed(currentSpeed);
    motor2.setSpeed(currentSpeed);
  }
}

bool MotorController::IsRunning()
{
  return motorIsRunning;
}

void MotorController::StartRunning(unsigned long startTime, unsigned int motor1Direction, unsigned int motor2Direction, unsigned long howLongToRun)
{
  motorIsRunning = true;
  motorEndTime = startTime + howLongToRun;

  // Start at speed of 100 so not instant power
  currentSpeed = 100;

  motor1.run(motor1Direction);
  motor2.run(motor2Direction);
}

void MotorController::Forward(unsigned long microsSinceBoot, unsigned long howLongToRun)
{
  motorStatus = MOTOR_STATUS_FORWARD;
  StartRunning(microsSinceBoot, FORWARD, FORWARD, howLongToRun);
}

void MotorController::Backward(unsigned long microsSinceBoot, unsigned long howLongToRun)
{
  motorStatus = MOTOR_STATUS_BACKWARD;
  StartRunning(microsSinceBoot, BACKWARD, BACKWARD, howLongToRun);
}

void MotorController::TurnLeft(unsigned long microsSinceBoot, unsigned long howLongToRun)
{
  motorStatus = MOTOR_STATUS_TURN_LEFT;
  StartRunning(microsSinceBoot, FORWARD, BACKWARD, howLongToRun);
}

void MotorController::TurnRight(unsigned long microsSinceBoot, unsigned long howLongToRun)
{
  motorStatus = MOTOR_STATUS_TURN_RIGHT;
  StartRunning(microsSinceBoot, BACKWARD, FORWARD, howLongToRun);
}

void MotorController::Stop()
{
  motorStatus = MOTOR_STATUS_STOPPED;
  motorIsRunning = false;
  motorEndTime = 0;
  currentSpeed = 0;
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
