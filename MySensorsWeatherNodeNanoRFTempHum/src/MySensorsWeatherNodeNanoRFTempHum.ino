/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - PROJETS DIY
 *
 * DESCRIPTION
 * Example sketch showing how to measue light level using a LM393 photo-resistor
 * http://www.mysensors.org/build/light
 */

// Active le mode debug / Enable debug prints to serial monitor
#define MY_DEBUG

// Mode Radio / Enable and select radio type attached
#define MY_RADIO_RF24
//#define MY_RADIO_RFM69

#define MY_RF24_PA_LEVEL RF24_PA_LOW

// Define NRF24 PIN
#define MY_RF24_CE_PIN 10
#define MY_RF24_CS_PIN 9

// Noeud de l'objet / Object Node
#define MY_NODE_ID 2

#include <MySensors.h>
#include <BME280I2C.h>
#include <Wire.h>
//#include <EnvironmentCalculations.h>

#define CHILD_ID_TEMP 0
#define CHILD_ID_HUM 1
//#define CHILD_ID_BARO 2
#define SLEEP_NODE true // Mettre à True pour activer la mise en sommeil (Sleep Mode) / True to activate Sleep Mode
unsigned long SLEEP_TIME = 900000; // Temps de mise en sommeil (en ms) / Sleep time between reads (in milliseconds)

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
//MyMessage msgPres(CHILD_ID_BARO, V_PRESSURE);

//#define barometerAltitude 50

  BME280I2C bme;  // Construct Object

void setup() {

  while(!Serial) {} // Wait
  Wire.begin();
  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(10000);
  }

  //bme.chipID(); // Deprecated. See chipModel().
  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}

void presentation()  {
  // Présenttion du sketch / Send the sketch version information to the gateway and Controller
  sendSketchInfo("Sonde Meteo", "1.0");

  // Déclaration des capteurs attachés au noeud
  present(CHILD_ID_TEMP, S_TEMP);
  present(CHILD_ID_HUM, S_HUM);
//  present(CHILD_ID_BARO, S_BARO);
}

void loop() {

  float temp(NAN), hum(NAN), pres(NAN);
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);
//  EnvironmentCalculations::AltitudeUnit envAltUnit  =  EnvironmentCalculations::AltitudeUnit_Meters;
//  EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;
  bme.read(pres, temp, hum, tempUnit, presUnit);
  delay(100);
  bme.read(pres, temp, hum, tempUnit, presUnit);
//  seaLevel = EnvironmentCalculations::EquivalentSeaLevelPressure(barometerAltitude, temp, pres, envAltUnit, envTempUnit);
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
  Serial.print("\t\tHumidity: ");
  Serial.print(hum);
  Serial.print("% RH\n");
//  Serial.print("\t\tPressure: ");
//  Serial.print(seaLevel);
//  Serial.println(" hPa");

  send(msgTemp.set(temp, 1));
  send(msgHum.set(hum, 1));
//  send(msgPres.set(seaLevel, 1));

  if (SLEEP_NODE) {
    Serial.println("Sleep");
    sleep(SLEEP_TIME);
  } else {
    delay (SLEEP_TIME);
  }
}
