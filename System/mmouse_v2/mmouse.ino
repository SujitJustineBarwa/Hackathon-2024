
#include "vl53l0x.h"
//#include "mpu6050.h"
//#include "tb6612.h"
#include "encoder.h"
//#include "PID.h"
#include "floodfill.h"

int current_x = 0, current_y = 0, previous_x = 0, previous_y = 0, orient = FORWARD;
char Direction = 0;
bool left=false, right=false, forward=false;

/**************************Start of Setup************************************************/
void setup() {
Serial.begin(9600);
Wire.begin();

tof_setup();
motor_setup();
mpu_setup();
encoder_setup();
//sdcard_setup();
  // initialize hardware interrupts
  attachInterrupt(LH_ENCODER_A, leftEncoderEvent, CHANGE);
  attachInterrupt(LH_ENCODER_B, leftEncoderEvent, CHANGE);
  attachInterrupt(RH_ENCODER_A, rightEncoderEvent, CHANGE);
  attachInterrupt(RH_ENCODER_B, rightEncoderEvent, CHANGE);

}
/**************************End of Setup*************************************************/


/**************************Start of loop************************************************/
void loop() {
dist_meas();
//data_write();
mpu.update();

  if(millis() - timer > 1000){ // print data every second
    Serial.print(F("TEMPERATURE: "));Serial.println(mpu.getTemp());
    Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
    Serial.print("\tY: ");Serial.print(mpu.getAccY());
    Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
  
    Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
    Serial.print("\tY: ");Serial.print(mpu.getGyroY());
    Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());
  
    Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
    Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());
    
    Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
    Serial.print("\tY: ");Serial.print(mpu.getAngleY());
    Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
    Serial.println(F("=====================================================\n"));
    timer = millis();
  }
// End of MPU loop
/****************************************************************************************/
calc_pid();
motorwpid_mpu();
print_encc();

  left = wallLeft();
  if(left)
    Serial.println("Wall Left");
  right = wallRight();
  if(right)
    Serial.println("Wall Right");
  forward = wallFront();
  if(forward)
    Serial.println("Wall Front");
  updateCells(current_x, current_y, orient, left, right, forward);
  if (floodArray[current_x][current_y] != 0)
  {
    floodFillUsingQueue(current_x, current_y,previous_x,previous_y);
  }
  else
  {
    Serial.println("Reach End!!");
    while(true);
  }
  Direction = whereToMove(current_x, current_y, previous_x, previous_y, orient);
  Serial.print("WhereToMove: ");
  Serial.println(Direction);
  if (Direction == 'L')
  {
    turnLeft();
    orient = orientation(orient, 'L');
  }
  else if (Direction == 'R')
  {
    turnRight();
    orient = orientation(orient, 'R');
  }
  else if (Direction == 'B')
  {
    turnLeft();
    orient = orientation(orient, 'L');
    turnLeft();
    orient = orientation(orient, 'L');
  }

  Serial.println("moveForward");
  //moveForward();
  previous_x = current_x;
  previous_y = current_y;

  updateCoordinates(orient, &current_x, &current_y);

}
/**************************End of loop**************************************************/
