#include <Arduino.h>
#include "config.h"
#include "WiFi_client.hpp"
#include "MQTT_client.hpp"
#include "rfid.hpp"

RFID rfid;
bool rfidInt = false;

/**
 * MFRC522 interrupt serving routine
 */
void IRAM_ATTR readCard(){ //Declarar en main para poder llamar a rf.read
  //Serial.println("Interrupt");
  rfidInt = true; //intentar leer tarjeta rfid dentro de la interrupcion
  rfid.readUID(); //Modificar, optimizar interrupcion para que sea el menor tiempo posible (solo leer uid)
}

void setup() {
  disableCore0WDT();
  //disableCore1WDT(); //Only with espidf
  disableLoopWDT();

  Serial.begin(115200); // Initialize serial communications with the PC

  wifiConnection();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  rfid.init();
  pinMode(IRQ_PIN, INPUT_PULLUP); //setup the IRQ pin
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), readCard, FALLING); //Activate the interrupt
}

void loop() {
  rfid.activateRec(); //The receiving block needs regular retriggering

  if(!client.connected()){
    reconnect();
  }

  // MQTT message example
  // if (client.connected()){
  //   String str = "counter -> " + String(count);
  //   str.toCharArray(msg,25);
  //   client.publish(root_topic_publish,msg);
  //   count++;
  //   delay(300);
  // }

  if(rfidInt){
    delay(100);
    rfidInt = false;
    if(rfid.validKey()){
      //TODO open door
      Serial.println("Door opened!");
    }else{
      Serial.println("Incorrect user");
    }
  }

  client.loop();
}