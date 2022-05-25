#include <Arduino.h>
#include "config.h"
#include "WiFi_client.hpp"
#include "MQTT_client.hpp"
#include "rfid.hpp"
#include "leds.hpp"

RFID rfid;
LEDs led;

int startTime;

void setup() {
  disableCore0WDT();
  //disableCore1WDT(); //Only with espidf
  disableLoopWDT();

  Serial.begin(115200); // Initialize serial communications with the PC

  wifiConnection();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  rfid.init();
  led.init();

  led.led1ON(GREEN);
  delay(10);
  startTime = millis();
}

void loop() {

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


  // Temperature sensor lecture
  if(millis()-startTime>10000){
    String randomNum = String(random(20, 25));
    randomNum.toCharArray(msg, 25);
    client.publish(root_topic_publish, msg);
    startTime = millis();
  }
  

  //Check if there's any booking
  if(booked==false){
    led.led1ON(GREEN);
  }else{
    led.led1ON(RED);
  }

  //Change RFID valid key
  rfid.setvalidKey(user);

  if(rfid.readCard()){
    delay(100);
    if(rfid.validKey()){
      //TODO open door
      led.led2BlinkLoop(GREEN);
      Serial.println("Door opened!");
      if (client.connected()){
        String str = "Hello";
        str.toCharArray(msg,25);
        client.publish(root_topic_publish, msg);
        delay(300);
      }
    }else{
      led.led2BlinkLoop(RED);
      Serial.println("Incorrect user");
    }
  }

  delay(20);
  client.loop();
}