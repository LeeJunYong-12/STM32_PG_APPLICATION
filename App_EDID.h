#ifndef	_APP_EDID_H
#define	_APP_EDID_H

#include	"E7602_PG_Application.h"

typedef	union{
	u8	rawData[128];

	__packed struct{
		//	Header										:	8 Byte
		u8	Header[8];
		//	Vendor & Product Identification				:	10 Byte
		u8	ID_Manufacturer_Name[2];
		u8	ID_Product_Code[2];
		u8	ID_Serial_Number[4];
		u8	Week_of_Manufacture;
		u8	Year_of_Manufacture;						//	Model Year
		//	EDID Structure Version & Revision			:	2 Byte
		u8	Version_Number;
		u8	Revision_Number;
		//	Basic Display Parameters & Features			:	5 Byte
		u8	Video_Input_Definition;
		u8	Horizontal_Screen_Size;						//	Aspect Ratio
		u8	Vertical_Screen_Size;						//	Aspect Ratio
		u8	Display_Transfer_Characteristic;			//	Gamma
		u8	Feature_Support;
		//	Color Characteristics						:	10 Byte
		u8	Red_Green;									//	Low Order Bits
		u8	Blue_White;									//	Low Order Bits
		u8	Red_x;										//	High Order Bits
		u8	Red_y;										//	High Order Bits
		u8	Green_x;									//	High Order Bits
		u8	Green_y;									//	High Order Bits
		u8	Blue_x;										//	High Order Bits
		u8	Blue_y;										//	High Order Bits
		u8	White_x;									//	High Order Bits
		u8	White_y;									//	High Order Bits
		//	Established Timings							:	3 Byte
		u8	Established_Timings_I;
		u8	Established_Timings_II;
		u8	Manufacturers_Reserved_Timings;
		//	Standard Timings : Identification 1 -> 8	:	16 Byte
		u8	Standard_Timing[16];
		//	18 Byte Data Blocks							:	72 Byte
		u8	Preferred_Timing_Mode[18];
		u8	Detailed_Timing[3][18];						//	Display Descriptor
		//	Extension Block Count N						:	1 Byte
		u8	Extension_Block_Count_N;
		//	Checksum C									:	1 Byte
		u8	Checksum_C;
	};
}edid_t;

typedef	struct{
	u32	Mode;
	u32	Week;
	u32	Year;
}edid_ManufactureDate_t;

typedef	struct{
	u32	Video_Signal_Interface;

//	Analog
	u32	Signal_Level_Standard;
	u32	Video_Setup;
	u32	Synchronization_Types;
	u32	Serrations;

//	Digital
	u32	Color_Bit_Depth;
	u32	Digital_Video_Interface_Standard_Supported;
}edid_VideoInputDefinition_t;

typedef	struct{
	u32		Mode;
	u32		Horizontal_Size;
	u32		Vertical_Size;
	float	Horizontal_Ratio;
	float	Vertical_Ratio;
}edid_HVSizeOrAspectRatio_t;

typedef struct{
	u32	Standby_Mode;
	u32	Suspend_Mode;
	u32	Active_Off;
	u32	Display_Color_Type;			//	Digital : Supported_Color_Encoding_Formats
	u32	sRGB_Standard;
	u32	Preferred_Timing_Mode;
	u32	Display_Continuous_Frequency;
}edid_FeatureSupport_t;

typedef	struct{
	u32	Red_x;
	u32	Red_y;
	u32	Green_x;
	u32	Green_y;
	u32	Blue_x;
	u32	Blue_y;
	u32	White_x;
	u32	White_y;
}edid_ColorCharacteristics_t;

typedef	struct{
	u32	data;
}edid_EstablishedTimings_t;

typedef	struct{
	u32	Value_Stored[8];
	u32	Image_Aspect_Ratio[8];
	u32	Field_Refresh_Rate[8];
	u32	Horizontal_Addressable_Pixels[8];
	u32	Vertical_Pixels[8];
}edid_StandardTimings_t;

typedef	struct{
	u32	Use;

	u32	Pixel_Clock;
	u32	Horizontal_Addressable_Video_in_pixels;
	u32	Horizontal_Blanking_in_pixels;
	u32	Vertical_Addressable_Video_in_lines;
	u32	Vertical_Blanking_in_lines;
	u32	Horizontal_Front_Porch_in_pixels;
	u32	Horizontal_Sync_Pulse_Width_in_pixels;
	u32	Vertical_Front_Porch_in_pixels;
	u32	Vertical_Sync_Pulse_Width_in_pixels;
	u32	Horizontal_Addressable_Video_Image_Size_in_mm;
	u32	Vertical_Addressable_Video_Image_Size_in_mm;
	u32	Horizontal_Border_in_pixels;
	u32	Vertical_Border_in_lines;

	u32	Signal_Interface_Type;
	u32	Stereo_Viewing_Support;
	u32	Sync_Signal_Definitions;	//	Analog or Digital
}edid_DetailedTiming_t;

typedef	struct{
	u32	Use;

	u8	String_Data[16];
}edid_DisplayProductSerialNumber_t;

typedef	struct{
	u32	Use;

	u8	String_Data[16];
}edid_AlphanumericDataString_t;

typedef	struct{
	u32	Use;

	u32	StartBreakFrequency;
	u32	C;
	u32	M;
	u32	K;
	u32	J;
}edid_GTFSecondaryCurveBlock_t;

typedef	struct{
	u32	Use;

	u32	CVT_Standard_Version_Number;
	u32	Additional_Pixel_Clock_Precision;
	u32	Maximum_Active_Pixel_per_Line;
	u32	Supported_Aspect_Ratios;
	u32	Preferred_Aspect_Ratio;
	u32	CVT_Blanking_Support;
	u32	Type_of_Display_Scaling_Supported;
	u32	Preferred_Vertical_Refresh_Rate;
}edid_CVTSupport_t;;

typedef	struct{
	u32	Use;

	u32	Vertical_Rate_Offset;
	u32	Horizontal_Rate_Offset;
	u32	Minimum_Vertical_Rate;
	u32	Maximum_Vertical_Rate;
	u32	Minimum_Horizontal_Rate;
	u32	Maximum_Horizontal_Rate;
	u32	Maximum_Pixel_Clock;
	u32	Video_Timing_Support_Flags;

	edid_GTFSecondaryCurveBlock_t	GTFSecondaryCurveBlock;
	edid_CVTSupport_t				CVTSupport;
}edid_DisplayRangeLimits_t;

typedef struct{
	u32	Use;

	u8	String_Data[16];
}edid_DisplayProductName_t;

typedef struct{
	u32	Use;

	u32	White_Point_Index_Number_1;
	u32	White_x_1;
	u32	White_y_1;
	u32	Gamma_1;
	u32	White_Point_Index_Number_2;
	u32	White_x_2;
	u32	White_y_2;
	u32	Gamma_2;
}edid_ColorPointData_t;

typedef struct{
	u32	Use;

	u32	Value_Stored[6];
	u32	Image_Aspect_Ratio[6];
	u32	Field_Refresh_Rate[6];
	u32	Horizontal_Addressable_Pixels[6];
	u32	Vertical_Pixels[6];
}edid_StandardTimings9to14_t;

typedef	struct{
	u32	Use;

	u32	Version;
	u32	Red_a3;
	u32	Red_a2;
	u32	Green_a3;
	u32	Green_a2;
	u32	Blue_a3;
	u32	Blue_a2;
}edid_ColorManagementData_t;

typedef	struct{
	u32	Use;

	u32	Version;
	u32	Addressable_Lines[4];
	u32	Aspect_Ratio[4];
	u32	Preferred_Vertical_Rate[4];
	u32	Supported_Vertical_Rate[4];		//	and Blanking Style
}edid_CVT3ByteTimingCode_t;

typedef	struct{
	u32	Use;

	u32	Revision;
	u32	TimingData[16];
}edid_EstablishedTimingsIII_t;

typedef struct{
	u32	Use;

	u8	SpecifiedData[16];
}edid_ManufacturerSpecifiedData_t;

typedef	struct{
	edid_DetailedTiming_t				PreferredTimingMode;
	edid_DetailedTiming_t				DetailedTiming[3];
	edid_DisplayProductSerialNumber_t	DisplayProductSerialNumber;
	edid_AlphanumericDataString_t		AlphanumericDataString;
	edid_DisplayRangeLimits_t			DisplayRangeLimits;
	edid_DisplayProductName_t			DisplayProductName;
	edid_ColorPointData_t				ColorPointData;
	edid_StandardTimings9to14_t			StandardTimings;
	edid_ColorManagementData_t			ColorManagementData;
	edid_CVT3ByteTimingCode_t			CVT3ByteTimingCode;
	edid_EstablishedTimingsIII_t		EstablishedTimingsIII;
	edid_ManufacturerSpecifiedData_t	ManufacturerSpecifiedData[16];
}edid_18ByteDataBlocks_t;

u8 EDID_Check_Header(void *pVoid);
u8 EDID_Get_IdManufacturerName(void *pVoid, u8 *pData);
u8 EDID_Get_IdProductCode(void *pVoid, u16 *pData);
u8 EDID_Get_IdSerialNumber(void *pVoid, u32 *pData);
u8 EDID_Get_ManufactureDate(void *pVoid, void *pvOutput);
u8 EDID_Get_VersionAndRevision(void *pVoid, u8 *pData);
u8 EDID_Get_VideoInputDefinition(void *pVoid, void *pvOutput);
u8 EDID_Get_HVScreenSizeOrAspectRatio(void *pVoid, void *pvOutput);
u8 EDID_Get_DisplayTransferCharacteristics(void *pVoid, u8 *pData);
u8 EDID_Get_FeatureSupport(void *pVoid, void *pvOutput);
u8 EDID_Get_ColorCharacteristics(void *pVoid, void *pvOutput);
u8 EDID_Get_EstablishedTimings(void *pVoid, void *pvOutput);
u8 EDID_Get_StandardTimings(void *pVoid, void *pvOutput);
u8 EDID_Get_18ByteDataBlocks(void *pVoid, void *pvOutput);
u8 EDID_Get_ExtensionBlockCount(void *pVoid, u8 *pData);
u8 EDID_Chk_Checksum(void *pVoid);

#endif	// _APP_EDID_H
