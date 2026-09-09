#include	"Cmd_System.h"

u8 PG_boot_flag = 0;
u32 filesize_pw = 0;
u32 FPGA_filesize = 0;

const	commandList_t	cmdList_System[] =	{
//												{	Command,				Function,					Option,		Mode,		RootMessage				Note}
												{	"help",					Cmd_Help,					NULL,		0x01,		";HELP",				NULL},	//ok
												{	"version",				Cmd_SystemVersion,			NULL,		NULL,		";VERSION",				NULL},	//ok
												{	"build",				Cmd_SystemBuild,			NULL,		NULL,		";BUILD",				NULL},	//ok
												{	"type",					Cmd_SystemType,				NULL,		NULL,		";TYPE",				NULL},	//ok
												{	"reset",				Cmd_SystemReset,			NULL,		NULL,		";RESET",				NULL},	//ok
												{	"config.clear",			Cmd_SystemConfigClear,		NULL,		NULL,		";CONFIG.CLEAR",		NULL},	//ok
												{	"config.load",			Cmd_SystemConfigLoad,		NULL,		NULL,		";CONFIG.LOAD",			NULL},	//ok
												{	"config.save",			Cmd_SystemConfigSave,		NULL,		NULL,		";CONFIG.SAVE",			NULL},	//ok
												{	"set.config",			Cmd_SystemSetConfig,		NULL,		NULL,		";SET.CONFIG",			NULL},	//ok
												{	"get.config",			Cmd_SystemGetConfig,		NULL,		NULL,		";GET.CONFIG",			NULL},	//ok
												{	"set.netconfig",		Cmd_SystemSetNetconfig,		NULL,		NULL,		";SET.NETCONFIG",		NULL},	//ok
												{	"get.netconfig",		Cmd_SystemGetNetconfig,		NULL,		NULL,		";GET.NETCONFIG",		NULL},	//ok
												{	"fan.enable",			Cmd_SystemFanEnable,		NULL,		NULL,		";FAN.ENABLE",			NULL},
												{	"fan.disable",			Cmd_SystemFanDisable,		NULL,		NULL,		";FAN.DISABLE",			NULL},
												{	"PG.boot",				Cmd_SystemPGBoot,			NULL,		NULL,		";PG.BOOT",				NULL},	//ok
												{	"Power.boot",			Cmd_SystemPowerBoot,		NULL,		NULL,		";POWER.BOOT",			NULL},	//ok
												{	"Power.Update",			Cmd_SystemPowerDown,		NULL,		NULL,		";POWER.DOWN",			NULL},	//ok
												{	"Power.BootStart",		Cmd_SystemPowerBootStart,	NULL,		NULL,		";POWER.BOOTCOMPLETE",	NULL},	//ok
												{	"Power.BootErase",		Cmd_SystemPowerBootErase,	NULL,		NULL,		";POWER.BOOTERASE",		NULL},	//ok
												{	"Power.BootCheck",		Cmd_SystemPowerBootCheck,	NULL,		NULL,		";POWER.BOOTCHECK",		NULL},	//ok
												{	"EEPROM.Clear",			Cmd_SystemEEPROMClear,		NULL,		NULL,		";POWER.EEPROMCLEAR",	NULL},	//ok
												{	"FPGA.Update",			Cmd_SystemFPGADown,			NULL,		NULL,		";FPGA.DOWN",			NULL},	//ok
												{	"FPGA.DDRUpdate",		Cmd_SystemDDRDown,			NULL,		NULL,		";DDR.DOWN",			NULL},	//ok
												{	"FPGA1.flashcheck",		Cmd_Systemflashcheck1,		NULL,		NULL,		";DDR.DOWN",			NULL},	//ok
												{	"FPGA2.flashcheck",		Cmd_Systemflashcheck2,		NULL,		NULL,		";DDR.DOWN",			NULL},	//ok
												{	"PGFPGA.flashcheck",	Cmd_SystemPGflashcheck,		NULL,		NULL,		";DDR.DOWN",			NULL},	//ok
												{	NULL,					NULL,						NULL,		NULL,		NULL,					NULL},
											};

u8 Cmd_SystemVersion(void *pVoid)
{
	u8	result = 0, rtn;
	u8	buffer[512];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = 1;

	memset(buffer, NULL, sizeof(buffer));

	switch(Board_GetConstruct())
	{
		case BOARD_CONSTRUCT_SINGLE:
			rtn = BASE_S_GetVersion(buffer);
			if(rtn)		CMD_Printf("%s", buffer);
			else		CMD_Printf(";VERSION_GET_ERROR");
			break;

		case BOARD_CONSTRUCT_MULTI:
			rtn = BASE_M_GetVersion(buffer);
			if(rtn)		CMD_Printf("%s", buffer);
			else		CMD_Printf(";VERSION_GET_ERROR");
			break;

		case BOARD_CONSTRUCT_RELIABLITY:
			rtn = BASE_R_GetVersion(buffer);
			if(rtn)		CMD_Printf("%s", buffer);
			else		CMD_Printf(";VERSION_GET_ERROR");
			break;

		default:
			CMD_Printf(";SYSTEM_TYPE_WRONG");
			result = 0;
			break;
	}

	return result;
}

u8 Cmd_SystemBuild(void *pVoid)
{
	u8	result = 0, rtn;
	u8	buffer[512];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = 1;

	memset(buffer, NULL, sizeof(buffer));

	switch(Board_GetConstruct())
	{
		case BOARD_CONSTRUCT_SINGLE:
			rtn = 0;
			if(rtn)		CMD_Printf("%s", buffer);
			else		CMD_Printf(";BUILDDATE_GET_ERROR");
			break;

		case BOARD_CONSTRUCT_MULTI:
			rtn = BASE_M_GetBuildDate(buffer);
			if(rtn)		CMD_Printf("%s", buffer);
			else		CMD_Printf(";BUILDDATE_GET_ERROR");
			break;

		case BOARD_CONSTRUCT_RELIABLITY:
			rtn = BASE_R_GetBuildDate(buffer);
			if(rtn)		CMD_Printf("%s", buffer);
			else		CMD_Printf(";BUILDDATE_GET_ERROR");
			break;

		default:
			CMD_Printf(";SYSTEM_TYPE_WRONG");
			result = 0;
			break;
	}

	return result;
}

u8 Cmd_SystemType(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32];
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	memset(buffer, NULL, sizeof(buffer));

	result = Board_GetFirmwareType(buffer);

	if(result)
	{
		CMD_Printf(";%s", buffer);
	}

	return result;
}

u8 Cmd_SystemReset(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = Board_ResetEnable();

	return result;
}

u8 Cmd_SystemConfigClear(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = Config_DataClear();

	return result;
}

u8 Cmd_SystemConfigLoad(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = Config_BoardData();

	return result;
}

u8 Cmd_SystemConfigSave(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	result = Config_DataSave();

	return result;
}

u8 Cmd_SystemSetConfig(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "modeltype"))
	{
		result = Config_SetData(CONFIG_DATA_MODELTYPE, pData->pArgv[1], strlen((char*)pData->pArgv[1]));
	}
	else if(CMD_Compare(pData->pArgv[0], "serial"))
	{
		result = Config_SetData(CONFIG_DATA_SERIALNO, pData->pArgv[1], strlen((char*)pData->pArgv[1]));
	}
	else if(CMD_Compare(pData->pArgv[0], "build"))
	{
		result = Config_SetData(CONFIG_DATA_BUILDDATE, pData->pArgv[1], strlen((char*)pData->pArgv[1]));
	}
	else						return result;

	return result;
}

u8 Cmd_SystemGetConfig(void *pVoid)
{
	u8	result = 0;
	u8	buffer[32], size;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)					return result;

	memset(buffer, NULL, sizeof(buffer));

	if(CMD_Compare(pData->pArgv[0], "modeltype"))
	{
		result = Config_GetData(CONFIG_DATA_MODELTYPE, buffer, &size);

		if(size != CONFIG_MODELTYPE_SIZE)	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "serial"))
	{
		result = Config_GetData(CONFIG_DATA_SERIALNO, buffer, &size);

		if(size != CONFIG_SERIALNO_SIZE)	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "build"))
	{
		result = Config_GetData(CONFIG_DATA_BUILDDATE, buffer, &size);

		if(size != CONFIG_BUILDDATE_SIZE)	result = 0;
	}
	else									return result;

	if(result)
	{
		CMD_Printf(";%s", buffer);
	}

	return result;
}

u8 Cmd_SystemSetNetconfig(void *pVoid)
{
	u8	result = 0, rtn;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

	if(CMD_Compare(pData->pArgv[0], "mode"))
	{
		u16	mode;

		if(CMD_Compare(pData->pArgv[1], "none"))		mode = NETWORK_MODE_NONE;
		else if(CMD_Compare(pData->pArgv[1], "tcp"))	mode = NETWORK_MODE_TCP;
		else if(CMD_Compare(pData->pArgv[1], "udp"))	mode = NETWORK_MODE_UDP;
		else					return result;

		result = Config_SetData(CONFIG_DATA_NETWORK_MODE, &mode, sizeof(mode));
	}
	else if(CMD_Compare(pData->pArgv[0], "ip"))
	{
		u8	ip[4];

		rtn = CMD_IpDataParsing(pData->pArgv[1], ip);
		if(rtn == 0)			return result;

		result = Config_SetData(CONFIG_DATA_NETWORK_IP, ip, sizeof(ip));
	}
	else if(CMD_Compare(pData->pArgv[0], "nm"))
	{
		u8	nm[4];

		rtn = CMD_IpDataParsing(pData->pArgv[1], nm);
		if(rtn == 0)			return result;

		result = Config_SetData(CONFIG_DATA_NETWORK_NETMASK, nm, sizeof(nm));
	}
	else if(CMD_Compare(pData->pArgv[0], "gw"))
	{
		u8	gw[4];

		rtn = CMD_IpDataParsing(pData->pArgv[1], gw);
		if(rtn == 0)			return result;

		result = Config_SetData(CONFIG_DATA_NETWORK_GATEWAY, gw, sizeof(gw));
	}
	else if(CMD_Compare(pData->pArgv[0], "tcpport"))
	{
		u16	port;

		port = CMD_StrToUL(pData->pArgv[1]);

		result = Config_SetData(CONFIG_DATA_NETWORK_TCPPORT, &port, sizeof(port));
	}
	else if(CMD_Compare(pData->pArgv[0], "udpport"))
	{
		u16	port;

		port = CMD_StrToUL(pData->pArgv[1]);

		result = Config_SetData(CONFIG_DATA_NETWORK_UDPPORT, &port, sizeof(port));
	}
	else						return result;

	return result;
}

u8 Cmd_SystemGetNetconfig(void *pVoid)
{
	u8	result = 0;
	u8	size;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	if(CMD_Compare(pData->pArgv[0], "mode"))
	{
		u16	mode;

		result = Config_GetData(CONFIG_DATA_NETWORK_MODE, &mode, &size);

		if((result) && (size == sizeof(mode)))
		{
			if(mode == NETWORK_MODE_NONE)		CMD_Printf(";MODE=NONE");
			else if(mode == NETWORK_MODE_TCP)	CMD_Printf(";MODE=TCP");
			else if(mode == NETWORK_MODE_UDP)	CMD_Printf(";MODE=UDP");
			else								CMD_Printf(";MODE=OTHER");
		}
		else	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "ip"))
	{
		u8	ip[4];

		result = Config_GetData(CONFIG_DATA_NETWORK_IP, ip, &size);

		if((result) && (size == sizeof(ip)))
		{
			CMD_Printf(";IP=%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
		}
		else	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "nm"))
	{
		u8	nm[4];

		result = Config_GetData(CONFIG_DATA_NETWORK_NETMASK, nm, &size);

		if((result) && (size == sizeof(nm)))
		{
			CMD_Printf(";NM=%d.%d.%d.%d", nm[0], nm[1], nm[2], nm[3]);
		}
		else	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "gw"))
	{
		u8	gw[4];

		result = Config_GetData(CONFIG_DATA_NETWORK_GATEWAY, gw, &size);

		if((result) && (size == sizeof(gw)))
		{
			CMD_Printf(";GW=%d.%d.%d.%d", gw[0], gw[1], gw[2], gw[3]);
		}
		else	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "tcpport"))
	{
		u16	port;

		result = Config_GetData(CONFIG_DATA_NETWORK_TCPPORT, &port, &size);

		if((result) && (size == sizeof(port)))
		{
			CMD_Printf(";TCPPORT=%d", port);
		}
		else	result = 0;
	}
	else if(CMD_Compare(pData->pArgv[0], "udpport"))
	{
		u16	port;

		result = Config_GetData(CONFIG_DATA_NETWORK_UDPPORT, &port, &size);

		if((result) && (size == sizeof(port)))
		{
			CMD_Printf(";UDPPORT=%d", port);
		}
		else	result = 0;
	}
	else						return result;

	return result;
}

u8 Cmd_SystemFanEnable(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]);

	if(channel == 0)			return result;

	channel -= 1;

	result = FanCtrl_SetStatus(channel, 1);

	return result;
}

u8 Cmd_SystemFanDisable(void *pVoid)
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]);

	if(channel == 0)			return result;

	channel -= 1;

	result = FanCtrl_SetStatus(channel, 0);

	return result;
}

u8 Cmd_SystemPGBoot(void *pVoid)
{
	u8	result = 0;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	PG_boot_flag = 1;

	result = 1;

	return result;
}

u8 Cmd_SystemPowerBoot(void *pVoid)
{
	u8	result = 0;
//	u8 	position = 2;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 0)		return result;

	PWR_BootEnter(2);
	PWR_BootEnter(3);

	result = 1;

	return result;
}

u8 Cmd_SystemPowerDown(void *pVoid)
{
	u8	result = 0;
	u8	*pFileData;
	u8 rtn;
	u32 filesize, rcvSize;
	u32 rxSrc;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

    rxSrc = pData->srcType;

    filesize = CMD_StrToUL(pData->pArgv[0]);

	pFileData = (u8*)MEMORY_MAP_SDRAM_FILE_DUMP_ADRS;

    if(rxSrc == CMD_COMM_SRC_TYPE_USB)
    {
		rtn = UsbPkt_FileDownload(pFileData, &rcvSize);
    }
    else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
    {
		rtn = NetworkPKT_FileDownload(pFileData, &rcvSize);
    }
    else	return result;

    if(filesize != rcvSize)
	{
		return result;
	}

    if(rtn == 1)
    {
		filesize_pw = filesize;
		result = 1;
    }

	return result;
}

u8 Cmd_SystemPowerBootStart(void *pVoid)	// system.power.bootstart [channel]		// channel = 1 or 2
{
	u8	result = 0, rtn, channel;
	cmdFunctionData_t	*pData;
	u8* adrs;
	u32 cnt = 0;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]);

	rtn = SB_DataInit();
	if(rtn == 0)					return result;

	sbParsingData.waitTime			= 10000;
	sbParsingData.channel			= channel + 1;
	sbParsingData.cmd				= SB_CMD_PWR_BOOT_FLASH;

	SYS_WordToHex(filesize_pw, &sbParsingData.pTxData[sbParsingData.txSize]);
	sbParsingData.txSize			+= 4;

	rtn = SB_SubBoardXfer_boot(&sbParsingData);

	if(rtn == 0)					return result;
	HAL_Delay(10);

	sbParsingData.txSize = 32;

	do{

		adrs = (u8*)MEMORY_MAP_SDRAM_FILE_DUMP_ADRS+cnt;

		memcpy(sbParsingData.pTxData, adrs ,32);

		SB_SubBoardXfer_boot(&sbParsingData);

		//HAL_Delay(10); // may be increased
		HAL_Delay(1); // may be increased

		cnt += 32;

		if(cnt >= filesize_pw)
		{
            rtn = 1;
			break;
		}

	}while(1); // test ok

	if(rtn)
	{
		HAL_Delay(100);
		result = 1;
		CMD_Printf(";POWERBOOT(%dch)", channel);
	}

	return result;
}

u8 Cmd_SystemPowerBootErase(void *pVoid)	// system.power.booterase [channel]		// channel = 1 or 2
{
	u8	result = 0;
	u8 	channel;
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]);

	PWR_BootErase(channel + 1);

	result = 1;

	return result;
}

u8 Cmd_SystemPowerBootCheck(void *pVoid)	// system.power.bootcheck [channel]		// channel = 1 or 2
{
	u8	result = 0;
	u8	channel;
	cmdFunctionData_t	*pData;
	u32 file_size;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

	channel = CMD_StrToUL(pData->pArgv[0]); 

  	PWR_BOOTCHECK(channel + 1, &file_size);
	if(filesize_pw == file_size)
	{
		result = 1;
	}
	return result;
}

u8 Cmd_SystemEEPROMClear(void *pVoid)
{
	u8	result = 0, rtn;
	u32 status;
    u8 status_8;
	
	cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 1)		return result;

 	status		= CMD_StrToUL(pData->pArgv[0]);

	status_8	= (u8)status;

	rtn = EEPROM_Write(0x400, &status_8, 1);   //test complete

	if(rtn == 1)
	{
		result = 1;
	}
	return result;
}

u8 Cmd_SystemFPGADown(void *pVoid)	// system.fpga.update [PG / CORE] [FILESIZE]
{
	u8	result = 0;
	u8	*pFileData;
	u8 rtn;
	u32 rcvSize;
	u32 rxSrc;
	u32 filesize_difference;
	cmdFunctionData_t	*pData;
	u32 pgFileSize = 8388608, coreFileSize = 25165824;
	u8 sel;

	pData = (cmdFunctionData_t*)pVoid;

	if(pData->argc != 2)		return result;

    rxSrc = pData->srcType;

	if(CMD_Compare(pData->pArgv[0], "pg"))					sel = 0;
	else if(CMD_Compare(pData->pArgv[0], "core"))			sel = 1;
	else													return result;

    FPGA_filesize = CMD_StrToUL(pData->pArgv[1]);

	pFileData = (u8*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

    if(rxSrc == CMD_COMM_SRC_TYPE_USB)
    {
		rtn = UsbPkt_FileDownload(pFileData, &rcvSize);
    }
    else if(rxSrc == CMD_COMM_SRC_TYPE_ETHERNET)
    {
		rtn = NetworkPKT_FileDownload(pFileData, &rcvSize);
    }
    else	return result;

    if(FPGA_filesize != rcvSize)
	{
		return result;
	}

    if(rtn == 1)
    {
		if(sel == 0)	// PG 파일크기는 UI로부터 받아서 
		{
			filesize_difference = pgFileSize - FPGA_filesize;
			for(u32 i = 0; i < filesize_difference; i++)
				pFileData[FPGA_filesize + i] = 0x00;
		}
		else	// CORE 
		{
			filesize_difference = coreFileSize - FPGA_filesize;
			for(u32 i = 0; i < filesize_difference; i++)
				pFileData[FPGA_filesize + i] = 0xff;
		}

		result = 1;
    }

	return result;
}

u8 Cmd_SystemDDRDown(void *pVoid)	// system.fpga.ddrupdate [PG / CORE / CORE1 / CORE2]
{
	u8	result = 0;
	u8 	rtn;
	u32 *FileData;
	u8	sel, chipSelect = 0;
	u32 size;

    cmdFunctionData_t	*pData;

	pData = (cmdFunctionData_t*)pVoid;

	if(CMD_Compare(pData->pArgv[0], "pg"))					sel = 0;
	else if(CMD_Compare(pData->pArgv[0], "core"))			sel = 1;
	else if(CMD_Compare(pData->pArgv[0], "core1"))			sel = 2;
	else if(CMD_Compare(pData->pArgv[0], "core2"))			sel = 3;
	else													return result;

	FileData = (u32*)MEMORY_MAP_SDRAM_IMAGE_DUMP_ADRS;

	if(sel == 0)	// PG UPDATE
	{
		size = 8388608;

		FPGA_WriteSingle(FPGA_CMD_INPUT_RGB10_MODE, 0x00000002);
		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_ADRS, 0x00000000);
		FPGA_WriteSingle(FPGA_CMD_REMOTE_UPDATE_SIZE, size);

		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_START, 0);
		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		size /= 4;	// 8388608/4
		for(u32 cnt = 0; cnt < size; cnt++)
		{
			FPGA_WriteDATA(FileData[cnt]);
		}
		FPGA_WriteSingle(FPGA_CMD_DDR_WRITE_END, 0);		

		rtn = FpgaPG_RemoteUpdateControl(1);	// UPDATE START(FPGA)
		if(rtn == 1)									result = 1;
	}
	else			// CORE UPDATE
	{
		size = 6291456; // 25165824/4

		FPGA_WriteSingle(FPGA_CMD_CORE_BUS_BYPASS, 1);	// Core FPGA Path Change

		FPGA_WriteCMD(FPGA_CMD_DDR_WRITE_DATA);

		if(sel == 1)
		{
			for(int i = sel + 1; i < 4; i++)
			{
				DPC_DirectRegWriteSingle(i, CORE_REG_VIDEO_STREAM_INFO_H_ACTIVE, 4096);
				HAL_Delay(1);
				DPC_DirectRegWriteSingle(i, CORE_REG_VIDEO_STREAM_INFO_V_ACTIVE, 1536);
				HAL_Delay(1);

				rtn = CoreVC_FPGADATAInputMode(i, BUS_DATA_WRITE);
				if(rtn == 0) 									return result;
				rtn = CoreVC_VideoFrameWriteStart(i);
				if(rtn == 0)									return result;
				rtn = CoreVC_VideoStreamInputEnable(i);
				if(rtn == 0)									return result;
			}
			
			for(u32 cnt = 0; cnt < size; cnt++)
			{
				FPGA_WriteDATA(FileData[cnt]);
			}
			
			for(int i = sel + 1; i < 4; i++)
			{
				rtn = CoreVC_VideoStreamInputDisable(i);
				if(rtn == 0)									return result;

				rtn = CoreVC_VideoFrameWriteStop(i);
				if(rtn == 0) 									return result;

				rtn = DP_ProcessStart(i, CORE_REG_REGION_COMMON_RESERVE_4);
				if(rtn == 0)									return result;
			}

			if(rtn == 1)										result = 1;			
		}
		else
		{
			chipSelect = sel;
			DPC_DirectRegWriteSingle(chipSelect, CORE_REG_VIDEO_STREAM_INFO_H_ACTIVE, 4096);
			HAL_Delay(1);
			DPC_DirectRegWriteSingle(chipSelect, CORE_REG_VIDEO_STREAM_INFO_V_ACTIVE, 1536);
			HAL_Delay(1);

			rtn = CoreVC_FPGADATAInputMode(chipSelect, BUS_DATA_WRITE);
			if(rtn == 0) 									return result;
			rtn = CoreVC_VideoFrameWriteStart(chipSelect);
			if(rtn == 0)									return result;
			rtn = CoreVC_VideoStreamInputEnable(chipSelect);
			if(rtn == 0)									return result;
			
			for(u32 cnt = 0; cnt < size; cnt++)
			{
				FPGA_WriteDATA(FileData[cnt]);
			}			

			rtn = CoreVC_VideoStreamInputDisable(chipSelect);
			if(rtn == 0)									return result;

			rtn = CoreVC_VideoFrameWriteStop(chipSelect);
			if(rtn == 0) 									return result;

			rtn = DP_ProcessStart(chipSelect, CORE_REG_REGION_COMMON_RESERVE_4);
			if(rtn == 1)									result = 1;
		}

		FPGA_WriteSingle(FPGA_CMD_CORE_BUS_BYPASS, 0);	// PG FPGA Path Return
	}
	return result;
}

u8 Cmd_Systemflashcheck1(void *pVoid)
{
	u8	result = 0;
	u8 	rtn;
	static u16 update_timeout = 0;

	rtn = DP_ProcessCheck_extend(2);	// CORE1

	if(rtn == 1)
	{
		CMD_Printf(";Download Complete");
		update_timeout = 0;
		result = 1;
	}
	else
	{
		if(update_timeout <= 80)
		{
			CMD_Printf(";Downloading ... (%d)", ++update_timeout);
			result = 1;
		}
		else
		{
			CMD_Printf(";Download Timeout");
			update_timeout = 0;
			return result;
		}
	}

	return result;
}

u8 Cmd_Systemflashcheck2(void *pVoid)
{
	u8	result = 0;
	u8 	rtn;
	static u16 update_timeout = 0;

	rtn = DP_ProcessCheck_extend(3);	// CORE2

	if(rtn == 1)
	{
		CMD_Printf(";Download Complete");
		update_timeout = 0;
		result = 1;
	}
	else
	{
		if(update_timeout <= 80)
		{
			CMD_Printf(";Downloading ... (%d)", ++update_timeout);
			result = 1;
		}
		else
		{
			CMD_Printf(";Download Timeout");
			update_timeout = 0;
			return result;
		}
	}

	return result;
}

u8 Cmd_SystemPGflashcheck(void *pVoid)	// PG FPGA CHECK
{
	u8	result = 0;
	u8 	rtn, error;
	static u16 update_timeout = 0;

	rtn = FpgaPG_RemoteUpdateStatus(UPDATE_DONE_STATUS);
	if(rtn == 1)
	{
		error = FpgaPG_RemoteUpdateStatus(UPDATE_ERROR_STATUS);
		
		if(error)						result = 0;
		else							result = 1;

		FpgaPG_RemoteUpdateControl(0);	// UPDATE STOP(FPGA)

		CMD_Printf(";Download Complete");
		update_timeout = 0;
	}
	else
	{
		if(update_timeout <= 80)
		{
			CMD_Printf(";Downloading ... (%d)", ++update_timeout);
			result = 1;
		}
		else
		{
			CMD_Printf(";Download Timeout");
			update_timeout = 0;
			FpgaPG_RemoteUpdateControl(0);	// UPDATE STOP(FPGA)
			return result;
		}
	}

	return result;
}