#include <Arduino.h>
#include <Servo.h>
#include <AFMotor.h>
#include "HX711.h"

#define LDT 30
#define LSCK 31
#define BTN 32

AF_DCMotor mymotor1(1);
AF_DCMotor mymotor2(4);
Servo myservo; // create Servo object to control a servo
HX711 myscale;

const float calibration_factor = 123790;
int sdg = 0;
float weight = 0;
unsigned fQR = 0;

void setup()
{
  Serial.begin(115200); // Serial monitor
  // Serial1.begin(9600);  // Serial communication with LILYGO T-Call

  pinMode(BTN, INPUT_PULLUP);

  myservo.attach(10);
  myservo.write(sdg);

  mymotor1.setSpeed(120);
  mymotor1.run(RELEASE);

  mymotor2.setSpeed(500);
  mymotor2.run(RELEASE);

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

  // if (sdg <= 0)
  // {
  //     sdg = 80;
  //     myservo.write(sdg);
  //     delay(1000);
  // }

  // if (sdg <= 80)
  // {
  //   sdg = 0;
  //   myservo.write(sdg);
  //   delay(1000);
  // }

  weight = myscale.get_units();
  int btnP = digitalRead(BTN);
  Serial.println(weight);
  if (btnP == 1 && weight > 0.005 && weight <= 0.2)
  {
    mymotor1.run(BACKWARD);
    mymotor2.run(RELEASE);
  }
  else if (btnP == 0 && weight > 0.0009 && weight <= 0.2)
  {
    mymotor1.run(RELEASE);
    mymotor2.run(BACKWARD);
    sdg = 80;
    myservo.write(sdg);
    delay(6000);
    // mymotor2.run(RELEASE);
    // sdg = 0;
    // myservo.write(sdg);
  }
  else if (weight > 0.2 && btnP == 1)
  {
    sdg = 80;
    myservo.write(sdg);
    mymotor1.run(RELEASE);
    mymotor2.run(FORWARD);
  }
  else // if (weight <= 0)
  {
    mymotor1.run(RELEASE);
    mymotor2.run(RELEASE);
    if (sdg == 80)
    {
      sdg = 0;
      myservo.write(sdg);
    }
  }

  delay(1500);
}
