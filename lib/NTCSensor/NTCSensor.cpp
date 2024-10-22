/**
 * Class        NTCSensor
 * Author       2024-10-18 Charles Geiser
 * 
 * Purpose      Implements a class to measure temperature by means of
 *              a voltage divider with an NTC resistor.   
 * 
 * Board        ESP32 DoIt DevKit V1
 * 
 * Wiring       ---+-- Vcc                            ---+--- Vcc
 *                 |                                     |
 *                .-.                                   .-. 
 *                | | Rs                                |/| NTC
 *                '-'                                   '-' 
 *                 |                                     | 
 *                 +---> Vin analog input                +---> Vin analog input
 *                 |                                     |   
 *                .-.                                   .-.
 *                |/| NTC                               | | Rs
 *                '-'                                   '-'
 *                 |                                     |
 *              ---+--- GND                           ---+--- GND
 * 
 * Remarks
 * References   
 */ 
#include "NTCsensor.h"


/** 
 * Initializes the sensor and reads in the measurement data. 
 * If no sensor is available, the program is terminated and 
 * an error message is displayed. 
 */
void NTCSensor::setup()
{
    _sData.sensorPin  = _adc.pin;
    pinMode(_adc.pin, INPUT);
    analogSetAttenuation(_adc.att);
    _sData.Roo = _ntc.Ro * exp(-(double)_ntc.beta / (_sData.To - _sData.Tabs)); // calculate the resistance of the NTC for T --> oo
    delay(1000);
    readSensor();   
    log_i("==> done");
}


/**
 * Read the sensor and calculate the temperature 
 * in Fahrenheit and in Kelvin.
 */
void NTCSensor::readSensor()
{
    _sData.analogValue = analogRead(_adc.pin);
    _sData.v = (_adc.Vref - _adc.Voff) / (double)_adc.Amax;
    _sData.vin = (_sData.analogValue * _sData.v) + _adc.Voff;
    _sData.k = _sData.vin / ( _adc.Vcc - _sData.vin);
    if (! _adc.ntcToGround) _sData.k = 1.0 / _sData.k;
    _sData.Rt = (double)_ntc.Rs * _sData.k;
    _sData.tKelvin = (double)_ntc.beta / log(_sData.Rt/_sData.Roo);  // Calculate  T from Rt, Roo and BETA
    _sData.tCelsius = _sData.tKelvin + _sData.Tabs;                  // Convert Kelvin to Celcius
    _sData.tFahrenheit = _sData.tCelsius * 9.0 / 5.0 + 32.0;         // Convert Celcius to Fahrenheit      
}


/**
 * Returns temperature in °C
 * To be called after a readSensor()
 */
float NTCSensor::getCelsius()
{
    return _sData.tCelsius;
}


/**
 * Returns a reference to the sensor data struct
 */
SensorData&  NTCSensor::getDataReference()
{
    return(_sData);
}


void NTCSensor::setNTCbeta(uint16_t beta)
{
    _ntc.beta = beta;
}

/**
 * Print sensor parameters to monitor
 * 
 * beta        material constant of NTC
 * Ro          resistance of NTC at nominal temoperature of 25°C
 * Rs          series resistanceconnected to NTC
 * Roo         calculated resistance for temperature --> oo
 * k           converion factor (analogMax/analogValue) - 1
 * To          nominal temperature To = 25 °C 
 * Tabs        absolute temperature -273.15 °C
 * analogMax   maximum value of Arduinos ADC
 * ntcToGround true if NTC is connected to ground, if connected to Vcc false
 */
void NTCSensor::printParams()
{
  Serial.printf(R"(--- NTC Parameters ---
beta        %d
Ro         %d
Rs         %d
Roo      %7.5f
To       %7.2f °C
Tabs     %7.2f °C
--- ADC Parameters ---
Pin         %d
Analog Max  %d
NTC to      %s
Vcc        %5.0f mV
Vref       %5.0f mV
Voff       %5.0f mV

)",
_ntc.beta, _ntc.Ro, _ntc.Rs, _sData.Roo, _sData.To, _sData.Tabs, 
_adc.pin, _adc.Amax, _adc.ntcToGround ? "GND" : "Vcc", _adc.Vcc, _adc.Vref, _adc.Voff );
}

/**
 * Print sensor readings to monitor
 * 
 * analogValue  reading from analog pin
 * Rt           calculated resistanc of NTC at temperature T
 * Tc           calculated temperature in °-Celsius
 * Tf           calculated temperature in °-Fahrenheit
 * Tk           calculated temperature in °-Kelvin
 */
void NTCSensor::printData()
{
  readSensor();
  Serial.printf(R"(--- Sensor Values ---
Analog Value %d
v        %7.5f
Vin      %7.0f mV
k        %7.5f
Rt         %5.0f
Tc         %5.1f °C
Tf         %5.1f °F
Tk         %5.1f °K

)", _sData.analogValue, _sData.v, _sData.vin, _sData.k, _sData.Rt, 
    _sData.tCelsius, _sData.tFahrenheit, _sData.tKelvin);
}
