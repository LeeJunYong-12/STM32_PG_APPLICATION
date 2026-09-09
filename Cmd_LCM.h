#ifndef	_CMD_LCM_H
#define	_CMD_LCM_H

#include	"E7602_PG_Application.h"

extern	const	commandList_t	cmdList_LCM[];

extern	char P_1;
extern	char P_2;
extern	char P_3;
extern	char Y;
extern	char W1;
extern	char W2;
extern	char D;
extern	char S1;
extern	char S2;
extern	char S3;
extern	char S4;
extern	char E1;
extern	char E2;
extern	char E3;
extern	char E4;
extern	char R;
extern	char X;
extern	u8 ptxbuffer[17];
extern	u8 prxbuffer[17];
extern	u8 setmodelchk;
extern	u8 E1_copy;
extern	u8 E2_copy;
extern	u8 E3_copy;
extern	u8 E4_copy;
extern	u8 P1_copy;
extern	u8 P2_copy;
extern	u8 P3_copy;


extern 	u8 setpppflag;
extern 	u8 seteeeeflag;

u8 Cmd_LcmSpiInit(void *pVoid);
u8 Cmd_LcmSpiDeInit(void *pVoid);
u8 Cmd_LcmSpiCsCtrl(void *pVoid);
u8 Cmd_LcmSpiWrite(void *pVoid);
u8 Cmd_LcmSpiRead(void *pVoid);
u8 Cmd_LcmSpiWriteRead(void *pVoid);
u8 Cmd_LcmSpiWriteCustom(void *pVoid);
u8 Cmd_LcmSpiReadCustom(void *pVoid);
u8 Cmd_LcmSpiWriteReadCustom(void *pVoid);
u8 Cmd_LcmSpiIoWrite(void *pVoid);
u8 Cmd_LcmSpiIoRead(void *pVoid);
u8 Cmd_LcmI2CInit(void *pVoid);
u8 Cmd_LcmI2CDeInit(void *pVoid);
u8 Cmd_LcmI2CGetReady(void *pVoid);
u8 Cmd_LcmI2CScanAdrs(void *pVoid);
u8 Cmd_LcmI2CWrite(void *pVoid);
u8 Cmd_LcmI2CRead(void *pVoid);
u8 Cmd_LcmI2CReadCustom(void *pVoid);
u8 Cmd_LcmI2CIoDirection(void *pVoid);
u8 Cmd_LcmI2CIoWrite(void *pVoid);
u8 Cmd_LcmI2CIoRead(void *pVoid);
u8 SerialCreate(void *pVoid);
u8 SerialSave(void *pVoid);
u8 SerialReadCheck(void *pVoid);
u8 Serialppp(void *pVoid);
u8 Serialeeee(void *pVoid);
u8 Serialprotocol(u8 x_result);

#endif	// _CMD_LCM_H