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
#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
/***********************************************************************
* Informations
***********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
//https://github.com/JoaoLopesF/RemoteDebug
//https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
//https://github.com/JoaoLopesF/RemoteDebug/blob/master/examples/RemoteDebug_Basic/RemoteDebug_Basic.ino
//https://github.com/JoaoLopesF/RemoteDebugApp
/***********************************************************************
* Declarations
***********************************************************************/


/***********************************************************************
* Constant
***********************************************************************/

/***********************************************************************
* Global Variable
***********************************************************************/
RemoteDebug Debug;
/***********************************************************************
* local Variable
***********************************************************************/

uint8_t wlan_connection_retry_cnt = 0;
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;

/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Local Funtions
***********************************************************************/
void foo();



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
    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
    WiFi.hostname(hostNameWifi);
    if (MDNS.begin(HOST_NAME)) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }

    MDNS.addService("telnet", "tcp", 23);

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

    // Initialize RemoteDebug

	  Debug.begin(HOST_NAME); // Initialize the WiFi server

    Debug.setResetCmdEnabled(true); // Enable the reset command

	  Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	  Debug.showColors(true); // Colors

    // End off setup

    Serial.println("* Arduino RemoteDebug Library");
    Serial.println("*");
    Serial.print("* WiFI connected. IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("*");
    Serial.println("* Please use the telnet client (telnet for Mac/Unix or putty and others for Windows)");
	  Serial.println("* or the RemoteDebugApp (in browser: http://joaolopesf.net/remotedebugapp)");
    Serial.println("*");
    Serial.println("* This sample will send messages of debug in all levels.");
    Serial.println("*");
	  Serial.println("* Please try change debug level in client (telnet or web app), to see how it works");
    Serial.println("*");


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
     // Each second

    if ((millis() - mLastTime) >= 1000) {

        // Time

        mLastTime = millis();

        mTimeSeconds++;

        // Blink the led

        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

        // Debug the time (verbose level)

        debugV("* Time: %u seconds (VERBOSE)", mTimeSeconds);

        if (mTimeSeconds % 5 == 0) { // Each 5 seconds

            // Debug levels

			debugV("* This is a message of debug level VERBOSE");
			debugD("* This is a message of debug level DEBUG");
			debugI("* This is a message of debug level INFO");
			debugW("* This is a message of debug level WARNING");
			debugE("* This is a message of debug level ERROR");

			// Call a function

			foo();
        }
     }

    // RemoteDebug handle

    Debug.handle();

    // Give a time for ESP

    yield();
}


// Function example to show a new auto function name of debug* macros

void foo() {

  uint8_t var = 1;

  debugV("this is a debug - var %u", var);
  debugV("This is a println");
}
