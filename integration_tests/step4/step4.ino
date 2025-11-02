#include <FastLED.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

// PINS DEFINITION
#define heartBeatLed LED_BUILTIN
#define BUTTON_PIN D4
#define DATA_PIN D3
#define MP3_RX_PIN D7
#define MP3_TX_PIN D8
// ---------------------

// VARIABLES
int heartBeatLedState = LOW;
unsigned long previousMillis = 0;
const long heartBeatInterval = 1000;

int lastButtonState = HIGH; // the previous state from the input pin
int currentButtonState;     // the current reading from the input pin
int currentLighsaberState = LOW;     // the current reading from the input pin

#define NUM_LEDS 288
#define OFFSET 144+30
CRGB leds[NUM_LEDS];
CRGB black = CRGB(0, 0, 0);

#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/MP3_RX_PIN, /*tx =*/MP3_TX_PIN);
#define FPSerial softSerial
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

unsigned long last_humming_time = 0;
unsigned long humming_limit = 55000;
#define VOLUME 20

Adafruit_MPU6050 mpu;
float acceleration[3];
float old_acceleration[]={0, 0, 0};
float gyro[3];
float old_gyro[]={0, 0, 0};
float total_acceleration;
sensors_event_t a, g, temp;
bool ignore_next_cycle=false;
#define SWING_THRESHOLD 20
#define READING_ERROR_THRESHOLD 35
// ---------------------

// Songs pointers
#define POWER_UP_AUDIO 1
#define HUMMING_AUDIO 2
#define SWING_AUDIO_START 3
#define SWING_AUDIO_END 4
#define SHUT_DOWN_AUDIO 5
int swing_audio=SWING_AUDIO_START;
// ---------------------

// Toggle function alternates between LOW and HIGH states.
int toggle(int state) {
  if (state == LOW) {
    return HIGH;
  }
  return LOW;
}

// Function where wake up pattern will be placed
void wake_up(int speed, CRGB blade_color){
  myDFPlayer.playMp3Folder(POWER_UP_AUDIO);
  delay(200);
  // Activate speed pixels at a time
  for(int pixel_position = OFFSET; pixel_position < NUM_LEDS; pixel_position = pixel_position + speed) {
    for(int offset = 0; offset < speed; offset++){
      leds[pixel_position+offset] = blade_color;
    }
    FastLED.show();
  }
  delay(500);
  myDFPlayer.playMp3Folder(HUMMING_AUDIO);
}

// Function where shut down pattern will be placed
void shut_down(int speed){
  // myDFPlayer.pause();
  myDFPlayer.playMp3Folder(SHUT_DOWN_AUDIO);
  delay(200);
  // Deactivate speed pixels at a time
  for(int pixel_position = NUM_LEDS-1; pixel_position >= OFFSET; pixel_position = pixel_position - speed) {
    for(int offset = 0; offset < speed; offset++){
      leds[pixel_position-offset] = black;
    }
    FastLED.show();
  }
}

// Function to control lighsaber state changes according to buttun pressing
int toggle_lightsaber_state(int state, int lastButtonState, int currentButtonState, int speed, CRGB blade_color){
  if(lastButtonState == LOW && currentButtonState == HIGH){
    Serial.println("Button releaseed, changing lighsaber state");

    if(state == LOW){
      wake_up(speed, blade_color);
    }else{
      shut_down(speed);
    }

    return not state;
  }
  return state;
}

// Function to print communication problems with mp3 module to the serial output
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}

// Function to compute the module of a vector
float compute_module(float vector[]){
  float total = 0;
  for(int index=0; index < 3; index++){
    total += pow(vector[index], 2);
  }
  return sqrt(total);
}

// Function to expand the gyroscopes readings into their virtual values and indicate if a swing is happening
bool process_gyroscope_reding(float *old_acceleration, float *acceleration, float *old_gyro, float *gyro, sensors_event_t a, sensors_event_t g){

  return false;
}

void setup() {
  FPSerial.begin(9600);

  // initialize serial monitor
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  // Check configuration of MP3 player
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(1000);

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
  
  // Set volume
  myDFPlayer.volume(VOLUME);  //Set volume value. From 0 to 30
  // Initialize pins
  pinMode(heartBeatLed, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

	// LED - Sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  // Configure LED strip
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  unsigned long currentMillis = millis();
  // Checks if ellapsed time is greater than heart beat interval and toggle state.
  if (currentMillis - previousMillis >= heartBeatInterval) {
    previousMillis = currentMillis;
    heartBeatLedState = toggle(heartBeatLedState);
    digitalWrite(heartBeatLed, heartBeatLedState);
  }

  // Get blade color and speed to be used
  CRGB blade_color = CRGB(255, 255, 00); // Yellow
  int speed = 2; // From 1 to 5

  // Read button state and toggle if necessary
  currentButtonState = digitalRead(BUTTON_PIN);
  currentLighsaberState = toggle_lightsaber_state(currentLighsaberState, lastButtonState, currentButtonState, speed, blade_color);
  
  // Checks if ellapsed time is greater than humming sound and play it again
  if (currentLighsaberState & (currentMillis - last_humming_time >= humming_limit)) {
    last_humming_time = currentMillis;
    // myDFPlayer.pause();
    myDFPlayer.playMp3Folder(HUMMING_AUDIO);
  }

  // Save the last state
  lastButtonState = currentButtonState;

  // Get gyroscope readings
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
  // Serial.printf("Acceleration: ||%.2f, %.2f, %.2f|| = %.2f\n", acceleration[0], acceleration[1], acceleration[2], compute_module(acceleration));
  // Serial.printf("Rotation:     ||%.2f, %.2f, %.2f|| = %.2f\n", gyro[0], gyro[1], gyro[2], compute_module(gyro));
  // Serial.printf("Temperature:  %.2f°C\n\n", temp.temperature);
  total_acceleration=compute_module(acceleration);
  if (currentLighsaberState & not ignore_next_cycle & READING_ERROR_THRESHOLD>total_acceleration & total_acceleration>SWING_THRESHOLD){
    Serial.printf("SWING: %f\n", total_acceleration);
    // myDFPlayer.pause();
    // myDFPlayer.volume(25);
    myDFPlayer.playMp3Folder(swing_audio);
    if (swing_audio==SWING_AUDIO_END){
      swing_audio=SWING_AUDIO_START;
    }else{
      swing_audio+=1;
    }
    delay(1500);
    // myDFPlayer.pause();
    // myDFPlayer.volume(VOLUME);
    myDFPlayer.playMp3Folder(HUMMING_AUDIO);
    ignore_next_cycle=true;
  }else{
    ignore_next_cycle=false;
  }

  // Save new readings as old ones
  old_acceleration[0]=acceleration[0]+old_acceleration[0];
  old_acceleration[1]=acceleration[1]+old_acceleration[1];
  old_acceleration[2]=acceleration[2]+old_acceleration[2];

  old_gyro[0]=gyro[0]+old_gyro[0];
  old_gyro[1]=gyro[1]+old_gyro[1];
  old_gyro[2]=gyro[2]+old_gyro[2];
  
  // Check MP3 module health
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  delay(100);
}
