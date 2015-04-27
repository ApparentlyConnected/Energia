//
//  RN4020 Microchip Bluetooth LE library
//  Functions to communicate with a RN4020 Bluetooth LE module over UART
//
//
//  Author :  Nathan Crum
//  Date   :  Dec 21, 2014
//  Version:  1.00
//  File   :  RN4020.h
//
//  Based on the Microchip RN4020 die example
//  
// 

#include <msp430.h>
#include "RN4020.h"

RN4020::RN4020() {
	this->_pinMLDP		= P_MLDP;
	this->_pinWAKE_HW	= P_WAKE_HW;
	this->_pinWAKE_SW	= P_WAKE_SW;
	this->_pinConn		= P_CONN;
	this->_pinRTS		= P_RTS;

	this->start_time = 0;
	this->diff_time = 0;
	this->timerstate = Timeout;
}

// Public Functions
void RN4020::begin() {
    pinMode(this->_pinMLDP, OUTPUT);
    pinMode(this->_pinWAKE_HW, OUTPUT);
    pinMode(this->_pinWAKE_SW, OUTPUT);
    pinMode(this->_pinConn, INPUT);
    pinMode(this->_pinRTS, OUTPUT);

    digitalWrite(this->_pinMLDP, LOW);
    digitalWrite(this->_pinWAKE_HW, HIGH);
    digitalWrite(this->_pinWAKE_SW, HIGH);
    digitalWrite(this->_pinRTS, HIGH);

	Serial.begin(115200);
}

String RN4020::WhoAmI() {
	return "RN4020 BTLE Shuriken";
}

int RN4020::BT_SetupModule(char *Version) {
    char buffer[50];
    unsigned char i, changes = FALSE;
    unsigned char chk;

	BT_CMD_mode();
    ClearRXBuffer();

    //Send "V" to get firmware version of module
    BT_SendCommand("v\r");                          //Get RN4020 module firmware version
    if(!BT_GetResponse(buffer))                     //Get response
        return SETUP_VERFAIL;

    for(i = 0; i < 8; i++)                          //Copy up to eight unsigned chars of version number text returned by the module
    {
        Version[i] = buffer[i + 11];                //Copy only the text after the "v" before the version number
        if(Version[i] == ' ')                       //Look for space after the version number
        {
            Version[i] = '\0';                      //End the line after the version number to ignore the date
            break;
        }
    }
    Version[8] = '\0';                              //For safety just in case no spaces were detected

    //Send "GR" to get feature settings
    BT_SendCommand("gr\r");                         //Get RN4020 module feature settings

    if(!BT_CheckResponse("18002C00\r\n", SHRT_TIMEOUT))           //Check if features are set for auto advertize, MLDP, flow control
    {                                               //auto enable MLDP, suppress messages during MLDP
        BT_SendCommand("sr,18002C00\r");            //Features not correect so set features
        if(!BT_CheckResponse("AOK\r\n", SHRT_TIMEOUT))
            return SETUP_FEATFAIL;
        changes = TRUE;                             //Have changed a setting so will have to reboot later
    }

    //Send "GN" to get the name of the device
    BT_SendCommand("gn\r");                         //Get RN4020 module name
    if(!BT_CheckResponse("SN_BTLE_Shkn\r\n", SHRT_TIMEOUT))        //Check if name is what we want
    {
        BT_SendCommand("sn,SN_BTLE_Shkn\r");         //Set name to what we want
        if(!BT_CheckResponse("AOK\r\n", SHRT_TIMEOUT))
            return SETUP_NAMEFAIL;
        changes = TRUE;                             //Have changed a setting so will have to reboot later
    }

    //If settings were changed then reboot and verify that changes were saved
    if(changes == TRUE)                             //See if we made any changes and need a reboot
    {
        BT_SendCommand("wc\r");                     //Command to clear script, just in case there is a script
        if(!BT_CheckResponse("AOK\r\n", SHRT_TIMEOUT))
            return SETUP_SCRPTFAIL;

        //Send "R,1" to save changes and reboot
        chk = BT_Reboot();
        if(chk !=  REBOOT_OK)
            return chk;

        //Send "GR" to get feature settings
        BT_SendCommand("gr\r");                     //Get RN4020 module feature settings
        if(!BT_CheckResponse("18002C00\r\n", SHRT_TIMEOUT))       //Check if features are correct
            return SETUP_CKFTFAIL;

        //Send "GN" to get the name of the device
        BT_SendCommand("gn\r");                     //Get RN4020 module name
        if(!BT_CheckResponse("SN_BTLE_Shkn\r\n", SHRT_TIMEOUT))    //Check if name is what we want
            return SETUP_CKNMFAIL;
    }

    return SETUP_OK;
}

char RN4020::BT_SendPacket(char *messageout) {
	char buffer[MAX_MESSAGE];

	if(!BT_checkMLDP())
		return FALSE;

	buffer[0] = BT_SOF_1;
	buffer[1] = BT_SOF_2;
	buffer[2] = '\0';
	strcat(buffer, messageout);
	strcat(buffer, "\r\n");
    uart_send(buffer);

    return TRUE;
}

char RN4020::BT_ReceivePacket(char *messagein) {
	static BluetoothDecodeState btDecodeState = WaitForData;
	char messageChar;                                   //unsigned char read from the UART buffer

    char buffer[MAX_MESSAGE];	// input message
    char *buf_ptr = buffer;
    buffer[0] = '\0';

    while (IsNewRxData())                               //Check if new data unsigned char from BT module and return if nothing new
    {
        messageChar = Serial.read();                   //Read the data unsigned char for processing

        switch(btDecodeState)
        {
//        case WaitForSOF1:                               //Waiting to receive first start of frame character, SOF1
//            if (messageChar == BT_SOF_1)                //See if we got the SOF1 character
//                btDecodeState = WaitForSOF2;            //then wait for the second start of frame character, SOF2
//            break;
//        case WaitForSOF2:                               //Waiting to receive second start of frame character, SOF2
//            if(messageChar == BT_SOF_2)                 //See if this is the second of the two unsigned char start Of frame
//                btDecodeState = WaitForData;         	//Is complete SOF1, SOF2 pattern so wait for command unsigned char
//            else
//                btDecodeState = WaitForSOF1;            //Did not get complete SOF1, SOF2 pattern so look for new start of frame
//            break;
//        case WaitForCommand:                            //Waiting to receive command unsigned char
//            Message->Command = messageChar;             //Save unsigned char as command unsigned char of the message
//            btDecodeState = WaitForData;                //Indicate now waiting for data unsigned char
//            break;
        case WaitForData:                               //Waiting to receive data unsigned char
            if(messageChar == '\r') {                   //See if this is the CR
                btDecodeState = WaitForLF;              //Is CR so wait for LF
                *buf_ptr = '\0';
            }
            else {
            	*buf_ptr = messageChar;					// append char to string
            	buf_ptr++;
            	if ((buf_ptr - buffer) >= (MAX_MESSAGE - 1)) {	// max message length reached
            		buffer[MAX_MESSAGE - 1] = '\0';
            		btDecodeState = WaitForLF;
            	}
            }
            break;
        case WaitForLF:
            btDecodeState = WaitForSOF1;                //Will be looking for a new packet next
            if(messageChar == '\n') {                   //See if this is the LF
            	strcpy(messagein, buffer);
                return TRUE;                            //Got a complete message!
            }
            break;
        default:                                        //Invalid state so start looking for a new start of frame
            btDecodeState = WaitForSOF1;
        }
    }
    return FALSE;
}

void RN4020::BT_SendCommand(char *data) {
	uart_send(data);
}

int RN4020::BT_Reboot(void) {
	ClearRXBuffer();
    //	Send "R,1" to save changes and reboot
	BT_SendCommand("r,1\r");                        //Force reboot
    if(!BT_CheckResponse("Reboot\r\n", SHRT_TIMEOUT))
        return REBOOT_CMDFAIL;

    startTimeout(3000);                     //Start  timeout to wait for the reboot to occur
    while(!IsNewRxData())                           //Wait for new unsigned char from UART
    {
        if(checkTimeout())             //Check if 3s timed out
            return REBOOT_3K_TO;                    //If timed out then return failure
    }

    if(!BT_CheckResponse("CMD\r\n", SHRT_TIMEOUT))  //Check that we received CMD indicating reboot is done
        return REBOOT_ERR;

    return REBOOT_OK;
}

void RN4020::BT_factReset(void) {
	int i;

	for(i=0; i<3; i++) {
		delay(100);
		BT_Sleep();
		delay(100);
		BT_Wake();
	}
	delay(3000);
}

void RN4020::BT_Wake(void) {
	digitalWrite(this->_pinWAKE_HW, HIGH);
	digitalWrite(this->_pinWAKE_SW, HIGH);
	BT_CMD_low();
}

void RN4020::BT_Sleep(void) {
	BT_CMD_mode();								// enter command mode
	digitalWrite(this->_pinWAKE_HW, LOW);
	digitalWrite(this->_pinWAKE_SW, LOW);
	BT_SendCommand("O");                        // Enter Dormant mode (<700nA)
}

// enable radio advertise info
int RN4020::BT_advertise(void) {

	BT_CMD_mode();								// enter command mode
	ClearRXBuffer();

	BT_SendCommand( "A\r" );					// Advertise
	if(!BT_CheckResponse("AOK\r\n", SHRT_TIMEOUT))  //Check that we received AOK confirmation
		return ADVERT_ERR;

    return TRUE;
}

//**********************************************************************************************************************
// Check if data has been added to the receive buffer
int RN4020::IsNewRxData(void) {

	return Serial.available();
}

// Private functions
int RN4020::BT_checkMLDP(void) {
	BT_CMD_mode();					// enter command mode so we can check if we succesfully enter MLDP

	ClearRXBuffer();
	BT_SendCommand("I\r");						// request MLDP mode
	BT_CMD_low();

	if(!BT_CheckResponse("MLDP\r\n", SHRT_TIMEOUT)) {	// if we're not ready to go - this will kick us out
		BT_CMD_mode();
		return FALSE;
	}

	return TRUE;
}

int RN4020::BT_GetResponse(char *data) {
   unsigned int byteCount = 0;
    char newByte;

    startTimeout(2000);               //Start 100ms timeout for routine

    while(byteCount < 50)                           //Don't accept more than the buffer size
    {
        if(IsNewRxData())                           //Check if new data bytes from BT module and return if nothing new
        {
            newByte = ReadRxBuffer();               //Read the data bytes for processing
            *data++ = newByte;                      //Add it to the buffer
            byteCount++;                            //Keep track of the number of bytes received
            if(newByte == '\n')                     //Check if got linefeed
                return TRUE;                        //If linefeed then return success
        }
        if(checkTimeout())            //Check if timed out
            return FALSE;                           //If timed out then return failure
    }
    return FALSE;
}

int RN4020::BT_GetStrResponse(char *data) {
    unsigned int byteCount = 0;
    char newByte;

    startTimeout(2000);                      //Start 100ms timeout for routine

    while( byteCount < 50)                          //Don't accept more than the buffer size
    {
        if(IsNewRxData())                           //Check if new data bytes from BT module and return if nothing new
        {
            newByte = (char) ReadRxBuffer();        //Read the data bytes for processing
            *data++ = newByte;                      //Add it to the buffer
            byteCount++;                            //Keep track of the number of bytes received

            if(newByte == '\r')                     //Check if got carraige return
            	*data++ = '\0';

            if(newByte == '\n')                     //Check if got carraige return
            	return TRUE;                        //If linefeed then return success

        }
        if(checkTimeout())                  //Check if timed out
            return FALSE;                           //If timed out then return failure
    }
    return FALSE;                                   //Did not reach end of string so return failure
}

int RN4020::BT_CheckResponse(char *data, int timeout) {
    unsigned int i, ByteCount = 0;
    char NewByte, Buffer[50], *BufPtr;

    startTimeout(timeout);                  //Start timeout for routine

    BufPtr = Buffer;
    while(ByteCount < 50)                           //Don't accept more than the buffer size
    {
        if(IsNewRxData())                           //Check if new data byte from BT module and return if nothing new
        {
            NewByte = (char) ReadRxBuffer();        //Read the data byte for processing
            *BufPtr++ = NewByte;                    //Add it to the buffer
            ByteCount++;
            if(NewByte == '\n')                     //Check if got linefeed
                break;                              //If linefeed then we have what we want
        }
        if(checkTimeout())                     //Check if timed out
            return FALSE;                           //If timed out then return failure
    }

    BufPtr = Buffer;
    for(i = 0; i < ByteCount; i++)                  //Compare all bytes received
    {
        if(*data == '\0')                           //See if reached end of string with no bytes different
            return TRUE;                            //No bytes were different so return success
        else if(*data++ != *BufPtr++)               //else see if the bytes are different
            return FALSE;                           //Bytes differ so return failure
    }
    return TRUE;                                    //All bytes matched so return success
}

int RN4020::BT_CheckResponseWithWildcard(char *data, char Wildcard) {
    unsigned int i, ByteCount = 0;
	char NewByte, Buffer[50], *BufPtr;

	startTimeout(Timeout);                  //Start timeout for routine

    BufPtr = Buffer;
    while(ByteCount < 50)                           //Don't accept more than the buffer size
    {
        if(IsNewRxData())                           //Check if new data bytes from BT module and return if nothing new
        {
            NewByte = (char) ReadRxBuffer();        //Read the data bytes for processing
            *BufPtr++ = NewByte;                    //Add it to the buffer
            ByteCount++;
            if(NewByte == '\n')                     //Check if got linefeed
                break;                              //If linefeed then we have what we want
        }
        if(checkTimeout())                  //Check if timed out
            return FALSE;                           //If timed out then return failure
    }

    BufPtr = Buffer;
    for(i = 0; i < ByteCount; i++)                  //Compare all bytes received
    {
        if(*data == '\0')                           //See if reached end of string with no bytes different
            return TRUE;                            //No bytes were different so return success
        else if(*data++ != *BufPtr++)               //else see if the bytes are different
            return FALSE;                           //bytes differ so return failure
    }
    return TRUE;                                    //All bytes matched so return success
}

int RN4020::BT_CompareResponse(char *data1, char *data2) {
    unsigned int i;

    for(i = 0; i < 50; i++)                         //Compare up to 50 bytes
    {
        if(*data1 == '\0')                          //See if reached end of string with no bytes different
            return TRUE;                            //No bytes were different so return success
        else if(*data1++ != *data2++)               //else see if the bytes are different
            return FALSE;                           //bytes differ so return failure
    }
    return FALSE;                                   //Did not reach end of string so return failure
}

void RN4020::BT_io_init(void) {
	BT_CMD_low();
	BT_Sleep();
}

void RN4020::BT_CMD_high(void) {
	digitalWrite(this->_pinMLDP, HIGH);
}

void RN4020::BT_CMD_low (void) {
	digitalWrite(this->_pinMLDP, LOW);
}

void RN4020::BT_CMD_mode(void) {	
	BT_CMD_high();
	BT_CMD_low();
}

//**********************************************************************************************************************
// Send a string to the transmit buffer
void RN4020::uart_send(char *data) {
	if(data == 0)
		return;

	if(*data == '\0')
		return;

	Serial.write(data);
}

//**********************************************************************************************************************
// Read a unsigned char from the receive buffer``````````````````````````````````````````````````````````````````````````````````````````````````````````````````
char RN4020::ReadRxBuffer(void)
{
	return Serial.read();
}

//**********************************************************************************************************************
// Clear the receive buffer``````````````````````````````````````````````````````````````````````````````````````````````````````````````````
void RN4020::ClearRXBuffer(void) {
	char junk;
	
	while(Serial.available() > 0) {
		junk = Serial.read();
	}
}

/* software watchdog timer (millisecond) */
void RN4020::startTimeout(uint32_t mils) {
	// Grab the current 32-bit micros value
	this->start_time = millis();

	// Reset timeout status
	this->timerstate = Running;
	this->diff_time = mils;
}

/* check watchdog timer status */
int RN4020::checkTimeout() {
	uint32_t cur_time;

	if(this->timerstate == Timeout)
		return 1;

	cur_time = millis();

	if (( cur_time - this->start_time ) > this->diff_time ) {
		this->timerstate = Timeout;
		return 1;
	}
	else
		return 0;

}
