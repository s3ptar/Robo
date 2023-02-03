/***********************************************************************
*! \file:                   main.cpp
*  \projekt:                Robo AZ_Envy
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/


/***********************************************************************
* Includes
***********************************************************************/
#include <Arduino.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" 
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "user_config.h"
#include "config.h"
#include "stdio.h"
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
//https://github.com/JoaoLopesF/RemoteDebug
//https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
/***********************************************************************
* Declarations
***********************************************************************/


/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/

/***********************************************************************
* local Variable
***********************************************************************/

uint8_t wlan_connection_retry_cnt = 0;

/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Local Funtions
***********************************************************************/




/***********************************************************************
*! \fn          void setup()
*  \brief       setup routine
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void setup() {
    // put your setup code here, to run once:
    wlan_connection_retry_cnt = 0;
    Serial.begin(115200);
    Serial.println("Booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(wlan_tonnection_timeout);
        if (wlan_connection_retry_cnt++ > wlan_connection_retry)
            ESP.restart();
  }

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);
  
    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");
  
    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");
  
    ArduinoOTA.onStart([]() {
        Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());


}


/***********************************************************************
*! \fn          void loop()
*  \brief       setup main loop funtion
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
void loop() {
  // put your main code here, to run repeatedly:
    //upload task
    ArduinoOTA.handle();
}