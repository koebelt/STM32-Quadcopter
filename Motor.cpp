#include <Servo.h>
#include <Arduino.h>
#include "Motor.h"

unsigned long Motor::chechBoundaries(unsigned long command)
{
    if (started == false) {
        return MOT_MINIMUM;
    }
    if (command < MOT_MINIMUM)
    {
        command = MOT_MINIMUM;
    }
    if (command > MOT_MAX)
    {
        command = MOT_MAX;
    }
    return command;
}

void Motor::start() {
    
    //this->serv.attach(this->pin, MOT_MINIMUM, MOT_MAX);
    this->ESC_command = 1150;
    this->serv.writeMicroseconds(MOT_START_MINIMUM);
    //this->serv.detach();
    this->started = true;
    
}

void Motor::stop() {
    //this->serv.attach(this->pin, MOT_MINIMUM, MOT_MAX);
    this->ESC_command = MOT_MINIMUM;
    this->serv.writeMicroseconds(MOT_MINIMUM);
    //this->serv.detach();
    this->started = false;
}

void Motor::doCommand() {
    this->serv.writeMicroseconds(ESC_command);
    /*unsigned long now = micros();
    if (now - this->timer >= this->ESC_command) {
      pinMode(this->pin, LOW);
    } else {
      pinMode(this->pin, HIGH);
    }
    if (now - this->timer >= 20000) {
      this->timer = now;
    }*/
}

void Motor::forceCommand(unsigned long command) {
  this->serv.writeMicroseconds(command);
}
