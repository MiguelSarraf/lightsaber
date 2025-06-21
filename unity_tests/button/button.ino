// PINS DEFINITION
#define BUTTON_PIN 5 // GPIO21 pin connected to button
#define LED_PIN 4
// ---------------------

// STATE VARIABLES
int lastButtonState = HIGH; // the previous state from the input pin
int currentButtonState;     // the current reading from the input pin
int currentLighsaberState = LOW;     // the current reading from the input pin
// ---------------------

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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Read button state and toggle if necessary
  currentButtonState = digitalRead(BUTTON_PIN);
  currentLighsaberState = toggle_lightsaber_state(currentLighsaberState, lastButtonState, currentButtonState);

  // Save the last state
  lastButtonState = currentButtonState;

  delay(100);
}
