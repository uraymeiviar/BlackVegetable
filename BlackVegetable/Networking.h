#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <ICMPPing.h>
extern "C" {
 #include "utility/w5100.h"
}

void getRemoteIP(EthernetClient& client, uint8_t *ptRemoteIP)
{
  W5100.readSnDIPR(client.getSocketNumber(), ptRemoteIP);
}

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

void formatIPAddr(const uint8_t* ip, char* out)
{
  sprintf(out, "%d.%d.%d.%d", ip[0], ip[1], ip[2],ip[3]);
}

void serialPrintIPAddr(const uint8_t* ip)
{
  char ipString[20];
  formatIPAddr((const uint8_t*)ip,ipString);
  Serial.print(ipString);
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

String getMACString()
{
  String result = "";
  for(int i=0 ; i<6 ; i++)
  {
    if(mac[i] <= 0x0F)
    {
      result += "0";
    }
    result += String(mac[i],16);
    if(i < 5)
    {
      result += ":";
    }
  }
  return result;
}

void printMyIPAddress(const uint32_t ip)
{
  String myIpStr = IpAddress2String(Ethernet.localIP());
  myIpStr +=" "+getMACString();

  Serial.print("My IP address: ");
  Serial.print(myIpStr.c_str());
  Serial.println();

  lcd.setTextSize(1);
  lcd.fillRect(130, 0, 110, 10, BLACK);
  lcd.setTextColor(WHITE);
  lcd.setCursor(132, 2);
  lcd.print(myIpStr.c_str());
}

void initNetworking(void)
{
  Serial.println("initializing networking...");
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
  }
  printMyIPAddress(Ethernet.localIP());
  Serial.println("networking initialization done.");
}

void loopNetworking(void)
{
    switch (Ethernet.maintain())
    {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;
  
    case 2:
      //renewed success
      Serial.println("Renewed success");
  
      //print your local IP address:
      printMyIPAddress(Ethernet.localIP());
      break;
  
    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;
  
    case 4:
      //rebind success
      Serial.println("Rebind success");
  
      //print your local IP address:
      printMyIPAddress(Ethernet.localIP());
      break;
  
    default:
      //nothing happened
      break;
  
    }
}
