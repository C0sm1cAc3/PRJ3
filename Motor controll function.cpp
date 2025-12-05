#include <Arduino.h>

// === Pin assignments (replace with your actual wiring) ===
int DC_Driver_A_In1 = 2;
int DC_Driver_A_In2 = 3;
int DC_Driver_A_In3 = 4;
int DC_Driver_A_In4 = 5;
int DC_Driver_B_In1 = 6;
int DC_Driver_B_In2 = 7;
int DC_Driver_B_In3 = 8;
int DC_Driver_B_In4 = 9;
int DC_Driver_A_EnA = 10; // must be PWM-capable
int DC_Driver_A_EnB = 11; // must be PWM-capable
int DC_Driver_B_EnA = 12; // must be PWM-capable
int DC_Driver_B_EnB = 13; // must be PWM-capable

// === Motor IDs ===
const int motor_A = 1;
const int motor_B = 2;
const int motor_C = 3;
const int motor_D = 4;
const int forwards = 1;
const int backwards = 2;

// === PWM test points ===
const int pwm_values[] = {64, 128, 192, 255};

// === RPM measurements per motor ===
const float rpm_values[4][4] = {
  {40.5, 94.5, 114.0, 129.0},   // Motor A
  {34.5, 82.02, 102.0, 118.5},  // Motor B
  {36.0, 90.0, 112.5, 118.5},   // Motor C
  {37.5, 90.0, 120.0, 136.5}    // Motor D
};

// Lowest max RPM across motors
const float MAX_RPM = 118.5;

// === Struct for pin mapping ===
struct MotorPins {
  int in1;
  int in2;
  int en;
};

// Array of motor pin mappings
MotorPins motors[] = {
  {DC_Driver_A_In1, DC_Driver_A_In2, DC_Driver_A_EnA}, // motor_A
  {DC_Driver_A_In3, DC_Driver_A_In4, DC_Driver_A_EnB}, // motor_B
  {DC_Driver_B_In1, DC_Driver_B_In2, DC_Driver_B_EnA}, // motor_C
  {DC_Driver_B_In3, DC_Driver_B_In4, DC_Driver_B_EnB}  // motor_D
};

// === Function: convert target RPM → PWM ===
int get_pwm_for_motor(int target_motor, float target_rpm) {
  const float* rpm_array = rpm_values[target_motor - 1];

  for (int i = 0; i < 3; i++) {
    if (target_rpm >= rpm_array[i] && target_rpm <= rpm_array[i+1]) {
      float ratio = (target_rpm - rpm_array[i]) / (rpm_array[i+1] - rpm_array[i]);
      return pwm_values[i] + ratio * (pwm_values[i+1] - pwm_values[i]);
    }
  }
  return pwm_values[3]; // max PWM if above range
}

// === Function: control motor ===
void motor_control(int target_motor, int direction, int power) {
  // Convert power (0–100%) into target RPM using float scaling
  float target_rpm = (power / 100.0) * MAX_RPM;
  int pwm_value = get_pwm_for_motor(target_motor, target_rpm);

  MotorPins m = motors[target_motor - 1];

  // Set direction
  if (direction == forwards) {
    digitalWrite(m.in1, HIGH);
    digitalWrite(m.in2, LOW);
  } else if (direction == backwards) {
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, HIGH);
  } else {
    // stop motor if invalid direction
    digitalWrite(m.in1, LOW);
    digitalWrite(m.in2, LOW);
    pwm_value = 0;
  }

  // Apply PWM
  analogWrite(m.en, pwm_value);
}

// === Function: stop motor ===
void stop_motor(int target_motor) {
  MotorPins m = motors[target_motor - 1];
  digitalWrite(m.in1, LOW);
  digitalWrite(m.in2, LOW);
  analogWrite(m.en, 0);
}

// === Setup ===
void setup() {
  // Initialize all pins
  for (int i = 0; i < 4; i++) {
    pinMode(motors[i].in1, OUTPUT);
    pinMode(motors[i].in2, OUTPUT);
    pinMode(motors[i].en, OUTPUT);
  }

  // Example: run all motors forward at 50% power
  motor_control(motor_A, forwards, 50);
  motor_control(motor_B, forwards, 50);
  motor_control(motor_C, forwards, 50);
  motor_control(motor_D, forwards, 50);
}

void loop() {
  // Add runtime control here if needed
}
