/**
 * Class        NTCsensor
 * 
 * Author       2024-10-18 Charles Geiser (https://www.dodeka.ch)
 * 
 * Purpose      Declaration of the class NTCsensor
 * 
 */  
#pragma once

#include <Arduino.h>
#include "SensorData.h"
#include "ISensor.h"


using ParamsNTC = struct parmsNtc { uint16_t Rs; uint16_t Ro; uint16_t beta; };
using ParamsADC = struct parmsAdc { uint8_t pin; bool ntcToGround; uint16_t Amax; adc_attenuation_t att; double Vcc; double Vref; double Voff; };


class NTCSensor : public ISensor
{
  public:
    NTCSensor(ParamsNTC &ntc, ParamsADC &adc, SensorData& sensorData )  : 
        _ntc(ntc), _adc(adc), _sData(sensorData)
      {
        pinMode(_adc.pin, INPUT);
        analogSetAttenuation(_adc.att);
        _sData.Roo = _ntc.Ro * exp(-(double)_ntc.beta / (_sData.To - _sData.Tabs)); // calculate the resistance of the NTC for T --> oo
      }

    void  setup() override;
    void  readSensor() override;  // read the sensor and update the measured values
    float getCelsius() override;  // return the temperature in °C
    void  printData()  override;   // print the measured values
    void  printParams(); // print the sensors parameters
    void  setNTCbeta(uint16_t beta);
    SensorData& getDataReference() override;

  private:
    ParamsNTC&  _ntc;
    ParamsADC&  _adc;
    SensorData& _sData;      
};
