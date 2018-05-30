#include <DHT.h>;
#define DHTPIN 23     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT sensorDHT(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
float currentAirHumidity  = 0.0f;  //Stores humidity value
float currentAirTemp      = 0.0f; //Stores temperature value

void initSensorDHT(void)
{
  sensorDHT.begin();
  lcd.setTextSize(1);
  lcd.fillRect(0, 10, 60, 10, BLACK);
  lcd.setTextColor(GREEN);
  lcd.setCursor(1, 12);
  lcd.print("Temp (C)");
  
  lcd.fillRect(70, 10, 60, 10, BLACK);
  lcd.setTextColor(LIGHTBLUE);
  lcd.setCursor(71, 12);
  lcd.print("RH (%)");
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
      lcd.setTextSize(2);
      lcd.fillRect(0, 20, 60, 30, BLACK);
      lcd.setTextColor(GREEN);
      lcd.setCursor(1, 21);
      lcd.print(currentAirTemp);
    }

    if(new_hum != currentAirHumidity){
      lcd.setTextSize(2);
      lcd.fillRect(71, 20, 60, 30, BLACK);
      lcd.setTextColor(LIGHTBLUE);
      lcd.setCursor(71, 21);
      lcd.print(currentAirHumidity);
    }

    currentAirHumidity = new_hum;
    currentAirTemp = new_temp;
}
