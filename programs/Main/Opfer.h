void opfer() {
    ReadDirection();
    currentdirection = direction;
    right();
    while (direction != abs(currentdirection-180)) {
        ReadDirection();
    }
    straight();
    // straight();
    // motors.flipLeftMotor(true);
    // motors.flipRightMotor(false);
    // delay(1000);
    // stop();
    // right();
    // delay(2000);
    // straight();
    // delay(5000);
    // while (calculateReflection() == "noLine") {
    //     stop();
    //     motors.flipLeftMotor(true);
    //     motors.flipRightMotor(false);
    //     motors.setSpeeds(50, 60);
    //     delay(1000);
    //     left();
    //     delay(2000);
    //     straight();
    //     delay(1000);
    //     right();
    //     delay(2000);
    //     straight();
    //     delay(5000);
    // }
}