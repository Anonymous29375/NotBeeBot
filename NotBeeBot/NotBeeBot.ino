#define IR_USE_AVR_TIMER1
#define IR_SEND_PIN 8
#define USE_NO_SEND_PWM 1

#include <IRremote.hpp>

#include "DistanceController.h"
#include "MotorController.h"
#include "BuzzerController.h"
#include "IRCodes.h"

const unsigned long soundBeep = 300000;
const unsigned long run15cm = 400000;
const unsigned long run90deg = 250000;

const int irPin = 7;
const int triggerPin = 9;
const int echoPin = 10;
const int buzzerPin = 11;

const int m1EnablePin = 4;
const int m1Output1Pin = 3;
const int m1Output2Pin = 2;

const int m2EnablePin = 5;
const int m2Output1Pin = 12;
const int m2Output2Pin = 6;

const float stopDistance = 7;

unsigned long microsecondsSinceBoot;

MotorController motors(m1EnablePin, m1Output1Pin, m1Output2Pin, m2EnablePin, m2Output1Pin, m2Output2Pin);
BuzzerController buzzer;
DistanceController distance;

void setup()
{
  motors.Initialise();
  buzzer.Initialise(buzzerPin);
  distance.Initialise(triggerPin, echoPin);

  Serial.begin(9600);

  IrReceiver.begin(irPin, false);
}

void loop()
{
  microsecondsSinceBoot = micros();

  float currentDistance = distance.Update(microsecondsSinceBoot);
  bool distanceClose = currentDistance < stopDistance;

  if (distanceClose)
  {
    motors.Stop();
  }

  if (IrReceiver.decode())
  {
    switch (IrReceiver.decodedIRData.command)
    {
    case IR_PROGRAM:
      motors.Stop();
      buzzer.On(microsecondsSinceBoot, soundBeep);
      break;

    case IR_FORWARD:
      if (distanceClose)
      {
        buzzer.On(microsecondsSinceBoot, soundBeep);
      }
      else
      {
        motors.Forward(microsecondsSinceBoot, run15cm);
      }
      break;

    case IR_BACKWARD:
      motors.Backward(microsecondsSinceBoot, run15cm);
      break;

    case IR_LEFT:
      if (distanceClose)
      {
        buzzer.On(microsecondsSinceBoot, soundBeep);
      }
      else
      {
        motors.TurnLeft(microsecondsSinceBoot, run90deg);
      }
      break;

    case IR_RIGHT:
      if (distanceClose)
      {
        buzzer.On(microsecondsSinceBoot, soundBeep);
      }
      else
      {
        motors.TurnRight(microsecondsSinceBoot, run90deg);
      }
      break;
    }
    IrReceiver.resume();
  }

  motors.Update(microsecondsSinceBoot);
  buzzer.Update(microsecondsSinceBoot);
}
