/**
 * Program      A thermostat with a sensor interface wich enables the use of different sensors.
 * 
 * Author       2024-10-18 Charles Geiser
 * 
 * Purpose      The program shows how a NTC resistor is used to measure temperature and it 
 *              also implements the functionality of a thermostat. That means the method
 *              turnHeatingOff() is called when temperature exceeds the set high tempereature 
 *              limit or the method turnHeatingOn() is called when tempereature falls below 
 *              the set low temperature limit. 
 *   
 *              The thermostat updates the measured values every msRefresh milliseconds and the 
 *              user supplied method processData() is called.
 * 
 
 * 
 * Board        ESP32 DoIt DevKit V1 
 * 
 * Wiring                                                                                  Solid State Relay
 *                                                                                            .---------.      
 *                                                                        5V(Vin of ESP32) ---|+       ~|------------- L 
 *           .------------------.                   .----------.                              |   SSR   |    .-.        ~230V 
 *           |    NTC ELEGOO    |-- GND --> GND   --|  ESP32   |-- 2 heartbeat           .----|-       ~|---( L )----- N
 *       .---| Ro 10K / B 2800  |-- Vcc --> 3.3V  --|          |                         |    `---------´    `-´
 *      o    | Rs 10K           |-- out --> 34    --|          |                     |¦--'                  Load
 *       `---| NTC to GND       |                   |          |                     |¦<-.  2N7000   
 *           '------------------´                   |          |--  4 --> heating ---|¦--|  N-CH MOSFET        
 *                                                  |          |                         | 
 *                                                  ´----------´                   GND --+--  
 * 
 *      The solid state relay should switch on at a voltage of 3V, but direct control with an output of the ESP32 
 *      does not work. Therefore the relay is connected to Vin (5 V) and switched on with the N-Channel MOSFET against GND 
 *                .---.
 *              /_____/|
 *              | 2N  ||
 *              | 7000||
 *              |_____|/
 *               | | |
 *               S G D
 *  
 * Remarks                
 * 
 * References
 */

#include <Arduino.h>
#include "Thermostat.h"

#define PIN_THERMOSTAT  GPIO_NUM_4   // pin to turn on/off the heating
#define PIN_HEARTBEAT   LED_BUILTIN  // indicates normal operation with 1 beat/sec 
#define PIN_ADC         GPIO_NUM_34

extern void heartbeat(uint8_t pin, uint8_t nBeats, uint8_t t, uint8_t duty);
extern void doMenu();
extern void setLowerLimit();
extern void setUpperLimit();
extern void setTempDelta();
extern void setInterval();
extern void toggleThermostat();
extern void showValues();
extern void showMenu();

bool heatingIsOn = false; 

// Forward declaration of the handler functions for the thermostat
void processData();
void turnHeatingOn();
void turnHeatingOff();

//                       Rs     Ro    beta     
ParamsNTC ntcRs10k  = { 10000, 10000, 2800 };
ParamsNTC ntcRs20k  = { 20000, 10000, 2800 };

ParamsADC adcEsp32_0   = { PIN_ADC, true, 4095, ADC_0db,   3300.0, 1100.0,  65.0 };
ParamsADC adcEsp32_2_5 = { PIN_ADC, true, 4095, ADC_2_5db, 3300.0, 1300.0,  65.0 };
ParamsADC adcEsp32_6   = { PIN_ADC, true, 4095, ADC_6db,   3300.0, 1800.0,  90.0 };
ParamsADC adcEsp32_11  = { PIN_ADC, true, 4095, ADC_11db,  3300.0, 3200.0, 130.0 };

SensorData sensorData; // holds measured and calculated sensor values (see SensorData.h)
NTCSensor  sensor(ntcRs10k, adcEsp32_11, sensorData); // sensor used for thermostat
Thermostat thermostat(sensor, processData, turnHeatingOn, turnHeatingOff);

// Called when refresh intervall expires
void processData()
{
  sensor.readSensor();
  sensor.printParams();
  sensor.printData();
  thermostat.printSettings(); 
}

// Called as onLowTemp() when the temperature falls below the set limit
void turnHeatingOn()
{
  if (! heatingIsOn)
  {
    log_i("===> switch on heating, it is: %s", heatingIsOn ? "on" : "off");
    digitalWrite(PIN_THERMOSTAT, HIGH);
    heatingIsOn = true;
  }
}

// Called as onHighTemp() when the temperature rises above the set limit
void turnHeatingOff()
{
  if (heatingIsOn)
  {
  log_i("===> switch off heating, it is: %s", heatingIsOn ? "on" : "off");
  digitalWrite(PIN_THERMOSTAT, LOW);
  heatingIsOn = false;
  }
}


void initOutputPins()
{
  pinMode(PIN_HEARTBEAT, OUTPUT);
  pinMode(PIN_THERMOSTAT, OUTPUT);
  digitalWrite(PIN_THERMOSTAT, LOW);
  log_i("==> done");  
}


void initThermostat()
{
  thermostat.setup();
  thermostat.enable();
  log_i("==> done");
}


void setup() 
{
  Serial.begin(115200);

  initOutputPins(); 
  initThermostat();
  showMenu();
}

void loop() 
{
  if(Serial.available()) doMenu();
  thermostat.loop();
  heartbeat(PIN_HEARTBEAT, 1, 1, 5);
}
