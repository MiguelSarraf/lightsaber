#include <FastLED.h>

// Parameters
#define NUM_LEDS 288
#define DATA_PIN D3
#define OFFSET 144+15
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
  CRGB blade_color = CRGB(255, 255, 00); // Yellow
  int speed = 2; // From 1 to 5

  // Activate speed pixels at a time
  for(int pixel_position = OFFSET; pixel_position < NUM_LEDS; pixel_position = pixel_position + speed) {
    for(int offset = 0; offset < speed; offset++){
      leds[pixel_position+offset] = blade_color;
    }
    FastLED.show();
  }

  delay(1000);

  // Deactivate speed pixels at a time
  for(int pixel_position = NUM_LEDS-1; pixel_position >= OFFSET; pixel_position = pixel_position - speed) {
    for(int offset = 0; offset < speed; offset++){
      leds[pixel_position-offset] = black;
    }
    FastLED.show();
  }

  delay(1000);
}
