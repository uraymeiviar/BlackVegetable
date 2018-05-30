byte relayIP[] = { 192, 168, 1, 98 };
const char relayInitCMD[] = {0xaa, 0x1e,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xbb};
uint16_t relayPort = 8080;
EthernetClient relayClient;

void serialPrintHex(const char value)
{
  Serial.print(" ");
  if(value < 16)
  {
    Serial.print("0");
  }
  Serial.print(value,HEX);
}

void initRelayClient()
{
  Serial.print("connecting to relay ");
  Serial.print(relayIP[0],DEC);
  Serial.print(".");
  Serial.print(relayIP[1],DEC);
  Serial.print(".");
  Serial.print(relayIP[2],DEC);
  Serial.print(".");
  Serial.print(relayIP[3],DEC);
  Serial.println();
  
  if (relayClient.connect(relayIP, relayPort)) 
  {
    Serial.println("connected to relay");
  } 
  else 
  {
    Serial.println("connection to relay failed");
  }
}

const int relayReplyLength = 20;
int relayReplyCount = 0;
char relayReply[20] = {0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                       0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

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

void loopRelayClient()
{
  if (relayClient.available()) 
  {
    do{
      relayReply[relayReplyCount] = relayClient.read();
      //serialPrintHex(relayReply[relayReplyCount]);
      relayReplyCount++;
    }
    while(relayClient.available() && relayReplyCount<relayReplyLength-1);
    
    if(relayReplyCount == relayReplyLength)
    {
      //Serial.println();
      if(relayReply[1] == 0x0c){
        Serial.println("relay switch state changed");
        for(int i=0 ; i<16 ; i++)
        {
          printRelayState(i);
        }
      }
      relayReplyCount = 0;
    }
  }

  if (!relayClient.connected()) 
  {
    Serial.println("Relay disconnected.");
    relayClient.stop();
  }
}

boolean relayGetState(uint8_t index)
{
  if(index >= 0 && index < 16)
  {
    return (relayReply[2+index] == 0x01);
  }
  else
  {
    return false;
  }
}

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
  relayClient.write(relayInitCMD, sizeof(relayInitCMD));
  loopRelayClient();
  relayClient.write(switchCMD, sizeof(switchCMD));
  loopRelayClient();
  return relayGetState(index);
}

