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
int gyro_x, gyro_y, gyro_z;
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

/****************************************************************************************/
// PID Setup

//   for (int cal_int = 0; cal_int < 1000 ; cal_int ++){                  
//     //Add the gyro x offset to the gyro_x_cal variable                                            
//     gyro_x_cal += mpu.getGyroX();
//     //Add the gyro y offset to the gyro_y_cal variable                                              
//     gyro_y_cal += mpu.getGyroY(); 
//     //Add the gyro z offset to the gyro_z_cal variable                                             
//     gyro_z_cal += mpu.getGyroZ(); 
//     //Delay 3us to have 250Hz for-loop                                             
//     delay(3);                                            
// }

//   gyro_x_cal /= 1000;                                                 
//   gyro_y_cal /= 1000;                                                 
//   gyro_z_cal /= 1000;

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
// PID loop

gyro_x=mpu.getGyroX();
gyro_y=mpu.getGyroY(); 
gyro_z=mpu.getGyroZ();

  //Subtract the offset values from the raw gyro values
  gyro_x -= gyro_x_cal;                                                
  gyro_y -= gyro_y_cal;                                                
  gyro_z -= gyro_z_cal;                                                
         
  //Gyro angle calculations . Note 0.0000611 = 1 / (250Hz x 65.5)
  
  //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_pitch += gyro_x * 0.0000611;  
  //Calculate the traveled roll angle and add this to the angle_roll variable
  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians                                
  angle_roll += gyro_y * 0.0000611; 
                                     
  //If the IMU has yawed transfer the roll angle to the pitch angle
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);
  //If the IMU has yawed transfer the pitch angle to the roll angle               
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               
  
  //Accelerometer angle calculations
  
  //Calculate the total accelerometer vector
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z)); 
   
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  //Calculate the pitch angle
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296; 
  //Calculate the roll angle      
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       
  
  //Accelerometer calibration value for pitch
  angle_pitch_acc -= 0.0;
  //Accelerometer calibration value for roll                                              
  angle_roll_acc -= 0.0;                                               

  if(set_gyro_angles){ 
  
  //If the IMU has been running 
  //Correct the drift of the gyro pitch angle with the accelerometer pitch angle                      
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004; 
    //Correct the drift of the gyro roll angle with the accelerometer roll angle    
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        
  }
  else{ 
    //IMU has just started  
    //Set the gyro pitch angle equal to the accelerometer pitch angle                                                           
    angle_pitch = angle_pitch_acc;
    //Set the gyro roll angle equal to the accelerometer roll angle                                       
    angle_roll = angle_roll_acc;
    //Set the IMU started flag                                       
    set_gyro_angles = true;                                            
  }
  
  //To dampen the pitch and roll angles a complementary filter is used
  //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1; 
  //Take 90% of the output roll value and add 10% of the raw roll value 
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1; 
  //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop  
  //-----------------done with mpu6050 calibration--------------------------------------// 
error = target - angle_pitch_output;// proportional
integral = integral + error; //integral
derivative = error - last_error; //derivative

angle = (error * Kp) + (integral * Ki) + (derivative * Kd);
Serial.println(angle_pitch_output);
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
