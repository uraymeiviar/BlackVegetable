
const char DeviceName[] = "BlackVegetableOS 1.0";
#include "LCD.h"
#include "Timing.h"
#include "Networking.h"
#include "SensorDHT.h"
#include "SensorLight.h"
#include "ClientRelay.h"
#include "WebServer.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  initLCD();
  initTiming();
  initNetworking();

  initSensorDHT();
  initSensorLight();
  initRelayClient();
  initWebServer();
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
