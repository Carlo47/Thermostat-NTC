#include <Arduino.h>

/**
 * Flashes the LED on pin nBeats times in t seconds
 * with a dutycycle of duty % [1..99]
 * When dutycycle is out of allowed range, 50% is forced.
 * You have to define a valid GPIO as PIN_HEARTBEAT, e.g.
 * Call the function in main loop or in case of an unrecoverable
 * error in an endless loop like this: while(true) heratbeat(3,1,50);
 * #define PIN_HEARTBEAT  LED_BUILTIN
 * Example: heartbeat(pin, 7, 13, 15) 
 *          flash the LED 7 times in 13 seconds 
 *          with a dutycycle of 15%
 */
void heartbeat(uint8_t pin, uint8_t nBeats, uint8_t t, uint8_t duty)
{
  duty = duty < 100 ? duty : 50;
  uint32_t module = 1000 * t / nBeats;
  uint32_t ms = module * duty / 100;
  digitalWrite(pin, millis() % module < ms ? HIGH : LOW);
}