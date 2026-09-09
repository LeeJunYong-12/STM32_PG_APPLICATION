#ifndef	_CMD_FPGA_H
#define	_CMD_FPGA_H

#include	"E7602_PG_Application.h"

extern	const	commandList_t	cmdList_FPGA[];

u8 Cmd_FpgaPowerOff(void *pVoid);
u8 Cmd_FpgaPowerOn(void *pVoid);
u8 Cmd_FpgaReset(void *pVoid);
u8 Cmd_FpgaRead(void *pVoid);
u8 Cmd_FpgaWrite(void *pVoid);
u8 Cmd_FpgaPTG(void *pVoid);
u8 Cmd_FpgaBUSPath(void *pVoid);
u8 Cmd_FpgaUpdateCheck(void *pVoid);

#endif	// _CMD_FPGA_H