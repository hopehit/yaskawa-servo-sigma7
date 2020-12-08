/****************************************************************************************************/
/*																									*/
/*																									*/
/*		RomTable.h : ROM Table for Micro-Program Compiler/Emulator									*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*																									*/
/*		1. RomSinTable[]	: Sin Table																*/
/*		2. RomAtanTable[]	: Atan Table															*/
/*		3. RomRootTable[]	: SQRT Root Table														*/
/*		4. RomVlmtTable01[] : Voltage Limit Table for JL-046/DRAGON									*/
/*		5. RomVlmtTable02[] : Voltage Limit Table for JL-054/JL-056									*/
/*		6. RomSpvectTable[] : Space Vector Voltage Limit Table										*/
/*		7. RomMatconTable[] : Matrix Converter Table (Alpha Table)									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		Rev.1.00 : 2011.09.19	Y.Tanaka															*/
/*																									*/
/*																									*/
/****************************************************************************************************/



/****************************************************************************************************/
/*																									*/
/*		Sin Table																					*/
/*																									*/
/****************************************************************************************************/
extern const	short	RomSinTable[1024];
extern const	short	RomAtanTable[256];
extern const	unsigned	short	RomRootTable[256];
extern const	short	RomVlmtTable01[512];
extern const	short	RomVlmtTable02[1024];
extern const	short	RomSpvectTable[1024];
extern const	short	RomMatconTable[0x400];




/******************************************* end of file ********************************************/
