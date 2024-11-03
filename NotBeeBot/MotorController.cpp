#include <Arduino.h>
#include "MotorController.h"

const unsigned int DIRECTION_STOPPED = 0;
const unsigned int DIRECTION_FORWARD = 1;
const unsigned int DIRECTION_BACKWARD = 2;
const unsigned int DIRECTION_TURN_LEFT = 3;
const unsigned int DIRECTION_TURN_RIGHT = 4;

const float WHEEL_CIRCUMFERENCE = 21.50;

MotorController::MotorController() : motor1(1), motor2(2)
{
  motorRunTime = 0;
  motorRunning = false;
  motorDirection = DIRECTION_STOPPED;
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

void MotorController::Update(unsigned long loop_time)
{
  if (motorDirection == DIRECTION_STOPPED)
  {
    Stop();
    return;
  }

  if (currentSpeed < motorSpeed)
  {
    currentSpeed++;
    motor1.setSpeed(currentSpeed);
    motor2.setSpeed(currentSpeed);
  }

  if (motorDirection == DIRECTION_FORWARD)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
}

void MotorController::Forward(unsigned int distance)
{
  motorDirection = DIRECTION_FORWARD;
  motorRunning = true;
  motorSpeed = 0;
  motorRunTime = 0;
  motorTargetRunTime = distance;
}

void MotorController::Backward(unsigned int distance)
{
}

void MotorController::TurnLeft(float angle)
{
}

void MotorController::TurnRight(float angle)
{
}

void MotorController::Stop()
{
  motorDirection = DIRECTION_STOPPED;
  currentSpeed = 0;
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
