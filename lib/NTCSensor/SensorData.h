#pragma once

// SensorData stores the values read by the sensor 
// and calculated on the basis of the measurements.
// This data structure depends on the capabilities of 
// the sensor and must be adapted accordingly.

using SensorData = struct sDat
{
    float    tCelsius;
    float    tFahrenheit;
    float    tKelvin;
    double   Roo;           // resistance for T --> oo
    double   k;             // k = Vin / (Vcc - Voff)
    double   v;             // v = (Vref - Voff) / analogMax
    double   vin;           // input voltage on ADC-pin
    double   Rt;            // calculated resistance at temperature T
    uint16_t analogValue;   // measured analog value Aval
    uint8_t  sensorPin;
    const double  To   = 25.0;    // nominal temperature
    const double  Tabs = -273.15; // absolute temperature 
};