#include	"BSP_Error.h"

void BSP_ERROR_Loop(u32 errorPos)
{
	volatile	bspError_t	error;
	volatile	u8	b = 0;

	error = (bspError_t)errorPos;

	while(1)
	{
		if(b == 1)		break;
	}
}

