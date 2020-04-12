// Basic demo for accelerometer & gyro readings from Adafruit
// LSM6DSOX sensor

#include <Adafruit_LSM6DSOX.h>

int IDLE_X_acc = -3.9;
int IDLE_Y_acc =  9;
int IDLE_Z_acc = 0.4;
int IDLE_X_GYRO = 0;
int IDLE_Y_GYRO = 0;
int IDLE_Z_GYRO = 0;


int vibration_motor = 5;
int timer = 1000;           // The higher the number, the slower the timing.
Adafruit_LSM6DSOX sox;
void setup(void) {
  pinMode(vibration_motor, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
    
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

  

   Serial.print(accel.acceleration.x);
   Serial.print(","); Serial.print(accel.acceleration.y);

   Serial.print(","); Serial.print(accel.acceleration.z);
   Serial.print(",");

   Serial.print(gyro.gyro.x);
   Serial.print(","); Serial.print(gyro.gyro.y);
   Serial.print(","); Serial.print(gyro.gyro.z);
   Serial.println();

    if((accel.acceleration.y - IDLE_Y_acc) <-6 && (accel.acceleration.x - IDLE_X_acc) <-5 && (gyro.gyro.x- IDLE_X_GYRO) <-0.4 && (gyro.gyro.z- IDLE_Z_GYRO) <-4  && (gyro.gyro.y- IDLE_Y_GYRO) <-0.4 ){
      analogWrite(vibration_motor, 250);
      delay(timer);
      analogWrite(vibration_motor, 0);
    }

   
//   delayMicroseconds(10000);
}
