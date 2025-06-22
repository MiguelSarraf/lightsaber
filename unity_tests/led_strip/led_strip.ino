#include <FastLED.h>

// Parameters
#define NUM_LEDS 144
#define DATA_PIN 0
// ---------------------

// LED strip variables
CRGB leds[NUM_LEDS];
CRGB black = CRGB(0, 0, 0);
// ---------------------

void setup() {
	// Sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  // Configure LED strip
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  // Get blade color and speed to be used
  CRGB blade_color = CRGB(255, 255, 0); // Yellow
  int speed = 6; // From 1 to 10

  // Activate one pixel at a time
  for(int pixel_position = 0; pixel_position < NUM_LEDS; pixel_position = pixel_position + 1) {
    leds[pixel_position] = blade_color;
    FastLED.show();
    delay(11-speed);
  }

  delay(1000);

  // Deactivate one pixel at a time
  for(int pixel_position = NUM_LEDS-1; pixel_position >= 0; pixel_position = pixel_position - 1) {
    leds[pixel_position] = black;
    FastLED.show();
    delay(11-speed);
  }

  delay(1000);
}
