#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_ONE_WIRE_PIN 22

OneWire oneWire(TEMP_ONE_WIRE_PIN);
DallasTemperature tempSensors(&oneWire);
DeviceAddress tempSensorsAddr[4];
float tempResult[4] = {0.0f,0.0f,0.0f,0.0f};
uint8_t tempSensorCount = 0;
unsigned long lastTempRequestMs = 0;

int lastLoopWaterTempSensorGaugePosX = 0;
int lastLoopWaterTempSensorGaugePosY = 0;
void loopSensorWaterTempUpdateGauge(float percentage, const char* value)
{
  const int gridX = 0;
  const int gridY = 1;
  const int color = LIGHTCYAN;
  const int gaugeColor = DARKCYAN;
  const int bgColor = BLACK;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int valueTextOffsetY = (oy+sensorGridHeight)-(sensorGridValueTextHeight+sensorGridLabelHeight)-2;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;
  
  int gx = ox+sensorGridWidth-1;
  int gy = oy+sensorGridHeight-1;
  const float r = (float)(sensorGridWidth-(sensorGridThickness/2));
  
  const float theta = (90.0f*percentage+180.0f)*PI/180.0f;
  int newX = gx + (int)(r*cos(theta));
  int newY = gy + (int)(r*sin(theta));

  lcd.startWrite();
  lcd.writeFillCircle(lastLoopWaterTempSensorGaugePosX,lastLoopWaterTempSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopWaterTempSensorGaugePosX = newX;
  lastLoopWaterTempSensorGaugePosY = newY;
}

int lastLoopWaterTempSensor2GaugePosX = 0;
int lastLoopWaterTempSensor2GaugePosY = 0;
void loopSensorWaterTempUpdateGauge2(float percentage, const char* value)
{
  const int gridX = 0;
  const int gridY = 2;
  const int color = LIGHTCYAN;
  const int gaugeColor = DARKCYAN;
  const int bgColor = BLACK;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int valueTextOffsetY = (oy+sensorGridHeight)-(sensorGridValueTextHeight+sensorGridLabelHeight)-2;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;
  
  int gx = ox+sensorGridWidth-1;
  int gy = oy+sensorGridHeight-1;
  const float r = (float)(sensorGridWidth-(sensorGridThickness/2));
  
  const float theta = (90.0f*percentage+180.0f)*PI/180.0f;
  int newX = gx + (int)(r*cos(theta));
  int newY = gy + (int)(r*sin(theta));

  lcd.startWrite();
  lcd.writeFillCircle(lastLoopWaterTempSensor2GaugePosX,lastLoopWaterTempSensor2GaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopWaterTempSensor2GaugePosX = newX;
  lastLoopWaterTempSensor2GaugePosY = newY;
}

String tempSensorHandleWebRequest(const String& url)
{
  String result = "\t\"temp\":[\r\n";
  for(int i=0 ; i<tempSensorCount ; i++)
  {
    char tempStr[10];
    dtostrf(tempResult[i], 3, 2, tempStr);
    result += "\t\t";
    result += String(tempStr);
    if(i < tempSensorCount-1){
      result +=",\r\n";
    }
  }
  result+= "]";
  return result;
}

void initSensorWaterTemp(void)
{
  Serial.println("Initializing Temp Sensor...");
  tempSensors.begin();
  tempSensorCount = tempSensors.getDeviceCount();
  Serial.print("detected ");
  Serial.print(tempSensorCount);
  Serial.println(" temp sensor(s)");
  if(tempSensorCount > 4) tempSensorCount = 4;
  
  for(uint8_t i=0 ; i<tempSensorCount ; i++)
  {
    if(tempSensors.getAddress(tempSensorsAddr[i],i)){
      Serial.print("temp sensor ");
      Serial.print(i);
      Serial.print(" address = ");
      for(int a=0 ; a<8 ; a++){
        Serial.print(tempSensorsAddr[i][a],HEX);
      }
      Serial.println();
      if(tempSensors.setResolution(tempSensorsAddr[i],12))
      {
        Serial.print("temp sensor ");
        Serial.print(i);
        Serial.println("set resolution 12 bit success");
      }
      
    }
  }
  
  int gridX = 0;
  int gridY = 1;
  const int color = LIGHTCYAN;
  const int gaugeColor = DARKCYAN;
  const int bgColor = BLACK;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;

  char tempStr[10];
  dtostrf(tempResult[0], 3, 2, tempStr);
      
  loopSensorWaterTempUpdateGauge(0.0f,"XX.XX");

  lcd.setTextSize(1);
  lcd.setTextColor(CYAN);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("WaterTemp");

  //---

  gridX = 0;
  gridY = 2;

  ox = getSensorGridPosX(gridX);
  oy = getSensorGridPosY(gridY);
  textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;

  tempStr[10];
  dtostrf(tempResult[1], 3, 2, tempStr);
      
  loopSensorWaterTempUpdateGauge2(0.0f,"XX.XX");

  lcd.setTextSize(1);
  lcd.setTextColor(CYAN);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("VRTemp");

  //---

  webServerURLHook[1] = &tempSensorHandleWebRequest;
  lastTempRequestMs = millis();
  Serial.println("Temp Sensor initialization done.");
}

void loopSensorWaterTemp(void)
{
  if(tempSensorCount > 0)
  {
    unsigned long currentMs = millis();
    if( (int)(currentMs - lastTempRequestMs) > (1000*tempSensorCount))
    {
      float oldTemp = tempResult[0];
      tempSensors.requestTemperatures(); 
      for(int i=0 ; i<tempSensorCount ; i++)
      {
        float prevTemp = tempResult[i];
        tempResult[i] = tempSensors.getTempC(tempSensorsAddr[i]);
        Serial.print("temp #");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(tempResult[i]);
        Serial.println("C");
        if(tempResult[i] > 99 || tempResult[i] < 0){
          tempResult[i] = prevTemp;
        }
      }
    
      if(tempResult[0] != oldTemp){
          char tempStr[10];
          dtostrf(tempResult[0], 3, 2, tempStr);
          loopSensorWaterTempUpdateGauge(tempResult[0]/50.0f,tempStr);
      }

      float oldTemp2 = tempResult[1];
      if(tempSensorCount > 1 && tempResult[1] != oldTemp){
          char tempStr[10];
          dtostrf(tempResult[1], 3, 2, tempStr);
          loopSensorWaterTempUpdateGauge2(tempResult[1]/50.0f,tempStr);
      }
      lastTempRequestMs = currentMs;
    }
  }
  else{
    if((int)(millis() - lastTempRequestMs) > 5000)
    {
      initSensorWaterTemp();
    }
  }
}
