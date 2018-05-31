#define LIGHT_ANALOG_PIN  A15
#define LIGHT_DIGITAL_PIN 22
int  LightSwitchState = 0xFF;  /* Holds the last digital value */
int  LightAnalogValue = 0; /* Holds the last analog value */

void initSensorLight(void)
{
  lcd.setTextSize(1);
  lcd.fillRect(180, 12, 90, 10, BLACK);
  lcd.setTextColor(YELLOW);
  lcd.setCursor(181, 14);
  lcd.print("Light (%)");
}

void loopSensorLight(void)
{
  int switch_state = digitalRead(LIGHT_DIGITAL_PIN);  
  if(switch_state != LightSwitchState)
  {
    if (switch_state == LOW)
    {
      Serial.println("Light Digital Signal ON ");
    }
    else
    {
      Serial.println("Light Digital Signal OFF ");
    }
    LightSwitchState = switch_state;
  }

  int newLightAnalogValue = analogRead(LIGHT_ANALOG_PIN);  //Read the voltage from sensor
  if(newLightAnalogValue != LightAnalogValue)
  {
    float lightPercent = (float)(1024-newLightAnalogValue)*100 / 1024.0;
    char lightString[10];
    dtostrf(lightPercent, 3, 1, lightString);
    /*
    lightString[5] = '%';

    char lightStringExt[20];
    sprintf(lightStringExt, "Light %d (%s)", newLightAnalogValue, lightString);
  
    Serial.print(lightStringExt);
    Serial.println();     */

    lcd.setTextSize(2);
    lcd.fillRect(180, 22, 90, 30, BLACK);
    lcd.setTextColor(YELLOW);
    lcd.setCursor(181, 23);
    lcd.print(lightString);
  }

   LightAnalogValue = newLightAnalogValue;
}
