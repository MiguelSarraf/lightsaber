#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

// GYROSCOPE INSTANCE
Adafruit_MPU6050 mpu;
// ---------------------

// GENERAL VARAIBLES
float acceleration[3];
float old_acceleration[]={0, 0, 0};
float gyro[3];
float old_gyro[]={0, 0, 0};
// ---------------------

// Function to compute the module of a vector
float compute_module(float vector[]){
  float total = 0;
  for(int index=0; index < 3; index++){
    total += pow(vector[index], 2);
  }
  return sqrt(total);
}

void setup(void) {
  // Wait until serial is initialized
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  // Initialize gyroscope
  if (!mpu.begin()) {
    Serial.println("\nFailed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("\nMPU6050 Found!");

  // Set gyroscope acceleration range
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // Set gyroscope gyro range
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  // Set gyroscope bandwidth
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  // Get gyroscope readings
  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  /*
  The sensor readings are coming with relatively large values
  even when it is completely immobile. I hipothetize it has
  something to do with gravity, even though the absolute value of
  the acceleration changes when I hold the sensor in different
  positions. In order to overcome this, I chose to use as "virtual
  reading" the difference between the actual reading and the
  previous one for both acceleration and gyro.
  For sure, there is a much better explanation and workaround
  for the fenmena, which I hope to unravel somewhen in the future.
  */

  // Calculate readings difference to previous ones
  acceleration[0] = a.acceleration.x-old_acceleration[0];
  acceleration[1] = a.acceleration.y-old_acceleration[1];
  acceleration[2] = a.acceleration.z-old_acceleration[2];

  gyro[0] = g.gyro.x-old_gyro[0];
  gyro[1] = g.gyro.y-old_gyro[1];
  gyro[2] = g.gyro.z-old_gyro[2];

  // Outputs read values for monitoring
  Serial.printf("Acceleration: ||%.2f, %.2f, %.2f|| = %.2f\n", acceleration[0], acceleration[1], acceleration[2], compute_module(acceleration));
  Serial.printf("Rotation:     ||%.2f, %.2f, %.2f|| = %.2f\n", gyro[0], gyro[1], gyro[2], compute_module(gyro));
  Serial.printf("Temperature:  %.2f°C\n\n", temp.temperature);

  // Save new readings as old ones
  old_acceleration[0]=acceleration[0]+old_acceleration[0];
  old_acceleration[1]=acceleration[1]+old_acceleration[1];
  old_acceleration[2]=acceleration[2]+old_acceleration[2];

  old_gyro[0]=gyro[0]+old_gyro[0];
  old_gyro[1]=gyro[1]+old_gyro[1];
  old_gyro[2]=gyro[2]+old_gyro[2];

  delay(500);
}
