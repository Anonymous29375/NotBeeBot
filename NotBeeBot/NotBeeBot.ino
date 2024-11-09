#define IR_USE_AVR_TIMER1
#define IR_SEND_PIN 8
#define USE_NO_SEND_PWM 1

#include <IRremote.hpp>

#include "DistanceController.h"
#include "MotorController.h"
#include "SoundController.h"

const unsigned long sound_beep = 300000;
const unsigned long run_15cm = 400000;
const unsigned long run_90deg = 250000;
const unsigned int motor_speed = 200;

const int irPin = 7;
const int triggerPin = 9;
const int echoPin = 10;
const int buzzerPin = 11;

const float stopDistance = 7;

unsigned long microsecondsSinceBoot;

MotorController motors;
SoundController sound;
DistanceController distance;

void setup()
{
  motors.Initialise(motor_speed);
  sound.Initialise(buzzerPin);
  distance.Initialise(triggerPin, echoPin);

  Serial.begin(9600);

  IrReceiver.begin(irPin, false);
}

void loop()
{
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume();
  }

  microsecondsSinceBoot = micros();
  motors.Update(microsecondsSinceBoot);
  sound.Update(microsecondsSinceBoot);
  float currentDistance = distance.Update(microsecondsSinceBoot);

  if (currentDistance < stopDistance)
  {
    sound.On(microsecondsSinceBoot, sound_beep);
    motors.Stop();
  }
}
