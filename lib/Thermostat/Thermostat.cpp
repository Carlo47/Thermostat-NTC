/**
 * Class        Thermostat
 * Author       2024-10-18 Charles Geiser
 * 
 * Purpose      Implements a class Thermostat with  an interface for the sensor.       
 * 
 * Board        ESP32 DoIt DevKit V1
 * 
 * Remarks
 *         
 * References
 */
#include "Thermostat.h"

void Thermostat::setup()
{
  _sensor.setup();
}

void Thermostat::loop()
{
  if((millis() % _msRefresh) == 0 && _isEnabled) 
  {
    _processData();
    if (_sensor.getCelsius() < _tLimitLow)  { _onLowTemp();  _switchIsOn = true; };
    if (_sensor.getCelsius() > _tLimitHigh) { _onHighTemp(); _switchIsOn = false; }
  }
}

void Thermostat::enable()
{
  _isEnabled = true;
}

void Thermostat::disable()
{
  _isEnabled = false;
}

bool Thermostat::isEnabled()
{
  return _isEnabled;
}

void Thermostat::setRefreshInterval(uint32_t msRefresh)
{
  _msRefresh = msRefresh;
}

uint32_t Thermostat::getRefreshInterval()
{
  return _msRefresh;
}

void Thermostat::setLimitLow(float tLow)
{
  _tLimitLow = tLow;
  _tLimitHigh = _tLimitLow + _tDelta;
}

void Thermostat::setLimitHigh(float tHigh)
{
  _tLimitHigh = tHigh;
  _tLimitLow = _tLimitHigh - _tDelta;
}

void Thermostat::setTempDelta(float delta)
{
    _tDelta = delta;
    _tLimitLow = _tLimitHigh - _tDelta;
}


float Thermostat::getLimitLow()
{
  return _tLimitLow;
}

float Thermostat::getLimitHigh()
{
  return _tLimitHigh;
}

float Thermostat::getTempDelta()
{
  return _tDelta;
}


void Thermostat::printSettings()
{
  Serial.printf(R"(--- Thermostat settings ---
Upper limit      %6.1f °C
Delta temp       %6.1f °C
Lower limit      %6.1f °C
Refresh interval %6u ms
Thermostat is %s and switch is %s

)", _tLimitHigh, _tDelta, _tLimitLow, _msRefresh, _isEnabled ? "enabled" : "disabled", _switchIsOn ? "on" : "off");
}


