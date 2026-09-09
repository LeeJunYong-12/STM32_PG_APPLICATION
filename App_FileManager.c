#include	"App_FileManager.h"

u8	fmDrive = 0;

fmIndexKey_t			fmIndexKey, fmIndexKeyCpy;
fmIndexKeyFile_t		*pFmIndexKeyFile;

fmIndexImage_t			fmIndexImage, fmIndexImageCpy;
fmIndexImageFile_t		*pFmIndexImageFile;

static FIL	fmIndexFile;
static DIR	fmIndexDir;

u8 FM_IndexFilePathInit()
{
	u8	result = 0;
	u32	keySize, imageSize;
	u32	keyAdrs, imageAdrs;

	keySize		= sizeof(fmIndexKeyFile_t);
	imageSize	= sizeof(fmIndexImageFile_t);

	keyAdrs		= MEMORY_MAP_SDRAM_FILE_LIST_ADRS;
	imageAdrs	= MEMORY_MAP_SDRAM_FILE_LIST_ADRS + (keySize + 1024);

	pFmIndexKeyFile		= (fmIndexKeyFile_t*)keyAdrs;
	pFmIndexImageFile	= (fmIndexImageFile_t*)imageAdrs;

	memset(pFmIndexKeyFile->u8Data, NULL, keySize);
	memset(pFmIndexImageFile->u8Data, NULL, imageSize);

	result = 1;

	return result;
}

u16 FM_IndexCrc16(u8 type, void *pVoid)
{
	u16	result = 0;
	u32	size;
	fmIndexKeyFile_t		*pKey;
	fmIndexImageFile_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:			//	KEY
			pKey = (fmIndexKeyFile_t*)pVoid;
			size = (u32)&pKey->crc16;
			size -= (u32)pKey;

			result = System_CalculateCRC16(pKey->u8Data, size);
			break;

		case FM_FILE_IMAGE:			//	IMAGE
			pImage = (fmIndexImageFile_t*)pVoid;
			size = (u32)&pImage->crc16;
			size -= (u32)pImage;

			result = System_CalculateCRC16(pImage->u8Data, size);
			break;
	}

	return result;
}

u8 FM_IndexWrite(u8 type, u16 no, void *pVoid)
{
	u8	result = 0;
	u32	adrs, size;
	fmIndexKey_t		*pKey;
	fmIndexImage_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:			//	KEY
			if(no >= FM_INDEX_KEY_LIST_MAX_COUNT)		break;
			pKey		= (fmIndexKey_t*)pVoid;
			adrs		= (u32)&pFmIndexKeyFile->key[no];
			pKey->fNo	= no;
			size		= sizeof(fmIndexKey_t);
			memcpy((u8*)adrs, (u8*)pKey, size);
			result = 1;
			break;

		case FM_FILE_IMAGE:			//	IMAGE
			if(no >= FM_INDEX_IMAGE_LIST_MAX_COUNT)		break;
			pImage		= (fmIndexImage_t*)pVoid;
			adrs		= (u32)&pFmIndexImageFile->image[no];
			pImage->fNo	= no;
			size		= sizeof(fmIndexImage_t);
			memcpy((u8*)adrs, (u8*)pImage, size);
			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexRead(u8 type, u16 no, void *pVoid)
{
	u8	result = 0;
	u32	adrs, size;
	fmIndexKey_t		*pKey;
	fmIndexImage_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:			//	KEY
			if(no >= FM_INDEX_KEY_LIST_MAX_COUNT)		break;
			pKey		= (fmIndexKey_t*)pVoid;
			adrs		= (u32)&pFmIndexKeyFile->key[no];
			size		= sizeof(fmIndexKey_t);
			memcpy((u8*)pKey, (u8*)adrs, size);
			result = 1;
			break;

		case FM_FILE_IMAGE:			//	IMAGE
			if(no >= FM_INDEX_IMAGE_LIST_MAX_COUNT)		break;
			pImage		= (fmIndexImage_t*)pVoid;
			adrs		= (u32)&pFmIndexImageFile->image[no];
			size		= sizeof(fmIndexImage_t);
			memcpy((u8*)pImage, (u8*)adrs, size);
			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexDataInit(u8 type, void *pVoid)
{
	u8	result = 0;
	u32	size;
	fmIndexKeyFile_t		*pKey;
	fmIndexImageFile_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:
			pKey	= (fmIndexKeyFile_t*)pVoid;
			size	= sizeof(fmIndexKeyFile_t);
			memset(pKey->u8Data, NULL, size);

			memcpy(pKey->header, "K_INDEX\r", 8);

			for(u16 cnt = 0; cnt < FM_INDEX_KEY_LIST_MAX_COUNT; cnt++)
			{
				memset(&fmIndexKey, NULL, sizeof(fmIndexKey));

				fmIndexKey.fNo = cnt;

				FM_IndexWrite(FM_FILE_KEY, cnt, &fmIndexKey);
			}
			result = 1;
			break;

		case FM_FILE_IMAGE:
			pImage	= (fmIndexImageFile_t*)pVoid;
			size 	= sizeof(fmIndexImageFile_t);
			memset(pImage->u8Data, NULL, size);

			memcpy(pImage->header, "I_INDEX\r", 8);

			for(u16 cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
			{
				memset(&fmIndexImage, NULL, sizeof(fmIndexImage));

				fmIndexImage.fNo = cnt;

				FM_IndexWrite(FM_FILE_IMAGE, cnt, &fmIndexImage);
			}
			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexDataAddCrc16(u8 type, void *pVoid)
{
	u8	result = 0;
	fmIndexKeyFile_t		*pKey;
	fmIndexImageFile_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:
			pKey			= (fmIndexKeyFile_t*)pVoid;
			pKey->crc16		= FM_IndexCrc16(FM_FILE_KEY, pKey);
			result = 1;
			break;

		case FM_FILE_IMAGE:
			pImage			= (fmIndexImageFile_t*)pVoid;
			pImage->crc16	= FM_IndexCrc16(FM_FILE_IMAGE, pImage);
			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexDataCheck(u8 type, void *pVoid)
{
	u8	result = 0;
	fmIndexKeyFile_t		*pKey;
	fmIndexImageFile_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:
			pKey	= (fmIndexKeyFile_t*)pVoid;
			if(strcmp((char*)pKey->header, "K_INDEX\r") != 0)					break;
			if(pKey->crc16 != FM_IndexCrc16(FM_FILE_KEY, pKey))				break;
			result = 1;
			break;

		case FM_FILE_IMAGE:
			pImage	= (fmIndexImageFile_t*)pVoid;
			if(strcmp((char*)pImage->header, "I_INDEX\r") != 0)					break;
			if(pImage->crc16 != FM_IndexCrc16(FM_FILE_IMAGE, pImage))		break;
			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexDirCheck()
{
	u8	result = 1, rtn;

	if(fmDrive == 0)
	{
		rtn = FatFS_DirOpen(&fmIndexDir, "0:/system");
	}
	else
	{
		rtn = FatFS_DirOpen(&fmIndexDir, "1:/system");
	}

	if(rtn == FR_NO_PATH)	result = 0;
	else if(rtn == FR_OK)	FatFS_DirClose(&fmIndexDir);
	else					result = 0xff;

	return result;
}

u8 FM_IndexDirMake()
{
	u8	result = 0, rtn;

	if(fmDrive == 0)
	{
		rtn = FatFS_DirMake("0:/system");
	}
	else
	{
		rtn = FatFS_DirMake("1:/system");
	}

	if(rtn == FR_OK)
	{
		result = FM_IndexDirCheck();
	}

	return result;
}

u8 FM_IndexFileCheck(u8 type)
{
	u8	result = 1, rtn;

	switch(type)
	{
		case FM_FILE_KEY:
			if(fmDrive == 0)
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "0:/system/key.kfl");
			}
			else
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "1:/system/key.kfl");
			}
			break;

		case FM_FILE_IMAGE:
			if(fmDrive == 0)
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "0:/system/image.ifl");
			}
			else
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "1:/system/image.ifl");
			}
			break;

		default:
			result = 0xff;
			return result;
			break;
	}

	if(rtn == FR_OK)			FatFS_FileClose(&fmIndexFile);
	else if(rtn == FR_NO_FILE)	result = 0;
	else						result = 0xff;

	return result;
}

u8 FM_IndexFileMake(u8 type)
{
	u8	result = 1, rtn;

	switch(type)
	{
		case FM_FILE_KEY:
			if(fmDrive == 0)
			{
				rtn = FatFS_FileMake(&fmIndexFile, "0:/system/key.kfl");
			}
			else
			{
				rtn = FatFS_FileMake(&fmIndexFile, "1:/system/key.kfl");
			}
			break;

		case FM_FILE_IMAGE:
			if(fmDrive == 0)
			{
				rtn = FatFS_FileMake(&fmIndexFile, "0:/system/image.ifl");
			}
			else
			{
				rtn = FatFS_FileMake(&fmIndexFile, "1:/system/image.ifl");
			}
			break;

		default:
			result = 0;
			return result;
			break;
	}

	if(rtn != FR_OK)	result = 0;

	return result;
}

u8 FM_IndexFileLoad(u8 type, void *pVoid)
{
	u8	result = 0, rtn;
	u32	size;
	fmIndexKeyFile_t		*pKey;
	fmIndexImageFile_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:
			pKey	= (fmIndexKeyFile_t*)pVoid;

			if(fmDrive == 0)
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "0:/system/key.kfl");
			}
			else
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "1:/system/key.kfl");
			}

			if(rtn != FR_OK)
			{
				break;
			}

			size = FatFS_FileSize(&fmIndexFile);

			if(size != sizeof(fmIndexKeyFile_t))
			{
				FatFS_FileClose(&fmIndexFile);
				result = 0xff;
				break;
			}

			rtn = FatFS_FilePtrWrite(&fmIndexFile, 0);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			rtn = FatFS_FileRead(&fmIndexFile, (u32)&pKey->u8Data[0], size);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			FatFS_FileClose(&fmIndexFile);

			result = 1;
			break;

		case FM_FILE_IMAGE:
			pImage	= (fmIndexImageFile_t*)pVoid;

			if(fmDrive == 0)
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "0:/system/image.ifl");
			}
			else
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "1:/system/image.ifl");
			}

			if(rtn != FR_OK)
			{
				break;
			}

			size = FatFS_FileSize(&fmIndexFile);

			if(size != sizeof(fmIndexImageFile_t))
			{
				FatFS_FileClose(&fmIndexFile);
				result = 0xff;
				break;
			}

			rtn = FatFS_FilePtrWrite(&fmIndexFile, 0);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			rtn = FatFS_FileRead(&fmIndexFile, (u32)&pImage->u8Data[0], size);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			FatFS_FileClose(&fmIndexFile);

			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexFileSave(u8 type, void *pVoid)
{
	u8	result = 0, rtn;
	u32	size;
	fmIndexKeyFile_t		*pKey;
	fmIndexImageFile_t		*pImage;

	switch(type)
	{
		case FM_FILE_KEY:
			pKey	= (fmIndexKeyFile_t*)pVoid;

			FM_IndexDataAddCrc16(FM_FILE_KEY, pKey);

			if(fmDrive == 0)
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "0:/system/key.kfl");
			}
			else
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "1:/system/key.kfl");
			}

			if(rtn != FR_OK)
			{
				break;
			}

			size = sizeof(fmIndexKeyFile_t);

			rtn = FatFS_FilePtrWrite(&fmIndexFile, 0);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			rtn = FatFS_FileWrite(&fmIndexFile, (u8*)&pKey->u8Data[0], size);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			FatFS_FileClose(&fmIndexFile);

			result = 1;
			break;

		case FM_FILE_IMAGE:
			pImage	= (fmIndexImageFile_t*)pVoid;

			FM_IndexDataAddCrc16(FM_FILE_IMAGE, pImage);

			if(fmDrive == 0)
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "0:/system/image.ifl");
			}
			else
			{
				rtn = FatFS_FileOpen(&fmIndexFile, "1:/system/image.ifl");
			}

			if(rtn != FR_OK)
			{
				break;
			}

			size = sizeof(fmIndexImageFile_t);

			rtn = FatFS_FilePtrWrite(&fmIndexFile, 0);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			rtn = FatFS_FileWrite(&fmIndexFile, (u8*)&pImage->u8Data[0], size);
			if(rtn != FR_OK)
			{
				FatFS_FileClose(&fmIndexFile);
				break;
			}

			FatFS_FileClose(&fmIndexFile);

			result = 1;
			break;
	}

	return result;
}

u8 FM_IndexListVerify(u8 type)
{
	u8	result = 0, rtn;

	switch(type)
	{
		case FM_FILE_KEY:
			for(u16 cnt = 0; cnt < FM_INDEX_KEY_LIST_MAX_COUNT; cnt++)
			{
				FM_IndexRead(FM_FILE_KEY, cnt, &fmIndexKey);

				if(fmIndexKey.fState == 0)
				{
					continue;
				}

				rtn = FatFS_FileOpen(&fmIndexFile, fmIndexKey.fName);

				if(rtn == FR_OK)
				{
					FatFS_FileClose(&fmIndexFile);

					continue;
				}

				FatFS_FileClose(&fmIndexFile);

				memset(&fmIndexKey, NULL, sizeof(fmIndexKey_t));

				fmIndexKey.fNo = cnt;

				FM_IndexWrite(FM_FILE_KEY, cnt, &fmIndexKey);

				result = 1;
			}
			break;

		case FM_FILE_IMAGE:
			for(u16 cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
			{
				FM_IndexRead(FM_FILE_IMAGE, cnt, &fmIndexImage);

				if(fmIndexImage.fState == 0)
				{
					continue;
				}

				rtn = FatFS_FileOpen(&fmIndexFile, fmIndexImage.fName);

				if(rtn == FR_OK)
				{
					FatFS_FileClose(&fmIndexFile);

					continue;
				}

				FatFS_FileClose(&fmIndexFile);

				memset(&fmIndexImage, NULL, sizeof(fmIndexImage_t));

				fmIndexImage.fNo = cnt;

				FM_IndexWrite(FM_FILE_IMAGE, cnt, &fmIndexImage);

				result = 1;
			}
			break;
	}

	return result;
}

u8 FM_IndexInit()
{
	u8	result = 0, rtn, save = 0;

	fmDrive			= 0;

//	Index File Variable Init
	FM_IndexFilePathInit();

//	Folder Check
	rtn = FM_IndexDirCheck();
	if(rtn == 0)		// NO FOLDER
	{
		rtn = FM_IndexDirMake();
		if(rtn == 0)
		{
			//	Drive ERROR
			return result;
		}
	}
	else if(rtn == 0xff)
	{
		//	Drive ERROR
		return result;
	}

/*
	KEY
*/
	save = 0;
//	File Check
	rtn = FM_IndexFileCheck(FM_FILE_KEY);
	if(rtn == 0)
	{
		rtn = FM_IndexFileMake(FM_FILE_KEY);
		if(rtn == 0)
		{
			//	Drive ERROR
			return result;
		}
	}
	else if(rtn == 0xff)
	{
		//	Drive ERROR
		return result;
	}

//	File Load
	rtn = FM_IndexFileLoad(FM_FILE_KEY, pFmIndexKeyFile);
	if(rtn == 0)
	{
		//	Drive ERROR
		return result;
	}
	else if(rtn == 0xff)
	{
		FM_IndexDataInit(FM_FILE_KEY, pFmIndexKeyFile);

		save = 1;
	}

//	File Check
	rtn = FM_IndexDataCheck(FM_FILE_KEY, pFmIndexKeyFile);
	if(rtn == 0)
	{
		//	File Init
		FM_IndexDataInit(FM_FILE_KEY, pFmIndexKeyFile);

		save = 1;
	}

//	File List Verify
	rtn = FM_IndexListVerify(FM_FILE_KEY);
	if((rtn == 1) || (save == 1))
	{
		rtn = FM_IndexFileSave(FM_FILE_KEY, pFmIndexKeyFile);
		if(rtn == 0)
		{
			//	Drive ERROR
			return result;
		}
	}

/*
	IMAGE
*/
	save = 0;
//	File Check
	rtn = FM_IndexFileCheck(FM_FILE_IMAGE);
	if(rtn == 0)
	{
		rtn = FM_IndexFileMake(FM_FILE_IMAGE);
		if(rtn == 0)
		{
			//	Drive ERROR
			return result;
		}
	}
	else if(rtn == 0xff)
	{
		//	Drive ERROR
		return result;
	}

//	File Load
	rtn = FM_IndexFileLoad(FM_FILE_IMAGE, pFmIndexImageFile);
	if(rtn == 0)
	{
		//	Drive ERROR
		return result;
	}
	else if(rtn == 0xff)
	{
		FM_IndexDataInit(FM_FILE_IMAGE, pFmIndexImageFile);

		save = 1;
	}

//	File Check
	rtn = FM_IndexDataCheck(FM_FILE_IMAGE, pFmIndexImageFile);
	if(rtn == 0)
	{
		//	File Init
		FM_IndexDataInit(FM_FILE_IMAGE, pFmIndexImageFile);

		save = 1;
	}

//	File List Verify
	rtn = FM_IndexListVerify(FM_FILE_IMAGE);
	if((rtn == 1) || (save == 1))
	{
		rtn = FM_IndexFileSave(FM_FILE_IMAGE, pFmIndexImageFile);
		if(rtn == 0)
		{
			//	Drive ERROR
			return result;
		}
	}

	result = 1;

	return result;
}

u8 FM_ImageIndexSort()
{
 	u8	result = 0;
	u16	cnt = 0;

	for(cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
	{
		FM_IndexRead(FM_FILE_IMAGE, cnt, &fmIndexImage);

		if(fmIndexImage.fNo == cnt)			continue;

		FM_IndexWrite(FM_FILE_IMAGE, cnt, &fmIndexImage);
	}

	result = 1;

	return result;
}

u8 FM_ImageIndexArrange()
{
	u8	result = 0;
	u16	cnt, indexCnt = 0;

	for(cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
	{
		FM_IndexRead(FM_FILE_IMAGE, cnt, &fmIndexImage);

		if(fmIndexImage.fState != 0)
		{
			FM_IndexWrite(FM_FILE_IMAGE, indexCnt++, &fmIndexImage);
		}
	}

	for(cnt = 0; cnt < FM_INDEX_IMAGE_LIST_MAX_COUNT; cnt++)
	{
		FM_IndexRead(FM_FILE_IMAGE, cnt, &fmIndexImage);

		if(fmIndexImage.fState == 0)
		{
			memset(&fmIndexImage, NULL, sizeof(fmIndexImage));
			FM_IndexWrite(FM_FILE_IMAGE, indexCnt++, &fmIndexImage);
		}
	}

	result = 1;

	return result;
}

u8 FM_IndexSave(u8 type)
{
	u8	result = 0;

	switch(type)
	{
		case FM_FILE_KEY:
			result = FM_IndexFileSave(FM_FILE_KEY, pFmIndexKeyFile);
			break;

		case FM_FILE_IMAGE:
			result = FM_IndexFileSave(FM_FILE_IMAGE, pFmIndexImageFile);
			break;
	}

	return result;
}

