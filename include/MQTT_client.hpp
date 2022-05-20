#ifndef MQTT_HPP
#define MQTT_HPP

#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt_config.h"

WiFiClient espClient;
PubSubClient client(espClient);

char msg[25];
long count=0;

void callback(char* topic, byte* payload, unsigned int length)
{
	String incoming = "";
	Serial.print("Message received from -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
	Serial.println("Message -> " + incoming);
}

void reconnect()
{
	while (!client.connected()) {
		Serial.print("Trying MQTT connection...");
		// Create client ID
		String clientId = "ESP32_";
		clientId += String(random(0xffff), HEX);

		// Try to connect
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
			Serial.println("Connected!");
			// Subscribe
			if(client.subscribe(root_topic_subscribe)){
        Serial.println("Subscription ok");
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