#include	"App_LCM.h"

u8 LCM_SpiInit(u8 position, void *pVoid)
{
	u8	result = 0, rtn;
	
	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_INIT;

	if(pVoid != NULL)
	{
		lcmSpiOption_t		*pData;

		pData = (lcmSpiOption_t*)pVoid;

		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)pData->baudrate;
		sbParsingData.txSize						+= 1;
		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)pData->polarity;
		sbParsingData.txSize						+= 1;
		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)pData->phase;
		sbParsingData.txSize						+= 1;
	}

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_SpiDeInit(u8 position)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_DEINIT;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_SpiCsCtrl(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_CS_CTRL;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_SpiTransmitData(u8 position, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	if(size > 1024)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_WRITE;
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pData, size);
	sbParsingData.txSize						+= size;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_SpiReceiveData(u8 position, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_READ;
	SYS_HWordToHex(size, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize						+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != size)			return result;

	memcpy(pData, sbParsingData.pRxData, size);

	result = 1;

	return result;
}

u8 LCM_SpiTransmitReceiveData(u8 position, u8 *pTxData, u8 *pRxData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_WRITE_READ;
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pTxData, size);
	sbParsingData.txSize						+= size;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != size)			return result;

	memcpy(pRxData, sbParsingData.pRxData, size);

	result = 1;

	return result;
}

u8 LCM_SpiTransmitDataCustom(u8 position, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	if(size > 1024)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_WRITE_CUSTOM;
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pData, size);
	sbParsingData.txSize						+= size;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_SpiReceiveDataCustom(u8 position, u8 *pTxData, u8 *pRxData, u16 txSize, u16 rxSize)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_READ_CUSTOM;
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pTxData, txSize);
	sbParsingData.txSize						+= txSize;
	SYS_HWordToHex(rxSize, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize						+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != rxSize)			return result;

	memcpy(pRxData, sbParsingData.pRxData, rxSize);

	result = 1;

	return result;
}

u8 LCM_SpiTransmitReceiveDataCustom(u8 position, u8 *pTxData, u8 *pRxData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_WRITE_READ_CUSTOM;
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pTxData, size);
	sbParsingData.txSize						+= size;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != size)			return result;

	memcpy(pRxData, sbParsingData.pRxData, size);

	result = 1;

	return result;
}

u8 LCM_SpiIoWrite(u8 position, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_IOWRITE;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_SpiIoRead(u8 position, u8 *pState)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_SPI_IOREAD;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 1)				return result;

	*pState = sbParsingData.pRxData[0];

	result = 1;

	return result;
}

u8 LCM_I2CInit(u8 position, u8 channel)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_INIT;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_I2CDeInit(u8 position, u8 channel)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_DEINIT;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_I2CGetReady(u8 position, u8 channel, u8 slaveAdrs, u8 *pReady)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_GET_READY;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= slaveAdrs;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 1)				return result;

	*pReady										= sbParsingData.pRxData[0];

	result = 1;

	return result;
}

u8 LCM_I2CGetSlaveAdrs(u8 position, u8 channel, u8 *pData, u8 *pCnt)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_GET_SLAVE_ADRS;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	memcpy(pData, sbParsingData.pRxData, sbParsingData.rxSize);

	*pCnt										= sbParsingData.rxSize;

	result = 1;

	return result;
}

u8 LCM_I2CTransmitData(u8 position, u8 channel, u8 slaveAdrs, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_WRITE;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= slaveAdrs;
	sbParsingData.txSize						+= 1;
	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pData, size);
	sbParsingData.txSize						+= size;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_I2CReceiveData(u8 position, u8 channel, u8 slaveAdrs, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_READ;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= slaveAdrs;
	sbParsingData.txSize						+= 1;
	SYS_HWordToHex(size, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize						+= 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != size)			return result;

	memcpy(pData, sbParsingData.pRxData, size);

	result = 1;

	return result;
}

u8 LCM_I2CMemWrite(u8 position, u8 channel, u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_WRITE;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= slaveAdrs;
	sbParsingData.txSize						+= 1;

	if(adrsMode)
	{
		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)(memAdrs >> 0);
		sbParsingData.txSize						+= 1;
		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)(memAdrs >> 8);
		sbParsingData.txSize						+= 1;
	}
	else
	{
		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)(memAdrs >> 0);
		sbParsingData.txSize						+= 1;
	}

	memcpy(&sbParsingData.pTxData[sbParsingData.txSize], pData, size);
	sbParsingData.txSize						+= size;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_I2CMemRead(u8 position, u8 channel, u8 slaveAdrs, u16 memAdrs, u8 adrsMode, u8 *pData, u16 size)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_READ_CUSTOM;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= slaveAdrs;
	sbParsingData.txSize						+= 1;

	if(adrsMode)
	{
		SYS_HWordToHex(memAdrs, &sbParsingData.pTxData[sbParsingData.txSize]);
		sbParsingData.txSize += 2;
	}
	else
	{
		sbParsingData.pTxData[sbParsingData.txSize]	= (u8)memAdrs;
		sbParsingData.txSize						+= 1;
	}

	SYS_HWordToHex(size, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize += 2;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != size)			return result;

	memcpy(pData, sbParsingData.pRxData, size);

	result = 1;

	return result;
}

u8 LCM_I2CIoDirection(u8 position, u8 channel, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_IO_DIRECTION;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_I2CIoWrite(u8 position, u8 channel, u8 state)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_IO_WRITE;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;
	sbParsingData.pTxData[sbParsingData.txSize]	= state;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 0)				return result;

	result = 1;

	return result;
}

u8 LCM_I2CIoRead(u8 position, u8 channel, u8 *pState)
{
	u8	result = 0, rtn;

	rtn = SB_DataInit();
	if(rtn == 0)								return result;

	sbParsingData.channel						= position;
	sbParsingData.cmd							= SB_CMD_PWR_M_I2C_IO_READ;
	sbParsingData.pTxData[sbParsingData.txSize]	= channel;
	sbParsingData.txSize						+= 1;

	rtn = SB_SubBoardXfer(&sbParsingData);
	if(rtn == 0)								return result;

	if(sbParsingData.rxSize != 1)				return result;

	*pState = sbParsingData.pRxData[0];

	result = 1;

	return result;
}

