int lastLoopWaterPPMSensorGaugePosX = 0;
int lastLoopWaterPPMSensorGaugePosY = 0;
void loopSensorWaterPPMUpdateGauge(float percentage, const char* value)
{
  const int gridX = 3;
  const int gridY = 1;
  const int color = LIGHTGREY;
  const int gaugeColor = DARKGREY;
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
  lcd.writeFillCircle(lastLoopWaterPPMSensorGaugePosX,lastLoopWaterPPMSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopWaterPPMSensorGaugePosX = newX;
  lastLoopWaterPPMSensorGaugePosY = newY;
}

void initSensorWaterPPM(void)
{
  const int gridX = 3;
  const int gridY = 1;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;
  
  loopSensorWaterPPMUpdateGauge(0.5,"XXXX");

  lcd.setTextSize(1);
  lcd.setTextColor(GREY);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("WaterPPM");
}

void loopSensorWaterPPM(void)
{

}
