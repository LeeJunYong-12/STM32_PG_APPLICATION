#include	"App_Board.h"

#if (BOARD_CHANNEL_MAX_COUNT != 4)
	#error	error, BOARD_CHANNEL_MAX_COUNT
#endif

static	bspGpio_t		ioBoardType, ioBoardID[4];
static	u8				boardHwType, boardID = BOARD_TYPE_END, boardConstruct = BOARD_CONSTRUCT_END;
static	boardPhyInfo_t	boardPhyInfo;
static	boardReset_t	boardReset;
boardChannel_t			boardChannel[BOARD_CHANNEL_MAX_COUNT];

extern u8 PG_boot_flag;

u8 Board_DataInit()
{
	u8	result = 0;

	memset(&boardReset, NULL, sizeof(boardReset));
	boardReset.timeOut = 1000;

	memset(&boardPhyInfo, NULL, sizeof(boardPhyInfo));
	boardPhyInfo.type	= BOARD_PHY_TYPE_NONE;

	result = 1;

	return result;
}

u8 Board_GpioInit()
{
	u8	result = 0, rtn, errCnt = 0;

	ioBoardType.Port		= GPIOF;
	ioBoardType.Pin			= GPIO_PIN_10;
	ioBoardType.Mode		= BSP_GPIO_MODE_INPUT;

	ioBoardID[0].Port		= GPIOB;
	ioBoardID[0].Pin		= GPIO_PIN_0;
	ioBoardID[0].Mode		= BSP_GPIO_MODE_INPUT;

	ioBoardID[1].Port		= GPIOB;
	ioBoardID[1].Pin		= GPIO_PIN_2;
	ioBoardID[1].Mode		= BSP_GPIO_MODE_INPUT;

	ioBoardID[2].Port		= GPIOJ;
	ioBoardID[2].Pin		= GPIO_PIN_10;
	ioBoardID[2].Mode		= BSP_GPIO_MODE_INPUT;

	ioBoardID[3].Port		= GPIOJ;
	ioBoardID[3].Pin		= GPIO_PIN_11;
	ioBoardID[3].Mode		= BSP_GPIO_MODE_INPUT;

	rtn = BSP_GPIO_Init(&ioBoardType);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&ioBoardID[0]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&ioBoardID[1]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&ioBoardID[2]);
	if(rtn == 0)			errCnt += 1;

	rtn = BSP_GPIO_Init(&ioBoardID[3]);
	if(rtn == 0)			errCnt += 1;

	if(errCnt == 0)			result = 1;

	return result;
}

static u8 Board_CheckType(u8 *pType)
{
	u8	result = 0;
	u8	readPin;

	readPin  = BSP_GPIO_ReadPin(&ioBoardType);

	if(readPin == 0)		*pType = 0;
	else					*pType = 1;

	result = 1;

	return result;
}

static u8 Board_CheckID(u8 *pID)
{
	u8	result = 0;
	bitCtrl8_t	bitData;

	bitData.u8Data	 = 0;

	bitData.b0 = BSP_GPIO_ReadPin(&ioBoardID[0]);
	bitData.b1 = BSP_GPIO_ReadPin(&ioBoardID[1]);
	bitData.b2 = BSP_GPIO_ReadPin(&ioBoardID[2]);
	bitData.b3 = BSP_GPIO_ReadPin(&ioBoardID[3]);

	*pID = bitData.u8Data;

	result = 1;

	return result;
}

u8 Board_LoadHwType()
{
	return Board_CheckType(&boardHwType);
}

u8 Board_GetHwType()
{
	return boardHwType;
}

u8 Board_TypeLoad()
{
	u8	result = 0, rtn, errCnt = 0;
	u32	memSize;

	rtn = IMG_PreloadListDataInit();
	if(rtn == 0)			errCnt += 1;

	memSize = 1024 * 1024 * 1024;
	memSize *= 2;						//	2GB

	rtn = IMG_PreloadListSetMemorySize(memSize);
	if(rtn == 0)			errCnt += 1;

	switch(boardHwType)
	{
		case 0:			//	LVDS/RGB
			break;

		case 1:			//	RGB 30/60Bit
			break;

		default:		//	ERROR
			break;
	}

	if(errCnt == 0)			result = 1;

	return result;
}

u8 Board_LoadID()
{
	return Board_CheckID(&boardID);
}

u8 Board_GetID()
{
	return boardID;
}

u8 Board_GetConstruct()
{
	return boardConstruct;
}

u8 Board_ChannelLoad()
{
	u8	result = 1, rtn, errCnt = 0;

	memset(boardChannel, NULL, sizeof(boardChannel));

	switch(boardID)
	{
		case BOARD_TYPE_EDP_MULTI_CHANNEL_2:
			boardPhyInfo.type			= BOARD_PHY_TYPE_EDP;
			boardPhyInfo.mipiHwType		= 0;
			boardPhyInfo.eDpHwType		= 0;

			boardConstruct				= BOARD_CONSTRUCT_MULTI;

			pCommandList				= (commandList_t*)cmdList_eDP_MultiChannel;

			boardChannel[0].hwConstruct	= BOARD_HW_CONSTRUCT_TRUE;
			boardChannel[0].useChannel	= BOARD_USE_CHANNEL_TRUE;
			boardChannel[0].position	= BOARD_POSITION_3;
			boardChannel[0].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(0, ExtGPIO_GetHandle(EXTGPIO_PIN_4));
			if(rtn == 0)				errCnt += 1;

			boardChannel[1].hwConstruct	= BOARD_HW_CONSTRUCT_TRUE;
			boardChannel[1].useChannel	= BOARD_USE_CHANNEL_TRUE;
			boardChannel[1].position	= BOARD_POSITION_4;
			boardChannel[1].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(1, ExtGPIO_GetHandle(EXTGPIO_PIN_5));
			if(rtn == 0)				errCnt += 1;

			boardChannel[2].hwConstruct	= BOARD_HW_CONSTRUCT_FALSE;
			boardChannel[2].useChannel	= BOARD_USE_CHANNEL_FALSE;
			boardChannel[2].position	= BOARD_POSITION_END;
			boardChannel[2].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(2, NULL);
			if(rtn == 0)				errCnt += 1;

			boardChannel[3].hwConstruct	= BOARD_HW_CONSTRUCT_FALSE;
			boardChannel[3].useChannel	= BOARD_USE_CHANNEL_FALSE;
			boardChannel[3].position	= BOARD_POSITION_END;
			boardChannel[3].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(3, NULL);
			if(rtn == 0)				errCnt += 1;
			break;

		case BOARD_TYPE_EDP_MULTI_CHANNEL_4_R:
			boardPhyInfo.type			= BOARD_PHY_TYPE_EDP;
			boardPhyInfo.eDpHwType		= EDP_HW_TYPE_CORE_MC4;

			boardConstruct				= BOARD_CONSTRUCT_RELIABLITY;

			pCommandList				= (commandList_t*)cmdList_eDP_Reliability;

			boardChannel[0].hwConstruct	= BOARD_HW_CONSTRUCT_TRUE;
			boardChannel[0].useChannel	= BOARD_USE_CHANNEL_TRUE;
			boardChannel[0].position	= BOARD_POSITION_1;
			boardChannel[0].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(0, ExtGPIO_GetHandle(EXTGPIO_PIN_2));
			if(rtn == 0)				errCnt += 1;

			boardChannel[1].hwConstruct	= BOARD_HW_CONSTRUCT_FALSE;
			boardChannel[1].useChannel	= BOARD_USE_CHANNEL_FALSE;
			boardChannel[1].position	= BOARD_POSITION_END;
			boardChannel[1].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(1, NULL);
			if(rtn == 0)				errCnt += 1;

			boardChannel[2].hwConstruct	= BOARD_HW_CONSTRUCT_FALSE;
			boardChannel[2].useChannel	= BOARD_USE_CHANNEL_FALSE;
			boardChannel[2].position	= BOARD_POSITION_END;
			boardChannel[2].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(2, NULL);
			if(rtn == 0)				errCnt += 1;

			boardChannel[3].hwConstruct	= BOARD_HW_CONSTRUCT_FALSE;
			boardChannel[3].useChannel	= BOARD_USE_CHANNEL_FALSE;
			boardChannel[3].position	= BOARD_POSITION_END;
			boardChannel[3].option		= BOARD_OPTION_NONE;

			rtn = FanCtrl_IoInit(3, NULL);
			if(rtn == 0)				errCnt += 1;
			break;

		default:
			result = 0;
			break;
	}

	if(errCnt != 0)		result = 0;

	return result;
}

u8 Board_GetPhyType(u32 *pPhyType)
{
	u8	result = 0;

	if(boardPhyInfo.type == BOARD_PHY_TYPE_NONE)	return result;
	if(boardPhyInfo.type >= BOARD_PHY_TYPE_END)		return result;

	*pPhyType = boardPhyInfo.type;

	result = 1;

	return result;
}

u8 Board_GetPhyHwType(u32 phyType, u32 *pHwType)
{
	u8	result = 0;

	switch(phyType)
	{
		case BOARD_PHY_TYPE_MIPI:
			if(boardPhyInfo.type != BOARD_PHY_TYPE_MIPI)	return result;
			*pHwType = boardPhyInfo.mipiHwType;
			break;

		case BOARD_PHY_TYPE_EDP:
			if(boardPhyInfo.type != BOARD_PHY_TYPE_EDP)	return result;
			*pHwType = boardPhyInfo.eDpHwType;
			break;

		default:
			return result;
			break;
	}

	result = 1;

	return result;
}

u8 Board_ChannelInit()
{
	u8	result = 0;
	u8	cnt;

	for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
	{
		if(boardChannel[cnt].hwConstruct == BOARD_HW_CONSTRUCT_TRUE)
		{
			boardChannel[cnt].useChannel	= BOARD_USE_CHANNEL_TRUE;
		}
	}

	return result;
}

u8 Board_ChannelSelect(u8 channel, u8 mode)
{
	u8	result = 0;

	if(channel	>= BOARD_CHANNEL_MAX_COUNT)
	{
		CMD_Printf("over max channel");
		return result;
	}
	if(mode		> BOARD_USE_CHANNEL_TRUE)
	{
		CMD_Printf("wrong mode");
		return result;
	}

	if(boardChannel[channel].hwConstruct != BOARD_HW_CONSTRUCT_TRUE)
	{
		CMD_Printf("hwConstruct is not BOARD_HW_CONSTRUCT_TRUE");
		return result;
	}

	boardChannel[channel].useChannel = mode;

	result = 1;

	return result;
}

u8 Board_GetHwConstruct(u8 channel)
{
	u8	result = 0;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)		return result;

	if(boardChannel[channel].hwConstruct != BOARD_HW_CONSTRUCT_TRUE)		return result;

	result = 1;

	return result;
}

u8 Board_GetChannelStatus(u8 channel)
{
	u8	result = 0;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)		return result;

	if(boardChannel[channel].hwConstruct	!= BOARD_HW_CONSTRUCT_TRUE)		return result;
	if(boardChannel[channel].useChannel		!= BOARD_USE_CHANNEL_TRUE)		return result;

	result = 1;

	return result;
}

u8 Board_GetChannelPosition(u8 channel, u32 *pPosition)
{
	u8	result = 0;

	if(channel >= BOARD_CHANNEL_MAX_COUNT)		return result;

	*pPosition = boardChannel[channel].position;

	result = 1;

	return result;
}

u8 Board_GetVersion(u8 *pData)
{
	u8	result = 0;
	u8	major, minor, release, type;

	major	= (u8)(applicationVersion >> 24) + 0x30;
	minor	= (u8)(applicationVersion >> 16) + 0x30;
	release	= (u8)(applicationVersion >> 8) + 0x30;
	type	= (u8)(applicationVersion >> 0) + 0x00;

	sprintf((char*)pData, "%c.%c.%c.%c\0", major, minor, release, type);

	result = 1;

	return result;
}

u8 Board_GetBuildDate(u8 *pData)
{
	u8	result = 0;
	u8	month, day;
	u16	year;

	year	= (u16)(applicationBuild >> 16);
	month	= (u8)(applicationBuild >> 8);
	day		= (u8)(applicationBuild >> 0);

	sprintf((char*)pData, "%04X.%02X.%02X\0", year, month, day);

	result = 1;

	return result;
}

u8 Board_GetFirmwareType(u8 *pData)
{
	u8	result = 1;

	switch(applicationType)
	{
		case APPLICATION_TYPE_PG:
			sprintf((char*)pData, "E7602_Rev2_Master\0");
			break;

		case APPLICATION_TYPE_PWR_E:
			sprintf((char*)pData, "E7602_Rev2_PWR_E\0");
			break;

		case APPLICATION_TYPE_PWR_R:
			sprintf((char*)pData, "E7602_Rev2_PWR_R\0");
			break;

		default:
			result = 0;
			break;
	}

	return result;
}

u8 Board_ResetEnable()
{
	u8	result = 0;

	if(boardReset.status != 0)		return result;

	boardReset.status = 1;

	boardReset.startTime = HAL_GetTick();

	result = 1;

	return result;
}

u8 Board_ResetProcess()
{
	u8	result = 0;
	u32	getTick;

	if(boardReset.status != 0)
	{
		getTick = HAL_GetTick();

		if((getTick - boardReset.startTime) > boardReset.timeOut)
		{
			result = 1;

			HAL_NVIC_SystemReset();
		}
	}

	return result;
}

u8 Board_BootMode()
{
	u8	result = 0;
	u8 eeprom_flag = 0x01;

	if(PG_boot_flag == 0)
	{
		return result;
	}	
	else 
	{
		EEPROM_Write(0x400, &eeprom_flag, 1); 

		HAL_NVIC_SystemReset();
	}

	return result;
}