#include "Energia.h"
#include "SPI.h"
#include "LCD_Sharp128_SPI.h"
#include "RN4020.h"
#include <string.h>
#include <stdlib.h>

LCD_Sharp128_SPI myScreen;
RN4020 myBTLE;

char buffer2[64];

void setup()
{
	int test, j;
	char buffer[64];
	char error[10];
	buffer[0] = '\0';

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);

	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);

	myBTLE.begin();

	myBTLE.BT_Wake();
	delay(500);

	test = myBTLE.BT_SetupModule(buffer);

	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);

	myScreen.begin();
	myScreen.setFont(0);

	myScreen.text(10, 40, "BTLE Initialized");
	myScreen.text(10, 50, buffer);
	myScreen.flush();

	myScreen.text(10, 30, "LCD Initialized");
	myScreen.flush();

	delay(3000);
	myScreen.clear();

	strcpy(buffer2, "Storm Ninja BTLE test..");
}

void loop()
{
	char buff[128];
	uint16_t j;
	uint8_t state, led, y;

	myBTLE.BT_advertise();
	myBTLE.BT_SendPacket(buffer2);

	myScreen.text(10, 30, "BTLE Packet Sent..");
	myScreen.flush();
	y = 30;

	for(j=2000; j>0; j--) {
		if(myBTLE.IsNewRxData()) {
			if(myBTLE.BT_ReceivePacket(buff)) {
				y += 10;
				if(y > 120)
					y = 40;
				if(buff[1] == '0')
					state = 0;
				if(buff[1] == '1')
					state = 1;
				if(buff[0] == 'O')
					digitalWrite(LED2, state);
				else if(buff[0] == 'R')
					digitalWrite(LED1, state);
				else {
					myScreen.text(10, y, buff);
					myScreen.flush();
				}
			}
		}
		delay(1);
	}

	myScreen.clear();
	delay(1000);
}
