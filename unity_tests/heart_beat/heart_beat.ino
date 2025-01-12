/*
Constants region.

Defines pins, states and other constants.
*/

const int heartBeatLed = LED_BUILTIN;

int heartBeatLedState = LOW;

unsigned long previousMillis = 0;
const long heartBeatInterval = 1000;

/*
Toggle function alternates between LOW and HIGH states.
*/
int toggle(int state) {
  if (state == LOW) {
    return HIGH;
  }
  return LOW;
}

void setup() {
  // Initialize heart beat LED pin as an output.
  pinMode(heartBeatLed, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  // Checks if ellapsed time is greater than heart beat interval and toggle state.
  if (currentMillis - previousMillis >= heartBeatInterval) {
    previousMillis = currentMillis;
    heartBeatLedState = toggle(heartBeatLedState);
    digitalWrite(heartBeatLed, heartBeatLedState);
  }
}