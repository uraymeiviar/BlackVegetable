#define MAX_WEBURL_HOOK 8
const uint16_t webServerPort = 80;
EthernetServer webServer(webServerPort);
#define webReceiveBufferMaxLen  256
char webReceiveBuffer[webReceiveBufferMaxLen];
int webReceiveBufferNdx = 0;
String (* webServerURLHook[MAX_WEBURL_HOOK])(const String& url);

void initWebServer() {
  for(int i=0 ; i<MAX_WEBURL_HOOK ; i++)
  {
    webServerURLHook[i] = NULL;
  }
  webServer.begin();
  Serial.print("webserver is at ");
  Serial.print(Ethernet.localIP());
  Serial.print(" port ");
  Serial.println(webServerPort);
}

#define HTTP_RESPONSE_HEADER "HTTP/1.1 200 OK\r\n\
Content-Type: application/json\r\n\
Cache-Control: no-cache\r\n\
Connection: close\r\n\
Content-Length: "

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
  String requestUrl(webReceiveBuffer);

  String response = "\r\n\r\n{\r\n\t\"device\":\"BlackVegetableOS 1.0\",\r\n";
  response += "\t\"mac\":\""+getMACString()+"\",\r\n";
  response += "\t\"ip\":\""+IpAddress2String(Ethernet.localIP())+"\"";
  
  for(int i=0 ; i<MAX_WEBURL_HOOK ; i++)
  {
    if(webServerURLHook[i] != NULL)
    {
      String reply = webServerURLHook[i](requestUrl);
      if(reply.length() > 0){
        response += ",\r\n";
        response += reply;
      }
    }
  }
  response += "\r\n}\r\n";
  
  webClient.print(HTTP_RESPONSE_HEADER);
  webClient.println(response.length());
  webClient.println(response.c_str());
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

