#include "PID_koebelt.h"
#include "Arduino.h"

void PID::compute() {
  float error_tmp = _input - _setpoint;
  //Serial1.println(error_tmp);
  //float now = millis() * 0.001;
  _integral += error_tmp * _ki;
  _integral = min(max(_integral, (float)((-1) * _outMax)), (float)_outMax); 

  float derivative = (error_tmp - _error);

  _output = error_tmp * _kp + _integral + derivative * _kd;
  _output = min(max(_output, (float)((-1) * _outMax)), (float)_outMax);

  _error = error_tmp;
}
