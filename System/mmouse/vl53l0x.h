#include "Adafruit_VL53L0X.h"

/****************************************************************************************/
// TOF Sensor Initialization

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

#define SHT_LOX1 25
#define SHT_LOX2 26
#define SHT_LOX3 27

uint16_t s1, s2, s3;

/***************************************************************************************/

/**********************************************************************************/

Adafruit_VL53L0X laser = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;

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
    //delay(10);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    digitalWrite(SHT_LOX3, HIGH);
    //delay(10);

    // activating LOX1 and resetting LOX2
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);

    // initing LOX1
    if(!laser.begin(LOX1_ADDRESS)) {
        Serial.println("Failed to boot first VL53L0X sensor");
        while(1);
    }
    //delay(10);

    // activating LOX2
    digitalWrite(SHT_LOX2, HIGH);
    //delay(10);
    if(!laser.begin(LOX2_ADDRESS)) {
        Serial.println("Failed to boot second sensor");
        while(1);
    }

    // activating LOX3
    digitalWrite(SHT_LOX3, HIGH);
    //delay(10);
    if(!laser.begin(LOX3_ADDRESS)) {
        Serial.println("Failed to boot third sensor");
        while(1);
    }
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
    // B1. Định địa chỉ
    laser.pMyDevice->I2cDevAddr = LOX1_ADDRESS & 0x7F;
    delay(10);
    // B2. Đọc giá trị
    laser.rangingTest(&measure, false);
    // B3. Lưu vào s1
    s1 = measure.RangeMilliMeter;

    laser.pMyDevice->I2cDevAddr = LOX2_ADDRESS & 0x7F;
    delay(10);
    laser.rangingTest(&measure, false);
    s2 = measure.RangeMilliMeter;

    laser.pMyDevice->I2cDevAddr = LOX3_ADDRESS & 0x7F;
    delay(10);
    laser.rangingTest(&measure, false);
    s3 = measure.RangeMilliMeter;
    
    Serial.print("1: ");
    Serial.print(s1);
    Serial.print(" mm \t");

    Serial.print("2: ");
    Serial.print(s2);
    Serial.print(" mm \t");

    Serial.print("3: ");
    Serial.print(s3);
    Serial.println(" mm");
    
    Serial.println();
    delay(10);

  }
