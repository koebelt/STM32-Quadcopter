#include "PID_koebelt.h"
#include "Arduino.h"

void PID::compute() {
  float error_tmp = _input - _setpoint;
  //Serial1.println(error_tmp);
  float now = millis() * 0.001;
  _integral += error_tmp;
  _integral = min(max(_integral, (float)((-1) * _outMax)), (float)_outMax); 

  float derivative = (error_tmp - _error) / (now  - _millis);

  float tmp_output = error_tmp * _kp + _integral * _ki + derivative * _kd;
  tmp_output = min(max(tmp_output, (float)((-1) * _outMax)), (float)_outMax);
  if (abs(abs(_output) - abs(tmp_output)) > 200);
  else _output = tmp_output;

  _millis = now;
  _error = error_tmp;
}
