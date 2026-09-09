#ifndef	_APP_SYSTEM_H
#define	_APP_SYSTEM_H

#include	"E7602_PG_Application.h"

typedef	struct{
	u8	microSD;
	u8	nandFlash;
}sysError_t;

#define SYSTEM_CONFIG_ADRS		0x400

#if 1 // yeom_20210118 [start]
typedef	union{
	u8	u8Data[256];
	__packed struct{
		u8	stx[2];			// "TE"
		u16	writeCnt;		// EEPROM Write Count
		u8	modelType[10];	// modelType ex: E7502-X000
		u8	serialNo[8];	// serialNo
		u8	buildDate[6];	// buildDate ex: 161216
		u32	secretCode;		// (Calculate CRC16 SerialNo) ^ (Calculate CRC16 buildDate)
		u8	hwType;
		u8	macAdrs[6];		// MAC
		u8	ipAdrs[4];		// IP
		u8	nmAdrs[4];		// Network Mask
		u8	gwAdrs[4];		// Gateway
		u8	ethEN;			// Ethernet Enable
		u8	userLogEnable;	// System Log Enable
		u8	dsiMode;		// MIPI DSI Mode(0=NONE, 1=SSD2829)
		u8	odPClk;
		u8	odBClk;
		u16	networkPort;
		u8	debugLogEnable;
		u8	mDrive;
		// Main Driver No.
		u8	networkMode;	// Ethernet Mode(TCP/UDP)

		u8	unUse[194];

		u16	crc16;
	};
}sysConfig_t;
#endif // yeom_20210118 [end]

#if 1 // yeom_20210118 [start]
extern	sysConfig_t	sysConfig;
#endif // yeom_20210118 [end]
extern	sysError_t	sysError;

u16 System_CalculateCRC16(u8 *pData, u32 size);
#if 1 // yeom_20210118 [start]
u8 System_SysConfigRead(sysConfig_t *pConfig);
u8 System_SysConfigWrite(sysConfig_t *pConfig);
u8 System_SysConfigCheck(sysConfig_t *pConfig);
#endif // yeom_20210118 [end]

#endif	// _APP_SYSTEM_H