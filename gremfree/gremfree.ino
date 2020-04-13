// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor
//included libraries go here
#include <Adafruit_LSM6DSOX.h>
#include "FIFO.h"

//definitions go here
#define FIFO_S 100
#define CALIBRATION_THRESHOLD 1.0
#define VIBRATION_MOTOR  5
#define VIBRATION_TIME  1000
#define SAMPLING_TIME 8     //IN MILLISECONDS


//global variables go here
float IDLE_X_acc = -3.9;
float IDLE_Y_acc =  9;
float IDLE_Z_acc = 0.4;
float IDLE_X_GYRO = 0;
float IDLE_Y_GYRO = 0;
float IDLE_Z_GYRO = 0;

struct sensor_data{
  float accelx;
  float accely;
  float accelz;
  float gyrox;
  float gyroy;
  float gyroz;  
};

fifo_t calibrationFIFO;
Adafruit_LSM6DSOX sox;

//todo : get a zero out function going.
/*steps: 1. make a fifo and fill it up
 * 2. get all data in the fifo and calculate min value, max value, and average. 
 * 3. if max val - min val < |threshold| (threshold is a very small number), return average
 * 4. else do it again
 * 5. vibrate motor a bit to signify calibration.
 */

//Functions definitions go here
/*
 * this function takes a N number of samples and calibrates zeros out the readings if the user stays still
 */
int zero_out_gyroAccel();
/*
 * this function takes a duration in millisecond, for a number of repetions and an intesity at which the motor vibrates
 * the gap(milliseconds) is the interval in between repetions where nothing happens
 */
void heart_beat(int duration,int repetitions, int intensity, int gap);


void setup(void) {
  pinMode(VIBRATION_MOTOR, OUTPUT);
  Serial.begin(115200);
//  while (!Serial)
//    delay(10); // will pause Zero, Leonardo, etc until serial console opens
    
  if (!sox.begin_I2C()) {
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DSOX Found!");

  // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
   sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
   sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

//  int counter = 0;
  heart_beat(300,2,250,200);
   while(!zero_out_gyroAccel()){
//    counter++;
//    Serial.println();
//    Serial.print(counter);
//    Serial.println("inside a while loop in setup");
      heart_beat(300,2,150,200);
    };

//    a successful set up gives 5 buzzess
   heart_beat(200,5,250,300);

}

void loop() {

  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

//  Serial.print("\t\tTemperature ");
//  Serial.print(temp.temperature);
//  Serial.println(" deg C");
//
//  /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print("\t\tAccel X: ");
//  Serial.print(accel.acceleration.x);
//  Serial.print(" \tY: ");
//  Serial.print(accel.acceleration.y);
//  Serial.print(" \tZ: ");
//  Serial.print(accel.acceleration.z);
//  Serial.println(" m/s^2 ");
//
//  /* Display the results (rotation is measured in rad/s) */
//  Serial.print("\t\tGyro X: ");
//  Serial.print(gyro.gyro.x);
//  Serial.print(" \tY: ");
//  Serial.print(gyro.gyro.y);
//  Serial.print(" \tZ: ");
//  Serial.print(gyro.gyro.z);
//  Serial.println(" radians/s ");
//  Serial.println();
//
//  delay(100);

  //  // serial plotter friendly format

//    Serial.print(temp.temperature);
//    Serial.print(",");

  
//
   Serial.print(accel.acceleration.x - IDLE_X_acc);
   Serial.print(","); Serial.print(accel.acceleration.y - IDLE_Y_acc);
//
   Serial.print(","); Serial.print(accel.acceleration.z - IDLE_Z_acc);
   Serial.print(",");
//
   Serial.print(gyro.gyro.x - IDLE_X_GYRO);
   Serial.print(","); Serial.print(gyro.gyro.y - IDLE_Y_GYRO);
   Serial.print(","); Serial.print(gyro.gyro.z - IDLE_Z_GYRO);
   Serial.println();

    if((accel.acceleration.y - IDLE_Y_acc) <-10 && (accel.acceleration.x - IDLE_X_acc) >2 && (gyro.gyro.x- IDLE_X_GYRO) > 0.2 ){//&& (gyro.gyro.z- IDLE_Z_GYRO) <-4  && (gyro.gyro.y- IDLE_Y_GYRO) <-0.4 ){
//    if((accel.acceleration.y - IDLE_Y_acc) <-6 ){
     
      analogWrite(VIBRATION_MOTOR, 250);
      delay(VIBRATION_TIME);
      analogWrite(VIBRATION_MOTOR, 0);
    }

   
//   delayMicroseconds(10000);
}

/*
 * ==========================Function inplementations go below here=====================
 */
int zero_out_gyroAccel(){
  calibrationFIFO = fifo_create(FIFO_S,sizeof(struct sensor_data));
  // check if FIFO was created
  if (calibrationFIFO == NULL) {
    Serial.println(F("Cannot create FIFO... halting!"));
    for (;;);
  } else {
    Serial.println(F("FIFO created successfully"));
  }

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);
  struct sensor_data data;
  
  //fill fifo up
  int counter = 0;
  while(!fifo_is_full(calibrationFIFO)){
    data.accelx = accel.acceleration.x;
    data.accely = accel.acceleration.y;
    data.accelz = accel.acceleration.z;
    data.gyrox  = gyro.gyro.x;
    data.gyroy  = gyro.gyro.y;
    data.gyroz  = gyro.gyro.z; 
    if (fifo_add(calibrationFIFO, &data)) {
      Serial.print(counter);
      Serial.println(F(" Item was added to queue!"));
    }
  }
  Serial.println("the fifo is full now");

  //in this section the mean and min max values are calcutaled and validated, also the buffer is emptied 
  //ALWAYS EMPTY THE BUFFER, OTHERWISE THE MEMORY WILL CRASH. 
  //the fifo buffer loses one item every time the fifo_get function is called.
  float min_val[] = {99,99,99,99,99,99};
  float max_val[] = {-99,-99,-99,-99,-99,-99};
  float mean[] = {420,420,420,420,420,420};
  float sum[] = {0,0,0,0,0,0};
  while (!fifo_is_empty(calibrationFIFO)){
      fifo_get(calibrationFIFO, &data);
      if( min_val[0]>data.accelx){min_val[0]=data.accelx;}if(max_val[0]<data.accelx){max_val[0]=data.accelx;}
      if( min_val[1]>data.accely){min_val[1]=data.accely;}if(max_val[1]<data.accely){max_val[1]=data.accely;}
      if( min_val[2]>data.accelz){min_val[2]=data.accelz;}if(max_val[2]<data.accelz){max_val[2]=data.accelz;}
      if( min_val[3]>data.gyrox){min_val[3]=data.gyrox;}if(max_val[3]<data.gyrox){max_val[3]=data.gyrox;}
      if( min_val[4]>data.gyroy){min_val[4]=data.gyroy;}if(max_val[4]<data.gyroy){max_val[4]=data.gyroy;}
      if( min_val[5]>data.gyroz){min_val[5]=data.gyroz;}if(max_val[5]<data.gyroz){max_val[5]=data.gyroz;}
      sum[0] = sum[0] + data.accelx;
      sum[1] = sum[1] + data.accely;
      sum[2] = sum[2] + data.accelz;
      sum[3] = sum[3] + data.gyrox;
      sum[4] = sum[4] + data.gyroy;
      sum[5] = sum[5] + data.gyroz;
      delay(SAMPLING_TIME);
    }
  mean[0] = sum[0]/FIFO_S;
  mean[1] = sum[1]/FIFO_S;
  mean[2] = sum[2]/FIFO_S;
  mean[3] = sum[3]/FIFO_S;
  mean[4] = sum[4]/FIFO_S;
  mean[5] = sum[5]/FIFO_S;

  if((abs(max_val[0]-min_val[0]) < CALIBRATION_THRESHOLD)  && (abs(max_val[1]-min_val[1]) < CALIBRATION_THRESHOLD) && (abs(max_val[2]-min_val[2]) < CALIBRATION_THRESHOLD) && (abs(max_val[3]-min_val[3]) < CALIBRATION_THRESHOLD) && (abs(max_val[4]-min_val[4]) < CALIBRATION_THRESHOLD) && (abs(max_val[5]-min_val[5]) < CALIBRATION_THRESHOLD))
  {
    IDLE_X_acc = mean[0];
    IDLE_Y_acc =  mean[1];
    IDLE_Z_acc = mean[2];
    IDLE_X_GYRO = mean[3];
    IDLE_Y_GYRO = mean[4];
    IDLE_Z_GYRO = mean[5];
    return 1;
    }  
  //get min, max, and mean
  return 0;
  }

  
void heart_beat(int duration,int repetitions, int intensity, int gap){
  for (int i = 0; i< repetitions; i++){
    analogWrite(VIBRATION_MOTOR, intensity);
    delay(duration);
    analogWrite(VIBRATION_MOTOR,0);
    delay(gap);
    }
  }
