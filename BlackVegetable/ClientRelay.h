byte relayIP[] = { 192, 168, 1, 17 };
IPAddress relayIPAddress(relayIP[0],relayIP[1],relayIP[2],relayIP[3]);
const char relayInitCMD[] = {0xaa, 0x1e,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xbb};
uint16_t relayPort = 8080;
EthernetClient relayClient;
SOCKET relayPingSocket = 0;
ICMPPing ping(relayPingSocket, (uint16_t)random(0, 255));
#define relayNotRespondingTimeout 5000
unsigned long relayLastRespondMs = 0;
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

boolean getRelayResponse()
{
  while(relayClient.connected() && relayClient.available() && relayReplyCount<relayReplyLength){
    relayReply[relayReplyCount] = relayClient.read();
    serialPrintHex(relayReply[relayReplyCount]);
    relayReplyCount++;
    relayLastRespondMs = millis();
  }

  if( relayReplyCount == relayReplyLength )
  {
    relayReplyCount = 0;
    Serial.println();
    if(relayReply[1] == 0x0c){
      Serial.println("relay switch state changed");
      for(int i=0 ; i<16 ; i++)
      {
        relayState[i] = (relayReply[2+i] == 0x01);
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
    Serial.print("relay ");
    Serial.print(index);
    Serial.print(" state = ");
    Serial.println((int)relayState[index]);
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

void initRelayClient()
{
  if(connectToRelay()){
    relayClient.write(relayInitCMD, sizeof(relayInitCMD));
    relayClient.flush();
    waitRelayResponse();
  }
}

