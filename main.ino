#include <NewPing.h>

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
  static void stop()
  {
    digitalWrite(RIGHT_FRONT, 0);
    digitalWrite(LEFT_FRONT, 0);
    digitalWrite(RIGHT_BACK, 0);
    digitalWrite(LEFT_BACK, 0);
  }

private:
  static const uint8_t RIGHT_BACK = 2;
  static const uint8_t RIGHT_FRONT = 3;
  static const uint8_t LEFT_FRONT = 4;
  static const uint8_t LEFT_BACK = 5;

  static const uint8_t MOTOR_A = 1;
  static const uint8_t MOTOR_B = 6;

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
};

class Bluetooth
{
public:
  static void setup()
  {
    Serial.begin(9600);
  }
  static char read()
  {
    if (Serial.available() > 0)
    {
      command = Serial.read();
    }
    return command;
  }

private:
  static char command;
};

char Bluetooth::command;

class DistanceSensor
{
public:
  static void setup()
  {
    sonar = NewPing(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
    delay(2000);
    distance = read();
    delay(100);
    distance = read();
    delay(100);
    distance = read();
    delay(100);
    distance = read();
    delay(100);
  }
  static int read()
  {
    delay(70);
    distance = sonar.ping_cm();
    return distance;
  }

private:
  static const uint8_t TRIG_PIN = A4;
  static const uint8_t ECHO_PIN = A5;
  static const uint8_t MAX_DISTANCE = 200;
  static uint8_t distance;
  static NewPing sonar;
};
uint8_t DistanceSensor::distance;
NewPing DistanceSensor::sonar(DistanceSensor::TRIG_PIN, DistanceSensor::ECHO_PIN, DistanceSensor::MAX_DISTANCE);

void setup()
{
  Movement::setup();
  Bluetooth::setup();
  DistanceSensor::setup();
}

void loop()
{
  if (DistanceSensor::read() > 20)
  {
    Movement::move(Bluetooth::read());
  }
  else
  {
    Movement::stop();
  }
}