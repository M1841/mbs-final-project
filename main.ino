#include <NewPing.h>
#include <SoftwareSerial.h>

#define RECEIVER_PIN 8
#define TRANSMITTER_PIN 9

#define TRIG_PIN A4
#define ECHO_PIN A5
#define MAX_DISTANCE 200

SoftwareSerial bluetooth(RECEIVER_PIN, TRANSMITTER_PIN);
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
char lastCommand = 'S';

class Movement
{
public:
  static void setup()
  {
    pinMode(MOTOR_A, OUTPUT);
    pinMode(MOTOR_B, OUTPUT);

    pinMode(RIGHT_BACK, OUTPUT);
    pinMode(RIGHT_FRONT, OUTPUT);
    pinMode(LEFT_BACK, OUTPUT);
    pinMode(LEFT_FRONT, OUTPUT);

    digitalWrite(MOTOR_A, HIGH);
    digitalWrite(MOTOR_B, HIGH);
  }
  static void move(char command)
  {
    switch (command)
    {
    case 'F':
      front();
      break;
    case 'B':
      back();
      break;
    case 'L':
      left();
      break;
    case 'R':
      right();
      break;
    case 'S':
      stop();
      break;
    }
  }

public:
  static void front()
  {
    digitalWrite(RIGHT_FRONT, 1);
    digitalWrite(LEFT_FRONT, 1);
    digitalWrite(RIGHT_BACK, 0);
    digitalWrite(LEFT_BACK, 0);
  }
  static void back()
  {
    digitalWrite(RIGHT_FRONT, 0);
    digitalWrite(LEFT_FRONT, 0);
    digitalWrite(RIGHT_BACK, 1);
    digitalWrite(LEFT_BACK, 1);
  }
  static void left()
  {
    digitalWrite(RIGHT_FRONT, 1);
    digitalWrite(LEFT_FRONT, 0);
    digitalWrite(RIGHT_BACK, 0);
    digitalWrite(LEFT_BACK, 1);
  }
  static void right()
  {
    digitalWrite(RIGHT_FRONT, 0);
    digitalWrite(LEFT_FRONT, 1);
    digitalWrite(RIGHT_BACK, 1);
    digitalWrite(LEFT_BACK, 0);
  }
  static void stop()
  {
    digitalWrite(RIGHT_FRONT, 0);
    digitalWrite(LEFT_FRONT, 0);
    digitalWrite(RIGHT_BACK, 0);
    digitalWrite(LEFT_BACK, 0);
  }

  static const uint8_t RIGHT_BACK = 2;
  static const uint8_t RIGHT_FRONT = 3;
  static const uint8_t LEFT_FRONT = 4;
  static const uint8_t LEFT_BACK = 5;

  static const uint8_t MOTOR_A = 1;
  static const uint8_t MOTOR_B = 6;
};

void setup()
{
  Movement::setup();
  bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop()
{
  delay(5);
  Serial.println(lastCommand);
  Movement::move(lastCommand);

  char command;

  if (bluetooth.available() > 0)
  {
    command = bluetooth.read();
    if (command != 'F' && command != 'B' && command != 'L' && command != 'R' && command != 'S')
    {
      command = lastCommand;
    }
  }
  else
  {
    command = lastCommand;
  }

  if (command == 'F')
  {
    uint8_t distance = sonar.ping_cm();
    distance = distance ? distance : 255;
    if (distance < 20)
    {
      command = 'S';
    }
  }

  if (lastCommand != command)
  {
    switch (command)
    {
    case 'F':
      bluetooth.println("Going Forward");
      break;
    case 'B':
      bluetooth.println("Going Backwards");
      break;
    case 'L':
      bluetooth.println("Turning Left");
      break;
    case 'R':
      bluetooth.println("Turning Right");
      break;
    case 'S':
      bluetooth.println("Stopping");
      break;
    }
  }

  lastCommand = command;
}
