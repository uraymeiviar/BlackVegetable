int lastLoopWaterPHSensorGaugePosX = 0;
int lastLoopWaterPHSensorGaugePosY = 0;
void loopSensorWaterPHUpdateGauge(float percentage, const char* value)
{
  const int gridX = 3;
  const int gridY = 0;
  const int color = LIGHTMAGENTA;
  const int gaugeColor = DARKMAGENTA;
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
  lcd.writeFillCircle(lastLoopWaterPHSensorGaugePosX,lastLoopWaterPHSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopWaterPHSensorGaugePosX = newX;
  lastLoopWaterPHSensorGaugePosY = newY;
}

void initSensorPH(void)
{
  const int gridX = 3;
  const int gridY = 0;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;
  
  loopSensorWaterPHUpdateGauge(0.4,"XX.XX");

  lcd.setTextSize(1);
  lcd.setTextColor(MAGENTA);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("Water pH");
}

void loopSensorPH(void)
{

}
