#include "BuzzerController.h"

void BuzzerController::Initialise(int buzzerPin) {
  this->buzzerPin = buzzerPin;
  pinMode(this->buzzerPin, OUTPUT);
}

void BuzzerController::On(unsigned long microsSinceStart, unsigned long duration) {
  digitalWrite(this->buzzerPin, HIGH);
  soundEndTime = microsSinceStart + duration;
  soundOn = true;
}

void BuzzerController::Off() {
  soundOn = false;
  digitalWrite(this->buzzerPin, LOW);
}

void BuzzerController::Update(unsigned long microsSinceStart) {
  if (soundOn == false || microsSinceStart > soundEndTime) {
    Off();
    return;
  }

  soundToggle = !soundToggle;
  digitalWrite(this->buzzerPin, soundToggle);
}