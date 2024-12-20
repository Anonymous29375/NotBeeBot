#include <Arduino.h>

class BuzzerController
{
public:
  void Initialise(int buzzerPin);
  void On(unsigned long microsSinceStart, unsigned long duration);
  void Off();
  void Update(unsigned long microsSinceStart);

private:
  int buzzerPin;
  unsigned long soundEndTime;
  bool soundOn;
  bool soundToggle;
};