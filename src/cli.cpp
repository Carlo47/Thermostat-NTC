/**
 * Program      Command line interface (cli) for the thermostat
 * 
 * Author       2024-10-12 Charles Geiser (https://www.dodeka.ch)
 * 
 * Purpose      Allows the thermostat parameters to be set and displayed for 
 *              test purposes, e.g. lower and upper temperature limit, refresh 
 *              interval, etc.
 * 
 * Remarks      For the sake of simplicity, the input of numerical values is 
 *              hidden, i.e. the digits are not displayed but are accepted 
 *              after the timeout time has elapsed. The input menu can be 
 *              shown again at any time by pressing 'S' (capital S).
 * 
 */
#include <Arduino.h>
#include "Thermostat.h"

extern Thermostat thermostat;
extern NTCSensor sensor;

// Forward declaration of menu actions
void setLowerLimit();
void setUpperLimit();
void setTempDelta();
void setInterval();
void setNTCbeta();
void toggleThermostat();
void showValues();
void showMenu();

using MenuItem = struct mi{ const char key; const char *txt; void (&action)(); };

MenuItem menu[] = 
{
  { 'l', "[l] Set lower limit      [°C]",         setLowerLimit },
  { 'u', "[u] Set upper limit      [°C]",         setUpperLimit },
  { 'd', "[d] Set temp delta       [°C]",         setTempDelta  },
  { 'b', "[b] Set beta of NTC      [°K]",         setNTCbeta },
  { 'i', "[i] Set refresh interval [ms]",         setInterval },
  { 't', "[t] Toggle thermostat enable/disable",  toggleThermostat },
  { 'v', "[v] Show values",                       showValues },
  { 'S', "[S] Show menu",                         showMenu },
};
constexpr uint8_t nbrMenuItems = sizeof(menu) / sizeof(menu[0]);

void showMenu()
{
  // title is packed into a raw string
  Serial.print(
  R"(
-----------------------------
    Thermostat with NTC
-----------------------------
)");

  for (int i = 0; i < nbrMenuItems; i++)
  {
    Serial.println(menu[i].txt);
  }
}

void doMenu()
{
  char key = Serial.read();

  for (int i = 0; i < nbrMenuItems; i++)
  {
    if (key == menu[i].key)
    {
      menu[i].action();
      break;
    }
  } 
}

void setLowerLimit()
{
  float value = 0;

  delay(2000);
  while (Serial.available())
  {
    value = Serial.parseFloat();
  }
  thermostat.setLimitLow(value);
}

void setUpperLimit()
{
  float value = 0;

  delay(2000);
  while (Serial.available())
  {
    value = Serial.parseFloat();
  }
  thermostat.setLimitHigh(value);
}


void setTempDelta()
{
  float value = 0;

  delay(2000);
  while (Serial.available())
  {
    value = Serial.parseFloat();
  }
  thermostat.setTempDelta(value);
}


void setInterval()
{
   long value = 0;

  delay(2000);
  while (Serial.available())
  {
    value = Serial.parseInt();
  }
  thermostat.setRefreshInterval(value);
}


void setNTCbeta()
{
  uint16_t value = 0;

  delay(2000);
  while (Serial.available())
  {
    value = Serial.parseInt();
  }
  sensor.setNTCbeta(value);
}


/**
 * Enable or disable thermostat
 */
void toggleThermostat()
{
  thermostat.isEnabled() ? thermostat.disable() : thermostat.enable();
  Serial.printf("Thermostat is %s\n", thermostat.isEnabled() ? "enabled" : "disabled");
}

void showValues()
{
  sensor.printData();
  thermostat.printSettings();
}