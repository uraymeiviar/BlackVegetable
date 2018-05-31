const uint16_t webServerPort = 80;
EthernetServer webServer(webServerPort);
void initWebServer() {
  webServer.begin();
  Serial.print("webserver is at ");
  Serial.print(Ethernet.localIP());
  Serial.print(" port ");
  Serial.println(webServerPort);
}

#define webReceiveBufferMaxLen  256
char webReceiveBuffer[webReceiveBufferMaxLen];
int webReceiveBufferNdx = 0;

void responseToRequest(EthernetClient& webClient)
{ 
  int urlStartNdx = -1;
  char* urlRequestPtr = &webReceiveBuffer[0];
  for(int i=0 ; i<webReceiveBufferNdx ; i++)
  {    
    if(webReceiveBuffer[i] == ' ')
    {
      if(urlStartNdx == -1)
      {
        urlStartNdx = i;
      }
      else
      {
        *urlRequestPtr = webReceiveBuffer[i];
        urlRequestPtr++;
        *urlRequestPtr = 0;
        break;
      }
    }
    else if(urlStartNdx >= 0)
    {
      *urlRequestPtr = webReceiveBuffer[i];
      urlRequestPtr++;
    }
  }
  Serial.print("URL request = ");
  Serial.println(webReceiveBuffer);
  
  webClient.println("HTTP/1.1 200 OK");
  webClient.println("Content-Type: application/json");
  webClient.println("Cache-Control: no-cache");
  webClient.println("Connection: close"); 
  webClient.println("Content-Length: 34");
  webClient.println("{\"device\":\"BlackVegetableOS 1.0\"}");
}

void loopWebServer()
{
  EthernetClient webClient = webServer.available();
  if (webClient) 
  {
    webReceiveBufferNdx = 0;
    uint8_t remoteIP[4];
    getRemoteIP(webClient,remoteIP);
    char ipString[20];
    formatIPAddr(remoteIP,ipString);
  
    Serial.print("webserver new client from ");
    Serial.println(ipString);
    
    boolean currentLineIsBlank = true;
    while (webClient.connected()) 
    {
      if (webClient.available()) 
      {
        char c = webClient.read();
        if(webReceiveBufferNdx < webReceiveBufferMaxLen-1)
        {
          webReceiveBuffer[webReceiveBufferNdx] = c;
          webReceiveBufferNdx++;
        }

        if ( c == '\n' && currentLineIsBlank) {
          webReceiveBuffer[webReceiveBufferNdx] = 0;
          responseToRequest(webClient);
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    webClient.stop();
    Serial.println("webserver client disconnected");
  }
}

