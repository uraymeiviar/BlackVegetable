
/*

light          = +5v G DO  AO
                       24 A15

air temp+hum   = +5v G DO
                       23

water temp 1   = +5v G DO 4.75k
                       22 (one-wire)

water temp 2   = +5v G DO 4.75k
                       22 (one-wire)

ph Sensor      = +5v G  AO  AO  DO
                       A12 A11  25

Buzzer         =     G DO
                       26

flow sensor    = +5v G PWM
                       52

water level  1 = +5v G  AO
                       A14

water level  2 = +5v G  AO
                       A13
 */

 // relay 13 = humidifier
 // relay 15 = fogger
 // relay  4 = water pump
 // relay 11 = cooler-1
 // relay  9 = cooler-2
 // relay  7 = circulation fan

 #define RELAY_FOGGER    14
 #define RELAY_CFAN       6
 #define RELAY_WATERPUMP  3
 #define RELAY_COOLER_1  10
 #define RELAY_COOLER_2   8
 #define RELAY_HUMID     12
 #define RELAY_AC_1      15
 #define RELAY_AC_2      13
 
const char DeviceName[] = "BlackVegetableOS 1.0";
#include "LCD.h"
#include "Timing.h"
#include "Networking.h"
#include "WebServer.h"
#include "SensorDHT.h"
#include "SensorLight.h"
#include "SensorPH.h"
#include "SensorTempWater.h"
#include "SensorLevelWater.h"
#include "SensorFlowWater.h"
#include "SensorPPMWater.h"
#include "ClientRelay.h"

unsigned long lastLogicMs = 0;
int foggerOnDuration = 60;
int foggerOffDuration = 60*2;
int humidifierOnDuration = 30;
int humidifierOffDuration = 60*10;

void setup() {
  lastLogicMs = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  initLCD();
  initTiming();
  initNetworking();
  initWebServer();
  initSensorDHT();
  initSensorLight();
  initSensorPH();
  initSensorWaterTemp();
  initSensorWaterLevel();
  initSensorWaterFlow();
  initSensorWaterPPM();
  initRelayClient();
}

unsigned long foggerStartMs = 0;
unsigned long foggerEndMs = 0;
void loopLogicFogger(){
  if(foggerStartMs == 0){
    foggerStartMs = millis();
    foggerEndMs = millis();
    relaySetState(RELAY_FOGGER, true);
    Serial.println("fogger logic counter first time start");
  }
  else
  {
    unsigned long currentMilis = millis();
    if(currentMilis < foggerStartMs) //overflow
    {
      foggerStartMs = 0;
      foggerEndMs = 0;
      Serial.println("fogger logic counter overflow, restarting");
    }
    else{
      if(relayGetState(RELAY_FOGGER) == true)
      {
        int remaining = foggerOnDuration - (int)((currentMilis - foggerStartMs)/1000);
        
        if(remaining <= 0)
        {
          Serial.println("turning off fogger");
          relaySetState(RELAY_FOGGER, false);
          foggerEndMs = currentMilis;
        }
        else{
          Serial.print("Fogger ON, turning off in ");
          Serial.print(remaining);
          Serial.println(" seconds");
        }
      }
      else
      {
        int remaining = foggerOffDuration - (int)((currentMilis - foggerEndMs)/1000);
        
        if( remaining <= 0)
        {
          Serial.println("turning on fogger");
          relaySetState(RELAY_FOGGER, true);
          foggerStartMs = currentMilis;
        }
        else{
          Serial.print("Fogger OFF, turning on in ");
          Serial.print(remaining);
          Serial.println(" seconds");
        }
      }
    }
  }
}

void loopLogicCirculationFan()
{
  if(relayGetState(RELAY_HUMID) == true)
  {
    if(relayGetState(RELAY_CFAN) == false){
      relaySetState(RELAY_CFAN, true);
    }
  }

  if(relayGetState(RELAY_FOGGER) == true)
  {
    if(relayGetState(RELAY_CFAN) == false){
      relaySetState(RELAY_CFAN, true);
    }
  }

  if(currentAirTemp > 35.0f)
  {
    if(relayGetState(RELAY_CFAN) == false){
      relaySetState(RELAY_CFAN, true);
    }
  }
  else if(tempResult[0] > 28.0f)
  {
    if(relayGetState(RELAY_CFAN) == false){
      relaySetState(RELAY_CFAN, true);
    }
  }
}

void loopLogicWaterPump()
{
  if(relayGetState(RELAY_WATERPUMP) == false)
  {
    relaySetState(RELAY_WATERPUMP, true);
  }
}

void loopLogicCooler()
{
  if(relayGetState(RELAY_WATERPUMP) == true)
  {
    if(relayGetState(RELAY_COOLER_1) == true && relayGetState(RELAY_COOLER_2) == false)
    {
      relaySetState(RELAY_COOLER_2, true);
    }
    if(relayGetState(RELAY_COOLER_1) == false){
      relaySetState(RELAY_COOLER_1, true);
    }
  }
  else{
    if(tempResult[0] > 28.0f)
    {
      if(relayGetState(RELAY_COOLER_1) == true && relayGetState(RELAY_COOLER_2) == false)
      {
        relaySetState(RELAY_COOLER_2, true);
      }
      if(relayGetState(RELAY_COOLER_1) == false){
        relaySetState(RELAY_COOLER_1, true);
      }
    }
    else
    {
      relaySetState(RELAY_COOLER_1, false);
      relaySetState(RELAY_COOLER_2, false);
    }
  }
}

unsigned long humidifierStartMs = 0;
unsigned long humidifierEndMs = 0;
void loopLogicHumidifier()
{
  if(humidifierStartMs == 0){
    humidifierStartMs = millis();
    humidifierEndMs = millis();
    Serial.println("humidifier logic counter first time start");
    relaySetState(RELAY_HUMID, true);
  }
  else
  {
    unsigned long currentMilis = millis();
    if(currentMilis < humidifierStartMs) //overflow
    {
      humidifierStartMs = 0;
      humidifierEndMs = 0;
      Serial.println("humidifier logic counter overflow, restarting");
    }
    else{
      if(currentAirHumidity < 40.0f)
      {
        if(relayGetState(RELAY_HUMID) == false)
        {
          Serial.println("turning on humidifier, because air humidity drop below 40.0C");
          relaySetState(RELAY_HUMID, true);
          humidifierStartMs = currentMilis;
        }
      }
      else
      {
        if(relayGetState(RELAY_HUMID) == true)
        {
          int remaining = humidifierOnDuration - (int)((currentMilis - humidifierStartMs)/1000);
          if( remaining <= 0)
          {
            Serial.println("turning off humidifier");
            relaySetState(RELAY_HUMID, false);
            humidifierEndMs = currentMilis;
          }
          else{
            Serial.print("Humidifier ON, turning off in ");
            Serial.print(remaining);
            Serial.println(" seconds");
          }
        }
        else
        {
          int remaining = humidifierOffDuration - (int)((currentMilis - humidifierEndMs)/1000);
          if( remaining <= 0)
          {
            Serial.println("turning on humidifier");
            relaySetState(RELAY_HUMID, true);
            humidifierStartMs = currentMilis;
          }
          else{
            Serial.print("Humidifier OFF, turning on in ");
            Serial.print(remaining);
            Serial.println(" seconds");
          }
        }
      }
    }
  }
}

void loopLogicAC1()
{
  if(relayGetState(RELAY_FOGGER) == true || relayGetState(RELAY_HUMID) == true)
  {
    if(relayGetState(RELAY_AC_1) == false){
      relaySetState(RELAY_AC_1, true);
    }
  }
  else{
      if(relayGetState(RELAY_AC_1) == true){
      relaySetState(RELAY_AC_1, false);
    }
  }
}

void loopLogicAC2()
{
  if(relayGetState(RELAY_AC_2) == false){
      relaySetState(RELAY_AC_2, true);
    }
}

boolean firstLogic = true;
void loopLogic(){
  if(firstLogic == true)
  {
    lastLogicMs = millis();
    firstLogic = false;
    relaySetState(0, false);
    relaySetState(1, false);
    relaySetState(2, false);
    relaySetState(3, false);
    relaySetState(4, false);
    relaySetState(5, false);
    relaySetState(6, false);
    relaySetState(7, false);
    relaySetState(8, false);
    relaySetState(9, false);
    relaySetState(10, false);
    relaySetState(11, false);
    relaySetState(12, false);
    relaySetState(13, false);
    relaySetState(14, false);
    relaySetState(15, false);
  }
  unsigned long currentMilis = millis();
  if(currentMilis < lastLogicMs) //overflow
  {
    lastLogicMs = millis();
  }
  if(currentMilis - lastLogicMs > 2000)
  {
    loopLogicFogger();
    loopLogicWaterPump();
    loopLogicCooler();
    loopLogicHumidifier();
    loopLogicCirculationFan();
    loopLogicAC1();
    loopLogicAC2();
    lastLogicMs = currentMilis;
  }
}

void loop() {
  loopTimingBegin();
  {
    loopNetworking();
    loopSensorDHT();
    loopSensorLight();
    loopRelayClient();
    loopSensorWaterTemp();
    loopWebServer();
    loopLogic();
  }
  loopTimingEnd();
}
