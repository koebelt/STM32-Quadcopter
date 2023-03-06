#include "MPU6050_koebelt.h"
#include <Wire.h>
#include "PID_koebelt.h"
#include "Motor.h"
#include "Radio_koebelt.h"


MPU6050 mpu6050(Wire);
PID yawPID(1, 0, 0, 400);
PID pitchPID(1, 0, 0, 400);
PID rollPID(1, 0, 0, 400);
Motor *mA;
Motor *mB;
Motor *mC;
Motor *mD;
unsigned long throttle = MOT_START_MINIMUM;
bool started = false;

#define pinCE   PA8             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  PB10             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
Radio radio;

void setup() {
  Serial1.begin(115200);
  mA = new Motor(PB12);
  mB = new Motor(PB13);
  mC = new Motor(PB14);
  mD = new Motor(PB15);
  Wire.begin();
  Wire.setClock(400000);
  mpu6050.begin();
  mpu6050.calcGyroOffsets();

  radio = Radio(pinCE, pinCSN);
  delay(3000);
  mA->setCommand(1000);
  mB->setCommand(1000);
  mC->setCommand(1000);
  mD->setCommand(1000);
    delay(2000);

  mA->setCommand(1200);
  mA->doCommand();
  delay(1000);
  mA->setCommand(1000);
  mA->doCommand();
  mB->setCommand(1200);
  mB->doCommand();
  delay(1000);
  mB->setCommand(1000);
  mB->doCommand();
  mC->setCommand(1200);
  mC->doCommand();
  delay(1000);
  mC->setCommand(1000);
  mC->doCommand();
  mD->setCommand(1200);
  mD->doCommand();
  delay(1000);
  mD->setCommand(1000);
  mD->doCommand();
  
  
}

// getRawGyroX = ACC Angulaire
// getGyroX = ACC ANGULAIRE stable
// getAccAngleX = ANGLE
// getGyroAngleX = ANGLE stable
// getAngleX = Angle plus stable

void loop() {
  radio.listener();

  mpu6050.update();

  yawPID.setInput(mpu6050.getAngleY());
  yawPID.setSetpoint(0);
  pitchPID.setInput(mpu6050.getAngleZ());
  pitchPID.setSetpoint(0);
  rollPID.setInput(mpu6050.getAngleX());
  rollPID.setSetpoint(0);
  
  //rollPID.setSetpoint(mpu6050.getGyroX());
  yawPID.compute();
  pitchPID.compute();
  rollPID.compute();

  if (radio._remoteData.button1 == 1) {
    if (!started) {
      mA->start();
      mB->start();
      mC->start();
      mD->start();
    } else {
      mA->stop();
      mB->stop();
      mC->stop();
      mD->stop();
    }
  }
  //Serial1.print("\tangle : ");
  //Serial1.print(mpu6050.getAngleX());
  //Serial1.print("\tPID output : ");
  //Serial1.println(rollPID.getOutput());
  /*Serial1.print("lX: ");
  Serial1.print(radio._remoteData.ljoystick.x);
  Serial1.print(" lY: ");
  Serial1.print(radio._remoteData.ljoystick.y);
  Serial1.print(" rX: ");
  Serial1.print(radio._remoteData.rjoystick.x);
  Serial1.print(" rY: ");
  Serial1.print(radio._remoteData.rjoystick.y);
  Serial1.print(" b1: ");
  Serial1.print(radio._remoteData.button1);
  Serial1.print(" b2: ");
  Serial1.print(radio._remoteData.button2);
  Serial1.print(" b3: ");
  Serial1.print(radio._remoteData.button3);
  Serial1.print(" b4: ");
  Serial1.println(radio._remoteData.button4);*/
  Serial1.print("roll : ");
  Serial1.print(mpu6050.getAngleX());
  Serial1.print("\tpitch : ");
  Serial1.print(mpu6050.getAngleY());
  Serial1.print("\tyaw : ");
  Serial1.println(mpu6050.getAngleZ());
  

  if (started) {
    mA->setCommand((throttle + rollPID.getOutput() + pitchPID.getOutput() + yawPID.getOutput()));
    mB->setCommand((throttle - rollPID.getOutput() + pitchPID.getOutput() + yawPID.getOutput()));
    mC->setCommand((throttle + rollPID.getOutput() + pitchPID.getOutput() + yawPID.getOutput()));
    mD->setCommand((throttle - rollPID.getOutput() + pitchPID.getOutput() + yawPID.getOutput()));
  }
  mA->doCommand();
  mB->doCommand();
  mC->doCommand();
  mD->doCommand();
  
  //radio.send("message");
}
