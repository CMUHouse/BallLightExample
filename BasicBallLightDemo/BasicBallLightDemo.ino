#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include "BallLight.h"
#define NUM_LEDS 50
#define LED_PIN 5

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN);
BallLight balls[NUM_LEDS];
BallLightSettings ballSettings(600, 2000, 30, 50);

void setup() 
{
  pixels.begin();
}

void loop() 
{  
  unsigned long t = millis();
  for (uint16_t i = 0; i < NUM_LEDS; i++) {    
    RGBColor col = balls[i].updateForTime(t, ballSettings);
    pixels.setPixelColor(i, pixels.Color(col.r, col.g, col.b));
  }
  pixels.show();
  delay(16);
}
