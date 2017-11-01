#include "LowPower.h"
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 12

#define DATA_PIN 4
#define DATA_PIN2 9

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

// Use pin 2 as wake up pin
const int wakeUpPin = 2;

const int zumbador = 8; // Pin digital para el zumbador

void wakeUp()
{
  // Just a handler for the pin interrupt.
}

void setup()
{
  // Configure wake up pin as input.
  // This will consumes few uA of current.
  pinMode(wakeUpPin, INPUT_PULLUP);
  pinMode(zumbador, OUTPUT); // Pin digital 8 como salida
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds2, NUM_LEDS);
  LEDS.setBrightness(84);
}

void loop()
{
  // Allow wake up pin to trigger interrupt on low.
  attachInterrupt(0, wakeUp, LOW);

  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // Disable external pin interrupt on wake up pin.
  detachInterrupt(0);

  // Do something here
  // Sound + leds
  efectos();
}

void efectos () {
  digitalWrite(zumbador, HIGH);
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
      leds2[i] = CRGB::Red;
      FastLED.show();
      delay(5);
      leds[i] = CRGB::Black;
      leds2[i] = CRGB::Black;
      FastLED.show();
      delay(5);
    }
    digitalWrite(zumbador, !digitalRead(zumbador));
  }
  digitalWrite(zumbador, LOW);
}


