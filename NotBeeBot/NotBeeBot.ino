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

const int MAX_PROGRAM_SEQUENCE = 6;

// Can record up to 6 movements
int programSequence[MAX_PROGRAM_SEQUENCE] = {0};

// Current number of steps in program
int programSteps = 0;

// Will be true if currently in program mode
bool inProgramMode = false;

// Will be true if program sequence if true
bool runningProgram = false;

// The next step number to run
int programStep = 0;

// Will be set to true if a program step is currently running
bool stepExecuting = false;

const int PGM_FORWARD = 1;
const int PGM_BACKWARD = 2;
const int PGM_TURN_LEFT = 3;
const int PGM_TURN_RIGHT = 4;

void PrintProgramCommand(int programCommand)
{
  switch (programCommand)
  {
  case PGM_FORWARD:
    Serial.print("PGM_FORWARD");
    break;
  case PGM_BACKWARD:
    Serial.print("PGM_BACKWARD");
    break;
  case PGM_TURN_LEFT:
    Serial.print("PGM_TURN_LEFT");
    break;
  case PGM_TURN_RIGHT:
    Serial.print("PGM_TURN_RIGHT");
    break;
  default:
    Serial.print("PGM_UNKNOWN");
    break;
  }
}

void PrintProgramSteps()
{
  Serial.print("Program steps: ");
  for (int i = 0; i < programSteps; i++)
  {
    PrintProgramCommand(programSequence[i]);
    Serial.print(", ");
  }
  Serial.println("");
}

void UpdateProgram(int command)
{
  if (programSteps >= MAX_PROGRAM_SEQUENCE)
  {
    // If there are 6 steps then don't add more
    Serial.println("Program full!");

    PrintProgramSteps();

    // Debounce key press
    delay(800);

    // Return if next code not the program button
    if (command != IR_PROGRAM)
    {
      return;
    }
  }

  switch (command)
  {
  case IR_PROGRAM:
    inProgramMode = false;
    buzzer.On(microsecondsSinceBoot, soundBeep);
    Serial.println("Exiting program mode...");
    break;

  case IR_FORWARD:
    programSequence[programSteps++] = PGM_FORWARD;
    break;

  case IR_BACKWARD:
    programSequence[programSteps++] = PGM_BACKWARD;
    break;

  case IR_LEFT:
    programSequence[programSteps++] = PGM_TURN_LEFT;
    break;

  case IR_RIGHT:
    programSequence[programSteps++] = PGM_TURN_RIGHT;
    break;
  }

  PrintProgramSteps();

  // Debounce key press
  delay(800);
}

void StopProgram()
{
  motors.Stop();
  runningProgram = false;
  programStep = 0;
  stepExecuting = false;
}

void ClearProgram()
{
  StopProgram();
  programSteps = 0;
}

void RunProgram()
{
  if (stepExecuting)
  {
    // A step is already executing
    return;
  }

  int programCommand = programSequence[programStep];

  Serial.print("Executing step: ");
  PrintProgramCommand(programCommand);
  Serial.println("");
  stepExecuting = true;

  switch (programCommand)
  {
  case PGM_FORWARD:
    motors.Forward(microsecondsSinceBoot, run15cm);
    break;
  case PGM_BACKWARD:
    motors.Backward(microsecondsSinceBoot, run15cm);
    break;
  case PGM_TURN_LEFT:
    motors.TurnLeft(microsecondsSinceBoot, run90deg);
    break;
  case PGM_TURN_RIGHT:
    motors.TurnRight(microsecondsSinceBoot, run90deg);
    break;
  }

  // Have we finished the program?
  if (programStep >= programSteps || programStep >= MAX_PROGRAM_SEQUENCE)
  {
    // We have finished the program
    StopProgram();

    Serial.println("Program sequence finished...");
    return;
  }

  // Move to next program sequence step
  programStep++;
}

void ProcessCommand(int command, bool distanceClose)
{
  switch (command)
  {
  case IR_PROGRAM:
    StopProgram();
    inProgramMode = true;
    buzzer.On(microsecondsSinceBoot, soundBeep);
    Serial.println("Entering program mode...");

    // Debounce key press
    delay(800);
    break;

  case IR_GO:
    // If there are no program steps then do nothing
    if (programSteps == 0)
    {
      Serial.println("No program found...");
    }
    else
    {
      // Now running program
      runningProgram = true;

      // Reset to start of program sequence
      programStep = 0;

      stepExecuting = false;

      Serial.println("Program go!");
    }

    // Debounce key press
    delay(800);
    break;

  case IR_CLEAR:
    StopProgram();
    programSteps = 0;

    // Debounce key press
    delay(800);
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
}

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

  if (runningProgram)
  {
    RunProgram();
  }
  else
  {

    if (IrReceiver.decode())
    {
      int command = IrReceiver.decodedIRData.command;

      if (inProgramMode)
      {
        UpdateProgram(command);
      }
      else
      {
        ProcessCommand(command, distanceClose);
      }

      IrReceiver.resume();
    }
  }

  if (!motors.Update(microsecondsSinceBoot) && runningProgram)
  {
    stepExecuting = false;
  }

  buzzer.Update(microsecondsSinceBoot);
}
