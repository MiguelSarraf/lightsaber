#include <FastLED.h>

// PINS DEFINITION
#define heartBeatLed LED_BUILTIN
#define BUTTON_PIN D1 // GPIO21 pin connected to button
#define DATA_PIN D3
// ---------------------

// STATE VARIABLES
int heartBeatLedState = LOW;
unsigned long previousMillis = 0;
const long heartBeatInterval = 1000;

int lastButtonState = HIGH; // the previous state from the input pin
int currentButtonState;     // the current reading from the input pin
int currentLighsaberState = LOW;     // the current reading from the input pin

#define NUM_LEDS 288
#define OFFSET 144+15
CRGB leds[NUM_LEDS];
CRGB black = CRGB(0, 0, 0);
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
  // Activate speed pixels at a time
  for(int pixel_position = OFFSET; pixel_position < NUM_LEDS; pixel_position = pixel_position + speed) {
    for(int offset = 0; offset < speed; offset++){
      leds[pixel_position+offset] = blade_color;
    }
    FastLED.show();
  }
}

// Function where shut down pattern will be placed
void shut_down(int speed){
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

void setup() {
  Serial.begin(9600);
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

  // Save the last state
  lastButtonState = currentButtonState;

  delay(100);
}
