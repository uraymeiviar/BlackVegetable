#define LIGHT_ANALOG_PIN  A15
#define LIGHT_DIGITAL_PIN 22
int  LightSwitchState = 0xFF;  /* Holds the last digital value */
int  LightAnalogValue = 0; /* Holds the last analog value */
float LightAvgValue = 50.0;

int lastLoopLightSensorGaugePosX = 0;
int lastLoopLightSensorGaugePosY = 0;
void loopSensorLightUpdateGauge(float percentage, const char* value)
{
  const int gridX = 2;
  const int gridY = 0;

  const int color = LIGHTYELLOW;
  const int gaugeColor = DARKYELLOW;
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
  lcd.writeFillCircle(lastLoopLightSensorGaugePosX,lastLoopLightSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopLightSensorGaugePosX = newX;
  lastLoopLightSensorGaugePosY = newY;
}

String lightSensorHandleWebRequest(const String& url)
{
  String result = "\t\"light\":";
  char lightString[10];
  dtostrf(LightAvgValue, 3, 2, lightString);
  result+= String(lightString);
  
  return result;
}

void initSensorLight(void)
{
  Serial.println("Initializing Light Sensor...");
  const int gridX = 2;
  const int gridY = 0;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;

  loopSensorLightUpdateGauge(0.0,"00.00");

  lcd.setTextSize(1);
  lcd.setTextColor(YELLOW);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("Light %");

  webServerURLHook[3] = &lightSensorHandleWebRequest;
  Serial.println("Light Sensor Initialization done.");
}

void loopSensorLight(void)
{
  int switch_state = digitalRead(LIGHT_DIGITAL_PIN);  
  if(switch_state != LightSwitchState)
  {
    /*
    if (switch_state == LOW)
    {
      Serial.println("Light Digital Signal ON ");
    }
    else
    {
      Serial.println("Light Digital Signal OFF ");
    }*/
    LightSwitchState = switch_state;
  }

  int newLightAnalogValue = analogRead(LIGHT_ANALOG_PIN);  //Read the voltage from sensor
  if(newLightAnalogValue != LightAnalogValue)
  {
    float lightPercent = ((float)(1024-newLightAnalogValue))*100.0f / 1024.0f;
    int oldLightAvgValue = (int)(LightAvgValue*10.0f);
    LightAvgValue = (0.1f * lightPercent) + (1.0f - 0.1f) * LightAvgValue;

    char lightString[10];
    dtostrf(LightAvgValue, 3, 2, lightString);

    if(oldLightAvgValue != (int)(LightAvgValue*10.0f))
    {
      /*
      lcd.setTextSize(2);
      lcd.fillRect(160, 22, 80, 30, BLACK);
      lcd.setTextColor(YELLOW);
      lcd.setCursor(161, 23);
      lcd.print(lightString);*/
      loopSensorLightUpdateGauge(LightAvgValue/100.0f,lightString);
    }
  }

   LightAnalogValue = newLightAnalogValue;
}
