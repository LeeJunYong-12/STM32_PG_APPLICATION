#include	"BSP_Flash.h"

static bspFlashBootApplication_t	*pFlashBootApplication;

void BSP_Flash_JumpToApplication(void *pVoid)
{
	pFlashBootApplication = (bspFlashBootApplication_t*)pVoid;

	if((pFlashBootApplication->stackAdrs & 0xfff00000) != BSP_FLASH_BOOT_STACK_ADRS)
	{
		return;
	}

	__set_MSP(pFlashBootApplication->stackAdrs);

	pFlashBootApplication->application();
}

u8 BSP_Flash_Unlock()
{
	u8	result = 0;
	HAL_StatusTypeDef	rtn;

	rtn = HAL_FLASH_Unlock();

	if(rtn == HAL_OK)	result = 1;

	return result;
}

u8 BSP_Flash_Lock()
{
	u8	result = 0;
	HAL_StatusTypeDef	rtn;

	rtn = HAL_FLASH_Lock();

	if(rtn == HAL_OK)	result = 1;

	return result;
}

u32 BSP_Flash_GetSector(u32 adrs)
{
	u32 sector;

	if((adrs >= BSP_FLASH_MEMORY_ADRS_0) && (adrs < BSP_FLASH_MEMORY_ADRS_1))			sector = FLASH_SECTOR_0;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_1) && (adrs < BSP_FLASH_MEMORY_ADRS_2))		sector = FLASH_SECTOR_1;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_2) && (adrs < BSP_FLASH_MEMORY_ADRS_3))		sector = FLASH_SECTOR_2;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_3) && (adrs < BSP_FLASH_MEMORY_ADRS_4))		sector = FLASH_SECTOR_3;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_4) && (adrs < BSP_FLASH_MEMORY_ADRS_5))		sector = FLASH_SECTOR_4;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_5) && (adrs < BSP_FLASH_MEMORY_ADRS_6))		sector = FLASH_SECTOR_5;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_6) && (adrs < BSP_FLASH_MEMORY_ADRS_7))		sector = FLASH_SECTOR_6;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_7) && (adrs < BSP_FLASH_MEMORY_ADRS_8))		sector = FLASH_SECTOR_7;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_8) && (adrs < BSP_FLASH_MEMORY_ADRS_9))		sector = FLASH_SECTOR_8;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_9) && (adrs < BSP_FLASH_MEMORY_ADRS_10))		sector = FLASH_SECTOR_9;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_10) && (adrs < BSP_FLASH_MEMORY_ADRS_11))	sector = FLASH_SECTOR_10;
	else if((adrs >= BSP_FLASH_MEMORY_ADRS_11) && (adrs < BSP_FLASH_MEMORY_ADRS_12))	sector = FLASH_SECTOR_11;
	else																				sector = 0xFFFFFFFF;

	return sector;
}

u8 BSP_Flash_UserProgramErase(u32 *adrss, u32 size)
{
	u8	result = 0;
	u32	sectorCnt = 0, sectorError = 0;
	HAL_StatusTypeDef	rtn;
	FLASH_EraseInitTypeDef	fErase;
	u32 adrs;

	BSP_Flash_Unlock();

	adrs = (u32)adrss;

	sectorCnt = BSP_Flash_GetSector(adrs + size) - BSP_Flash_GetSector(adrs) + 1;

	fErase.TypeErase		= FLASH_TYPEERASE_SECTORS;
	fErase.Banks			= FLASH_BANK_1;
	fErase.Sector			= BSP_Flash_GetSector(adrs);
	fErase.NbSectors		= sectorCnt;
	fErase.VoltageRange		= FLASH_VOLTAGE_RANGE_3;

	rtn = HAL_FLASHEx_Erase(&fErase, (uint32_t*)&sectorError);

	BSP_Flash_Lock();

	if(rtn == HAL_OK)		result = 1;

	return result;
}

u8 BSP_Flash_UserProgramWrite(u32 *adrss, u8 *pData, u32 size)
{
	u8	result = 0;
	u32	writeType, writeAdrs, writeCnt;
	u32	ableWriteCheck;
	HAL_StatusTypeDef	rtn;
	u32 adrs;

	if(size == 0)				return result;

	BSP_Flash_Unlock();

	adrs = (u32)adrss;

	ableWriteCheck	= BSP_FLASH_MEMORY_ADRS_0 + BSP_FLASH_MEMORY_SIZE;
	ableWriteCheck	-= adrs;

	if(size > ableWriteCheck)	return result;

	writeType		= size % 4;
	writeAdrs		= adrs;
	writeCnt		= 0;

	if((writeType % 4 == 0) && (writeAdrs % 4 == 0))
	{
		u32	*pProgram;

		pProgram = (u32*)pData;

		do{
			rtn = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, writeAdrs, pProgram[writeCnt]);

			if(rtn != HAL_OK)
			{
				BSP_Flash_Lock();
				break;
			}	

			writeAdrs	+= 4;

			writeCnt	+= 1;

			if((writeCnt * 4) >= size)
			{
				result = 1;
				BSP_Flash_Lock();
				break;
			}
		}while(1);
	}
	else
	{
		u8	*pProgram;

		pProgram = (u8*)pData;

		do{
			rtn = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, writeAdrs, pProgram[writeCnt]);

			if(rtn != HAL_OK)
			{
				BSP_Flash_Lock();
				break;
			}	

			writeAdrs	+= 1;

			writeCnt	+= 1;

			if((writeCnt * 1) >= size)
			{
				result = 1;
				BSP_Flash_Lock();
				break;
			}
		}while(1);
	}

	return result;
}

u8 BSP_Flash_UserProgramRead(u32 *adrss, u8 *pData, u32 size)
{
	u8	result = 0;
	u8 *pBuffer;
	u32 adrs;

	BSP_Flash_Unlock();

	adrs = (u32)adrss;

	pBuffer = (u8*)adrs;

	memcpy(pData, pBuffer, size);

	BSP_Flash_Lock();

	result = 1;

	return result;
}

u8 BSP_Flash_UserProgramVerify(u32 *adrss, u8 *pData, u32 size)
{
	u8	result = 1;
	u8	*pBuffer;
	u32	cnt;
	u32 adrs;

	BSP_Flash_Unlock();

	adrs = (u32)adrss;

	pBuffer = (u8*)adrs;

	for(cnt = 0; cnt < size; cnt++)
	{
		if(pBuffer[cnt] != pData[cnt])
		{
			result = 0;
		}
	}

	BSP_Flash_Lock();

	return result;
}

