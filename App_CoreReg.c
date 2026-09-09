#include	"App_CoreReg.h"

static	u16	coreRegCurrentPage[CORE_SPI_MAX_COUNT];

u8 CoreReg_PageInit(u8 position)
{
	u8	result = 0;

	if(position >= CORE_SPI_MAX_COUNT)			return result;

	coreRegCurrentPage[position] = 0xffff;

	result = 1;

	return result;
}

u8 CoreReg_SetPage(u8 position, u16 page)
{
	u8	result = 0, rtn;
	u8	cmd;
	u16	data;

	cmd		= 0;
	data	= (page >> 8) & 0x00ff;
	
	if(position >= CORE_SPI_MAX_COUNT)
	{
		CMD_Printf("position over CORE_SPI_MAX_COUNT");
		return result;
	}

	CoreSPI_CsCtrl(position, LOW);

	rtn = CoreSPI_Write(CORE_SPI_WRITE_MODE_CMD, &cmd, 1);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	rtn = CoreSPI_Write(CORE_SPI_WRITE_MODE_DATA, (u8*)&data, 2);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	CoreSPI_CsCtrl(position, HIGH);

	coreRegCurrentPage[position] = page;

	result = 1;

	return result;
}

u8 CoreReg_Write(u8 position, u16 cmd, u8 *pData, u32 size)
{
	u8	result = 0, rtn;
	u8	wCmd;

	if(position >= CORE_SPI_MAX_COUNT)
	{
		CMD_Printf("position over CORE_SPI_MAX_COUNT");
		return result;
	}

	if((coreRegCurrentPage[position] & 0xff00) != (cmd & 0xff00))
	{
		rtn = CoreReg_SetPage(position, cmd);
		if(rtn == 0)						return result;
	}

	wCmd = (u8)cmd;

	CoreSPI_CsCtrl(position, LOW);

	rtn = CoreSPI_Write(CORE_SPI_WRITE_MODE_CMD, &wCmd, 1);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	rtn = CoreSPI_Write(CORE_SPI_WRITE_MODE_DATA, pData, size);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	CoreSPI_CsCtrl(position, HIGH);

	result = 1;
	
	return result;
}

static u8 CoreReg_ReadMax128Byte(u8 position, u16 cmd, u8 *pData, u8 size)
{
	u8	result = 0, rtn;
	u8	wCmd;

	if(position >= CORE_SPI_MAX_COUNT)		return result;
	if(size > 128)							return result;

	if((coreRegCurrentPage[position] & 0xff00) != (cmd & 0xff00))
	{
		rtn = CoreReg_SetPage(position, cmd);
		if(rtn == 0)						return result;
	}

	wCmd = (u8)cmd;

	CoreSPI_CsCtrl(position, LOW);

	rtn = CoreSPI_Write(CORE_SPI_WRITE_MODE_CMD, &wCmd, 1);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	rtn = CoreSPI_Write(CORE_SPI_WRITE_MODE_CMD, &size, 1);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	rtn = CoreSpi_Read(pData, size);
	if(rtn == 0)
	{
		CoreSPI_CsCtrl(position, HIGH);

		return result;
	}

	CoreSPI_CsCtrl(position, HIGH);

	result = 1;

	return result;
}

u8 CoreReg_Read(u8 position, u16 cmd, u8 *pData, u32 size)
{
	u8	result = 0, rtn;
	u32	cnt = 0, remainSize = size;

	do{
		if(remainSize > 128)
		{
			rtn = CoreReg_ReadMax128Byte(position, cmd, &pData[cnt], 128);
			if(rtn == 0)		return result;

			remainSize	-= 128;
			cnt			+= 128;
		}
		else
		{
			rtn = CoreReg_ReadMax128Byte(position, cmd, &pData[cnt], remainSize);
			if(rtn == 0)		return result;

			break;
		}
	}while(1);

	result = 1;

	return result;
}

u8 CoreReg_RegisterWirte(u8 position, u16 cmd, u16 *pData, u16 size)
{
	u8	result = 0, rtn;
	u32	wSize = size * 2;

	rtn = CoreReg_Write(position, cmd, (u8*)pData, wSize);
	if(rtn != 0)			result = 1;

	return result;
}

u8 CoreReg_RegisterRead(u8 position, u16 cmd, u16 *pData, u16 size)
{
	u8	result = 0, rtn;
	u32	rSize = size * 2;

	rtn = CoreReg_Read(position, cmd, (u8*)pData, rSize);
	if(rtn != 0)			result = 1;

	return result;
}

u8 CoreReg_RegisterWriteSingle(u8 position, u16 cmd, u16 data)
{
	u8	result = 0, rtn;

	rtn = CoreReg_RegisterWirte(position, cmd, &data, 1);
	if(rtn != 0)			result = 1;

	return result;
}

u16 CoreReg_RegisterReadSingle(u8 position, u16 cmd)
{
	u8	rtn;
	u16	result = 0, rData;

	rtn = CoreReg_RegisterRead(position, cmd, &rData, 1);
	if(rtn != 0)			result = rData;

	return result;
}

u8 CoreReg_BufferWirte(u8 position, u16 cmd, u32 *pData, u16 size)
{
	u8	result = 0, rtn;
	u32	wSize = size * 4;

	rtn = CoreReg_Write(position, cmd, (u8*)pData, wSize);
	if(rtn != 0)			result = 1;

	return result;
}

u8 CoreReg_BufferRead(u8 position, u16 cmd, u32 *pData, u16 size)
{
	u8	result = 0, rtn;
	u32	rSize = size * 4;

	rtn = CoreReg_Read(position, cmd, (u8*)pData, rSize);
	if(rtn != 0)			result = 1;

	return result;
}

u8 CoreReg_BufferWriteSingle(u8 position, u16 cmd, u32 data)
{
	u8	result = 0, rtn;

	rtn = CoreReg_BufferWirte(position, cmd, &data, 1);
	if(rtn != 0)			result = 1;

	return result;
}

u32 CoreReg_BufferReadSingle(u8 position, u16 cmd)
{
	u8	rtn;
	u32	result = 0, rData;

	rtn = CoreReg_BufferRead(position, cmd, &rData, 1);
	if(rtn != 0)			result = rData;

	return result;
}

