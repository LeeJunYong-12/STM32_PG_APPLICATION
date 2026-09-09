#ifndef	_APP_FILEMANAGER_H
#define	_APP_FILEMANAGER_H

#include	"E7602_PG_Application.h"

#define	FM_INDEX_KEY_LIST_MAX_COUNT			8
#define	FM_INDEX_IMAGE_LIST_MAX_COUNT		200
#define	FM_INDEX_IMAGE_LIST_AVAILABLE_COUNT	(FM_INDEX_IMAGE_LIST_MAX_COUNT / 4)

typedef	struct{
	u16	fNo;
	u16	fState;
	u8	fName[256];		// fileName with path
	u32	fSize;
}fmIndexKey_t;

typedef union{
	u8	u8Data[2124];

	struct{
		u8				header[8];
		fmIndexKey_t	key[FM_INDEX_KEY_LIST_MAX_COUNT];
		u16				crc16;
		u16				dummy;
	};
}fmIndexKeyFile_t;

typedef	struct{
	u16	fNo;
	u16	fState;
	u8	fName[256];
	u32	fSize;
	u16	hPixel;
	u16	vLine;
}fmIndexImage_t;

typedef union{
	u8	u8Data[53612];

	struct{
		u8					header[8];
		fmIndexImage_t		image[FM_INDEX_IMAGE_LIST_MAX_COUNT];
		u16					crc16;
		u16					dummy;
	};
}fmIndexImageFile_t;

enum{
	FM_FILE_KEY = 0,
	FM_FILE_IMAGE,
};

extern	u8	fmDrive;

extern	fmIndexKey_t			fmIndexKey, fmIndexKeyCpy;
extern	fmIndexKeyFile_t		*pFmIndexKeyFile;

extern	fmIndexImage_t			fmIndexImage, fmIndexImageCpy;
extern	fmIndexImageFile_t		*pFmIndexImageFile;

u8 FM_IndexFilePathInit();
u16 FM_IndexCrc16(u8 type, void *pVoid);
u8 FM_IndexWrite(u8 type, u16 no, void *pVoid);
u8 FM_IndexRead(u8 type, u16 no, void *pVoid);
u8 FM_IndexDataInit(u8 type, void *pVoid);
u8 FM_IndexDataAddCrc16(u8 type, void *pVoid);
u8 FM_IndexDataCheck(u8 type, void *pVoid);
u8 FM_IndexDirCheck();
u8 FM_IndexDirMake();
u8 FM_IndexFileCheck(u8 type);
u8 FM_IndexFileMake(u8 type);
u8 FM_IndexFileLoad(u8 type, void *pVoid);
u8 FM_IndexFileSave(u8 type, void *pVoid);
u8 FM_IndexListVerify(u8 type);
u8 FM_IndexInit();
u8 FM_ImageIndexSort();
u8 FM_ImageIndexArrange();
u8 FM_IndexSave(u8 type);

#endif	// _APP_FILEMANAGER_H
