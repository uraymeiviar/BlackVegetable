uint8_t clockHour; 
uint8_t clockMin;
uint8_t clockSec; 
const unsigned long loopDelayMS = 100;

uint8_t clockStringToNum(const char* p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9') v = *p - '0';
  return 10 * v + *++p - '0';
}

unsigned long clock_startTiming = 0;
unsigned long clock_msCumulative = 0;
unsigned long clock_msDrift = 0;

void initTiming(void)
{
  clockHour = clockStringToNum(__TIME__);
  clockMin = clockStringToNum(__TIME__ + 3);
  clockSec = clockStringToNum(__TIME__ + 6);
  clock_startTiming = millis();
}

void updateClock(void)
{
  unsigned long currentMilis = millis();
  unsigned long deltaMs = currentMilis - clock_startTiming;
  //Serial.println(deltaMs);
  clock_msCumulative +=  deltaMs;
  if(clock_msCumulative >= 1000)
  {
    clockSec += clock_msCumulative / 1000;
    clock_msDrift += clock_msCumulative % 1000;
    if(clock_msDrift > 1000)
    {
      clockSec += clock_msDrift / 1000;
      clock_msDrift = clock_msDrift % 1000;
    }
    //Serial.println(clock_msDrift);
    clock_msCumulative = 0;

    char clockString[20];
    if (clockSec > 59) 
    {
          clockSec = 0;
          clockMin++;
          if (clockMin > 59) 
          {
              clockMin = 0;
              clockHour++;
              if (clockHour > 23) clockHour = 0;
          }
     }

    sprintf(clockString, "%02d:%02d:%02d.%03d", clockHour, clockMin, clockSec, clock_msCumulative+clock_msDrift);
    lcd.setTextSize(1);
    lcd.fillRect(240, 0, 80, 10, WHITE);
    lcd.setTextColor(BLACK);
    lcd.setCursor(242, 2);
    lcd.print(clockString);
  }
  clock_startTiming = currentMilis;
}

unsigned long loopInterval = 0;
unsigned long startMillis = 0;
void loopTimingBegin()
{
  startMillis = millis();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loopTimingEnd()
{
  char timingString[10];
  sprintf(timingString, "%dms", loopInterval);
  
  lcd.setTextSize(1);
  if(loopInterval > loopDelayMS)
  {
    lcd.fillRect(130, 0, 30, 10, RED);
    lcd.setTextColor(WHITE);
  }
  else{
    lcd.fillRect(130, 0, 30, 10, BLACK);
    lcd.setTextColor(GREEN);
  }
  lcd.setCursor(131, 2);
  lcd.print(timingString);
  
  updateClock();
  
  digitalWrite(LED_BUILTIN, LOW);
  loopInterval = millis() - startMillis;

  if(loopInterval < loopDelayMS)
  {
    delay(loopDelayMS-loopInterval);
  }
}

