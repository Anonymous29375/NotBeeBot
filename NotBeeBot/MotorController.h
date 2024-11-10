const unsigned int MOTOR_STATUS_STOPPED = 0;
const unsigned int MOTOR_STATUS_FORWARD = 1;
const unsigned int MOTOR_STATUS_BACKWARD = 2;
const unsigned int MOTOR_STATUS_TURN_LEFT = 3;
const unsigned int MOTOR_STATUS_TURN_RIGHT = 4;

const int FORWARD = 1;
const int BACKWARD = 2;

class MotorController
{
public:
  MotorController(int m1EnablePin, int m1Output1, int m1Output2, int m2EnablePin, int m2Output1, int m2Output2);
  void Initialise();
  bool Update(unsigned long microsSinceStart);

  void Forward(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void Backward(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void TurnLeft(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void TurnRight(unsigned long microsSinceBoot, unsigned long howLongToRun);
  void Stop();

  bool IsRunning();

private:
  int m1EnablePin; 
  int m1Output1;
  int m1Output2;
  int m2EnablePin;
  int m2Output1;
  int m2Output2;

  // How long the motor has been running for in ticks
  unsigned long motorEndTime;
  unsigned int motorStatus;
  byte currentSpeed;

  // This will be set to true if the motor is currently running
  bool motorIsRunning;

  void StartRunning(unsigned long startTime, unsigned int motor1Direction, unsigned int motor2Direction, unsigned long howLongToRun);
};
