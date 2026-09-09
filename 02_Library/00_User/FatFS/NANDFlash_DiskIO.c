#include	<string.h>
#include	"FatFS_GenDrv.h"

static volatile DSTATUS Stat = STA_NOINIT;

char	uNandPath[4] = {0, 0, 0, 0};
FATFS	uNandFatFs;
FIL		uNandFile;

DSTATUS NAND_initialize (BYTE);
DSTATUS NAND_status (BYTE);
DRESULT NAND_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT NAND_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT NAND_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */
  
const Diskio_drvTypeDef  NAND_Driver =
{
	NAND_initialize,
	NAND_status,
	NAND_read, 
#if  _USE_WRITE == 1
	NAND_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
	NAND_ioctl,
#endif /* _USE_IOCTL == 1 */
};

extern u8 MEMCTRL_NAND_StatusCheck();

DSTATUS NAND_initialize(BYTE lun)
{
	Stat = STA_NOINIT;

	if(MEMCTRL_NAND_StatusCheck())
	{
		Stat &= ~STA_NOINIT;
	}

	return Stat;
}

DSTATUS NAND_status(BYTE lun)
{
	u8 rtn;
	
	Stat = STA_NOINIT;

	rtn = MEMCTRL_NAND_StatusCheck();
	if(rtn)
	{
		Stat &= ~STA_NOINIT;
	}
	
	return Stat;
}

extern u8 MEMCTRL_NAND_AdrsCalculator(u32 pageNo, NAND_AddressTypeDef *pAdrs);
extern u8 MEMCTRL_NAND_ReadPage(NAND_AddressTypeDef *pAdrs, u8 *pData, u32 size);

DRESULT NAND_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
	u8	rtn;
	NAND_AddressTypeDef	adrs;
	DRESULT res = RES_ERROR;

	rtn = MEMCTRL_NAND_AdrsCalculator(sector, &adrs);
	if(rtn == 0)		return res;

	rtn = MEMCTRL_NAND_ReadPage(&adrs, (uint8_t*)buff, count);
	if(rtn != 0)		res = RES_OK;

	return res;
}

#if _USE_WRITE == 1
extern u8 MEMCTRL_NAND_WritePage(NAND_AddressTypeDef *pAdrs, u8 *pData, u32 size);

DRESULT NAND_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = RES_ERROR;
	uint8_t rtn;
	NAND_AddressTypeDef	adrs;

	rtn = MEMCTRL_NAND_AdrsCalculator(sector, &adrs);
	if(rtn == 0)		return res;

	rtn = MEMCTRL_NAND_WritePage(&adrs, (uint8_t*)buff, count);
	if(rtn != 0)		res = RES_OK;
	
	return res;
}
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
extern u32 MEMCTRL_NAND_GetSectorCnt();
extern u32 MEMCTRL_NAND_GetSectorSize();
extern u32 MEMCTRL_NAND_GetBlockSize();

DRESULT NAND_ioctl(BYTE lun, BYTE cmd, void *buff)
{
	DRESULT res = RES_ERROR;

	if (Stat & STA_NOINIT) return RES_NOTRDY;

	switch (cmd)
	{
		/* Make sure that no pending write process */
		case CTRL_SYNC :
			res = RES_OK;
			break;

		/* Get number of sectors on the disk (DWORD) */
		case GET_SECTOR_COUNT :
			*(DWORD*)buff = MEMCTRL_NAND_GetSectorCnt();
			res = RES_OK;
			break;

		/* Get R/W sector size (WORD) */
		case GET_SECTOR_SIZE :
			*(WORD*)buff = MEMCTRL_NAND_GetSectorSize();
			res = RES_OK;
			break;

		/* Get erase block size in unit of sector (DWORD) */
		case GET_BLOCK_SIZE :
			*(DWORD*)buff = MEMCTRL_NAND_GetBlockSize();
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
	}

	return res;
}
#endif /* _USE_IOCTL == 1 */

