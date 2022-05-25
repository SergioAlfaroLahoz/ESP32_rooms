#ifndef MQTT_HPP
#define MQTT_HPP

#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);

char msg[25];
long count=0;

bool booked = false;
uint8_t user = 0;

const char *room1 = "room1";
const char *room1user = "room1/user";

const char *user1 = "user1";
const char *user2 = "user2";
const char *user3 = "user3";
const char *user4 = "user4";
const char *user5 = "user5";

void callback(char* topic, byte* payload, unsigned int length)
{
	String incoming = "";
	Serial.print("Message received from -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	Serial.println("Message -> " + incoming);

	if(strcmp(topic, room1)==0){
		if(incoming=="booked"){
			booked = true;
		}else if(incoming=="available"){
			booked = false;
		}
	}else if(strcmp(topic, room1user)==0){
		if(incoming=="user1"){
			user = 1;
		}else if(incoming=="user2"){
			user = 2;
		}else if(incoming=="user3"){
			user = 3;
		}else if(incoming=="user4"){
			user = 4;
		}else if(incoming=="user5"){
			user = 5;
		}
	}
	incoming.trim();
}

void reconnect()
{
	while (!client.connected()) {
		Serial.print("Trying MQTT connection...");
		// Create client ID
		String clientId = "ESP32_";
		clientId += String(random(0xffff), HEX);

		// Try to connect
		if (client.connect(clientId.c_str())) {
			Serial.println("Connected!");
			// Subscribe
			if(client.subscribe(root_topic_subscribe)){
        		Serial.println("Subscription 1 ok");
      		}else{
        		Serial.println("Subscription failed");
      		}
			if(client.subscribe(user_topic_subscribe)){
        		Serial.println("Subscription 2 ok");
      		}else{
        		Serial.println("Subscription failed");
      		}
		} else {
			Serial.print("Error -> ");
			Serial.print(client.state());
			Serial.println(" Let's try again in 5 seconds...");
			delay(5000);
		}
	}
}


#endif