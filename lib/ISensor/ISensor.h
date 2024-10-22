#pragma once
#include "SensorData.h"

/**
 * Sensor interface is a pure abstract class. It declares the
 * methods that must be implemented by the inheriting sensor class
 */
class ISensor
{
  public: 
    virtual void setup()       = 0; // initialize the sensor 
    virtual void readSensor()  = 0; // read the sensor values into the sonsor data struct
    virtual float getCelsius() = 0; // returns the temperature in °C
    virtual void printData()   = 0; // print the data read from the sensor data struct
    virtual SensorData& getDataReference() = 0; // get a reference to the sensor data struct
};