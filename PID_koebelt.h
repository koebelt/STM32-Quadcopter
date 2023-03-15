#ifndef PID_KOEBELT_H
#define PID_KOEBELT_H

class PID {
  public:

  PID(float kp, float ki, float kd, float outMax){_kp = kp; _ki = ki; _kd = kd; _outMax = outMax;};

  void compute();
  void setInput(float input){_input = input;};
  void setSetpoint(float setpoint){_setpoint = setpoint;};
  float getOutput(){return _output;};
  void resetIntegral() {_integral = 0.0;};

  private:
  float _error;
  float _millis;
  float _setpoint, _input, _output;
  float _kp, _ki, _kd;
  float _outMax;
  float _integral;
};


#endif
