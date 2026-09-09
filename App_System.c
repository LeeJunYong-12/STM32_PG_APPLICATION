#include	"App_System.h"

#if 1 // yeom_20210118 [start]
sysConfig_t	sysConfig;
#endif // yeom_20210118 [end]
sysError_t	sysError;

u16 System_CalculateCRC16(u8 *pData, u32 size)
{
	u8	bitCnt, cData;
	u16 result = 0xffff;
	u32 byteCnt;

	if(size > 0)
	{
		for(byteCnt = 0; byteCnt < size; byteCnt++)
		{
			cData = pData[byteCnt];

			for(bitCnt = 0; bitCnt < 8; bitCnt++)
			{
				if(((result & 0x0001) ^ ((0x0001 * cData) & 0x0001)) > 0)
				{
					result = ((result >> 1) & 0x7fff) ^ 0x8408;
				}
				else
				{
					result = (result >> 1) & 0x7fff;
				}
				cData = (cData >> 1) & 0x7f;
			}
		}
	}

	return result;
}

#if 1 // yeom_20210118 [start]
void System_SysConfigAddCRC16(sysConfig_t *pConfig)
{
	pConfig->crc16 = System_CalculateCRC16(pConfig->u8Data, sizeof(sysConfig_t)-2);
}

u16 System_SysConfigCheckCRC16(sysConfig_t *pConfig)
{
	return System_CalculateCRC16(pConfig->u8Data, sizeof(sysConfig_t)-2);
}

u8 System_SysConfigRead(sysConfig_t *pConfig)
{
	EEPROM_Read(SYSTEM_CONFIG_ADRS, pConfig->u8Data, sizeof(sysConfig_t));

	return 1;
}

u8 System_SysConfigWrite(sysConfig_t *pConfig)
{
	u8	macAdrs[6];
	u16 secretCode[2];

	memset(macAdrs, NULL, sizeof(macAdrs));

	secretCode[0] = System_CalculateCRC16(pConfig->serialNo, 8);
	secretCode[1] = System_CalculateCRC16(pConfig->buildDate, 6);

	macAdrs[0] = 0xe7;
	macAdrs[1] = 0x50;
	macAdrs[2] = 0x20;
	macAdrs[3] = ((pConfig->serialNo[2] & 0x0f) << 4) | (pConfig->serialNo[3] & 0x0f);
	macAdrs[4] = ((pConfig->serialNo[4] & 0x0f) << 4) | (pConfig->serialNo[5] & 0x0f);
	macAdrs[5] = ((pConfig->serialNo[6] & 0x0f) << 4) | (pConfig->serialNo[7] & 0x0f);

	memcpy(pConfig->macAdrs, macAdrs, 6);
	
	pConfig->secretCode = secretCode[0];

	pConfig->secretCode <<= 16;

	pConfig->secretCode |= secretCode[1];

	pConfig->writeCnt += 1;

	pConfig->crc16 = System_CalculateCRC16(pConfig->u8Data, sizeof(sysConfig_t)-2);

	EEPROM_Write(SYSTEM_CONFIG_ADRS, pConfig->u8Data, sizeof(sysConfig_t));

	return 1;
}

u8 System_SysConfigCheck(sysConfig_t *pConfig)
{
	u8 result = 0;

	if(((pConfig->stx[0] == 'T') && (pConfig->stx[1] == 'E')) &&
		(pConfig->crc16 == System_SysConfigCheckCRC16(pConfig)))
	{
		result = 1;
	}

	return result;
}
#endif // yeom_20210118 [end]

