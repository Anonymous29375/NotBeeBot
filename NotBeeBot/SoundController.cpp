#include "SoundController.h"

void SoundController::Initialise(int buzzerPin)
{
  this->buzzerPin = buzzerPin;
  pinMode(this->buzzerPin, OUTPUT);
}

void SoundController::On(unsigned long microsSinceStart, unsigned long duration)
{
  digitalWrite(this->buzzerPin, HIGH);
  soundEndTime = microsSinceStart + duration;
  soundOn = true;
}

void SoundController::Update(unsigned long microsSinceStart)
{
  if (soundOn == false || microsSinceStart > soundEndTime)
  {
    digitalWrite(this->buzzerPin, LOW);
    soundOn = false;
    return;
  }

  soundToggle = !soundToggle;
  digitalWrite(this->buzzerPin, soundToggle);
}