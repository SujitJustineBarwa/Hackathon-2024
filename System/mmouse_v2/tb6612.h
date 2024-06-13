#include "PID.h"
/****************************************************************************************/
// Motor Initialization
// Motor A
#define AIN1 16
#define AIN2 4
#define PWMA 17

// Motor B
#define BIN1 2
#define BIN2 15
#define PWMB 0

const float correctionFactor = 2.3; // Adjust this value as needed
const int baseSpeed = 220; // Adjust the base motor speed as needed
float angle = 0.0, rawAngle = 0.0,prevAngle = 0.0;

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

void motor_left(){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  } 

void motor_right(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
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
void motor_stop(){
    /****************************************************************************************/
// Start of Motor loop
  // Motor A forward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0); // Speed 0-255

// End of Motor loop
/****************************************************************************************/
    }    


void turnRight() {
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();  // Initial sensor calibration
  mpu.update();
  
  rawAngle = mpu.getAngleZ();
  angle = abs(rawAngle);



  float targetAngle = prevAngle + 85.0; // Calculate the target angle
  
  while (angle < targetAngle) {
    mpu.update();
    rawAngle = mpu.getAngleZ();
    angle = abs(rawAngle);

    Serial.print("Angle: ");
    Serial.println(angle);

    // Calculate the correction factor
    float correction = (targetAngle - angle) * correctionFactor;
    

    motor_right();
    analogWrite(PWMA, max((int)(baseSpeed + correction),100));
    analogWrite(PWMB, max((int)(baseSpeed - correction),100));

  }
  
  //Stop();
  
  prevAngle = angle; // Update the previous angle
}


void turnLeft() {
  Wire.begin();
  mpu.begin();
  mpu.calcOffsets();  // Initial sensor calibration
  mpu.update();
  

  rawAngle = mpu.getAngleZ();
  angle = abs(rawAngle);
  
  float targetAngle = prevAngle + 85.0; // Calculate the target angle

  while (angle < targetAngle) {
    mpu.update();
    rawAngle = mpu.getAngleZ();
    angle = abs(rawAngle);

    Serial.print("Angle: ");
    Serial.println(angle);

    // Calculate the correction factor
    float correction = (targetAngle - angle) * correctionFactor;
    

    motor_left();
    analogWrite(PWMA, max((int)(baseSpeed + correction),100));
    analogWrite(PWMB, max((int)(baseSpeed - correction),100));

  }
  
  //Stop();
  
  prevAngle = angle; // Update the previous angle

}
    
