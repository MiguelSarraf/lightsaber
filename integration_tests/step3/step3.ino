#include <FastLED.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// PINS DEFINITION
#define heartBeatLed LED_BUILTIN
#define BUTTON_PIN D1 // GPIO21 pin connected to button
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
#define VOLUME 15
// ---------------------

// Songs pointers
#define POWER_UP_AUDIO 1
#define HUMMING_AUDIO 2
#define SWING_AUDIO_START 3
#define SWING_AUDIO_END 4
#define SHUT_DOWN_AUDIO 5
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
    myDFPlayer.playMp3Folder(HUMMING_AUDIO);
  }

  // Save the last state
  lastButtonState = currentButtonState;
  
  // Check MP3 module health
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  delay(100);
}
