#ifndef MOTOR
#define MOTOR

#include <Servo.h>
#include <Arduino.h>

#define MOT_START_MINIMUM 1100 // µs nécessaire pour décoller et avoir correction d'assiette possible
#define MOT_MINIMUM 1000 // µs min pour l'ESC
#define MOT_MAX 2000

class Motor
{
    public:
        Motor(int pin) { this->pin = pin;
            this->serv.attach(this->pin, MOT_MINIMUM, MOT_MAX);
            this->serv.writeMicroseconds(MOT_MINIMUM);
            //this->serv.detach();
        };
        Motor() = default;
        //Motor(bool a) {};
        ~Motor() {this->serv.detach();};

        void start();
        void stop();
        void doCommand();
        void forceCommand(unsigned long command);
        void setOffsetCoeff(float offsetCoefff) { this->offsetCoeff = offsetCoefff; };
        void setCommand(unsigned long command) { ESC_command = chechBoundaries(command * offsetCoeff); };
        unsigned long getCommand() { return ESC_command;};
    private:
        int pin;
        unsigned long timer = 0;
        unsigned long ESC_command = MOT_MINIMUM;
        bool started = false;
        float offsetCoeff = 1;
        Servo serv;
        unsigned long chechBoundaries(unsigned long command);
};

#endif
