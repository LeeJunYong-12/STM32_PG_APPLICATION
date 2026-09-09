#include	"SB_Interface.h"

sbHandle_t		sbHandle;

u8 SB_RegisterInterface(void *pVoid)
{
	u8	result = 0;

	if(pVoid != NULL)
	{
		memcpy(&sbHandle, pVoid, sizeof(sbHandle));

		result = 1;
	}

	return result;
}

