#include	"App_EDID.h"

u8 EDID_Check_Header(void *pVoid)
{
	u8	result = 0;
	u8	buffer[10] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00};
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	if(memcmp(&pEDID->Header[0], buffer, 8) == NULL)	result = 1;

	return result;
}

u8 EDID_Get_IdManufacturerName(void *pVoid, u8 *pData)
{
	u8	result = 0;
	u16	data;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	data	= pEDID->ID_Manufacturer_Name[0];
	data	<<= 8;
	data	|= pEDID->ID_Manufacturer_Name[1];

	if((data & 0x8000) != 0)		return result;

	pData[0] = ((data >> 10) & 0x001F) + 'A';
	pData[1] = ((data >> 5) & 0x001F) + 'A';
	pData[2] = ((data >> 0) & 0x001F) + 'A';

	result = 1;

	return result;
}

u8 EDID_Get_IdProductCode(void *pVoid, u16 *pData)
{
	u8	result = 0;
	u16	data;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	data	= pEDID->ID_Product_Code[1];
	data	<<= 8;
	data	|= pEDID->ID_Product_Code[0];

	*pData = data;

	result = 1;

	return result;
}

u8 EDID_Get_IdSerialNumber(void *pVoid, u32 *pData)
{
	u8	result = 0;
	u32	data;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	data	= pEDID->ID_Serial_Number[3];
	data	<<= 8;
	data	|= pEDID->ID_Serial_Number[2];
	data	<<= 8;
	data	|= pEDID->ID_Serial_Number[1];
	data	<<= 8;
	data	|= pEDID->ID_Serial_Number[0];

	*pData = data;

	result = 1;

	return result;
}

u8 EDID_Get_ManufactureDate(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	edid_t	*pEDID;
	edid_ManufactureDate_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_ManufactureDate_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_ManufactureDate_t));

	pOutput->Week	= pEDID->Week_of_Manufacture;
	pOutput->Year	= pEDID->Year_of_Manufacture;

	if(pOutput->Week == 0xFF)	pOutput->Mode = 1;

	result = 1;

	return result;
}

u8 EDID_Get_VersionAndRevision(void *pVoid, u8 *pData)
{
	u8	result = 0;
	u8	data;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	data	= pEDID->Version_Number & 0x0F;
	data	<<= 4;
	data	|= pEDID->Revision_Number & 0x0F;

	*pData	= data;

	result = 1;

	return result;
}

u8 EDID_Get_VideoInputDefinition(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	u8	data;
	edid_t	*pEDID;
	edid_VideoInputDefinition_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_VideoInputDefinition_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_VideoInputDefinition_t));

	data = pEDID->Video_Input_Definition;

	if((data & 0x80) == 0)		//	Analog
	{
		pOutput->Video_Signal_Interface = 0;

		pOutput->Signal_Level_Standard						= (data >> 5) & 0x03;
		pOutput->Video_Setup								= (data >> 4) & 0x01;
		pOutput->Synchronization_Types						= (data >> 1) & 0x07;
		pOutput->Serrations									= (data >> 0) & 0x01;
	}
	else						//	Digital
	{
		pOutput->Video_Signal_Interface = 1;

		pOutput->Color_Bit_Depth							= (data >> 4) & 0x07;
		pOutput->Digital_Video_Interface_Standard_Supported	= (data >> 0) & 0x0F;
	}

	result = 1;

	return result;
}

u8 EDID_Get_HVScreenSizeOrAspectRatio(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	edid_t	*pEDID;
	edid_HVSizeOrAspectRatio_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_HVSizeOrAspectRatio_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_HVSizeOrAspectRatio_t));

	if((pEDID->Horizontal_Screen_Size == 0) && (pEDID->Vertical_Screen_Size == 0))
	{
		//	Unknown or Undefined

		pOutput->Mode	= 0;
	}
	if(pEDID->Horizontal_Screen_Size == 0)
	{
		float	gain, offset;

		//	Aspect Ratio, Portrait

		pOutput->Mode	= 1;

		gain = (3.54 - 1) / (255 - 1);
		offset	= (gain * 1) - 1;

		pOutput->Horizontal_Ratio	= ((float)pEDID->Horizontal_Screen_Size * gain) - offset;
		pOutput->Vertical_Ratio		= 1;
	}
	else if(pEDID->Vertical_Screen_Size == 0)
	{
		float	gain, offset;

		//	Aspect Ratio, Landscape

		pOutput->Mode				= 2;

		gain = (0.99 - 0.28) / (255 - 1);
		offset	= (gain * 1) - 0.28;

		pOutput->Horizontal_Ratio	= ((float)pEDID->Vertical_Screen_Size * gain) - offset;
		pOutput->Vertical_Ratio		= 1;
	}
	else
	{
		pOutput->Mode				= 3;

		pOutput->Horizontal_Size		= pEDID->Horizontal_Screen_Size;
		pOutput->Vertical_Size		= pEDID->Vertical_Screen_Size;
	}

	result = 1;

	return result;
}

u8 EDID_Get_DisplayTransferCharacteristics(void *pVoid, u8 *pData)
{
	u8	result = 0;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	if(pEDID->Display_Transfer_Characteristic == 0)		return result;

	*pData = pEDID->Display_Transfer_Characteristic;

	result = 1;

	return result;
}

u8 EDID_Get_FeatureSupport(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	u8	data;
	edid_t	*pEDID;
	edid_FeatureSupport_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_FeatureSupport_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_FeatureSupport_t));

	data = pEDID->Feature_Support;

	pOutput->Standby_Mode					= (data >> 7) & 0x01;
	pOutput->Suspend_Mode					= (data >> 6) & 0x01;
	pOutput->Active_Off						= (data >> 5) & 0x01;
	pOutput->Display_Color_Type				= (data >> 3) & 0x03;
	pOutput->sRGB_Standard					= (data >> 2) & 0x01;
	pOutput->Preferred_Timing_Mode			= (data >> 1) & 0x01;
	pOutput->Display_Continuous_Frequency	= (data >> 0) & 0x01;

	result = 1;

	return result;
}

u8 EDID_Get_ColorCharacteristics(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	edid_t	*pEDID;
	edid_ColorCharacteristics_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_ColorCharacteristics_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_ColorCharacteristics_t));

	pOutput->Red_x		= pEDID->Red_x;
	pOutput->Red_x		<<= 2;
	pOutput->Red_x		|= (pEDID->Red_Green >> 6) & 0x03;
	
	pOutput->Red_y		= pEDID->Red_y;
	pOutput->Red_y		<<= 2;
	pOutput->Red_y		|= (pEDID->Red_Green >> 4) & 0x03;

	pOutput->Green_x	= pEDID->Green_x;
	pOutput->Green_x	<<= 2;
	pOutput->Green_x	|= (pEDID->Red_Green >> 2) & 0x03;
	
	pOutput->Green_y	= pEDID->Green_y;
	pOutput->Green_y	<<= 2;
	pOutput->Green_y	|= (pEDID->Red_Green >> 0) & 0x03;

	pOutput->Blue_x		= pEDID->Blue_x;
	pOutput->Blue_x		<<= 2;
	pOutput->Blue_x		|= (pEDID->Blue_White >> 6) & 0x03;
	
	pOutput->Blue_y		= pEDID->Blue_y;
	pOutput->Blue_y		<<= 2;
	pOutput->Blue_y		|= (pEDID->Blue_White >> 4) & 0x03;

	pOutput->White_x	= pEDID->White_x;
	pOutput->White_x	<<= 2;
	pOutput->White_x	|= (pEDID->Blue_White >> 2) & 0x03;
	
	pOutput->White_y	= pEDID->White_y;
	pOutput->White_y	<<= 2;
	pOutput->White_y	|= (pEDID->Blue_White >> 0) & 0x03;

	result = 1;

	return result;
}

u8 EDID_Get_EstablishedTimings(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	edid_t	*pEDID;
	edid_EstablishedTimings_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_EstablishedTimings_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_EstablishedTimings_t));

	pOutput->data	= pEDID->Established_Timings_I;
	pOutput->data	<<= 8;
	pOutput->data	|= pEDID->Established_Timings_II;
	pOutput->data	<<= 8;
	pOutput->data	|= pEDID->Manufacturers_Reserved_Timings;

	result = 1;

	return result;
}

u8 EDID_Get_StandardTimings(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	edid_t	*pEDID;
	edid_StandardTimings_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_StandardTimings_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_StandardTimings_t));

	for(u8 cnt = 0; cnt < 8; cnt++)
	{
		pOutput->Value_Stored[cnt]					= pEDID->Standard_Timing[cnt * 2];
		pOutput->Image_Aspect_Ratio[cnt]			= (pEDID->Standard_Timing[(cnt * 2) + 1] >> 6) & 0x03;
		pOutput->Field_Refresh_Rate[cnt]			= pEDID->Standard_Timing[(cnt * 2) + 1] & 0x3F;

		pOutput->Horizontal_Addressable_Pixels[cnt]	= (pOutput->Value_Stored[cnt] + 31) * 8;

		if(pOutput->Image_Aspect_Ratio[cnt] == 0)
		{
			//	16:10

			pOutput->Vertical_Pixels[cnt] = (pOutput->Horizontal_Addressable_Pixels[cnt] / 16) * 10;
		}
		else if(pOutput->Image_Aspect_Ratio[cnt] == 1)
		{
			//	4:3

			pOutput->Vertical_Pixels[cnt] = (pOutput->Horizontal_Addressable_Pixels[cnt] / 4) * 3;
		}
		else if(pOutput->Image_Aspect_Ratio[cnt] == 2)
		{
			//	5:4

			pOutput->Vertical_Pixels[cnt] = (pOutput->Horizontal_Addressable_Pixels[cnt] / 5) * 4;
		}
		else if(pOutput->Image_Aspect_Ratio[cnt] == 3)
		{
			//	16:9

			pOutput->Vertical_Pixels[cnt] = (pOutput->Horizontal_Addressable_Pixels[cnt] / 16) * 9;
		}
	}

	result = 1;

	return result;
}

u8 EDID_Get_18ByteDataBlocks(void *pVoid, void *pvOutput)
{
	u8	result = 0;
	edid_t	*pEDID;
	edid_18ByteDataBlocks_t	*pOutput;

	pEDID = (edid_t*)pVoid;

	pOutput = (edid_18ByteDataBlocks_t*)pvOutput;

	memset(pOutput, NULL, sizeof(edid_18ByteDataBlocks_t));

//	Preferred Timing Mode

	pOutput->PreferredTimingMode.Use = 1;

	pOutput->PreferredTimingMode.Pixel_Clock									= pEDID->Preferred_Timing_Mode[1];
	pOutput->PreferredTimingMode.Pixel_Clock									<<= 8;
	pOutput->PreferredTimingMode.Pixel_Clock									|= pEDID->Preferred_Timing_Mode[0];
	pOutput->PreferredTimingMode.Pixel_Clock									*= 10000;

	pOutput->PreferredTimingMode.Horizontal_Addressable_Video_in_pixels			= (pEDID->Preferred_Timing_Mode[4] >> 4) & 0x0f;
	pOutput->PreferredTimingMode.Horizontal_Blanking_in_pixels					= (pEDID->Preferred_Timing_Mode[4] >> 0) & 0x0f;
	pOutput->PreferredTimingMode.Horizontal_Addressable_Video_in_pixels			<<= 8;
	pOutput->PreferredTimingMode.Horizontal_Blanking_in_pixels					<<= 8;
	pOutput->PreferredTimingMode.Horizontal_Addressable_Video_in_pixels			|= pEDID->Preferred_Timing_Mode[2];
	pOutput->PreferredTimingMode.Horizontal_Blanking_in_pixels					|= pEDID->Preferred_Timing_Mode[3];

	pOutput->PreferredTimingMode.Vertical_Addressable_Video_in_lines			= (pEDID->Preferred_Timing_Mode[7] >> 4) & 0x0f;
	pOutput->PreferredTimingMode.Vertical_Blanking_in_lines						= (pEDID->Preferred_Timing_Mode[7] >> 0) & 0x0f;
	pOutput->PreferredTimingMode.Vertical_Addressable_Video_in_lines			<<= 8;
	pOutput->PreferredTimingMode.Vertical_Blanking_in_lines						<<= 8;
	pOutput->PreferredTimingMode.Vertical_Addressable_Video_in_lines			|= pEDID->Preferred_Timing_Mode[5];
	pOutput->PreferredTimingMode.Vertical_Blanking_in_lines						|= pEDID->Preferred_Timing_Mode[6];

	pOutput->PreferredTimingMode.Horizontal_Front_Porch_in_pixels				= (pEDID->Preferred_Timing_Mode[11] >> 6) & 0x03;
	pOutput->PreferredTimingMode.Horizontal_Sync_Pulse_Width_in_pixels			= (pEDID->Preferred_Timing_Mode[11] >> 4) & 0x03;
	pOutput->PreferredTimingMode.Vertical_Front_Porch_in_pixels					= (pEDID->Preferred_Timing_Mode[11] >> 2) & 0x03;
	pOutput->PreferredTimingMode.Vertical_Sync_Pulse_Width_in_pixels			= (pEDID->Preferred_Timing_Mode[11] >> 0) & 0x03;
	pOutput->PreferredTimingMode.Horizontal_Front_Porch_in_pixels				<<= 8;
	pOutput->PreferredTimingMode.Horizontal_Sync_Pulse_Width_in_pixels			<<= 8;
	pOutput->PreferredTimingMode.Vertical_Front_Porch_in_pixels					<<= 4;
	pOutput->PreferredTimingMode.Vertical_Sync_Pulse_Width_in_pixels			<<= 4;
	pOutput->PreferredTimingMode.Horizontal_Front_Porch_in_pixels				|= pEDID->Preferred_Timing_Mode[8];
	pOutput->PreferredTimingMode.Horizontal_Sync_Pulse_Width_in_pixels			|= pEDID->Preferred_Timing_Mode[9];
	pOutput->PreferredTimingMode.Vertical_Front_Porch_in_pixels					|= (pEDID->Preferred_Timing_Mode[10] >> 4) & 0x0f;
	pOutput->PreferredTimingMode.Vertical_Sync_Pulse_Width_in_pixels			|= (pEDID->Preferred_Timing_Mode[10] >> 0) & 0x0f;

	pOutput->PreferredTimingMode.Horizontal_Addressable_Video_Image_Size_in_mm	= (pEDID->Preferred_Timing_Mode[14] >> 4) & 0x0f;
	pOutput->PreferredTimingMode.Vertical_Addressable_Video_Image_Size_in_mm	= (pEDID->Preferred_Timing_Mode[14] >> 0) & 0x0f;
	pOutput->PreferredTimingMode.Horizontal_Addressable_Video_Image_Size_in_mm	<<= 8;
	pOutput->PreferredTimingMode.Vertical_Addressable_Video_Image_Size_in_mm	<<= 8;
	pOutput->PreferredTimingMode.Horizontal_Addressable_Video_Image_Size_in_mm	|= pEDID->Preferred_Timing_Mode[12];
	pOutput->PreferredTimingMode.Vertical_Addressable_Video_Image_Size_in_mm	|= pEDID->Preferred_Timing_Mode[13];

	pOutput->PreferredTimingMode.Horizontal_Border_in_pixels					= pEDID->Preferred_Timing_Mode[15];
	pOutput->PreferredTimingMode.Vertical_Border_in_lines						= pEDID->Preferred_Timing_Mode[16];

	pOutput->PreferredTimingMode.Signal_Interface_Type							= (pEDID->Preferred_Timing_Mode[17] >> 7) & 0x01;
	pOutput->PreferredTimingMode.Stereo_Viewing_Support							= (pEDID->Preferred_Timing_Mode[17] >> 4) & 0x06;
	pOutput->PreferredTimingMode.Stereo_Viewing_Support							|= (pEDID->Preferred_Timing_Mode[17] >> 0) & 0x01;
	pOutput->PreferredTimingMode.Signal_Interface_Type							= (pEDID->Preferred_Timing_Mode[17] >> 1) & 0x0f;

	for(u8 cnt = 0; cnt < 3; cnt++)
	{
		if(	(pEDID->Detailed_Timing[cnt][0] == 0) && 
			(pEDID->Detailed_Timing[cnt][1] == 0) && 
			(pEDID->Detailed_Timing[cnt][2] == 0) && 
			(pEDID->Detailed_Timing[cnt][4] == 0))
		{
			if(pEDID->Detailed_Timing[cnt][3] == 0xFF)
			{
				//	Display Product Serial Number
				pOutput->DisplayProductSerialNumber.Use = 1;

				memcpy(pOutput->DisplayProductSerialNumber.String_Data, &pEDID->Detailed_Timing[cnt][5], 13);
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xFE)
			{
				//	Alphanumeric Data String(ASCII)
				pOutput->AlphanumericDataString.Use = 1;

				memcpy(pOutput->AlphanumericDataString.String_Data, &pEDID->Detailed_Timing[cnt][5], 13);
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xFD)
			{
				//	Display Range Limits
				pOutput->DisplayRangeLimits.Use = 1;

				pOutput->DisplayRangeLimits.Vertical_Rate_Offset		= (pEDID->Detailed_Timing[cnt][4] >> 0) & 0x03;
				pOutput->DisplayRangeLimits.Horizontal_Rate_Offset		= (pEDID->Detailed_Timing[cnt][4] >> 2) & 0x03;
				pOutput->DisplayRangeLimits.Minimum_Vertical_Rate		= pEDID->Detailed_Timing[cnt][5];
				if(pOutput->DisplayRangeLimits.Minimum_Vertical_Rate != 0)
				{
					if(pOutput->DisplayRangeLimits.Vertical_Rate_Offset & 0x01)		pOutput->DisplayRangeLimits.Minimum_Vertical_Rate += 255;
				}
				pOutput->DisplayRangeLimits.Maximum_Vertical_Rate		= pEDID->Detailed_Timing[cnt][6];
				if(pOutput->DisplayRangeLimits.Maximum_Vertical_Rate != 0)
				{
					if(pOutput->DisplayRangeLimits.Vertical_Rate_Offset & 0x02)		pOutput->DisplayRangeLimits.Maximum_Vertical_Rate += 255;
				}
				pOutput->DisplayRangeLimits.Minimum_Horizontal_Rate		= pEDID->Detailed_Timing[cnt][7];
				if(pOutput->DisplayRangeLimits.Minimum_Horizontal_Rate != 0)
				{
					if(pOutput->DisplayRangeLimits.Horizontal_Rate_Offset & 0x01)	pOutput->DisplayRangeLimits.Minimum_Horizontal_Rate += 255;
				}
				pOutput->DisplayRangeLimits.Maximum_Horizontal_Rate		= pEDID->Detailed_Timing[cnt][8];
				if(pOutput->DisplayRangeLimits.Maximum_Horizontal_Rate != 0)
				{
					if(pOutput->DisplayRangeLimits.Horizontal_Rate_Offset & 0x01)	pOutput->DisplayRangeLimits.Maximum_Horizontal_Rate += 255;
				}
				pOutput->DisplayRangeLimits.Maximum_Pixel_Clock			= pEDID->Detailed_Timing[cnt][9];

				pOutput->DisplayRangeLimits.Video_Timing_Support_Flags	= pEDID->Detailed_Timing[cnt][10];

				if(pOutput->DisplayRangeLimits.Video_Timing_Support_Flags == 0x02)
				{
					//	GTF Secondary Curve Block
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.Use					= 1;

					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.StartBreakFrequency	= pEDID->Detailed_Timing[cnt][12];
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.C					= pEDID->Detailed_Timing[cnt][13];
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.M					= pEDID->Detailed_Timing[cnt][15];
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.M					<<= 8;
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.M					|= pEDID->Detailed_Timing[cnt][14];
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.K					= pEDID->Detailed_Timing[cnt][16];
					pOutput->DisplayRangeLimits.GTFSecondaryCurveBlock.J					= pEDID->Detailed_Timing[cnt][17];
				}
				else if(pOutput->DisplayRangeLimits.Video_Timing_Support_Flags == 0x04)
				{
					//	CVT Support
					pOutput->DisplayRangeLimits.CVTSupport.Use									= 1;

					pOutput->DisplayRangeLimits.CVTSupport.CVT_Standard_Version_Number			= pEDID->Detailed_Timing[cnt][11];
					pOutput->DisplayRangeLimits.CVTSupport.Additional_Pixel_Clock_Precision		= (pEDID->Detailed_Timing[cnt][12] >> 2) & 0x3f;
					pOutput->DisplayRangeLimits.CVTSupport.Maximum_Active_Pixel_per_Line		= pEDID->Detailed_Timing[cnt][12] & 0x03;
					pOutput->DisplayRangeLimits.CVTSupport.Maximum_Active_Pixel_per_Line		<<= 8;
					pOutput->DisplayRangeLimits.CVTSupport.Maximum_Active_Pixel_per_Line		|= pEDID->Detailed_Timing[cnt][13];
					pOutput->DisplayRangeLimits.CVTSupport.Supported_Aspect_Ratios				= pEDID->Detailed_Timing[cnt][14];
					pOutput->DisplayRangeLimits.CVTSupport.Preferred_Aspect_Ratio				= ((pEDID->Detailed_Timing[cnt][15] >> 2) & 0x38) | (pEDID->Detailed_Timing[cnt][15] & 0x07);
					pOutput->DisplayRangeLimits.CVTSupport.CVT_Blanking_Support					= (pEDID->Detailed_Timing[cnt][15] >> 3) & 0x03;
					pOutput->DisplayRangeLimits.CVTSupport.Type_of_Display_Scaling_Supported	= pEDID->Detailed_Timing[cnt][16];
					pOutput->DisplayRangeLimits.CVTSupport.Preferred_Vertical_Refresh_Rate		= pEDID->Detailed_Timing[cnt][17];
				}
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xFC)
			{
				//	Display Product Name
				pOutput->DisplayProductName.Use		= 1;

				memcpy(pOutput->DisplayProductName.String_Data, &pEDID->Detailed_Timing[cnt][5], 13);
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xFB)
			{
				//	Color Point Data
				pOutput->ColorPointData.Use							= 1;

				pOutput->ColorPointData.White_Point_Index_Number_1	= pEDID->Detailed_Timing[cnt][5];
				pOutput->ColorPointData.White_x_1					= (pEDID->Detailed_Timing[cnt][6] >> 2) & 0x03;
				pOutput->ColorPointData.White_x_1					<<= 8;
				pOutput->ColorPointData.White_x_1					|= pEDID->Detailed_Timing[cnt][7];
				pOutput->ColorPointData.White_y_1					= (pEDID->Detailed_Timing[cnt][6] >> 0) & 0x03;
				pOutput->ColorPointData.White_y_1					<<= 8;
				pOutput->ColorPointData.White_y_1					|= pEDID->Detailed_Timing[cnt][8];
				pOutput->ColorPointData.Gamma_1						= pEDID->Detailed_Timing[cnt][9];

				pOutput->ColorPointData.White_Point_Index_Number_2	= pEDID->Detailed_Timing[cnt][10];
				pOutput->ColorPointData.White_x_2					= (pEDID->Detailed_Timing[cnt][11] >> 2) & 0x03;
				pOutput->ColorPointData.White_x_2					<<= 8;
				pOutput->ColorPointData.White_x_2					|= pEDID->Detailed_Timing[cnt][12];
				pOutput->ColorPointData.White_y_2					= (pEDID->Detailed_Timing[cnt][11] >> 0) & 0x03;
				pOutput->ColorPointData.White_y_2					<<= 8;
				pOutput->ColorPointData.White_y_2					|= pEDID->Detailed_Timing[cnt][13];
				pOutput->ColorPointData.Gamma_2						= pEDID->Detailed_Timing[cnt][14];
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xFA)
			{
				//	Standard Timing Identifications
				pOutput->StandardTimings.Use			= 1;

				for(u8 timingCnt = 0; timingCnt < 6; timingCnt++)
				{
					pOutput->StandardTimings.Value_Stored[timingCnt]					= pEDID->Detailed_Timing[cnt][5 + (timingCnt * 2)];
					pOutput->StandardTimings.Image_Aspect_Ratio[timingCnt]				= (pEDID->Detailed_Timing[cnt][5 + (timingCnt * 2) + 1] >> 6) & 0x03;
					pOutput->StandardTimings.Field_Refresh_Rate[timingCnt]				= (pEDID->Detailed_Timing[cnt][5 + (timingCnt * 2) + 1] >> 6) & 0x3F;

					pOutput->StandardTimings.Horizontal_Addressable_Pixels[timingCnt]	= (pOutput->StandardTimings.Value_Stored[timingCnt] + 31) * 8;

					if(pOutput->StandardTimings.Image_Aspect_Ratio[timingCnt] == 0)
					{
						//	16:10

						pOutput->StandardTimings.Vertical_Pixels[timingCnt]				= (pOutput->StandardTimings.Horizontal_Addressable_Pixels[timingCnt] / 16) * 10;
					}
					else if(pOutput->StandardTimings.Image_Aspect_Ratio[timingCnt] == 1)
					{
						//	4:3

						pOutput->StandardTimings.Vertical_Pixels[timingCnt]				= (pOutput->StandardTimings.Horizontal_Addressable_Pixels[timingCnt] / 4) * 3;
					}
					else if(pOutput->StandardTimings.Image_Aspect_Ratio[timingCnt] == 2)
					{
						//	5:4

						pOutput->StandardTimings.Vertical_Pixels[timingCnt]				= (pOutput->StandardTimings.Horizontal_Addressable_Pixels[timingCnt] / 5) * 4;
					}
					else if(pOutput->StandardTimings.Image_Aspect_Ratio[timingCnt] == 3)
					{
						//	16:9

						pOutput->StandardTimings.Vertical_Pixels[timingCnt]				= (pOutput->StandardTimings.Horizontal_Addressable_Pixels[timingCnt] / 16) * 9;
					}
				}
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xF9)
			{
				//	Display Color Management Data
				pOutput->ColorManagementData.Use		= 1;

				pOutput->ColorManagementData.Version	= pEDID->Detailed_Timing[cnt][5];

				pOutput->ColorManagementData.Red_a3		= pEDID->Detailed_Timing[cnt][7];
				pOutput->ColorManagementData.Red_a3		<<= 8;
				pOutput->ColorManagementData.Red_a3		|= pEDID->Detailed_Timing[cnt][6];
				pOutput->ColorManagementData.Red_a2		= pEDID->Detailed_Timing[cnt][9];
				pOutput->ColorManagementData.Red_a2		<<= 8;
				pOutput->ColorManagementData.Red_a2		|= pEDID->Detailed_Timing[cnt][8];

				pOutput->ColorManagementData.Green_a3	= pEDID->Detailed_Timing[cnt][11];
				pOutput->ColorManagementData.Green_a3	<<= 8;
				pOutput->ColorManagementData.Green_a3	|= pEDID->Detailed_Timing[cnt][10];
				pOutput->ColorManagementData.Green_a2	= pEDID->Detailed_Timing[cnt][13];
				pOutput->ColorManagementData.Green_a2	<<= 8;
				pOutput->ColorManagementData.Green_a2	|= pEDID->Detailed_Timing[cnt][12];

				pOutput->ColorManagementData.Blue_a3	= pEDID->Detailed_Timing[cnt][15];
				pOutput->ColorManagementData.Blue_a3	<<= 8;
				pOutput->ColorManagementData.Blue_a3	|= pEDID->Detailed_Timing[cnt][14];
				pOutput->ColorManagementData.Blue_a2	= pEDID->Detailed_Timing[cnt][17];
				pOutput->ColorManagementData.Blue_a2	<<= 8;
				pOutput->ColorManagementData.Blue_a2	|= pEDID->Detailed_Timing[cnt][16];
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xF8)
			{
				//	CVT 3 Byte Timing Codes
				pOutput->CVT3ByteTimingCode.Use			= 1;

				pOutput->CVT3ByteTimingCode.Version		= pEDID->Detailed_Timing[cnt][5];

				for(u8 descriptCnt = 0; descriptCnt < 4; descriptCnt++)
				{
					pOutput->CVT3ByteTimingCode.Addressable_Lines[descriptCnt]			= (pEDID->Detailed_Timing[cnt][6 + (descriptCnt * 3) + 1] >> 4) & 0x0F;
					pOutput->CVT3ByteTimingCode.Addressable_Lines[descriptCnt]			<<= 8;
					pOutput->CVT3ByteTimingCode.Addressable_Lines[descriptCnt]			|= pEDID->Detailed_Timing[cnt][6 + (descriptCnt * 3) + 0];
					pOutput->CVT3ByteTimingCode.Aspect_Ratio[descriptCnt]				= (pEDID->Detailed_Timing[cnt][6 + (descriptCnt * 3) + 1] >> 2) & 0x03;
					pOutput->CVT3ByteTimingCode.Preferred_Vertical_Rate[descriptCnt]	= (pEDID->Detailed_Timing[cnt][6 + (descriptCnt * 3) + 2] >> 5) & 0x03;
					pOutput->CVT3ByteTimingCode.Supported_Vertical_Rate[descriptCnt]	= (pEDID->Detailed_Timing[cnt][6 + (descriptCnt * 3) + 2] >> 0) & 0x1F;
				}
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0xF7)
			{
				//	Established Timing III
				pOutput->EstablishedTimingsIII.Use		= 1;

				pOutput->EstablishedTimingsIII.Revision	= pEDID->Detailed_Timing[cnt][5];

				memcpy(pOutput->EstablishedTimingsIII.TimingData, &pEDID->Detailed_Timing[cnt][6], 12);
			}
			else if(pEDID->Detailed_Timing[cnt][3] >= 0x11)		//	Reserved
			{
			}
			else if(pEDID->Detailed_Timing[cnt][3] == 0x10)		//	Dummy Descriptor
			{
			}
			else												//	0x00 ~ 0x0F, Manufacturer Specified Display Descriptors
			{
				u8	tagNumber;

				tagNumber = pEDID->Detailed_Timing[cnt][3];

				pOutput->ManufacturerSpecifiedData[tagNumber].Use	= 1;

				memcpy(pOutput->ManufacturerSpecifiedData[tagNumber].SpecifiedData, &pEDID->Detailed_Timing[cnt][5], 13);
			}
		}
		else
		{
			//	Detailed Timing Data
			pOutput->DetailedTiming[cnt].Use	= 1;

			pOutput->DetailedTiming[cnt].Pixel_Clock									= pEDID->Detailed_Timing[cnt][1];
			pOutput->DetailedTiming[cnt].Pixel_Clock									<<= 8;
			pOutput->DetailedTiming[cnt].Pixel_Clock									|= pEDID->Detailed_Timing[cnt][0];
			pOutput->DetailedTiming[cnt].Pixel_Clock									*= 10000;

			pOutput->DetailedTiming[cnt].Horizontal_Addressable_Video_in_pixels			= (pEDID->Detailed_Timing[cnt][4] >> 4) & 0x0f;
			pOutput->DetailedTiming[cnt].Horizontal_Blanking_in_pixels					= (pEDID->Detailed_Timing[cnt][4] >> 0) & 0x0f;
			pOutput->DetailedTiming[cnt].Horizontal_Addressable_Video_in_pixels			<<= 8;
			pOutput->DetailedTiming[cnt].Horizontal_Blanking_in_pixels					<<= 8;
			pOutput->DetailedTiming[cnt].Horizontal_Addressable_Video_in_pixels			|= pEDID->Detailed_Timing[cnt][2];
			pOutput->DetailedTiming[cnt].Horizontal_Blanking_in_pixels					|= pEDID->Detailed_Timing[cnt][3];

			pOutput->DetailedTiming[cnt].Vertical_Addressable_Video_in_lines			= (pEDID->Detailed_Timing[cnt][7] >> 4) & 0x0f;
			pOutput->DetailedTiming[cnt].Vertical_Blanking_in_lines						= (pEDID->Detailed_Timing[cnt][7] >> 0) & 0x0f;
			pOutput->DetailedTiming[cnt].Vertical_Addressable_Video_in_lines			<<= 8;
			pOutput->DetailedTiming[cnt].Vertical_Blanking_in_lines						<<= 8;
			pOutput->DetailedTiming[cnt].Vertical_Addressable_Video_in_lines			|= pEDID->Detailed_Timing[cnt][5];
			pOutput->DetailedTiming[cnt].Vertical_Blanking_in_lines						|= pEDID->Detailed_Timing[cnt][6];

			pOutput->DetailedTiming[cnt].Horizontal_Front_Porch_in_pixels				= (pEDID->Detailed_Timing[cnt][11] >> 6) & 0x03;
			pOutput->DetailedTiming[cnt].Horizontal_Sync_Pulse_Width_in_pixels			= (pEDID->Detailed_Timing[cnt][11] >> 4) & 0x03;
			pOutput->DetailedTiming[cnt].Vertical_Front_Porch_in_pixels					= (pEDID->Detailed_Timing[cnt][11] >> 2) & 0x03;
			pOutput->DetailedTiming[cnt].Vertical_Sync_Pulse_Width_in_pixels			= (pEDID->Detailed_Timing[cnt][11] >> 0) & 0x03;
			pOutput->DetailedTiming[cnt].Horizontal_Front_Porch_in_pixels				<<= 8;
			pOutput->DetailedTiming[cnt].Horizontal_Sync_Pulse_Width_in_pixels			<<= 8;
			pOutput->DetailedTiming[cnt].Vertical_Front_Porch_in_pixels					<<= 4;
			pOutput->DetailedTiming[cnt].Vertical_Sync_Pulse_Width_in_pixels			<<= 4;
			pOutput->DetailedTiming[cnt].Horizontal_Front_Porch_in_pixels				|= pEDID->Detailed_Timing[cnt][8];
			pOutput->DetailedTiming[cnt].Horizontal_Sync_Pulse_Width_in_pixels			|= pEDID->Detailed_Timing[cnt][9];
			pOutput->DetailedTiming[cnt].Vertical_Front_Porch_in_pixels					|= (pEDID->Detailed_Timing[cnt][10] >> 4) & 0x0f;
			pOutput->DetailedTiming[cnt].Vertical_Sync_Pulse_Width_in_pixels			|= (pEDID->Detailed_Timing[cnt][10] >> 0) & 0x0f;

			pOutput->DetailedTiming[cnt].Horizontal_Addressable_Video_Image_Size_in_mm	= (pEDID->Detailed_Timing[cnt][14] >> 4) & 0x0f;
			pOutput->DetailedTiming[cnt].Vertical_Addressable_Video_Image_Size_in_mm	= (pEDID->Detailed_Timing[cnt][14] >> 0) & 0x0f;
			pOutput->DetailedTiming[cnt].Horizontal_Addressable_Video_Image_Size_in_mm	<<= 8;
			pOutput->DetailedTiming[cnt].Vertical_Addressable_Video_Image_Size_in_mm	<<= 8;
			pOutput->DetailedTiming[cnt].Horizontal_Addressable_Video_Image_Size_in_mm	|= pEDID->Detailed_Timing[cnt][12];
			pOutput->DetailedTiming[cnt].Vertical_Addressable_Video_Image_Size_in_mm	|= pEDID->Detailed_Timing[cnt][13];

			pOutput->DetailedTiming[cnt].Horizontal_Border_in_pixels					= pEDID->Detailed_Timing[cnt][15];
			pOutput->DetailedTiming[cnt].Vertical_Border_in_lines						= pEDID->Detailed_Timing[cnt][16];

			pOutput->DetailedTiming[cnt].Signal_Interface_Type							= (pEDID->Detailed_Timing[cnt][17] >> 7) & 0x01;
			pOutput->DetailedTiming[cnt].Stereo_Viewing_Support							= (pEDID->Detailed_Timing[cnt][17] >> 4) & 0x06;
			pOutput->DetailedTiming[cnt].Stereo_Viewing_Support							|= (pEDID->Detailed_Timing[cnt][17] >> 0) & 0x01;
			pOutput->DetailedTiming[cnt].Signal_Interface_Type							= (pEDID->Detailed_Timing[cnt][17] >> 1) & 0x0f;
		}
	}

	result = 1;

	return result;
}

u8 EDID_Get_ExtensionBlockCount(void *pVoid, u8 *pData)
{
	u8	result = 0;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	*pData = pEDID->Extension_Block_Count_N;

	result = 1;

	return result;
}

u8 EDID_Chk_Checksum(void *pVoid)
{
	u8	result = 0;
	u8	checkSum = 0;
	edid_t	*pEDID;

	pEDID = (edid_t*)pVoid;

	for(u8 cnt = 0; cnt < 127; cnt++)
	{
		checkSum += pEDID->rawData[cnt];
	}

	checkSum = 256 - checkSum;

	if(pEDID->Checksum_C == checkSum)	result = 1;

	return result;
}

