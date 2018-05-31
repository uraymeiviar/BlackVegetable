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

void printMyIPAddress(const uint32_t ip)
{
  char ipString[20];
  formatIPAddr((const uint8_t*)&ip,ipString);

  Serial.print("My IP address: ");
  Serial.print(ipString);
  Serial.println();

  lcd.setTextSize(1);
  lcd.fillRect(160, 0, 80, 10, BLACK);
  lcd.setTextColor(WHITE);
  lcd.setCursor(162, 2);
  lcd.print(ipString);
}

void initNetworking(void)
{
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
  }
  printMyIPAddress(Ethernet.localIP());
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
