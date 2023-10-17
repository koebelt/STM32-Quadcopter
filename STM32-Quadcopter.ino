#include "MPU6050_koebelt.h"
#include <Wire.h>
#include "PID_koebelt.h"
#include "Motor.h"
#include "Radio_koebelt.h"


MPU6050 mpu6050(Wire);
float yaw, pitch, roll = 0;
//PID yawPID(0, 0.00, 0, 200);
PID yawPID(0.7, 0, 0, 400);
PID pitchPID(0.6, 0.00, 25, 400);

// PID pitchPID(0.6, 0.00, 25, 400); New best yet
// pitch = pitch * 0.995 + mpu6050.getGyroY() * 0.005; // new best yet

//PID pitchPID(0.6, 0.00, 12.5, 400); // OLD best yet
//pitch = pitch * 0.99 + mpu6050.getGyroY() * 0.01; OLD BEST YET


PID rollPID(0.6, 0.00, 25, 400);

PID throttlePID(0.3, 0.004, 0, 400);
Motor *mA;
Motor *mB;
Motor *mC;
Motor *mD;
unsigned long throttle = MOT_START_MINIMUM;
bool started = false;

#define pinCE   PA8             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  PB10             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
Radio radio;

#define GREEN_LED PB3
#define RED_LED PB4
#define BATTERY_INPUT PA4
float batteryVoltage = 0.0;
uint32_t loop_timer;


void setup() {
  Serial1.begin(115200);
  mA = new Motor(PB14);
  mB = new Motor(PB12);
  mC = new Motor(PB15);
  mD = new Motor(PB13);
  delay(1000);
  Wire.begin();
  Wire.setClock(400000);
  delay(500);
  mpu6050.begin();
  mpu6050.calcGyroOffsets();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BATTERY_INPUT, INPUT);


  radio = Radio(pinCE, pinCSN);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  delay(3000);
  mA->setCommand(1000);
  mB->setCommand(1000);
  mC->setCommand(1000);
  mD->setCommand(1000);
  delay(2000);

  /*mA->forceCommand(1200);
  delay(1000);
  mA->forceCommand(1000);
  mB->forceCommand(1200);
  delay(1000);
  mB->forceCommand(1000);
  mC->forceCommand(1200);
  delay(1000);
  mC->forceCommand(1000);
  mD->forceCommand(1200);
  delay(1000);
  mD->forceCommand(1000);*/
  
}

// getRawGyroX = ACC Angulaire
// getGyroX = ACC ANGULAIRE stable
// getAccAngleX = ANGLE
// getGyroAngleX = ANGLE stable
// getAngleX = Angle plus stable

void loop() {
  batteryVoltage = ((float)analogRead(BATTERY_INPUT) / 28.6);
  if (batteryVoltage < 10.0) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  //Serial1.print(batteryVoltage);
  //Serial1.print("\t");
  //Serial1.println((float)analogRead(BATTERY_INPUT));
  
  radio.listener();

  mpu6050.update();
  yaw = yaw * 0.995 + mpu6050.getGyroZ() * 0.005;
  pitch = pitch * 0.995 + mpu6050.getGyroY() * 0.005;
  //pitch = pitch * 0.95 - mpu6050.getAccX() * 0.2;
  roll = roll * 0.995 + mpu6050.getGyroX() * 0.005;
  //roll = roll * 0.95 - mpu6050.getAccY() * 0.2;

  yawPID.setInput(yaw);
  yawPID.setSetpoint(radio._remoteData.rjoystick.y / 5);
  
  //pitchPID.setInput(mpu6050.getAngleY());
  pitchPID.setInput(pitch);
  pitchPID.setSetpoint(radio._remoteData.rjoystick.y / 3);
  
  //rollPID.setInput(mpu6050.getAngleX());
  rollPID.setInput(roll);
  rollPID.setSetpoint(radio._remoteData.rjoystick.x / 3);
  
  throttlePID.setInput(0);
  throttlePID.setSetpoint(radio._remoteData.ljoystick.y);
  
  yawPID.compute();
  pitchPID.compute();
  rollPID.compute();
  throttlePID.compute();

  if (radio._remoteData.button1 == 1) {
      started = true;
      mA->start();
      mB->start();
      mC->start();
      mD->start();
      yawPID.resetIntegral();
      rollPID.resetIntegral();
      pitchPID.resetIntegral();
      throttlePID.resetIntegral();
  }
  if (radio._remoteData.button2 == 1) {
      started = false;
      mA->stop();
      mB->stop();
      mC->stop();
      mD->stop();
      mA->setCommand(1000);
      mB->setCommand(1000);
      mC->setCommand(1000);
      mD->setCommand(1000);
  }
  /*Serial1.print("angle : ");
  Serial1.print(batteryVoltage);
  Serial1.print("\tangle1 : ");
  Serial1.println(1/ batteryVoltage);
  /*Serial1.print("\tyawPID : ");
  Serial1.print(yawPID.getOutput());*/
  /*Serial1.print("pitchPID : ");
  Serial1.print(pitchPID.getOutput());
  Serial1.print(" \trollPID : ");
  Serial1.println(rollPID.getOutput());
  /*Serial1.print(" throttlePID : ");
  Serial1.println(throttlePID.getOutput());
  /*Serial1.print("lX: ");
  Serial1.print(radio._remoteData.ljoystick.x);
  Serial1.print(" lY: ");
  Serial1.print(radio._remoteData.ljoystick.y);
  Serial1.print(" rX: ");
  Serial1.print(radio._remoteData.rjoystick.x);
  Serial1.print(" rY: ");
  Serial1.print(radio._remoteData.rjoystick.y);
  Serial1.print(" output: ");
  Serial1.println(throttlePID.getOutput());
  /*Serial1.print(" b1: ");
  Serial1.print(radio._remoteData.button1);
  Serial1.print(" b2: ");
  Serial1.print(radio._remoteData.button2);
  Serial1.print(" b3: ");
  Serial1.print(radio._remoteData.button3);
  Serial1.print(" b4: ");
  Serial1.println(radio._remoteData.button4);*/
  /*Serial1.print("roll : ");
  Serial1.print(mpu6050.getAccX());
  Serial1.print("\tpitch : ");
  Serial1.print(mpu6050.getAccY());
  Serial1.print("\tyaw : ");
  Serial1.println(mpu6050.getAccZ() - 1);*/
  

  if (started) {
    mA->setCommand((throttle - throttlePID.getOutput() - rollPID.getOutput() + pitchPID.getOutput() - yawPID.getOutput()) * 0.94);
    mB->setCommand((throttle - throttlePID.getOutput() + rollPID.getOutput() + pitchPID.getOutput() + yawPID.getOutput()) * 0.94);
    mC->setCommand((throttle - throttlePID.getOutput() - rollPID.getOutput() - pitchPID.getOutput() + yawPID.getOutput()) * 1.075 * (0.92 + (1 / batteryVoltage)));
    mD->setCommand((throttle - throttlePID.getOutput() + rollPID.getOutput() - pitchPID.getOutput() - yawPID.getOutput()) * 1.085 * (0.92 + (1 / batteryVoltage)));
  }
  mA->doCommand();
  mB->doCommand();
  mC->doCommand();
  mD->doCommand();
  /*Serial1.print("\tmotA : ");
  Serial1.print(mA->getCommand());
  Serial1.print("\tmotB : ");
  Serial1.print(mB->getCommand());
  Serial1.print("\tmotC : ");
  Serial1.print(mC->getCommand());
  Serial1.print("\tmotD : ");
  Serial1.println(mD->getCommand());
  
  //radio.send("message");*/

  while (micros() - loop_timer < 4000);  //We wait until 4000us are passed.
  loop_timer = micros();  
}
