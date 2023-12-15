#include <Arduino.h>

#include <config.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <utils.h>
#include <wifi_com.h>
#include <web_com.h>
#include <mqtt_com.h>
#include <scroller.h>

#ifdef ENABLE_OTA
#include <ota.h>
#endif

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

startEEPROM();    
  delay(10);

  startWiFi();
  startMQTT();
  startWeb();


#ifdef ENABLE_OTA
    startOTA();
#endif

    startScroller();
    Serial.println("setup done.");
    Serial.flush();

}

void loop() {
  #ifdef ENABLE_OTA
  handleOTA();
  #endif
  handleMQTT();
  handleWeb();
  handleScroller();
  #ifdef AUTO_RESTART_SECONDS
   uint16_t timePassed = round(millis()/1000);
   if (timePassed>AUTO_RESTART_SECONDS) {
    delay(2000);
    ESP.restart();
   }
  #endif
}

