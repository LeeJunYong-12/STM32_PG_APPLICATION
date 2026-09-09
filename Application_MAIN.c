#include	"Application_MAIN.h"

#pragma location=0x08080200
const	u32	applicationVersion	= APPLICATION_VERSION;

#pragma	location=0x08080204
const	u32	applicationBuild	= APPLICATION_BUILD;

#pragma	location=0x08080208
const	u32	applicationType		= APPLICATION_TYPE;

void main()
{
	Init_System();

	while(1)
	{
		Script_Process();	
		
		Board_BootMode();

		CMD_RcvCheck();

		CMD_Process();

		Board_ResetProcess();

		IMG_RepeatProcess();

		FatFS_MsdChecker();

		FatFS_ProcessMicroSD();
	
		HPD_EVENT();
		//CRC_Test_Function();

		DisplayTimerFunction();
	}
}
