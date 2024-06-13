/****************************************************************************************/
// MPU Initialization

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

long timer = 0;
// End of MPU Initialization
/****************************************************************************************/
void mpu_setup(){
  /****************************************************************************************/
// MPU Setup  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
// End of MPU Setup
/****************************************************************************************/
  }
