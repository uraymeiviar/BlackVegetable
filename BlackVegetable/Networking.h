#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};

void printIPAddress()
{
  char ipString[20];
  sprintf(ipString, "%d.%d.%d.%d", 
    Ethernet.localIP()[0], 
    Ethernet.localIP()[1], 
    Ethernet.localIP()[2],
    Ethernet.localIP()[3]);

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
  printIPAddress();
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
      printIPAddress();
      break;
  
    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;
  
    case 4:
      //rebind success
      Serial.println("Rebind success");
  
      //print your local IP address:
      printIPAddress();
      break;
  
    default:
      //nothing happened
      break;
  
    }
}
