/**
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
//#define MY_RF24_IRQ_PIN 6
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 10

// select the input pin for the battery sense point
// int BATTERY_SENSE_PIN = A0;

// Noeud de l'objet / Object Node
#define MY_NODE_ID 3

#include <MySensors.h>
#include <NewRemoteTransmitter.h>

/**
* Declare constants like pin settings etc.
*/
// Create a transmitter on address 14732658, using digital pin 3 to transmit,
// with a period duration of 260ms (default), repeating the transmitted
// code 2^2=4 times.
#define REMOTE_CODE 14732658
#define TRMITR_PIN 3
#define TRMITR_DUR 260
#define TRMITR_CNT 2

#define CHILD_ID_REMOTE_0 0 // Declare the DEVICE A as a separate switch
#define CHILD_ID_REMOTE_1 1
#define CHILD_ID_REMOTE_2 2
//#define CHILD_ID_TEMP 0
//#define CHILD_ID_HUM 1
//#define CHILD_ID_BARO 2

//#define SLEEP_NODE true // Mettre à True pour activer la mise en sommeil (Sleep Mode) / True to activate Sleep Mode
//unsigned long SLEEP_TIME = 900000; // Temps de mise en sommeil (en ms) / Sleep time between reads (in milliseconds)

//MyMessage msgHum(CHILD_ID_HUM, V_HUM);
//MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
//MyMessage msgPres(CHILD_ID_BARO, V_PRESSURE);
MyMessage msgDeviceA(CHILD_ID_REMOTE_0, V_LIGHT );
MyMessage msgDeviceB(CHILD_ID_REMOTE_1, V_LIGHT );
MyMessage msgDeviceC(CHILD_ID_REMOTE_2, V_LIGHT );

//#define barometerAltitude 50

NewRemoteTransmitter transmitter(REMOTE_CODE, TRMITR_PIN, TRMITR_DUR, TRMITR_CNT);  // Construct Object

void setup() {
// Nothing to setup
}

void presentation()  {

  // Présenttion du sketch / Send the sketch version information to the gateway and Controller
  sendSketchInfo("RF433 Bridge", "1.0");

  // Déclaration des capteurs attachés au noeud
  //present(CHILD_ID_TEMP, S_TEMP);
  //present(CHILD_ID_HUM, S_HUM);
  //present(CHILD_ID_BARO, S_BARO);
  present(CHILD_ID_REMOTE_0, S_LIGHT ); // Present Device A to the gateway
  present(CHILD_ID_REMOTE_1, S_LIGHT ); // Present Device B to the gateway
  present(CHILD_ID_REMOTE_2, S_LIGHT ); // Present Device C to the gateway
  sendBatteryLevel(100); // It just looks better in Domoticz if we let Domoticz no we're running on 100% power.
}

void loop() {
  // I have nothing to do if only attach a transmitter.
  wait(1000); // process incoming message whenever it is possible
  }

void receive(const MyMessage &message) {

   // We only expect one type of message from controller. But we better check anyway.
   #ifdef MYDEBUG
   Serial.println("message");
   #endif

   if (message.type==V_LIGHT) {
   #ifdef MYDEBUG
   Serial.println("Command received for local node " + MY_NODE_ID + " on id " + (String)message.sensor + " request to turn " + (message.getBool()?"on":"off") );
   #endif

     if ( message.sensor >= 0 && message.sensor <= 2 ) { // Impuls/Action system only allows device '0' to '2'
        transmitter.sendUnit(message.sensor, message.getBool());
     }
   }
}


// ==============
// Examples
// ==============

// Switch unit 1 and 2 On
// transmitter.sendUnit(0, true);
// transmitter.sendUnit(1, true);
// Switch unit 1 and 2 Off
// transmitter.sendUnit(0, false);
// transmitter.sendUnit(1, false);

// Switch all devices in the group off
// transmitter.sendGroup(false);
// Switch all devices in the group on
// transmitter.sendGroup(true);

// Set unit 1 to dim-level 3 (range 0-15)
// transmitter.sendDim(1, 3);
// Set unit 1 to dim-level 15, full brightness.
// transmitter.sendDim(1, 15);
