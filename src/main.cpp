#include <Arduino.h>
#include <Servo.h>
#include <AFMotor.h>
#include "HX711.h"

#define LDT 30
#define LSCK 31

AF_DCMotor mymotor(1);
Servo myservo; // create Servo object to control a servo
HX711 myscale;

const float calibration_factor = 123790;
int sdg = 0;

void setup()
{
  Serial.begin(115200); // Serial monitor
  // Serial1.begin(9600);  // Serial communication with LILYGO T-Call

  myservo.attach(10);
  myservo.write(sdg);

  mymotor.setSpeed(200);
  mymotor.run(RELEASE);

  delay(2000);
  Serial.println("Initailing...");

  myscale.begin(LDT, LSCK);
  myscale.set_scale(calibration_factor);
  myscale.tare();

  long zero_factor = myscale.read_average();
  Serial.println(zero_factor);
}

void loop()
{
  // if (Serial1.available())
  // {
  //   String data = Serial1.readStringUntil('\n');
  //   Serial.println("Received from LILYGO: " + data);
  // }

  // // If you want to send data to LILYGO
  // if (Serial.available())
  // {
  //   String data = Serial.readStringUntil('\n');
  //   Serial1.println(data);
  // }

  // mymotor.run(FORWARD);
  // for (int i = 0; i < 255; i++)
  // {
  //   mymotor.setSpeed(i);
  //   delay(10);
  // }

  if (sdg <= 0)
  {
    sdg = 80;
    myservo.write(sdg);
    delay(10000);
  }

  if (sdg <= 80)
  {
    sdg = 0;
    myservo.write(sdg);
    delay(1000);
  }

  float weight = myscale.get_units();
  if (weight > 0 && weight <= 0.6)
  {
    mymotor.run(FORWARD);
  }
  else if (weight > 1)
  {
    mymotor.run(BACKWARD);
  }

  delay(1500);
}
