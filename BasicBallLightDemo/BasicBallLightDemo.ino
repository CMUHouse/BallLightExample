#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include "BallLight.h"
#define NUM_LEDS 50
#define LED_PIN 5

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN);
BallLight lights[NUM_LEDS];

void setup() 
{
  for (int i = 0; i < NUM_LEDS; i++) {
    lights[i] = BallLight(600, 2000, 30, 50);
  }

  pixels.begin();
}

void loop() 
{  
  unsigned long t = millis();
  for (int i = 0; i < NUM_LEDS; i++) {  
    lights[i].updateForTime(t);
    RGBColor col = lights[i].color();
    pixels.setPixelColor(i, pixels.Color(col.r, col.g, col.b));
  }
  pixels.show();
  delay(16);
}
