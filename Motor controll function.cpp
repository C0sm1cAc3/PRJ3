#include <Arduino.h>

const int pwm_values[] = {64, 128, 192, 255};
const float rpm_values_A[] = {40.5, 94.5, 114.0, 129.0};
const float rpm_values_B[] = {34.5, 82.02, 102.0, 118.5};
const float rpm_values_C[] = {36.0, 90.0, 112.5, 118.5};
const float rpm_values_D[] = {37.5, 90.0, 120.0, 136.5};


const int motor_A = 1;
const int motor_B = 2;
const int motor_C = 3;
const int motor_D = 4;
const int forwards = 1; 
const int backwards = 2; 

// Function to get PWM for a given motor and target RPM
int get_pwm_for_motor(int target_motor, float target_rpm) {
  const float* rpm_array;

  // Select correct RPM array based on motor ID
  switch(target_motor) {
    case 1: rpm_array = rpm_values_A; break;
    case 2: rpm_array = rpm_values_B; break;
    case 3: rpm_array = rpm_values_C; break;
    case 4: rpm_array = rpm_values_D; break;
    default: return 0; // invalid motor
  }

  // Interpolate between nearest RPM points
  for (int i = 0; i < 3; i++) {
    if (target_rpm >= rpm_array[i] && target_rpm <= rpm_array[i+1]) {
      float ratio = (target_rpm - rpm_array[i]) / (rpm_array[i+1] - rpm_array[i]);
      return pwm_values[i] + ratio * (pwm_values[i+1] - pwm_values[i]);
    }
  }

  // If above max RPM, return max PWM
  return pwm_values[3];
}



void motor_control(int target_motor, int direction, int power ){
    get_pwm_for_motor(target_motor, map(power, 0, 100, 0, 118.5));
}

void setup(){
    motor_control(motor_A, forwards, 100); //should send motor A forwards with 100% power
}
