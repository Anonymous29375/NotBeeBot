#include "MotorController.h"

const unsigned int motor_speed = 200;

unsigned long loop_time;
unsigned long run_time = 400000;

MotorController motors;

void setup()
{
  motors.Initialise(motor_speed);

  // Run motors forward for 5cm
  motors.Forward(5);
}

void loop()
{
  loop_time = micros();

  motors.Update(loop_time);

  if (loop_time > run_time)
  {
    motors.Stop();
  }

  delay(1);
}
