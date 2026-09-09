#include	"App_EEPROM.h"

/*
	USE		: Microchip 24LC16B
	Adrs	: SoftAdrs = 0xa0
			  HardAdrs Not USE
*/

#define	EEPROM_DEV_ADRS				0xA0
#define	EEPROM_PAGE_SIZE			16
#define	EEPROM_WRITE_DELAY			5
#define	EEPROM_MAX_SIZE				2048

u8 EEPROM_Read(u16 adrs, u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	devAdrs = EEPROM_DEV_ADRS, mask = 0xF0;

	devAdrs &= mask;
	devAdrs |= (adrs >> 7) & 0x0E;

	rtn = I2C_MemRead(devAdrs, (u8)adrs, 0, pData, size);
	if(rtn == 0)			return result;

	result = 1;

	return result;
}

u8 EEPROM_Write(u16 adrs, u8 *pData, u16 size)
{
	u8	result = 0, rtn;
	u8	devAdrs = EEPROM_DEV_ADRS, mask = 0xF0;
	u16	cnt = 0, remainSize = size;

	do{
		if(remainSize > EEPROM_PAGE_SIZE)
		{
			devAdrs &= mask;
			devAdrs |= ((adrs + cnt) >> 7) & 0x0E;

			rtn = I2C_MemWrite(devAdrs, (u8)(adrs + cnt), 0, &pData[cnt], EEPROM_PAGE_SIZE);
			if(rtn == 0)		return result;

			cnt			+= EEPROM_PAGE_SIZE;
			remainSize	-= EEPROM_PAGE_SIZE;
		}
		else
		{
			devAdrs &= mask;
			devAdrs |= ((adrs + cnt) >> 7) & 0x0E;

			rtn = I2C_MemWrite(devAdrs, (adrs + cnt), 0, &pData[cnt], remainSize);
			if(rtn == 0)		return result;

			break;
		}

		HAL_Delay(EEPROM_WRITE_DELAY);
	}while(1);

	result = 1;

	return result;
}

