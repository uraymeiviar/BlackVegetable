int lastLoopWaterFlowSensorGaugePosX = 0;
int lastLoopWaterFlowSensorGaugePosY = 0;
void loopSensorWaterFlowUpdateGauge(float percentage, const char* value)
{
  const int gridX = 2;
  const int gridY = 1;
  const int color = LIGHTORANGE;
  const int gaugeColor = DARKORANGE;
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
  lcd.writeFillCircle(lastLoopWaterFlowSensorGaugePosX,lastLoopWaterFlowSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopWaterFlowSensorGaugePosX = newX;
  lastLoopWaterFlowSensorGaugePosY = newY;
}

void initSensorWaterFlow(void)
{
  const int gridX = 2;
  const int gridY = 1;

  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;
  
  loopSensorWaterFlowUpdateGauge(0.9,"XX.XX");

  lcd.setTextSize(1);
  lcd.setTextColor(ORANGE);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("WaterFlow");
}

void loopSensorWaterFlow(void)
{

}
