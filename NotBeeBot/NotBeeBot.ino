#define IR_USE_AVR_TIMER1
#define IR_SEND_PIN 8
#define USE_NO_SEND_PWM 1

#include <IRremote.hpp>

#include "DistanceController.h"
#include "MotorController.h"
#include "BuzzerController.h"

const unsigned long sound_beep = 300000;
const unsigned long run_15cm = 400000;
const unsigned long run_90deg = 250000;

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
BuzzerController sound;
DistanceController distance;

void setup() {
  motors.Initialise();
  sound.Initialise(buzzerPin);
  distance.Initialise(triggerPin, echoPin);

  Serial.begin(9600);

  IrReceiver.begin(irPin, false);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume();
  }

  microsecondsSinceBoot = micros();
  motors.Update(microsecondsSinceBoot);
  sound.Update(microsecondsSinceBoot);
  float currentDistance = distance.Update(microsecondsSinceBoot);

  bool distanceClose = currentDistance < stopDistance;

  if (distanceClose) {
    sound.On(microsecondsSinceBoot, sound_beep);
    motors.Backward(micros(), 4000000000);
  } else {
    motors.Stop();
  }
}
