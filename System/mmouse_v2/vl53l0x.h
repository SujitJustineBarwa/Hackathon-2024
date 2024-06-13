#include "Adafruit_VL53L0X.h"

/****************************************************************************************/
// TOF Sensor Initialization

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
//#define LOX4_ADDRESS 0x33

#define SHT_LOX1 32
#define SHT_LOX2 33
#define SHT_LOX3 25
//#define SHT_LOX4 26

#define CELL 315.0 //in mm

uint16_t s1, s2, s3;

/***************************************************************************************/

/**********************************************************************************/

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;

/**
 * @brief Set ID for each sensor
 */
void setID() {

    unsigned long startTime;
    unsigned long endTime;

    // all reset
    digitalWrite(SHT_LOX1, LOW);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    //digitalWrite(SHT_LOX4, LOW);
    //delay(10);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    digitalWrite(SHT_LOX3, HIGH);
    //digitalWrite(SHT_LOX4, HIGH);
    //delay(10);

    // activating LOX1 and resetting LOX2
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    //digitalWrite(SHT_LOX4, LOW);

    // initing LOX1
    if(!lox1.begin(LOX1_ADDRESS)) {
        Serial.println("Failed to boot first VL53L0X sensor");
        while(1);
    }
    //delay(10);

    // activating LOX2
    digitalWrite(SHT_LOX2, HIGH);
    //delay(10);
    if(!lox2.begin(LOX2_ADDRESS)) {
        Serial.println("Failed to boot second sensor");
        while(1);
    }

    // activating LOX3
    digitalWrite(SHT_LOX3, HIGH);
    //delay(10);
    if(!lox3.begin(LOX3_ADDRESS)) {
        Serial.println("Failed to boot third sensor");
        while(1);
    }
    
    // activating LOX4
//    digitalWrite(SHT_LOX4, HIGH);
//    //delay(10);
//    if(!laser.begin(LOX4_ADDRESS)) {
//        Serial.println("Failed to boot fourth sensor");
//        while(1);
//    }
}
// End of TOF Sensor Initialization
/****************************************************************************************/

void tof_setup(){
  /****************************************************************************************/
// TOF Setup
    unsigned long startTime;
    unsigned long endTime;

    startTime = millis();
    while (! Serial) delay(1);  

    pinMode(SHT_LOX1, OUTPUT);
    pinMode(SHT_LOX2, OUTPUT);
    pinMode(SHT_LOX3, OUTPUT);
   // pinMode(SHT_LOX4, OUTPUT);

    Serial.println("Starting...");

    setID();
    endTime = millis();

    Serial.print("Time taken for initialization : ");
    Serial.print(endTime - startTime);
    Serial.println(" ms");
// End of TOF Setup
/****************************************************************************************/
  }

/****************************************************************************************/
// TOF Distance Measurement
void dist_meas(){

    lox1.pMyDevice->I2cDevAddr = LOX1_ADDRESS & 0x7F;
    delay(10);
    lox1.rangingTest(&measure1, false);
    s1 = measure1.RangeMilliMeter;

    lox2.pMyDevice->I2cDevAddr = LOX2_ADDRESS & 0x7F;
    delay(10);
    lox2.rangingTest(&measure2, false);
    s2 = measure2.RangeMilliMeter;

    lox3.pMyDevice->I2cDevAddr = LOX3_ADDRESS & 0x7F;
    delay(10);
    lox3.rangingTest(&measure3, false);
    s3 = measure3.RangeMilliMeter;

//    laser.pMyDevice->I2cDevAddr = LOX4_ADDRESS & 0x7F;
//    delay(10);
//    laser.rangingTest(&measure, false);
//    s4 = measure.RangeMilliMeter;
    
    Serial.print("1: ");
    Serial.print(s1);
    Serial.print(" mm \t");

    Serial.print("2: ");
    Serial.print(s2);
    Serial.print(" mm \t");

    Serial.print("3: ");
    Serial.print(s3);
    Serial.println(" mm");

//    Serial.print("4: ");
//    Serial.print(s4);
//    Serial.println(" mm");
    
    Serial.println();
    delay(10);

  }


bool wallLeft()
{
  int readings = 0;
  for(int i = 1; i--;)
  {
    lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    if (measure1.RangeStatus != 4)
      readings += measure1.RangeMilliMeter < CELL;
  }
  return readings > 0;
}

bool wallRight()
{
  int readings = 0;
  for(int i = 1; i--;)
  {
    lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
    if (measure2.RangeStatus != 4)
      readings += measure2.RangeMilliMeter < CELL;
  }
  return readings > 0;
}

bool wallFront()
{
  int readings = 0;
  for(int i = 1; i--;)
  {
    lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
    if (measure3.RangeStatus != 4)
      readings += measure3.RangeMilliMeter < CELL;
  }
  return readings > 0;
}
