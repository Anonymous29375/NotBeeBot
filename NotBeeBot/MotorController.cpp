#include <Arduino.h>
#include "MotorController.h"

MotorController::MotorController(int m1EnablePin, int m1Output1, int m1Output2, int m2EnablePin, int m2Output1, int m2Output2)
    : m1EnablePin(m1EnablePin), m1Output1(m1Output1), m1Output2(m1Output2), m2EnablePin(m2EnablePin), m2Output1(m2Output1), m2Output2(m2Output2)
{
  // Initialise to a stopped state
  Stop();
}

void MotorController::Initialise()
{
  pinMode(m1EnablePin, OUTPUT);
  pinMode(m1Output1, OUTPUT);
  pinMode(m1Output2, OUTPUT);
  pinMode(m2EnablePin, OUTPUT);
  pinMode(m2Output1, OUTPUT);
  pinMode(m2Output2, OUTPUT);

  // Make sure initialised to a stopped status
  Stop();
}

void MotorController::Update(unsigned long microsSinceBoot)
{
  if (motorStatus == MOTOR_STATUS_STOPPED || microsSinceBoot > motorEndTime)
  {
    // Stop the motors if we have passed end time
    Stop();
    return;
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

  switch (motor1Direction)
  {
  case FORWARD:
    digitalWrite(m1Output1, LOW);
    digitalWrite(m1Output2, HIGH);
    digitalWrite(m2Output1, LOW);
    digitalWrite(m2Output2, HIGH);
    break;

  case BACKWARD:
    digitalWrite(m1Output1, HIGH);
    digitalWrite(m1Output2, LOW);
    digitalWrite(m2Output1, HIGH);
    digitalWrite(m2Output2, LOW);
    break;

  default:
    // If there is an unknown state then stop and return (fail safe)
    Stop();
    return;
  }

  // Enable motors
  digitalWrite(m1EnablePin, HIGH);
  digitalWrite(m2EnablePin, HIGH);
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

  // Disable motor drivers
  digitalWrite(m1EnablePin, LOW);
  digitalWrite(m2EnablePin, LOW);
}
