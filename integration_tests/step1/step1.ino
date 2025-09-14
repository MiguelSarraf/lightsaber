// PINS DEFINITION
#define heartBeatLed LED_BUILTIN
#define BUTTON_PIN D1
#define LED_PIN D2
// ---------------------

// STATE VARIABLES
int heartBeatLedState = LOW;
unsigned long previousMillis = 0;
const long heartBeatInterval = 1000;

int lastButtonState = HIGH; // the previous state from the input pin
int currentButtonState;     // the current reading from the input pin
int currentLighsaberState = LOW;     // the current reading from the input pin
// ---------------------

// Toggle function alternates between LOW and HIGH states.
int toggle(int state) {
  if (state == LOW) {
    return HIGH;
  }
  return LOW;
}

// Function where wake up pattern will be placed
void wake_up(){
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(400);
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(200);
  digitalWrite(LED_PIN, HIGH);
}

// Function where shut down pattern will be placed
void shut_down(){
  digitalWrite(LED_PIN, LOW);
  delay(200);
  digitalWrite(LED_PIN, HIGH);
  delay(400);
  digitalWrite(LED_PIN, LOW);
  delay(200);
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
}

// Function to control lighsaber state changes according to buttun pressing
int toggle_lightsaber_state(int state, int lastButtonState, int currentButtonState){
  if(lastButtonState == LOW && currentButtonState == HIGH){
    Serial.println("Button releaseed, changing lighsaber state");

    if(state == LOW){
      wake_up();
    }else{
      shut_down();
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
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  // Checks if ellapsed time is greater than heart beat interval and toggle state.
  if (currentMillis - previousMillis >= heartBeatInterval) {
    previousMillis = currentMillis;
    heartBeatLedState = toggle(heartBeatLedState);
    digitalWrite(heartBeatLed, heartBeatLedState);
  }

  // Read button state and toggle if necessary
  currentButtonState = digitalRead(BUTTON_PIN);
  currentLighsaberState = toggle_lightsaber_state(currentLighsaberState, lastButtonState, currentButtonState);

  // Save the last state
  lastButtonState = currentButtonState;

  delay(100);
}
