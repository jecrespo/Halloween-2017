#include "LowPower.h"
#include "FastLED.h"
#include <SoftwareSerial.h> //For LCD management

#define NUM_LEDS 12 // How many leds in your strip
#define DATA_PIN 4
#define DATA_PIN2 9
#define WAKEUP_PIN 2  //Wakeup pin is connected to Infrared line track sensor digital output
#define BUZZER_PIN 8

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

int candy_counter = 0;

// Attach the serial display's RX line to digital pin 6
SoftwareSerial mySerial(7, 6); // pin 6 = TX, pin 7 = RX (unused)

void setup()
{
  // Configure wake up pin as input.
  // This will consumes few uA of current.
  pinMode(WAKEUP_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT); // Pin digital 8 como salida
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds2, NUM_LEDS);
  LEDS.setBrightness(84);
  mySerial.begin(9600); // set up serial port for 9600 baud
  delay(500); // wait for display to boot up
  mySerial.write(254); // move cursor to beginning of first line
  mySerial.write(128);
  mySerial.write("                "); // clear display
  mySerial.write(254); // move cursor to beginning of first line
  mySerial.write(192);
  mySerial.write("                "); // clear display
  mySerial.write(254); // move cursor to beginning of first line
  mySerial.write(128);
  mySerial.print("Number Candies:");
  mySerial.write(254); // move cursor to beginning of second line
  mySerial.write(192);
  mySerial.print(candy_counter);
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
  candy_counter ++;
  mySerial.write(254); // move cursor to beginning of second line
  mySerial.write(192);
  mySerial.print(candy_counter);
  digitalWrite(BUZZER_PIN, HIGH);
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
    digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
  }
  digitalWrite(BUZZER_PIN, LOW);
}

void wakeUp()
{
  // Just a handler for the pin interrupt.
}
