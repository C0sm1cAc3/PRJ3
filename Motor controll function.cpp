#include <Arduino.h>

const int motor_A = 1;
const int motor_B = 2;
const int motor_C = 3;
const int motor_D = 4;
const int forwards = 1; 
const int backwards = 2; 

void motor_control(int motor, int direction, int power ){

}

void setup(){
    motor_control(motor_A, forwards, 100); //should send motor A forwards with 100% power
}
