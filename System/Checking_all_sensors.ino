/****************************************************************************************/
// TOF Sensor Initialization
#include "Adafruit_VL53L0X.h"

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

#define SHT_LOX1 25
#define SHT_LOX2 26
#define SHT_LOX3 27


/***************************************************************************************/
//PID variables
//variables for PID control
float target = 0;
float error = 0;
float integral = 0;
float derivative = 0;
float last_error = 0;

//the 'k' values are the ones you need to fine tune before your program will work. Note that these are arbitrary values that you just need to experiment with one at a time.
float Kp = 11;
float Ki = 0.09;
float Kd = 10;

int mtrSpd = 127.5;

float angle = 0;

//Variables for Gyroscope
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles;

long acc_x, acc_y, acc_z, acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
float angle_pitch_output, angle_roll_output;

// Setup timers and temp variables
long loop_timer;
int temp;
int state = 0;

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


/****************************************************************************************/
// MPU Initialization

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

long timer = 0;
// End of MPU Initialization
/****************************************************************************************/



/****************************************************************************************/
// Motor Initialization
// Motor A
const int AIN1 = 17;
const int AIN2 = 5;
const int PWMA = 32;

// Motor B
const int BIN1 = 18;
const int BIN2 = 19;
const int PWMB = 33;
// End of Motor Initialization
/****************************************************************************************/



/**************************Start of Setup************************************************/
void setup() {

/****************************************************************************************/
// TOF Setup
    unsigned long startTime;
    unsigned long endTime;

    startTime = millis();
    Serial.begin(9600);
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


/****************************************************************************************/
// MPU Setup
  Serial.begin(9600);
  Wire.begin();
  
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



  for (int cal_int = 0; cal_int < 1000 ; cal_int ++){                  
    //Add the gyro x offset to the gyro_x_cal variable                                            
    gyro_x_cal += mpu.getGyroX();
    //Add the gyro y offset to the gyro_y_cal variable                                              
    gyro_y_cal += mpu.getGyroY(); 
    //Add the gyro z offset to the gyro_z_cal variable                                             
    gyro_z_cal += mpu.getGyroZ(); 
    //Delay 3us to have 250Hz for-loop                                             
    delay(3);                                            
}

  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;

// End of PID Setup
/****************************************************************************************/

/****************************************************************************************/
// Motor Setup
  // Set all the motor control pins to outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

// End of Motor Setup
/****************************************************************************************/
}
/**************************End of Setup*************************************************/


/**************************Start of loop************************************************/
void loop() {

/****************************************************************************************/
// TOF loop
      uint16_t s1, s2, s3;
    
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


    /* ====== Main code here ====== */
    
    // In giá trị cảm biến 1
    Serial.print("1: ");
    Serial.print(s1);
    Serial.print(" mm \t");

    // In giá trị cảm biến 2
    Serial.print("2: ");
    Serial.print(s2);
    Serial.print(" mm \t");

    // In giá trị cảm biến 3
    Serial.print("3: ");
    Serial.print(s3);
    Serial.println(" mm");
    
    Serial.println();

    delay(10);
// End of TOF loop
/****************************************************************************************/


/****************************************************************************************/
// MPU loop
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

/****************************************************************************************/
// MPU loop



// End of MPU loop
/****************************************************************************************/

/****************************************************************************************/
// Start of Motor loop
  // Motor A forward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 255); // Speed 0-255

  // Motor B forward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 255); // Speed 0-255

// End of Motor loop
/****************************************************************************************/

}
/**************************End of loop**************************************************/
