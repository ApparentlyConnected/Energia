#include "Energia.h"
#include "SPI.h"
#include "LCD_Sharp128_SPI.h"

static const int pinDISP = 11;  // D02 (P2.2)
static const int pinVCC  = 11;  // D02 (P2.2)
static const int pinCS   = 06;  // D07 (P1.0)

LCD_Sharp128_SPI myScreen(pinCS, pinDISP, pinVCC);
uint8_t k = 0;

void setup()
{
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  
  myScreen.begin();
  
  myScreen.setFont(1);
  myScreen.text(10, 10, "Hello!");
  myScreen.flush();  
  
  delay(1000);
  myScreen.clear();
}

void loop()
{
   k++;
    myScreen.clearBuffer();
    myScreen.setFont(0);
    myScreen.text(k, 10, "ABCDE");
    for (uint8_t i=10; i<LCD_HORIZONTAL_MAX-10; i++) {
      myScreen.setXY(i,20,1);
    }
    
    for (uint8_t i=0; i<=20; i++) {
      myScreen.setXY(50+i,30,1);
    }
    for (uint8_t i=0; i<=20; i++) {
      myScreen.setXY(50,30+i,1);
    }
    for (uint8_t i=0; i<=20; i++) {
      myScreen.setXY(50+i,50,1);
    }
    for (uint8_t i=0; i<=20; i++) {
      myScreen.setXY(70,30+i,1);
    }
    
    myScreen.setFont(1);
    myScreen.text(10, 60, "ABC");
    myScreen.flush();  
    delay(200);
}
