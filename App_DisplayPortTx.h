#ifndef	_APP_DISPLAYPORTTX_H
#define	_APP_DISPLAYPORTTX_H

#include	"E7602_PG_Application.h"

enum{
	DPTX_ALPM_MODE_NONE		= 0,
	DPTX_ALPM_MODE_ENTER,
	DPTX_ALPM_MODE_EXIT,
	DPTX_ALPM_MODE_ENABLE,
	DPTX_ALPM_MODE_DISABLE,
	DPTX_ALPM_MODE_LINKRATE_CHANGE,
};

enum{
	RP_DP_TX_DEFAULT_SETUP_MODE_NONE					= 0,
	RP_DP_TX_DEFAULT_SETUP_MODE_HPD_EVENT_CONFIG,
};

enum{
	RP_DP_TX_LINK_TRAINING_MODE_NONE		= 0,
	RP_DP_TX_LINK_TRAINING_MODE_NORMAL,
	RP_DP_TX_LINK_TRAINING_MODE_FAST,
	RP_DP_TX_LINK_TRAINING_MODE_QUICK,
	RP_DP_TX_LINK_TRAINING_MODE_CUSTOM,
	RP_DP_TX_LINK_TRAINING_MODE_END,
};

enum{
	RP_DP_TX_SFUNCTION_MODE_NONE						= 0,
	RP_DP_TX_SFUNCTION_MODE_VIDEO_OUTPUT_CTRL,
	RP_DP_TX_SFUNCTION_MODE_COLOR_SPACE_REINIT,

	RP_DP_TX_SFUNCTION_MODE_ASSR_CTRL					= 0x0070,
	RP_DP_TX_SFUNCTION_MODE_LTDONE_AUTO_VIDEO_OUTPUT	= 0x0071,

	RP_DP_TX_SFUNCTION_MODE_ALPDP_DUMMY_VIDEO			= 0x0080,
	RP_DP_TX_SFUNCTION_MODE_FIXED_MVID_CTRL				= 0x0090,

	RP_DP_TX_SFUNCTION_MODE_ALPM_CTRL					= 0x0100,

	RP_DP_TX_SFUNCTION_MODE_STATUS_READ					= 0xF000,

	RP_DP_TX_SFUNCTION_MODE_DP_REG_WRITE				= 0xFFFE,
	RP_DP_TX_SFUNCTION_MODE_DP_REG_READ					= 0xFFFF,
};

enum{
	RP_DP_TX_TRANSACTION_MODE_NONE						= 0,
	RP_DP_TX_TRANSACTION_MODE_AUX_WRITE,
	RP_DP_TX_TRANSACTION_MODE_AUX_READ,
	RP_DP_TX_TRANSACTION_MODE_EDID_READ,
	RP_DP_TX_TRANSACTION_MODE_I2C_WRITE,
	RP_DP_TX_TRANSACTION_MODE_I2C_READ,
};

enum{
	DP_TX_REG_DATA_MODE_NONE			= 0,
	DP_TX_REG_DATA_MODE_TYPE_1,

	DP_TX_REG_DATA_MODE_END,
};

typedef	struct{
	u32	channel;
	u32	regAdrs;
	u32	dataAdrs;
	u32	dataSize;
	u8	*pBuffer;
}dptxRegData_t;

typedef	struct{
	u32	mode;

	u8	(*DataWrite)		(void*);
	u8	(*DataWritemd)		(void*);
	u8	(*DataWritemx)		(void*);
	u8	(*DataWritemxlong)	(void*);
	u8	(*DataRead)			(void*);
	u8	(*DataReadmd)		(void*);
	u8	(*DataReadmx)		(void*);
}dptxRegHandle_t;

typedef	struct{
	u32	channel;
	u32	mode;
	u32	buffer[16];
}dptxDefaultSetup_t;

typedef	struct{
	u32	channel;
	u32	mode;

	u32	linkRate;
	u32	laneCount;
	u32	voltSwing;
	u32	preEmphasis;

	u32	fTPattern;
	u32	sTPattern;

	u32	fTDelay;
	u32	sTDelay;

	u32	dpcdCheck;
	u32	capabilityCheck;
	u32	statusCheck;
}dptxLinkTraining_t;

typedef	struct{
	u32	channel;
	u32	mode;
	u32	ctrl[4];
	u32	buffer[16];
}dptxSFunction_t;

typedef	struct{
	u32	channel;
	u32	mode;
	u32	adrs;
	u8	*pData;
	u32	size;
	u32	status;
}dptxTransaction_t;

typedef	struct{
	u32	enable;

	u32	alpdpEnable;
	u32	tryOnce;

//	COMMON
	u32	pdwnSignal;
	u32	pdwnSeqPeriod;
	u32	pdwnSeqNumber;

	u32	wakeCount;
	u32	t1Count;
	u32	t2Count;

//	ALPDP
	u32	linkRateChange;
	u32	linkRateValue;
	u32	selectTPS;
	u32	t12timing;
	u32	clkRecoveryCount;
	u32	symbolLockCount;
}dptxALPM_t;

u8 DPTX_DefaultSetup(void *pVoid);
u8 DPTX_LinkTraining(void *pVoid);
u8 DPTX_SpecialFunction(void *pVoid);
u8 DPTX_Transaction(void *pVoid);
u8 DPTX_SecondaryDataPacketContainClear(u8 channel);
u8 DPTX_SecondaryDataPacketContain(u8 chipSelect, u16 adrs, u8 *pData, u16 size);
u8 DPTX_SecondaryDataPacketWrite(u8 channel, u8 *pData, u16 size);
u8 DPTX_SecondaryDataPacketMultiContain(u8 channel, u8 *pData, u16 size);
u8 DPTX_SecondaryDataPacketMultiWrite(u8 channel);
u8 DPTX_AuxWrite(u8 channel, u32 adrs, u8 *pData, u16 size);
u8 DPTX_AuxRead(u8 channel, u32 adrs, u8 *pData, u16 size);
u8 DPTX_AlpmCtrl(u8 channel, void *pVoid);
u8 DPTX_RegDataWrite_TYPE_1(void *pVoid);
u8 DPTX_RegDataWrite_TYPE_1_md(void *pVoid);
u8 DPTX_RegDataWrite_TYPE_1_mx(void *pVoid);
u8 DPTX_RegDataWrite_TYPE_1_mx_long(void *pVoid);
u8 DPTX_RegDataRead_TYPE_1(void *pVoid);
u8 DPTX_RegDataRead_TYPE_1_md(void *pVoid);
u8 DPTX_RegDataRead_TYPE_1_mx(void *pVoid);
u8 DPTX_SetDataModeDIC(u32 mode);
u32 DPTX_GetDataModeDIC();
u8 DPTX_RegDataWriteDIC(void *pVoid);
u8 DPTX_RegDataWriteDIC_md(void *pVoid);
u8 DPTX_RegDataWriteDIC_mx(void *pVoid);
u8 DPTX_RegDataWriteDIC_mx_long(void *pVoid);
u8 DPTX_RegDataReadDIC(void *pVoid);
u8 DPTX_RegDataReadDIC_md(void *pVoid);
u8 DPTX_RegDataReadDIC_mx(void *pVoid);
u8 DPTX_RegWrite(u8 channel, u32 adrs, u32 data);
u8 DPTX_RegRead(u8 channel, u32 adrs, u32 *pData);
u8 DPTX_LaneConfig(u8 channel, u32 state);
u8 DPTX_AssrCtrl(u8 channel, u8 state);
u8 DPTX_DummyVideoCtrl(u8 channel, u8 state);
u8 DPTX_FixedMVID(u8 channel, u8 state);
u8 DPTX_AlpmEnter(u8 channel);
u8 DPTX_AlpmExit(u8 channel);
u8 DPTX_AlpmEnable(u8 channel, u8 mode);
u8 DPTX_AlpmDisable(u8 channel);
u8 DPTX_AlpmLinkRateChange(u8 channel, u32 lrc_enable, u32 lrc_mode, u32 lrc_frame_period);
u8 DPTX_LtAutoVideoOutput(u8 channel, u8 autoVideoOutput);
u8 DP_VVB(u8 chipSelect, u32 common, u32 hz);

#endif	// _APP_DISPLAYPORTTX_H
