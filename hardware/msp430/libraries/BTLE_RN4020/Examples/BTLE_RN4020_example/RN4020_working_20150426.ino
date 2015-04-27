#include "Energia.h"
#include "SPI.h"
#include "LCD_Sharp128_SPI.h"
#include "RN4020.h"
#include <string.h>
#include <stdlib.h>

LCD_Sharp128_SPI myScreen;
RN4020 myBTLE;
char gbuffer[30];

void setup()
{
  int test;
  char buffer[32];
  char buffer2[16];
  buffer[0] = '\0';

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  myScreen.begin();
  myScreen.setFont(0);
  myScreen.text(10, 30, "LCD Initialized");
  myScreen.flush();

  myBTLE.begin();

//  myBTLE.BT_Sleep();
  myBTLE.BT_Wake();
  delay(500);

  test = myBTLE.BT_SetupModule(buffer2);

  if(test == SETUP_OK) {
	  myScreen.text(10, 40, "BTLE Initialized");
	  strcpy(buffer, "Version: ");
	  strcat(buffer, buffer2);
	  myScreen.text(10, 50, buffer2);
  }
  else
	  myScreen.text(10, 40, "BTLE Setup Failed");
  myScreen.flush();

  delay(3000);
  myScreen.clear();

  strcpy(gbuffer, "Storm Ninja BTLE Test..");
}

void loop()
{
	uint16_t i;
	char sbuffer[64];

	myScreen.text(10, 30, "BTLE Send Packet..");
	myScreen.flush();

	myBTLE.BT_advertise();
	myBTLE.BT_SendPacket(gbuffer);
	delay(2000);

	myScreen.clear();
	delay(1000);
}
