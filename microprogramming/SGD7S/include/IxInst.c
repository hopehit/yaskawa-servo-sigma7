/****************************************************************************************************/
/*																									*/
/*																									*/
/*		IxInst.c : IxInst Functions for Debug Micro Program											*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*																									*/
/*		1. Include Files																			*/
/*																									*/
/*		2. Work Register Definition																	*/
/*																									*/
/*		3. The Functions for Micro Program Instruction												*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/************** Copyright Yaskawa Electric Corporation **********************************************/
/*																									*/
/*		Rev.1.00 : 2007.02.24	T.Taniguchi															*/
/*		Rev.1.10 : 2008.01.08	T.Taniguchi		Add/Modify IxIntegral(),IxLpfilter1(),etc			*/
/*		Rev.1.20 : 2009.01.27	J.Watanabe		èCê≥ IxSAR32Imm,IxSAR32Reg,IxSAR64Imm,IxSAR64Reg	*/
/*																									*/
/*																									*/
/****************************************************************************************************/
#include "IxInst.h"
//#include "RcInst.c"
#ifdef WIN32
//#include "RomTable.c"
#include "RomTable.h"
#else
#define ASIP_CC
#include "tables.h"
#endif

/****************************************************************************************************/
/*		Work Register Definition for IxInst Functions												*/
/****************************************************************************************************/
FLAG	Flag;									/* Flag												*/
IXREG	IxReg;									/* Work Register for IxInst							*/
/*--------------------------------------------------------------------------------------------------*/


#ifndef ASIP_CC
/****************************************************************************************************/
/*																									*/
/*		IFALGçÏê¨èàóù																				*/
/*																									*/
/****************************************************************************************************/
void	MakeIFLAG( )
{
		IFLAG = (Flag.Zero) | (Flag.Sign<<1) | (Flag.Carry<<2) | (Flag.Borrow<<3)
				| (Flag.Limit<<4) | (Flag.DivZero<<5) | (Flag.DivOver<<6);
		return;
}
#endif


/****************************************************************************************************/
/*																									*/
/*		IxNop																						*/
/*																									*/
/****************************************************************************************************/
void	IxNop( )
{
		return;
}


#ifndef ASIP_CC
/****************************************************************************************************/
/*																									*/
/*		IxSetFuncAdr																				*/
/*																									*/
/****************************************************************************************************/
void	IxSetFuncAdr( USHORT *Dst16, void (*Func)(void) )
{
		Dst16 = (USHORT*)Func;
		return;
}
#endif


#if 0
/****************************************************************************************************/
/*																									*/
/*		IxSetCtblAdr																				*/
/*																									*/
/****************************************************************************************************/
//void	IxSetCtblAdr( SHORT *Dst16, const void *Ctbl )
void	IxSetCtblAdr( CSHORT *Dst16, CSHORT *Ctbl )
{
		*Dst16 = 0;
		IxReg.ConstTbl = Ctbl;
		return;
}
#endif


/****************************************************************************************************/
/*																									*/
/*		IxLmtImm16																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtImm16( SHORT *Dst16, SHORT Imm16 )
{
LONG	x = (LONG)(*Dst16);
LONG	y = LPX_ABS( (LONG)Imm16 );
SHORT	z;

		if( x > y )
		{
			Flag.Limit = 1;
			z = (SHORT)y;
			*Dst16 = (SHORT)y;
		}
		else if( x < -y )
		{
			Flag.Limit = 1;
			z = (SHORT)(-y);
			*Dst16 = (SHORT)(-y);
		}
		else
		{
			Flag.Limit = 0;
			z = (SHORT)x;
		}

		Flag.Zero = ( z == 0 );
		Flag.Sign = ( z <  0 );
		Flag.Carry  = 0;
		Flag.Borrow = 0;
#ifndef ASIP_CC
		MakeIFLAG( );
#endif
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtReg16																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtReg16( SHORT *Dst16, SHORT Src16A, SHORT Src16B )
{
LONG	x = (LONG)Src16A;
LONG	y = LPX_ABS( (LONG)Src16B );
SHORT	z;

		if( x > y )
		{
			Flag.Limit = 1;
			z = (SHORT)y;
			*Dst16 = (SHORT)y;
		}
		else if( x < -y )
		{
			Flag.Limit = 1;
			z = (SHORT)(-y);
			*Dst16 = (SHORT)(-y);
		}
		else
		{
			Flag.Limit = 0;
			z = Src16A;
			*Dst16 = Src16A;
		}

		Flag.Zero = ( z == 0 );
		Flag.Sign = ( z <  0 );
		Flag.Carry  = 0;
		Flag.Borrow = 0;
#ifndef ASIP_CC
		MakeIFLAG( );
#endif
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtzImm16																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtZImm16( SHORT *Dst16, SHORT Imm16 )
{
LONG	x = (LONG)(*Dst16);
LONG	y = LPX_ABS( (LONG)Imm16 );

		if( ((*Dst16) ^ Imm16) & 0x8000 )
		{
			*Dst16 = 0;
		}
		else if( x > y )
		{
			*Dst16 = (SHORT)y;
		}
		else if( x < -y )
		{
			*Dst16 = (SHORT)(-y);
		}

		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtzReg16																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtZReg16( SHORT *Dst16, SHORT Src16A, SHORT Src16B )
{
LONG	x = (LONG)Src16A;
LONG	y = LPX_ABS( (LONG)Src16B );

		if( (Src16A ^ Src16B) & 0x8000 )
		{
			*Dst16 = 0;
		}
		else if( x > y )
		{
			*Dst16 = (SHORT)y;
		}
		else if( x < -y )
		{
			*Dst16 = (SHORT)(-y);
		}
		else
		{
			*Dst16 = Src16A;
		}

		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtImm32																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtImm32( LONG *Dst32, LONG Imm32 )
{
INT64	x = (INT64)(*Dst32);
INT64	y = LPX_ABS( (INT64)Imm32 );
LONG	z;

		if( x > y )
		{
			Flag.Limit = 1;
			z = (LONG)y;
			*Dst32 = (LONG)y;
		}
		else if( x < -y )
		{
			Flag.Limit = 1;
			z = (LONG)(-y);
			*Dst32 = (LONG)(-y);
		}
		else
		{
			Flag.Limit = 0;
			z = (LONG)x;
		}

		Flag.Zero = ( z == 0 );
		Flag.Sign = ( z <  0 );
		Flag.Carry  = 0;
		Flag.Borrow = 0;
#ifndef ASIP_CC
		MakeIFLAG( );
#endif
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtReg32																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtReg32( LONG *Dst32, LONG Src32A, LONG Src32B )
{
INT64	x = (INT64)Src32A;
INT64	y = LPX_ABS( (INT64)Src32B );
LONG	z;

		if( x > y )
		{
			Flag.Limit = 1;
			z = (LONG)y;
			*Dst32 = (LONG)y;
		}
		else if( x < -y )
		{
			Flag.Limit = 1;
			z = (LONG)(-y);
			*Dst32 = (LONG)(-y);
		}
		else
		{
			Flag.Limit = 0;
			z = Src32A;
			*Dst32 = Src32A;
		}

		Flag.Zero = ( z == 0 );
		Flag.Sign = ( z <  0 );
		Flag.Carry  = 0;
		Flag.Borrow = 0;
#ifndef ASIP_CC
		MakeIFLAG( );
#endif
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtzImm32																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtZImm32( LONG *Dst32, LONG Imm32 )
{
INT64	x = (INT64)(*Dst32);
INT64	y = LPX_ABS( (INT64)Imm32 );

		if( ((*Dst32) ^ Imm32) & (LONG)0x80000000 )
		{
			*Dst32 = 0;
		}
		else if( x > y )
		{
			*Dst32 = (LONG)y;
		}
		else if( x < -y )
		{
			*Dst32 = (LONG)(-y);
		}

		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxLmtzReg32																					*/
/*																									*/
/****************************************************************************************************/
void	IxLmtZReg32( LONG *Dst32, LONG Src32A, LONG Src32B )
{
INT64	x = (INT64)Src32A;
INT64	y = LPX_ABS( (INT64)Src32B );

		if( (Src32A ^ Src32B) & (LONG)0x80000000 )
		{
			*Dst32 = 0;
		}
		else if( x > y )
		{
			*Dst32 = (LONG)y;
		}
		else if( x < -y )
		{
			*Dst32 = (LONG)(-y);
		}
		else
		{
			*Dst32 = Src32A;
		}

		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxTblSin16																					*/
/*																									*/
/****************************************************************************************************/
void	IxTblSin16( SHORT *Dst16, SHORT Src16 )
{
		*Dst16 = LPX_RDTBL( RomSinTable, Src16 );
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxTblAtan16																					*/
/*																									*/
/****************************************************************************************************/
void	IxTblAtan16( SHORT *Dst16, SHORT Src16 )
{
		*Dst16 = LPX_RDTBL( RomAtanTable, Src16 );
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxTblSqrt16																					*/
/*																									*/
/****************************************************************************************************/
void	IxTblSqrt16( SHORT *Dst16, SHORT Src16 )
{
		*Dst16 = LPX_RDTBL( RomRootTable, Src16 );
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxTblVlmt16																					*/
/*																									*/
/****************************************************************************************************/
void	IxTblVlmt16( SHORT *Dst16, SHORT Src16 )
{
		*Dst16 = LPX_RDTBL( RomVlmtTableXX, Src16 );
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxTblSpvect16																				*/
/*																									*/
/****************************************************************************************************/
void	IxTblSpvect16( SHORT *Dst16, SHORT Src16 )
{
		*Dst16 = LPX_RDTBL( RomSpvectTable, Src16 );
		return;
}



/****************************************************************************************************/
/*																									*/
/*		IxTblMatcon16																				*/
/*																									*/
/****************************************************************************************************/
void	IxTblMatcon16( SHORT *Dst16, SHORT Src16 )
{
		*Dst16 = LPX_RDTBL( RomMatconTable, Src16 );
		return;
}



#if 0
/****************************************************************************************************/
/*																									*/
/*		IxLoadMpmem16																				*/
/*																									*/
/****************************************************************************************************/
void	IxLoadMpmem16( SHORT *Dst16, SHORT Src16, SHORT Imm16 )
{
LONG	i = Src16 * 2;

		if( Imm16 == 0 )
		{
			*Dst16 = IxReg.ConstTbl[i+0];
		}
		else
		{
			*Dst16 = IxReg.ConstTbl[i+1];
		}
		return;
}
#endif






/******************************************* end of file ********************************************/
