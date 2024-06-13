#include "tb6612.h"

#define RH_ENCODER_A 34 
#define RH_ENCODER_B 35
#define LH_ENCODER_A 18
#define LH_ENCODER_B 5

// variables to store the number of encoder pulses
// for each motor
int leftPower = 150, rightPower = leftPower;
volatile long leftCount = 0;
volatile long rightCount = 0;
long prevLeftCount = 0;
long prevRightCount = 0;

volatile int lastEncodedleft = 0; // Here updated value of encoder store.
volatile int lastEncodedright = 0; // Here updated value of encoder store.

void encoder_setup() {
  pinMode(LH_ENCODER_A, INPUT_PULLUP);
  pinMode(LH_ENCODER_B, INPUT_PULLUP);
  pinMode(RH_ENCODER_A, INPUT_PULLUP);
  pinMode(RH_ENCODER_B, INPUT_PULLUP);

  digitalWrite(LH_ENCODER_A, HIGH); //turn pullup resistor on
  digitalWrite(LH_ENCODER_B, HIGH); //turn pullup resistor on
  digitalWrite(RH_ENCODER_A, HIGH); //turn pullup resistor on
  digitalWrite(RH_ENCODER_B, HIGH); //turn pullup resistor on
  
}
//// encoder event for the interrupt call
//void leftEncoderEvent() {
//  if (digitalRead(LH_ENCODER_A) == HIGH) {
//    if (digitalRead(LH_ENCODER_B) == LOW) {
//      leftCount++;
//    } else {
//      leftCount--;
//    }
//  } else {
//    if (digitalRead(LH_ENCODER_B) == LOW) {
//      leftCount--;
//    } else {
//      leftCount++;
//    }
//  }
//}
//
//// encoder event for the interrupt call
//void rightEncoderEvent() {
//  if (digitalRead(RH_ENCODER_A) == HIGH) {
//    if (digitalRead(RH_ENCODER_B) == LOW) {
//      rightCount++;
//    } else {
//      rightCount--;
//    }
//  } else {
//    if (digitalRead(RH_ENCODER_B) == LOW) {
//      rightCount--;
//    } else {
//      rightCount++;
//    }
//  }
//}


void leftEncoderEvent(){
  int MSB = digitalRead(LH_ENCODER_A); //MSB = most significant bit
  int LSB = digitalRead(LH_ENCODER_B); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncodedleft << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) leftCount --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) leftCount ++;

  lastEncodedleft = encoded; //store this value for next time

}

void rightEncoderEvent(){
  int MSB = digitalRead(RH_ENCODER_A); //MSB = most significant bit
  int LSB = digitalRead(RH_ENCODER_B); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncodedright << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) rightCount --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) rightCount ++;

  lastEncodedright = encoded; //store this value for next time

}

void driveStraight() {

  // use wheel encoders to drive straight continuously

  // amount to offset motor powers to drive straight
  int offset = 5;
  
  // calculate increase in count from previous reading
  long leftDiff = leftCount - prevLeftCount;
  long rightDiff = rightCount - prevRightCount;

  // store current counts as "previous" counts for next reading
  prevLeftCount = leftCount;
  prevRightCount = rightCount;

  // adjust left & right motor powers to keep counts similar (drive straight)
  // if left rotated more than right, slow down left & speed up right
  if (leftDiff > rightDiff) {
    leftPower = leftPower - offset;
    rightPower = rightPower + offset;
  }
  // if right rotated more than left, speed up left & slow down right
  else if (leftDiff < rightDiff) {
    leftPower = leftPower + offset;
    rightPower = rightPower - offset;
  }

  // apply adjusted motor powers
  motorA_fwd(leftPower);
  motorB_fwd(rightPower);
  delay(10);  // short delay before next reading
}

void print_encc(){
  Serial.print("Right Count: ");
  Serial.println(rightCount);
  Serial.print("Left Count: ");
  Serial.println(leftCount);
  Serial.println();
  delay(500);
  }
