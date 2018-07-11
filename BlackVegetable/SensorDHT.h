#include <DHT.h>;
#define DHTPIN 23     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT sensorDHT(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float currentAirHumidity  = 0.0f;  //Stores humidity value
float currentAirTemp      = 0.0f; //Stores temperature value

int lastLoopAirTempSensorGaugePosX = 0;
int lastLoopAirTempSensorGaugePosY = 0;
void loopSensorAirTempUpdateGauge(float percentage, const char* value)
{
  const int gridX = 0;
  const int gridY = 0;

  const int color = LIGHTGREEN;
  const int gaugeColor = DARKGREEN;
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
  lcd.writeFillCircle(lastLoopAirTempSensorGaugePosX,lastLoopAirTempSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopAirTempSensorGaugePosX = newX;
  lastLoopAirTempSensorGaugePosY = newY;
}

int lastLoopAirHumdSensorGaugePosX = 0;
int lastLoopAirHumdSensorGaugePosY = 0;
void loopSensorAirHumdUpdateGauge(float percentage, const char* value)
{
  const int gridX = 1;
  const int gridY = 0;

  const int color = LIGHTBLUE;
  const int gaugeColor = DARKBLUE;
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
  lcd.writeFillCircle(lastLoopAirHumdSensorGaugePosX,lastLoopAirHumdSensorGaugePosY,sensorGridThickness/2+2,bgColor);
  drawQuarterArc1(gx, gy, sensorGridWidth-2, gaugeColor, sensorGridThickness);
  lcd.writeFillCircle(newX,newY,sensorGridThickness/2+2,color);

  lcd.writeFillRect(textOffsetX, valueTextOffsetY, sensorGridLabelWidth, sensorGridValueTextHeight, bgColor);
  lcd.endWrite();

  lcd.setTextSize(2);
  lcd.setTextColor(color);
  lcd.setCursor(textOffsetX, valueTextOffsetY);
  lcd.print(value);  

  lastLoopAirHumdSensorGaugePosX = newX;
  lastLoopAirHumdSensorGaugePosY = newY;
}


String dhtSensorHandleWebRequest(const String& url)
{
  String result = "\t\"airTemp\":";
  char airTempStr[10];
  dtostrf(currentAirTemp, 3, 2, airTempStr);
  result += String(airTempStr);

  result += ",\r\n\t\"airHumidity\":";
  dtostrf(currentAirHumidity, 3, 2, airTempStr);
  result += String(airTempStr);
  
  return result;
}

void initSensorDHT(void)
{
  Serial.println("initializing DHT sensor...");
  sensorDHT.begin();

  int gridX = 0;
  int gridY = 0;
  
  int ox = getSensorGridPosX(gridX);
  int oy = getSensorGridPosY(gridY);
  int textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  int labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;

  loopSensorAirTempUpdateGauge(0.0,"XX.XX");

  lcd.setTextSize(1);
  lcd.setTextColor(GREEN);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("AirTemp C");
  
  gridX = 1;
  gridY = 0;
  
  ox = getSensorGridPosX(gridX);
  oy = getSensorGridPosY(gridY);
  textOffsetX      = (ox+sensorGridWidth)-sensorGridLabelWidth;
  labelTextOffsetY = (oy+sensorGridHeight)-sensorGridLabelHeight;

  loopSensorAirHumdUpdateGauge(0.0,"XX.XX");

  lcd.setTextSize(1);
  lcd.setTextColor(BLUE);
  lcd.setCursor(textOffsetX, labelTextOffsetY);
  lcd.print("RH %");

  webServerURLHook[2] = &dhtSensorHandleWebRequest;
  Serial.println("DHT sensor initialization done.");
}

void loopSensorDHT(void)
{
     //Read data and store it to variables hum and temp
    float new_hum  = sensorDHT.readHumidity();
    float new_temp = sensorDHT.readTemperature();
    
    //Print temp and humidity values to serial monitor
    /*
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");*/

    if(new_temp != currentAirTemp){
      char airTempStr[10];
      dtostrf(currentAirTemp, 3, 2, airTempStr);
      loopSensorAirTempUpdateGauge(currentAirTemp/50.0f,airTempStr);
    }

    if(new_hum != currentAirHumidity){
      char humdString[10];
      dtostrf(currentAirHumidity, 3, 2, humdString);
      loopSensorAirHumdUpdateGauge(currentAirHumidity/100.0f,humdString);
    }

    currentAirHumidity = new_hum;
    currentAirTemp = new_temp;
}
