#include "DistanceController.h"

void DistanceController::Initialise(int triggerPin, int echoPin)
{
  this->triggerPin = triggerPin;
  this->echoPin = echoPin;
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  distance = 0;
}

unsigned long nextMilli = 0;

int state = 0;

float DistanceController::Update(unsigned long microsSinceStart)
{
  if (state == 0)
  {
    digitalWrite(triggerPin, LOW);
    nextMilli = microsSinceStart + 2;
    state = 1;
  }

  if (state == 1)
  {
    if (microsSinceStart > nextMilli)
    {
      digitalWrite(triggerPin, HIGH);
      nextMilli = microsSinceStart = 10;
      state = 2;
    }

    return;
  }

  if (state == 2)
  {
    if (microsSinceStart > nextMilli)
    {
      digitalWrite(triggerPin, LOW);
      float duration = pulseIn(echoPin, HIGH);
      distance = (duration * 0.0343) / 2;
      state = 0;
    }
  }

  return distance;
}