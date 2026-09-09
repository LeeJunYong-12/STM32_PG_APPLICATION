#ifndef	_APP_CORESPI_H
#define	_APP_CORESPI_H

#include	"E7602_PG_Application.h"

#define	CORE_SPI_MAX_COUNT			BOARD_CHANNEL_MAX_COUNT

enum{
	CORE_SPI_DATA_MODE_READ		= 0,
	CORE_SPI_DATA_MODE_WRITE,
};

enum{
	CORE_SPI_WRITE_MODE_CMD			= 0,
	CORE_SPI_WRITE_MODE_DATA,
};

u8 CoreSPI_Init();
u8 CoreSPI_IoInit();
u8 CoreSPI_CsCtrl(u8 position, u8 data);
u8 CoreSPI_DataModeCtrl(u8 mode);
u8 CoreSPI_WriteByte(u8 type, u8 data);
u8 CoreSPI_Write(u8 type, u8 *pData, u32 size);
u8 CoreSpi_Read(u8 *pData, u32 size);
u8 CoreSPI_TransmitData(u8 *pData, u16 size);
u8 CoreSPI_ReceiveData(u8 *pData, u16 size);

#endif	// _APP_CORESPI_H