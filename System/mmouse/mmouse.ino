
#include "vl53l0x.h"
#include "mpu6050.h"
//#include "tb6612.h"
#include "encoder.h"
//#include "PID.h"

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
  attachInterrupt(0, leftEncoderEvent, CHANGE);
  attachInterrupt(1, rightEncoderEvent, CHANGE);

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
}
/**************************End of loop**************************************************/
