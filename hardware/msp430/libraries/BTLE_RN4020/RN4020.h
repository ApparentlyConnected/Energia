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
//  Functions to communicate with a RN4020 Bluetooth LE module over a UART
//  

#ifndef RN4020_h
#define RN4020_h

#include "Energia.h"

#define BT_SOF_1 '='			// Start of frame to or from BLE Central
#define BT_SOF_2 '>'			// Start of frame to or from BLE Central

// ************ Characteristic Property Bits ************ //
#define	EXT_PROP	0b10000000	// Additional property available
#define AUTH_WRITE	0b01000000	// Write characteristic with authentication from client to server
#define	INDICATE	0b00100000	// Indicate value of characteristic with acknowledgement from server to client
#define	NOTIFY		0b00010000	// Notify value of characteristic without acknowledgement from server to client
#define WRITE		0b00001000	// Write value of characteristic with acknowledgement from client to server
#define	WR_WO_R		0b00000100	// Write value of characteristic without acknowledgement from client to server
#define	READ		0b00000010	// Read value of characteristic. Value is sent from server to client
#define	BROADCAST	0b00000001	// Broadcast value of characteristic

#define SHRT_TIMEOUT	200
#define LONG_TIMEOUT	3000

// ************ Error Codes ************ //
#define SETUP_OK		99
#define SETUP_VERFAIL	98
#define SETUP_FEATFAIL	97
#define SETUP_NAMEFAIL	96
#define SETUP_SCRPTFAIL	95
#define SETUP_CKFTFAIL	94
#define SETUP_CKNMFAIL	93
#define SETUP_FCRSTFAIL	92

#define REBOOT_OK		89
#define REBOOT_CMDFAIL	88
#define REBOOT_200_TO	87
#define REBOOT_0ERR		86
#define REBOOT_3K_TO	85
#define REBOOT_ERR		84

#define ADVERT_ERR		79

// ************ Default Pins ************ //
#define P_MLDP			4
#define P_WAKE_HW	2
#define P_WAKE_SW	3
#define P_CONN			6
#define P_RTS				5

// ************ Communication ************ //
#define MAX_MESSAGE		128
#define TRUE			1
#define FALSE			0

struct BT_Message
{
    char Command;
    char Data;
};

typedef enum {Running, Timeout} TimeoutState;
typedef enum {NotConnected, Connected} WhichBluetoothState;
typedef enum {WaitForSOF1, WaitForSOF2, WaitForCommand, WaitForData, WaitForCR, WaitForLF} BluetoothDecodeState;

class RN4020 {
public:
//
	RN4020();
	void begin();
	String WhoAmI();
	int BT_SetupModule(char *Version);
	char BT_SendPacket(char *messageout);
	char BT_ReceivePacket(char *messagein);
	void BT_SendCommand(char *data);
	int BT_Reboot(void);
	void BT_factReset(void);
	void BT_Wake(void);
	void BT_Sleep(void);
	int BT_advertise(void);
	int IsNewRxData(void);
//
private:
//
	int BT_checkMLDP(void);
	int BT_GetResponse(char *data);
	int BT_GetStrResponse(char *data);
	int BT_CheckResponse(char *data, int timeout);
	int BT_CheckResponseWithWildcard(char *data, char Wildcard);
	int BT_CompareResponse(char *data1, char *data2);
	void BT_io_init(void);
	void BT_CMD_high(void);
	void BT_CMD_low (void);
	void BT_CMD_mode(void);
	void sleep_UART(void);
	void uart_send(char *data);
	char ReadRxBuffer(void);
	void ClearRXBuffer(void);

	void startTimeout(uint32_t mils);	/* software watchdog timer (millisecond) */
	int checkTimeout();					/* check timer status */
	TimeoutState timerstate;
	uint32_t start_time;
	uint32_t diff_time;

	uint8_t _pinMLDP;
	uint8_t _pinWAKE_HW;
	uint8_t _pinWAKE_SW;
	uint8_t _pinConn;
	uint8_t _pinRTS;
//
};

#endif
