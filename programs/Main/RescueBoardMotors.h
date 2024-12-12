#pragma once

#include <Arduino.h>

class RescueBoardMotors {
  public:
    const uint8_t DIR_L = D4;
    const uint8_t PWM_L = D5;

    const uint8_t DIR_R = D16;
    const uint8_t PWM_R = D17;

    RescueBoardMotors();
    void initialize();

    // enable/disable flipping of motors
    void flipLeftMotor(boolean flip);
    void flipRightMotor(boolean flip);

    // set speed for left, right, or both motors
    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void setSpeeds(int leftSpeed, int rightSpeed);

  private:
    boolean flipLeft = false;
    boolean flipRight = false;

    void doSetSpeed(int speed, uint8_t pwm_pin, uint8_t dir_pin, bool flip);
};
