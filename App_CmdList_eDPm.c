 #include	"App_CmdList_eDPm.h"

static u8 Cmd_DisplayPortCore(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_DisplayPortCore);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_DisplayPortCore);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_RXDPCore(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_RXDPCore);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_RXDPCore);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_GPIO(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_GPIO_M);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_GPIO_M);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_LCM(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_LCM);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_LCM);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_Power(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_Power_M);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_Power_M);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_Extension(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_Extension);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_Extension);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_ExtCtrl(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_ExtCtrl);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_ExtCtrl);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_EquipmentNY(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_EquipmentNY);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_EquipmentNY);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_VideoStream(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	result = CMD_SubCommandExecute(pVoid, (void*)cmdList_VideoStream);

	return result;
}

static u8 Cmd_DisplayPortTX(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(u8 cnt = 0; cnt < DP_CHANNEL_MAX_COUNT; cnt++)
		{
			pData->position = cnt + DP_CHANNEL_TX1;

			pData->channel = (pData->position - DP_CHANNEL_TX1) + 1;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_DisplayPortTX);
			if(rtn == 0)			errCnt += 1;
		}
	}
	else
	{
		pData->position	= pData->option;

		pData->channel = (pData->position - DP_CHANNEL_TX1) + 1;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_DisplayPortTX);
		if(rtn == 0)			errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_DisplayPortRX(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(u8 cnt = 0; cnt < DP_CHANNEL_MAX_COUNT; cnt++)
		{
			pData->position = cnt + DP_CHANNEL_RX1;

			pData->channel = (pData->position - DP_CHANNEL_RX1) + 1;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_DisplayPortRX);
			if(rtn == 0)			errCnt += 1;
		}
	}
	else
	{
		pData->position	= pData->option;

		pData->channel = (pData->position - DP_CHANNEL_RX1) + 1;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_DisplayPortRX);
		if(rtn == 0)			errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_MBTest(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_MBtest);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_MBtest);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_MBTestPrev(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_MBtest_prev);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_MBtest_prev);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_HingeTest(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_Hingetest);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_Hingetest);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_BoardTest(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_BoardTest);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_BoardTest);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}

static u8 Cmd_EtTest(void *pVoid)
{
	u8	result = 0, rtn, errCnt = 0;
	u32	cnt;
	cmdFunctionData_t	*pData;
	boardChannel_t		*pBoardChannel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc == 0)		return result;

	if(pData->option == NULL)
	{
		for(cnt = 0; cnt < BOARD_CHANNEL_MAX_COUNT; cnt++)
		{
			pBoardChannel = &boardChannel[cnt];

			if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)		continue;
			if(pBoardChannel->useChannel == BOARD_USE_CHANNEL_FALSE)		continue;

			pData->channel	= cnt + 1;
			pData->position	= pBoardChannel->position;

			rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_EtTest);
			if(rtn == 0)		errCnt += 1;
		}
	}
	else
	{
		cnt = pData->option - 1;
		pBoardChannel = &boardChannel[cnt];

		if(pBoardChannel->hwConstruct == BOARD_HW_CONSTRUCT_FALSE)
		{
			CMD_Printf(">>NO_COMMAND");
			return result;
		}

		pData->channel = cnt + 1;
		pData->position = pBoardChannel->position;

		rtn = CMD_SubCommandExecute(pVoid, (void*)cmdList_EtTest);
		if(rtn == 0)		errCnt += 1;
	}

	if(errCnt == 0)				result = 1;

	return result;
}



const	commandList_t	cmdList_eDP_MultiChannel[] = {
//	{	Command,				Function,					Option,			Mode,		RootMessage				Note}
	{	"help",					Cmd_Help,					NULL,			NULL,		">>HELP",				NULL},
	{	"board",				Cmd_Board,					NULL,			NULL,		">>BOARD",				NULL},
	{	"file",					Cmd_File,					NULL,			NULL,		">>FILE",				NULL},
	{	"fpga",					Cmd_FPGA,					NULL,			NULL,		">>FPGA",				NULL},
	{	"image",				Cmd_IMAGE,					NULL,			NULL,		">>IMAGE",				NULL},
	{	"system",				Cmd_System,					NULL,			NULL,		">>SYSTEM",				NULL},
	{	"test",					Cmd_Test,					NULL,			NULL,		">>TEST",				NULL},
	{	"delay", 				Cmd_Delay,					NULL,			NULL,		">>DELAY",				NULL},

	{	"DPCORE",				Cmd_DisplayPortCore,		NULL,			NULL,		">>DPCORE",				NULL},
	{	"DPCORE1",				Cmd_DisplayPortCore,		DP_CORE_1,		NULL,		">>DPCORE1",			NULL},
	{	"DPCORE2",				Cmd_DisplayPortCore,		DP_CORE_2,		NULL,		">>DPCORE2",			NULL},

	{	"RXCORE",				Cmd_RXDPCore,				NULL,			NULL,		">>RXCORE",				NULL},
	{	"RXCORE1",				Cmd_RXDPCore,				DP_CORE_1,		NULL,		">>RXCORE1",			NULL},
	{	"RXCORE2",				Cmd_RXDPCore,				DP_CORE_2,		NULL,		">>RXCORE2",			NULL},

	{	"gpio",					Cmd_GPIO,					NULL,			NULL,		">>GPIO",				NULL},
	{	"gpio1",				Cmd_GPIO,					0x01,			NULL,		">>GPIO1",				NULL},
	{	"gpio2",				Cmd_GPIO,					0x02,			NULL,		">>GPIO2",				NULL},
	{	"gpio3",				Cmd_GPIO,					0x03,			NULL,		">>GPIO3",				NULL},
	{	"gpio4",				Cmd_GPIO,					0x04,			NULL,		">>GPIO4",				NULL},
	{	"lcm",					Cmd_LCM,					NULL,			NULL,		">>LCM",				NULL},
	{	"lcm1",					Cmd_LCM,					0x01,			NULL,		">>LCM1",				NULL},
	{	"lcm2",					Cmd_LCM,					0x02,			NULL,		">>LCM2",				NULL},
	{	"lcm3",					Cmd_LCM,					0x03,			NULL,		">>LCM3",				NULL},
	{	"lcm4",					Cmd_LCM,					0x04,			NULL,		">>LCM4",				NULL},
	{	"power",				Cmd_Power,					NULL,			NULL,		">>POWER",				NULL},
	{	"power1",				Cmd_Power,					0x01,			NULL,		">>POWER1",				NULL},
	{	"power2",				Cmd_Power,					0x02,			NULL,		">>POWER2",				NULL},
	{	"power3",				Cmd_Power,					0x03,			NULL,		">>POWER3",				NULL},
	{	"power4",				Cmd_Power,					0x04,			NULL,		">>POWER4",				NULL},
	{	"extension",			Cmd_Extension,				NULL,			NULL,		">>EXTENSION",			NULL},
	{	"extension1",			Cmd_Extension,				0x01,			NULL,		">>EXTENSION1",			NULL},
	{	"extension2",			Cmd_Extension,				0x02,			NULL,		">>EXTENSION2",			NULL},
	{	"extension3",			Cmd_Extension,				0x03,			NULL,		">>EXTENSION3",			NULL},
	{	"extension4",			Cmd_Extension,				0x04,			NULL,		">>EXTENSION4",			NULL},
	{	"extctrl",				Cmd_ExtCtrl,				NULL,			NULL,		">>EXTCTRL",			NULL},
	{	"extctrl1",				Cmd_ExtCtrl,				0x01,			NULL,		">>EXTCTRL1",			NULL},
	{	"extctrl2",				Cmd_ExtCtrl,				0x02,			NULL,		">>EXTCTRL2",			NULL},
	{	"extctrl3",				Cmd_ExtCtrl,				0x03,			NULL,		">>EXTCTRL3",			NULL},
	{	"extctrl4",				Cmd_ExtCtrl,				0x04,			NULL,		">>EXTCTRL4",			NULL},
	{	"eNY",					Cmd_EquipmentNY,			NULL,			NULL,		">>ENY",				NULL},	//map?
	{	"eNY1",					Cmd_EquipmentNY,			0x01,			NULL,		">>ENY1",				NULL},
	{	"eNY2",					Cmd_EquipmentNY,			0x02,			NULL,		">>ENY2",				NULL},
	{	"eNY3",					Cmd_EquipmentNY,			0x03,			NULL,		">>ENY3",				NULL},
	{	"eNY4",					Cmd_EquipmentNY,			0x04,			NULL,		">>ENY4",				NULL},
	{	"Video",				Cmd_VideoStream,			NULL,			NULL,		">>VIDEO",				NULL},
	{	"mbtest",				Cmd_MBTest,					NULL,			NULL,		"",						NULL},
	{	"mbtest1",				Cmd_MBTest,					0x01,			NULL,		"",						NULL},
	{	"mbtest2",				Cmd_MBTest,					0x02,			NULL,		"",						NULL},
	{	"mbtest3",				Cmd_MBTest,					0x03,			NULL,		"",						NULL},
	{	"mbtest4",				Cmd_MBTest,					0x04,			NULL,		"",						NULL},
	{	"mbtestprev",			Cmd_MBTestPrev,				NULL,			NULL,		"",						NULL},
	{	"mbtestprev1",			Cmd_MBTestPrev,				0x01,			NULL,		"",						NULL},
	{	"mbtestprev2",			Cmd_MBTestPrev,				0x02,			NULL,		"",						NULL},
	{	"mbtestprev3",			Cmd_MBTestPrev,				0x03,			NULL,		"",						NULL},
	{	"mbtestprev4",			Cmd_MBTestPrev,				0x04,			NULL,		"",						NULL},
	{	"hingetest",			Cmd_HingeTest,				NULL,			NULL,		"",						NULL},
	{	"hingetest1",			Cmd_HingeTest,				0x01,			NULL,		"",						NULL},
	{	"hingetest2",			Cmd_HingeTest,				0x02,			NULL,		"",						NULL},
	{	"hingetest3",			Cmd_HingeTest,				0x03,			NULL,		"",						NULL},
	{	"hingetest4",			Cmd_HingeTest,				0x04,			NULL,		"",						NULL},
	{	"boardtest",			Cmd_BoardTest,				NULL,			NULL,		">>BOARDTEST",			NULL},
	{	"boardtest1",			Cmd_BoardTest,				0x01,			NULL,		">>BOARDTEST1",			NULL},
	{	"boardtest2",			Cmd_BoardTest,				0x02,			NULL,		">>BOARDTEST2",			NULL},
	{	"boardtest3",			Cmd_BoardTest,				0x03,			NULL,		">>BOARDTEST3",			NULL},
	{	"boardtest4",			Cmd_BoardTest,				0x04,			NULL,		">>BOARDTEST4",			NULL},
	{	"ettest",				Cmd_EtTest,					NULL,			NULL,		">>ETTEST",				NULL},
	{	"ettest1",				Cmd_EtTest,					0x01,			NULL,		">>ETTEST1",			NULL},
	{	"ettest2",				Cmd_EtTest,					0x02,			NULL,		">>ETTEST2",			NULL},
	{	"ettest3",				Cmd_EtTest,					0x03,			NULL,		">>ETTEST3",			NULL},
	{	"ettest4",				Cmd_EtTest,					0x04,			NULL,		">>ETTEST4",			NULL},
#if 1 // yeom_20210118 [start]
	{	"log",					Cmd_Log,					NULL,			NULL,		">>LOG",				NULL},
#endif // yeom_20210118 [end]

	{	"DPTX",					Cmd_DisplayPortTX,			NULL,					NULL,		">>DPTX",				NULL},
	{	"DPTX1",				Cmd_DisplayPortTX,			DP_CHANNEL_TX1,			NULL,		">>DPTX1",				NULL},
	{	"DPTX2",				Cmd_DisplayPortTX,			DP_CHANNEL_TX2,			NULL,		">>DPTX2",				NULL},

	{	"DPRX",					Cmd_DisplayPortRX,			NULL,					NULL,		">>DPRX",				NULL},
	{	"DPRX1",				Cmd_DisplayPortRX,			DP_CHANNEL_RX1,			NULL,		">>DPRX1",				NULL},
	{	"DPRX2",				Cmd_DisplayPortRX,			DP_CHANNEL_RX2,			NULL,		">>DPRX2",				NULL},

	{	NULL,					NULL,						NULL,			NULL,		NULL,					NULL},
};
