#include "PID.h"
/****************************************************************************************/
// Motor Initialization
// Motor A
#define AIN1 17
#define AIN2 5
#define PWMA 32

// Motor B
#define BIN1 18
#define BIN2 19
#define PWMB 33
// End of Motor Initialization
/****************************************************************************************/

void motor_fwd(int speed);
void motor_rev(int speed);
void motorA_fwd(int speed);
void motorA_rev(int speed);
void motorB_fwd(int speed);
void motorB_rev(int speed);

/****************************************************************************************/
// Motor Setup
  // Set all the motor control pins to outputs
void motor_setup(){
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  }

// End of Motor Setup
/****************************************************************************************/
void motorwpid_mpu(){
  //using the pid control outputs
//setting the deafault speed of the motors

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, mtrSpd); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, mtrSpd); // Speed 0-255

 
 //setting the steering command if it is veering to the right
 if(angle_pitch_output > 0){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, mtrSpd-abs(angle)); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, mtrSpd+abs(angle)); // Speed 0-255
 
 
 }
 
 else if(angle_pitch_output < 0){ //setting the steering command if it is veering to the left
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, mtrSpd+abs(angle)); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, mtrSpd-abs(angle)); // Speed 0-255

 }
error = last_error;

// End of PID loop
/****************************************************************************************/
  }
  
void motorA_fwd(int speed){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed); // Speed 0-255
  }
void motorA_rev(int speed){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, speed); // Speed 0-255
  }
void motorB_fwd(int speed){
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed); // Speed 0-255
  }
void motorB_rev(int speed){
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, speed); // Speed 0-255
  }

  
void motor_fwd(int speed){
    /****************************************************************************************/
// Start of Motor loop
  // Motor A forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed); // Speed 0-255

// End of Motor loop
/****************************************************************************************/
    }
void motor_rev(int speed){
    /****************************************************************************************/
// Start of Motor loop
  // Motor A forward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 255); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 255); // Speed 0-255

// End of Motor loop
/****************************************************************************************/
    }
