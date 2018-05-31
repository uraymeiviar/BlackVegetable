
const char DeviceName[] = "BlackVegetableOS 1.0";
#include "LCD.h"
#include "Timing.h"
#include "Networking.h"
#include "WebServer.h"
#include "SensorDHT.h"
#include "SensorLight.h"
#include "ClientRelay.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  initLCD();
  initTiming();
  initNetworking();
  initWebServer();
  initSensorDHT();
  initSensorLight();
  initRelayClient();
}

void loop() {
  loopTimingBegin();
  {
    loopNetworking();
    loopSensorDHT();
    loopSensorLight();
    loopRelayClient();
    loopWebServer();
  }
  loopTimingEnd();
}
