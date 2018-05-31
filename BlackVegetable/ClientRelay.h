byte relayIP[] = { 192, 168, 1, 17 };
IPAddress relayIPAddress(relayIP[0],relayIP[1],relayIP[2],relayIP[3]);
const char relayInitCMD[] = {0xaa, 0x1e,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xbb};
uint16_t relayPort = 8080;
EthernetClient relayClient;
SOCKET relayPingSocket = 0;
ICMPPing ping(relayPingSocket, (uint16_t)random(0, 255));
#define relayNotRespondingTimeout 5000
#define relayMinSetStateInterval 1000
unsigned long relayLastRespondMs = 0;
unsigned long relayLastSetStateMs = 0;
const int relayReplyLength = 20;
int relayReplyCount = 0;
char relayReply[20] = {0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                       0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
boolean relayState[16] = {false,false,false,false,false,false,false,false,
                          false,false,false,false,false,false,false,false};

//=================================================================================

void serialPrintHex(const char value)
{
  if(value < 16)
  {
    Serial.print("0");
  }
  Serial.print(value,HEX);
  Serial.print(" ");
}

//.................................................................................

void printRelayState(int index)
{
  Serial.print("Relay State ");
  if(index < 10)
  {
    Serial.print("0");
  }
  Serial.print(index);
  Serial.print(":");
  if(relayReply[2+index] == 0x01){
    Serial.println("1");
  }
  else
  {
    Serial.println("0");
  }
}

//.................................................................................

boolean connectToRelay()
{
  Serial.print("connecting to relay ");
  serialPrintIPAddr(relayIP);
  Serial.println();
  
  if (relayClient.connect(relayIP, relayPort)) 
  {
    relayLastRespondMs = millis();
    Serial.println("connected to relay");
    return true;
  } 
  else 
  {
    Serial.println("connection to relay failed");
    return false;
  }
}

//.................................................................................

void updateRelayState(int index, boolean value)
{
  if(relayState[index] != value)
  {
    lcd.setTextSize(2);
    relayState[index] = value;
    if(value)
    {
      lcd.setTextColor(LIGHTGREEN);
      if(index == 0)
      {
        lcd.fillRect(  4, 386, 38, 38, DARKGREEN);
        lcd.setCursor(6, 388);
        lcd.print("01");
      }
      else if(index == 1)
      {
        lcd.fillRect(  4, 425, 38, 38, DARKGREEN);
        lcd.setCursor(6, 427);
        lcd.print("02");
      }
      else if(index == 2)
      { 
        lcd.fillRect( 43, 386, 38, 38, DARKGREEN);
        lcd.setCursor(45, 388);
        lcd.print("03");
      }
      else if(index == 3)
      { 
        lcd.fillRect( 43, 425, 38, 38, DARKGREEN);
        lcd.setCursor(45, 427);
        lcd.print("04");
      }
      else if(index == 4)
      { 
        lcd.fillRect( 82, 386, 38, 38, DARKGREEN);
        lcd.setCursor(84, 388);
        lcd.print("05");
      }
      else if(index == 5)
      { 
        lcd.fillRect( 82, 425, 38, 38, DARKGREEN);
        lcd.setCursor(84, 427);
        lcd.print("06");
      }
      else if(index == 6)
      { 
        lcd.fillRect(121, 386, 38, 38, DARKGREEN);
        lcd.setCursor(123, 388);
        lcd.print("07");
      }
      else if(index == 7)
      { 
        lcd.fillRect(121, 425, 38, 38, DARKGREEN);
        lcd.setCursor(123, 427);
        lcd.print("08");
      }
      else if(index == 8)
      { 
        lcd.fillRect(160, 386, 38, 38, DARKGREEN);
        lcd.setCursor(162, 388);
        lcd.print("09");
      }
      else if(index == 9)
      { 
        lcd.fillRect(160, 425, 38, 38, DARKGREEN);
        lcd.setCursor(162, 427);
        lcd.print("10");
      }
      else if(index == 10)
      { 
        lcd.fillRect(199, 386, 38, 38, DARKGREEN);
        lcd.setCursor(201, 388);
        lcd.print("11");
      }
      else if(index == 11)
      { 
        lcd.fillRect(199, 425, 38, 38, DARKGREEN);
        lcd.setCursor(201, 427);
        lcd.print("12");
      }
      else if(index == 12)
      { 
        lcd.fillRect(238, 386, 38, 38, DARKGREEN);
        lcd.setCursor(240, 388);
        lcd.print("13");
      }
      else if(index == 13)
      { 
        lcd.fillRect(238, 425, 38, 38, DARKGREEN);
        lcd.setCursor(240, 427);
        lcd.print("14");
      }
      else if(index == 14)
      { 
        lcd.fillRect(277, 386, 38, 38, DARKGREEN);
        lcd.setCursor(279, 388);
        lcd.print("15");
      }
      else if(index == 15)
      { 
        lcd.fillRect(277, 425, 38, 38, DARKGREEN);
        lcd.setCursor(279, 427);
        lcd.print("16");
      }
    }
    else
    {
      lcd.setTextColor(LIGHTRED);
      if(index == 0)
      {
        lcd.fillRect(  4, 386, 38, 38, DARKRED);
        lcd.setCursor(6, 388);
        lcd.print("01");
      }
      else if(index == 1)
      {
        lcd.fillRect(  4, 425, 38, 38, DARKRED);
        lcd.setCursor(6, 427);
        lcd.print("02");
      }
      else if(index == 2)
      { 
        lcd.fillRect( 43, 386, 38, 38, DARKRED);
        lcd.setCursor(45, 388);
        lcd.print("03");
      }
      else if(index == 3)
      { 
        lcd.fillRect( 43, 425, 38, 38, DARKRED);
        lcd.setCursor(45, 427);
        lcd.print("04");
      }
      else if(index == 4)
      { 
        lcd.fillRect( 82, 386, 38, 38, DARKRED);
        lcd.setCursor(84, 388);
        lcd.print("05");
      }
      else if(index == 5)
      { 
        lcd.fillRect( 82, 425, 38, 38, DARKRED);
        lcd.setCursor(84, 427);
        lcd.print("06");
      }
      else if(index == 6)
      { 
        lcd.fillRect(121, 386, 38, 38, DARKRED);
        lcd.setCursor(123, 388);
        lcd.print("07");
      }
      else if(index == 7)
      { 
        lcd.fillRect(121, 425, 38, 38, DARKRED);
        lcd.setCursor(123, 427);
        lcd.print("08");
      }
      else if(index == 8)
      { 
        lcd.fillRect(160, 386, 38, 38, DARKRED);
        lcd.setCursor(162, 388);
        lcd.print("09");
      }
      else if(index == 9)
      { 
        lcd.fillRect(160, 425, 38, 38, DARKRED);
        lcd.setCursor(162, 427);
        lcd.print("10");
      }
      else if(index == 10)
      { 
        lcd.fillRect(199, 386, 38, 38, DARKRED);
        lcd.setCursor(201, 388);
        lcd.print("11");
      }
      else if(index == 11)
      { 
        lcd.fillRect(199, 425, 38, 38, DARKRED);
        lcd.setCursor(201, 427);
        lcd.print("12");
      }
      else if(index == 12)
      { 
        lcd.fillRect(238, 386, 38, 38, DARKRED);
        lcd.setCursor(240, 388);
        lcd.print("13");
      }
      else if(index == 13)
      { 
        lcd.fillRect(238, 425, 38, 38, DARKRED);
        lcd.setCursor(240, 427);
        lcd.print("14");
      }
      else if(index == 14)
      { 
        lcd.fillRect(277, 386, 38, 38, DARKRED);
        lcd.setCursor(279, 388);
        lcd.print("15");
      }
      else if(index == 15)
      { 
        lcd.fillRect(277, 425, 38, 38, DARKRED);
        lcd.setCursor(279, 427);
        lcd.print("16");
      }
    }
  }
}

//.................................................................................

boolean getRelayResponse()
{
  while(relayClient.connected() && relayClient.available() && relayReplyCount<relayReplyLength){
    relayReply[relayReplyCount] = relayClient.read();
    //serialPrintHex(relayReply[relayReplyCount]);
    relayReplyCount++;
    relayLastRespondMs = millis();
  }

  if( relayReplyCount == relayReplyLength )
  {
    relayReplyCount = 0;
    //Serial.println();
    if(relayReply[1] == 0x0c){
      Serial.println("relay switch state changed");
      for(int i=0 ; i<16 ; i++)
      {
        updateRelayState(i, relayReply[2+i] == 0x01);
        printRelayState(i);
      }
    }
    return true;
  }
  else{
    return false;
  }
}

//.................................................................................

boolean waitRelayResponse(int retryCount = 10, int retryDelay = 10)
{
  if (!relayClient.connected()) 
  {
    Serial.println("Relay disconnected.");
    relayClient.stop();
    return false;
  }

  int retryCounter = 0;
  while (relayClient.connected()) 
  {
    if(getRelayResponse())
    {
      return true;
    }

    delay(retryDelay);
    if(retryCounter++ > retryCount){
      return false;
    }
  }
  return false;
}

//.................................................................................

unsigned long pingRelay()
{
  ICMPEchoReply echoReply = ping(relayIPAddress, 4);
  if (echoReply.status == SUCCESS)
  {
    unsigned long ms = millis() - echoReply.data.time;
    Serial.print("relay ping (ms) = ");
    Serial.println(ms);
    return ms;
  }
  else
  {
    Serial.println("relay ping failed");
    return (unsigned long)-1;
  }
}

//.................................................................................

void loopRelayClient()
{
  if (relayClient.connected()) 
  {
    if(!getRelayResponse())
    {
      unsigned long deltams = millis() - relayLastRespondMs;
      if(deltams > relayNotRespondingTimeout){
         Serial.print("relay not responding for ");
         Serial.print(deltams);
         Serial.println(" ms ");
         if(pingRelay() == (unsigned long)(-1))
         {
            Serial.println("Relay disconnected.");
            relayClient.stop();
         }
         else{
            relayLastRespondMs = millis();
         }
      }
    }
  }
  else
  {
    Serial.println("relay not connected");
    if(connectToRelay())
    {
      relayClient.write(relayInitCMD, sizeof(relayInitCMD));
      relayClient.flush();
      waitRelayResponse();
    }
  }
}

//.................................................................................

boolean relayGetState(uint8_t index)
{
  if(index >= 0 && index < 16)
  {
    return relayState[index];
  }
  else
  {
    return false;
  }
}

//.................................................................................

boolean relaySetState(uint8_t index, boolean state)
{
  unsigned long setStateInterval = millis() - relayLastSetStateMs;
  if( setStateInterval < relayMinSetStateInterval)
  {
    delay(relayMinSetStateInterval - setStateInterval);
    Serial.println("relay set state too fast, delaying..");
  }
  char stateValue = 0x02;
  if(state == true)
  {
    stateValue = 0x01;
  }
  char switchCMD[] = { 0xaa,  0xf, index, stateValue, 
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xbb};
  Serial.print("Set Relay ");
  Serial.print(index,DEC);
  Serial.print(" state = ");
  if(state == true){
    Serial.println("ON");
  }
  else
  {
    Serial.println("OFF");
  }

  relayClient.write(switchCMD, sizeof(switchCMD));
  relayClient.flush();
  waitRelayResponse();
  relayLastSetStateMs = millis();

  return relayGetState(index);
}

//.................................................................................

boolean relayToggleState(uint8_t index)
{
  boolean currentState = relayGetState(index);
  currentState = !currentState;
  return relaySetState(index, currentState);
}

//.................................................................................

String relayHandleWebRequest(const String& url)
{
  if(url.startsWith("/relay/"))
  {
    int relayNdxPos = url.indexOf('/',7);
    String relayNdxStr = url.substring(7, relayNdxPos);
    int relayNdx = relayNdxStr.toInt();
    if(relayNdx > 0 && relayNdx <= 16)
    {
      String relayStateStr = url.substring(relayNdxPos+1);
      relayStateStr.trim();
      if(relayStateStr.equalsIgnoreCase(String("on")))
      {
        relaySetState(relayNdx-1, true);
      }
      else if(relayStateStr.equalsIgnoreCase(String("off")))
      {
        relaySetState(relayNdx-1, false);
      }
      else if(relayStateStr.equalsIgnoreCase(String("toggle")))
      {
        relayToggleState(relayNdx-1);
      }
      else
      {
        Serial.print("(web) relay set invalid value for ndx ");
        Serial.println(relayNdxStr.c_str());
        Serial.println(relayStateStr.c_str());
      }
    }
    else{
      Serial.print("(web) relay set invalid ndx");
    }
  }
  String result = "\"relay\":[";
  for(int i=0 ; i<16 ; i++)
  {
    if(relayGetState(i))
    {
      result += "1";
    }
    else
    {
      result += "0";
    }
    if(i <= 14){
      result +=",";
    }
  }
  result+= "]";
  return result;
}

//.................................................................................

void initRelayClient()
{
  lcd.drawLine(197,480,197,467,DARKGREEN);
  lcd.drawLine(197,467,320,467,DARKGREEN);
  lcd.drawLine(0  ,382,320,382,DARKGREEN);

  lcd.setTextColor(LIGHTRED);
  lcd.setTextSize(2);
  
  lcd.fillRect(  4, 386, 38, 38, DARKRED);
  lcd.fillRect( 43, 386, 38, 38, DARKRED);
  lcd.fillRect( 82, 386, 38, 38, DARKRED);
  lcd.fillRect(121, 386, 38, 38, DARKRED);
  lcd.fillRect(160, 386, 38, 38, DARKRED);
  lcd.fillRect(199, 386, 38, 38, DARKRED);
  lcd.fillRect(238, 386, 38, 38, DARKRED);
  lcd.fillRect(277, 386, 38, 38, DARKRED);
  
  lcd.fillRect(  4, 425, 38, 38, DARKRED);
  lcd.fillRect( 43, 425, 38, 38, DARKRED);
  lcd.fillRect( 82, 425, 38, 38, DARKRED);
  lcd.fillRect(121, 425, 38, 38, DARKRED);
  lcd.fillRect(160, 425, 38, 38, DARKRED);
  lcd.fillRect(199, 425, 38, 38, DARKRED);
  lcd.fillRect(238, 425, 38, 38, DARKRED);
  lcd.fillRect(277, 425, 38, 38, DARKRED);

  lcd.setCursor(6, 388);
  lcd.print("01");

  lcd.setCursor(45, 388);
  lcd.print("03");

  lcd.setCursor(84, 388);
  lcd.print("05");

  lcd.setCursor(123, 388);
  lcd.print("07");

  lcd.setCursor(162, 388);
  lcd.print("09");

  lcd.setCursor(201, 388);
  lcd.print("11");

  lcd.setCursor(240, 388);
  lcd.print("13");

  lcd.setCursor(279, 388);
  lcd.print("15");

  lcd.setCursor(6, 427);
  lcd.print("02");

  lcd.setCursor(45, 427);
  lcd.print("04");

  lcd.setCursor(84, 427);
  lcd.print("06");

  lcd.setCursor(123, 427);
  lcd.print("08");

  lcd.setCursor(162, 427);
  lcd.print("10");

  lcd.setCursor(201, 427);
  lcd.print("12");

  lcd.setCursor(240, 427);
  lcd.print("14");

  lcd.setCursor(279, 427);
  lcd.print("16");
  
  lcd.setCursor(4, 470);
  lcd.setTextColor(GREEN);
  lcd.setTextSize(1);
  lcd.print("Relay State");
  
  webServerURLHook[0] = &relayHandleWebRequest;
  if(connectToRelay()){
    relayClient.write(relayInitCMD, sizeof(relayInitCMD));
    relayClient.flush();
    waitRelayResponse();
  }
}



