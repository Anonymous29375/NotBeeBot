#include <Arduino.h>

class DistanceController
{
public:
  void Initialise(int triggerPin, int echoPin);
  float Update(unsigned long microsSinceStart);

private:
  int triggerPin;
  int echoPin;
  float distance;
};