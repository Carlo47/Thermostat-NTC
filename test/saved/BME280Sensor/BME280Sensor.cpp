/**
 * Class        BME280Sensor.cpp
 * Author       2022-01-24 Charles Geiser
 * 
 * Purpose      A class to measure temperature, relative humidity and air pressure
 *              by means of a BME280 sensor. It also calculates the dewpoint and 
 *              the local standard air pressure when local altitude is known.
 *              If the local altitude is known, the sensor can be calibrated to 
 *              this height and used as an altimeter.  
 * 
 * Board        ESP32 DoIt DevKit V1
 * Remarks
 * References   
 */ 
#include "BME280sensor.h"

/** Initializes the sensor and reads the measurement data 
 * or gives an error message if the sensor was not found 
 * and stops the program
*/
void BME280Sensor::setup()
{
    char buf[40];
    int tries = 5;
    while (! begin(_i2cAddress) && (tries > 0))
    {
        delay(1000);
        tries--;
    }
    if(tries <= 0)
    {
        snprintf(buf, sizeof(buf),"BME280 not found at i2c address %#x", _i2cAddress);
        Serial.println(buf);
        while(true) delay(10000UL);
    }
    _readSensor();
}

/**
 * Adjusts the normal pressure at sea level so that the 
 * measured local air pressure corresponds to the local altitude.
 * 
 * altitude     local altitude in m.a.s.l. 
 */
void BME280Sensor::calibrateForAltitude(float altitude)
{
    char buf[128];
    _sData.pSeaLevel = seaLevelForAltitude(altitude, _sData.pLocal);
    Serial.println("Calibrate for Altitude:");
    snprintf(buf, sizeof(buf), "Altitude = %6.1f, p0 = %6.1f, pLocal = %6.1f", (double)altitude, (double)_sData.pSeaLevel, (double)_sData.pLocal);
    Serial.println(buf);
}

/**
 * Set the local altitude in meter above sea level 
 */
void BME280Sensor::setLocalAltitude(float altitude)
{
    _sData.altLocal = altitude;
}

/**
 * Calculate dew point in °C from temperature and humidity
 */
float BME280Sensor::_calculateDewPoint(void) 
{
    float k;
    k = log(_sData.relHumidity/100) + (17.62 * _sData.tCelsius) / (243.12 + _sData.tCelsius);
    return 243.12 * k / (17.62 - k);
}

/**
 * Calculate local normal air pressure at given altitude
 * 
 * kapa = 1.235
 * K0 = kapa / (kapa -1) = 5.255
 * T0 = 288.15 °K = 15 °C (According an international convention this temperatur is used as reference)
 * gradT = 0.0065 K/m (Temperature gradient Kelvin / Meter
 * P0 = 1013.25 hPa (accord. int. convention, P0 = pSeaLevel))
 * H0 = T0 / gradT = 44330 m
 * pLocal = P0 * (1 - h/H0) ^ K0
 */ 
float BME280Sensor::_calculateNpLocal()
{
    _sData.npLocal = 1013.25 * pow( (1.0 - _sData.altLocal / 44330.0), 5.255);
    return _sData.npLocal;
}

/**
 * Read the sensor and calculate local normal pressure
 * and dew point
 */
void BME280Sensor::_readSensor()
{
    _sData.pLocal      = readPressure() / 100.0;
    _sData.relHumidity = readHumidity();
    _sData.tCelsius    = readTemperature();
    _sData.tFahrenheit = _sData.tCelsius * 9.0 / 5.0 + 32.0;
    //_altLocal    = readAltitude(_pSeaLevel); // altitude is set by user
    _sData.npLocal     = _calculateNpLocal();
    _sData.dewPoint    = _calculateDewPoint();      
}

float BME280Sensor::getCelsius()
{
    _readSensor();
    return _sData.tCelsius;
}


float BME280Sensor::getRelHumidity()
{
    _readSensor();
    return _sData.relHumidity;
}

float BME280Sensor::getDewPoint()
{
    _readSensor();
    return _sData.dewPoint;
}

float BME280Sensor::getLocalPressure()
{
    _readSensor();
    return _sData.pLocal;    
}

float BME280Sensor::getLocalAltitude()
{
    _readSensor();
    return _sData.altLocal;    
}

void  BME280Sensor::getSensorData(SensorData &sensorData)
{
    _readSensor();
    sensorData.tCelsius    = _sData.tCelsius;
    sensorData.relHumidity = _sData.relHumidity;
    sensorData.pLocal      = _sData.pLocal;
    sensorData.dewPoint    = _sData.dewPoint;
    sensorData.npLocal     = _sData.npLocal;
    sensorData.altLocal    = _sData.altLocal;
    sensorData.pSeaLevel   = _sData.pSeaLevel;
}

void BME280Sensor::printSensorData()
{
    char buf[232];
    _readSensor();
    // printf("Temperature      %6.1f °C\n",    _tCelsius);
    // printf("Dewpoint         %6.1f °C\n",    _dewPoint);
    // printf("Humidity         %6.1f %%rH\n",  _relHumidity);
    // printf("Local pressure   %6.1f hPa\n",   _pLocal);
    // printf("Local altitude   %6.1f m.a.s.l.\n", _altLocal);
    // printf("nP at altitude   %6.1f hPa\n", _npLocal);
    // printf("nP at sea level  %6.1f hPa\n", _pSeaLevel);
    snprintf(buf, sizeof(buf), R"(---   Sensor Readings   ---
Temperature      %6.1f °C
Dewpoint         %6.1f °C
Humidity         %6.1f %%rH
Local pressure   %6.1f hPa
Local altitude   %6.1f m.a.s.l.
nP at altitude   %6.1f hPa
nP at sea level  %6.1f hPa
)", (double)_sData.tCelsius, (double)_sData.dewPoint, (double)_sData.relHumidity, (double)_sData.pLocal, 
       (double)_sData.altLocal, (double)_sData.npLocal,  (double)_sData.pSeaLevel);
Serial.println(buf);
}
