#include "Energia.h"
#include "SPI.h"
#include "LCD_Sharp128_SPI.h"
#include "RN4020.h"
#include <string.h>
#include <stdlib.h>

LCD_Sharp128_SPI myScreen;
RN4020 myBTLE;

#define BTLE_CON	6

void btle_interrupt(void);

char buffer2[64];
char MAC_addr[64];

void setup()
{
	int test, i;
	char buffer[64];
	char error[10];
	buffer[0] = '\0';

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);

	digitalWrite(LED1, HIGH);
	digitalWrite(LED2, HIGH);

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

	if(test == SETUP_OK)
		myScreen.text(10, 40, "BTLE Initialized");
	else
		myScreen.text(10, 40, "BTLE Init Failed");
	myScreen.text(10, 50, buffer);
	myScreen.flush();

	myScreen.text(10, 30, "LCD Initialized");
	myScreen.flush();

	delay(3000);
	myScreen.clear();

	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);

	attachInterrupt(BTLE_CON, btle_interrupt, RISING);

	myBTLE.BT_GetDeviceInfo(buffer2);
	for(i=0; i<6; i++) {
		MAC_addr[i*3] = buffer2[i*2+4];
		MAC_addr[i*3+1] = buffer2[i*2+5];
		MAC_addr[i*3+2] = '.';
	}
	MAC_addr[17] = '\0';

	strcpy(buffer2, "Storm Ninja BTLE test..");
}

void loop()
{
	char buff[128];
	uint16_t j;
	uint8_t state, led, y;
	uint8_t btle_con = 1;

	y = 30;

	if(myBTLE.IsConnected()) {							// if we're connected
		btle_con = 1;
		myScreen.clear();								// clear the screen
		myBTLE.BT_SendPacket(buffer2);					// send a text packet
		myScreen.text(10, 20, "BTLE Connected");		// update the screen
		myScreen.text(10, 30, "BTLE Packet Sent..");	// update the screen
		myScreen.flush();

		while(myBTLE.IsConnected()) {					// process packets while connected
			if(myBTLE.IsNewRxData()) {
				delay(5);
				if(myBTLE.BT_ReceivePacket(buff)) {
					y += 10;
					if(y > 120)
						y = 40;
					if(buff[1] == '0')
						state = LOW;
					if(buff[1] == '1')
						state = HIGH;
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
			delay(100);
		}
	}
	else {
		if(btle_con) {
			btle_con = 0;
			myBTLE.BT_advertise();
			myBTLE.BT_Rest();
			myScreen.clear();								// clear the screen
			myScreen.text(10, 10, "BTLE Ready..");			// update the screen
			myScreen.text(10, 20, MAC_addr);
			myScreen.flush();
		}
		disableWatchDog();
		__bis_SR_register(LPM3_bits + GIE);
		enableWatchDog();
	}

//	if(myBTLE.IsEvent())
//		myBTLE.BT_Wake();

	delay(2000);
}

//__attribute__((interrupt(PORT3_VECTOR)))
void btle_interrupt(void) {
}
