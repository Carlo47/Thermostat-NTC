/**
 * BME280Sensor.h
 * 
 * Declaration of the class BME280Sensor. It inherits from Adafruit_BME280.
 * The constructors argument is the I2C address of the sensor. 
 */ 
#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Isensor.h"


class BME280Sensor : public Isensor,  Adafruit_BME280
{  
    public:
        BME280Sensor(uint8_t i2cAddress, SensorData& sensorData) : 
            Adafruit_BME280(), _i2cAddress(i2cAddress), _sData(sensorData)
        {}

        void  setup();           // implementation of the pure virtual method declared in sensor interface Isensor 
        void  calibrateForAltitude(float altitude);
        void  setLocalAltitude(float altitude);
        float getCelsius();      // implementation of the pure virtual method declared in sensor interface Isensor
        float getRelHumidity();
        float getDewPoint();
        float getLocalPressure();
        float getLocalAltitude();
        void  printSensorData();  // implementation of the pure virtual method declared in sensor interface Isensor
        void  getSensorData(SensorData& sensorData);  // implementation of the pure virtual method declared in sensor interface Isensor

    private:
        void  _readSensor();
        float _calculateDewPoint();
        float _calculateNpLocal();

        uint8_t _i2cAddress;
        SensorData& _sData;
};
