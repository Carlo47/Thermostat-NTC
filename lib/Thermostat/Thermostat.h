/**
 * Class        Thermostat
 * 
 * Author       2024-10-18 Charles Geiser (https://www.dodeka.ch)
 * 
 * Purpose      Declaration of the class Thermostat.
 *              The arguments of the constructor are the generic interface of the  
 *              sensor and references to the 3 callbacks processData(), onLowTemp() 
 *              and onHighTemp(), which must be supplied by the application  
 *              program of the class.
 * 
 * Remarks      The generic interface enables the use of various sensors such as BME280, 
 *              SHT31, DHT11/22 or a simple NTC-Resstor. To use these sensors, you must adapt
 *              the sensor data struct in SensorData.h and provide a corresponding sensor class
 *              like BME230Sensor, SHT31Sensor, DHTSensor or NTCSensor.
 */
#pragma once
#include "NTCSensor.h"

using Callback = void(&)();

class Thermostat 
{
  public:
    Thermostat(ISensor& sensor, Callback processData, Callback onLowTemp, Callback onHighTemp) : 
      _sensor(sensor), _processData(processData), _onLowTemp(onLowTemp), _onHighTemp(onHighTemp) 
    {}

    void setup();
    void loop();
    void enable();
    void disable();
    bool isEnabled();
    void setRefreshInterval(uint32_t msRefresh);  // msec
    void setLimitLow(float tLimitLow);            // °C
    void setLimitHigh(float tLimitHigh);          // °C
    void setTempDelta(float delta);               // °C
    float getLimitLow();
    float getLimitHigh();
    float getTempDelta();
    uint32_t getRefreshInterval();  
    void printSettings();

  private:
    ISensor& _sensor;
    bool     _isEnabled  = false;
    bool     _switchIsOn = false;
    float    _tLimitLow  = 18.0;
    float    _tLimitHigh = 21.0;
    float    _tDelta     =  3.0;
    uint32_t _msRefresh  = 10000;
    Callback _processData;;
    Callback _onLowTemp;
    Callback _onHighTemp;
};
