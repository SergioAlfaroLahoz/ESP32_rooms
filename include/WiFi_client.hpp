#ifndef WIFI_HPP
#define WIFI_HPP

#include <WiFi.h>

void wifiConnection()
{
    delay(10);

	Serial.println();
	Serial.print("Connecting to ssid: ");
	Serial.println(wifi_ssid);

	WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    // WiFi.config(ip, gateway, subnet);

	while (WiFi.status() != WL_CONNECTED) {
		delay(300);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Connected to WiFi network!");
	Serial.print("IP: ");
	Serial.println(WiFi.localIP());
}


#endif