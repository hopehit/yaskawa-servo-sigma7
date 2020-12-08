/****************************************************************************************************/
/*																									*/
/*																									*/
/*		IxInst.h : IxInst Header File																*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*																									*/
/*		1. Data Type Definition																		*/
/*																									*/
/*		2. Work Register Definition																	*/
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
/*		Rev.1.10 : 2008.01.08	T.Taniguchi		Add CREG											*/
/*																									*/
/*																									*/
/*		<1> 2013.05.07 T.yamada	modify IxLIMIT														*/
/****************************************************************************************************/
#ifdef WIN32			/* VC�̂ݗL��(ASIP�R���p�C���Ή�)			*/
#include <windows.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <direct.h>
#include <sys\stat.h>
#include "Basedef.h"
#else	//#ifdef WIN32			/* chessde			*/
#define ASIP_CC					/* chessde(ASIP�R���p�C��)�p�R���p�C���X�C�b�`��`			*/
#include "Basedef.h"
#endif	//#ifdef WIN32			/* VC�̂ݗL��(ASIP�R���p�C���Ή�)			*/
/*--------------------------------------------------------------------------------------------------*/




#ifdef ASIP_CC					/* chessde�ɂ��f�o�b�O������`					*/
/****************************************************************************************************/
/*		Macro Definition(for Chessde)																*/
/****************************************************************************************************/
#define	LPX_ABS( x )			abs( (x) )
#define LPX_RDTBL( tbl, idx )	(tbl)[(idx)  & ((sizeof(tbl)/sizeof(tbl[0])) - 1) ])
/*--------------------------------------------------------------------------------------------------*/
#define	IxLIMITUL( u, ulim, llim )	( u > ulim )? ulim:\
										( u < llim )? llim:u;
//<1>#define	IxLIMIT( u, lim )		IxLIMITUL( u, LPX_ABS( (lim) ), -LPX_ABS( (lim) ) )
#define	IxLIMIT( u, lim )		limit(u, lim)
/*--------------------------------------------------------------------------------------------------*/
#else	//#ifdef ASIP_CC					/* VC�ɂ��f�o�b�O������`					*/
/****************************************************************************************************/
/*		Macro Definition(for VC)																	*/
/****************************************************************************************************/
#define	LPX_ABS( x )			(((x) >= 0)? (x) : -(x))
#define LPX_RDTBL( tbl, idx )	(tbl[ (idx) & ((sizeof(tbl)/sizeof(tbl[0])) - 1) ])
/*--------------------------------------------------------------------------------------------------*/
#define	IxLIMITUL( u, ulim, llim )	( u > ulim )? ulim:\
										( u < llim )? llim:u;
#define	IxLIMIT( u, lim )		IxLIMITUL( u, LPX_ABS( (lim) ), -LPX_ABS( (lim) ) )
/*--------------------------------------------------------------------------------------------------*/
#ifndef RomVlmtTableXX
#define RomVlmtTableXX	RomVlmtTable02
#endif
#endif	//#ifdef ASIP_CC					/* chessde�ɂ��f�o�b�O������`					*/

/****************************************************************************************************/
/*		Const Data Definition																		*/
/****************************************************************************************************/
#define	LPX_REG16_MAX	 32767
#define	LPX_REG16_MIN	-32768
/*--------------------------------------------------------------------------------------------------*/
#define	LPX_REG32_MAX	(LONG)0x7FFFFFFF
#define	LPX_REG32_MIN	(LONG)0x80000000
/*--------------------------------------------------------------------------------------------------*/
#define	NUM_SIN_TBL		0x3FF					/* sin�e�[�u����									*/
#define	NUM_ATAN_TBL	0xFF					/* arctan�e�[�u����									*/
#define	NUM_SQRT_TBL	0xFF					/* �������e�[�u����									*/
#define	NUM_VLIM_TBL	0x3FF					/* �d�����~�b�g�e�[�u����							*/
#define	NUM_SPVEC_TBL	0x3FF					/* ��ԃx�N�g�����Z�p�e�[�u����						*/


#if 0 /* 2012.09.26 Y.Oka Basedef.h�ֈړ� */
/****************************************************************************************************/
/*		Data Type Definition																		*/
/****************************************************************************************************/
//typedef __int64 INT64;							/* 64bit Integer									*/
/*--------------------------------------------------------------------------------------------------*/
typedef union {									/* Double Word Register								*/
		INT64	dl;								/* 64bit Register									*/
//		UINT64	udl;							/* 64bit Register									*//* ���f����UINT64�ɑΉ����Ă��Ȃ����߃R�����g�A�E�g(�Ή��\��)<V6.05>/
		LONG	l[2];							/* Long Register									*/
		ULONG	ul[2];							/* Long Register									*/
		SHORT	s[4];							/* Short Register									*/
		USHORT	us[4];							/* Short Register									*/
} DLREG;
/*--------------------------------------------------------------------------------------------------*/
typedef union {									/* Double Word Register								*/
		LONG	l;								/* Long Register									*/
		ULONG	ul;								/* Long Register									*/
		SHORT	s[2];							/* Short Register									*/
		USHORT	us[2];							/* Short Register									*/
} DWREG;
/*--------------------------------------------------------------------------------------------------*/
typedef struct {								/* Constant Data Register							*/
		DWREG	Zero;							/* Zero(0) Register									*/
		DWREG	PlusOne;						/* PlusOne(1) Register								*/
		DWREG	MinusOne;						/* MinusOne(-1) Register							*/
} CREG;
/*--------------------------------------------------------------------------------------------------*/
#endif


#ifndef ASIP_CC					/* VC�ɂ��f�o�b�O������`					*/
/****************************************************************************************************/
/*		Flag Definition																				*/
/****************************************************************************************************/
typedef struct {
		UINT	Zero		:1;					/* Zero Flag										*/
		UINT	Sign		:1;					/* Sign Flag										*/
		UINT	Carry		:1;					/* Carry Flag										*/
		UINT	Borrow		:1;					/* Borrow Flag										*/
		UINT	Limit		:1;					/* Limit Flag										*/
		UINT	DivZero		:1;					/* Zero Divide Flag									*/
		UINT	DivOver		:1;					/* Divide Result Over Flow Flag						*/
} FLAG;
/****************************************************************************************************/
/*		Mac Register Definition																		*/
/****************************************************************************************************/
typedef union {
                INT64	R64;							/* Mac Register 64bit								*/
		struct	{ LONG L; LONG H;} R32;			/* Mac Register 32bit(Low/High)						*/
} MAC;
/****************************************************************************************************/
/*		Work Register Definition for IxInst Functions												*/
/****************************************************************************************************/
typedef struct {
		MAC		Mac;							/* Mac Register										*/
		MAC		SftMac;							/* Shift Mac Register								*/
	/*----------------------------------------------------------------------------------------------*/
		LONG	DivSet32;						/* DivSet Register									*/
		SHORT	DivRem16;						/* DivRem Register									*/
	/*----------------------------------------------------------------------------------------------*/
		const SHORT	*ConstTbl;					/* Const Table Pointer								*/
} IXREG;
/****************************************************************************************************/
/*		Work Register Definition for RcxxInst Functions												*/
/****************************************************************************************************/
typedef struct {
		LONG	SrcA;							/* Source-A Register								*/
		LONG	SrcB;							/* Source-B Register								*/
		LONG	SrcC;							/* Source-C Register								*/
		LONG	SrcD;							/* Source-D Register								*/
		LONG	SrcE;							/* Source-E Register								*/
		LONG	SrcF;							/* Source-F Register								*/
		LONG	SrcG;							/* Source-G Register								*/
/*--------------------------------------------------------------------------------------------------*/
		LONG*	DstX;							/* Destination-X Register							*/
		LONG*	DstY;							/* Destination-X Register							*/
/*--------------------------------------------------------------------------------------------------*/
		UCHAR	OP1;							/* Operation Code-1									*/
		UCHAR	OP2;							/* Operation Code-2									*/
		UCHAR	OP3;							/* Operation Code-3									*/
/*--------------------------------------------------------------------------------------------------*/
		UCHAR	Add1;							/* ADD1 Signed Cal. Flag							*/
		UCHAR	Mul1;							/* MUL1 Signed Cal. Flag							*/
		UCHAR	Mul2;							/* MUL2 Signed Cal. Flag							*/
/*--------------------------------------------------------------------------------------------------*/
		UCHAR	Enb1;							/* ENB1												*/
		UCHAR	Enb2;							/* ENB2												*/
/*--------------------------------------------------------------------------------------------------*/
		UCHAR	Sft1;							/* SFT1												*/
		UCHAR	Sft2;							/* SFT2												*/
} RCREG;
/*--------------------------------------------------------------------------------------------------*/



/****************************************************************************************************/
/*		Extern Variables																			*/
/****************************************************************************************************/
extern	FLAG	Flag;							/* Flag												*/
extern	IXREG	IxReg;							/* Work Register for IxInst							*/
extern	RCREG	RcReg;							/* Work Register for RcxxInst						*/
/*--------------------------------------------------------------------------------------------------*/
extern	DWREG	MacL;							/* Mac Register Low									*/
extern	DWREG	MacH;							/* Mac Register High								*/
extern	SHORT	DivRem;							/* Divide Remainder Register						*/
extern	DWREG	SftMacL;						/* Shift Mac Register Low							*/
extern	DWREG	SftMacH;						/* Shift Mac Register High							*/
extern	SHORT	IFLAG;							/* Flag Status										*/
/*--------------------------------------------------------------------------------------------------*/
#endif	//#ifndef ASIP_CC					/* VC�ɂ��f�o�b�O������`					*/


/****************************************************************************************************/
/*																									*/
/*		�֐���`(�C���g�����V�b�N�֐��̉�����)														*/
/*																									*/
/****************************************************************************************************/
#ifdef ASIP_CC			/* for chessde		*/
//assembly void IxNop( void )
//{
//	asm_begin
//	nop
//	asm_end
//}												/* IxNop�b���									*/
//#define	IxNop( )														/* IxNop�b���									*/
//#define	IxSetFuncAdr( Dst16, Func )	( *(VUSHORT*)(Dst16) = *(VUSHORT*)(Func) )
#define	IxSetFuncAdr( Dst16, Func )	( (Dst16) = (VUSHORT*)(Func) )
#define	IxSetCtblAdr( Dst16, Ctbl )	(Dst16) = (Ctbl)									/* IxSetCtblAdr								*/
#define	IxLmtzImm16( Dst16, Imm16 )	(Dst16) = limitz( (Dst16), (Imm16) )						/* IxLmtzImm16								*/
#define	IxLmtzReg16( Dst16, Src16A, Src16B )	(Dst16) = limitz( (Src16A), (Src16B) )						/* IxLmtzImm16								*/
#define	IxLmtzImm32( Dst32, Imm32 )	(Dst32) = limitz( (Dst32), (Imm32) )						/* IxLmtzImm32								*/
#define	IxLmtzReg32( Dst32, Src32A, Src32B )	(Dst32) = limitz( (Src32A), (Src32B) )						/* IxLmtzImm32								*/
#define	IxTblSin16( Dst16, Src16 )		(Dst16) = tblrs( (Src16) & NUM_SIN_TBL )				/* IxTblSin16								*/
#define	IxTblAtan16( Dst16, Src16 )		(Dst16) = tblra( (Src16) & NUM_ATAN_TBL )				/* IxTblAtan16								*/
#define	IxTblSqrt16( Dst16, Src16 )		(Dst16) = tblrr( (Src16) & NUM_SQRT_TBL )				/* IxTblSqrt16								*/
#define	IxTblVlmt16( Dst16, Src16 )		(Dst16) = tblrv( (Src16) & NUM_VLIM_TBL )				/* IxTblVlmt16								*/
#define	IxTblSpvect16( Dst16, Src16 )	(Dst16) = tblrv2( (Src16) & NUM_SPVEL_TBL )				/* IxTblSpvect16							*/
//#define	IxTblMatcon16( SHORT *Dst16, SHORT Src16 );						/* IxTblMatcon16							*/
#define	IxLoadMpmem16( Dst16, Src16, Imm16 )	(Dst16) = *((Src16) + (Imm16))		/* IxLoadMpmem16							*/
#define	IxLmtCBS16( Src16 )		limitf( Src16 )							/* limizcb( Src16 )���܂��R���p�C���ɖ������Ȃ̂ō��̂Ƃ��뒼�ڑ��		*/
#define	IxLmtCBS32( Src32 )		limitf( Src32 )							/* limizcb( Src32 )���܂��R���p�C���ɖ������Ȃ̂ō��̂Ƃ��뒼�ڑ��		*/
#define IlibASR16( src, sft)	asr( src , sft )				/* VC�̃V�t�g�Ƌ��ʉ�					*/
#define IlibASR32( src, sft)	asr( src , sft )				/* VC�̃V�t�g�Ƌ��ʉ�					*/
#define IlibASR64( src, sft)	asr( src , sft )				/* VC�̃V�t�g�Ƌ��ʉ�					*/
/*--------------------------------------------------------------------------------------------------*/
#else	//#ifdef ASIP_CC			/* for VC		*/
void	IxNop( );														/* IxNop									*/
void	IxSetFuncAdr( USHORT *Dst16, void (*Func)(void) );				/* IxSetFuncAdr								*/
//void	IxSetCtblAdr( CSHORT *Dst16, CSHORT *Ctbl );				/* IxSetCtblAdr								*/
#define	IxSetCtblAdr( Dst16, Ctbl )	(Dst16) = (Ctbl)									/* IxSetCtblAdr								*/
void	IxLmtZImm16( SHORT *Dst16, SHORT Imm16 );						/* IxLmtzImm16								*/
void	IxLmtZReg16( SHORT *Dst16, SHORT Src16A, SHORT Src16B );		/* IxLmtzReg16								*/
//void	IxLmtZImm32( LONG *Dst32, LONG Imm32 );							/* IxLmtzImm32								*/
//void	IxLmtZReg32( LONG *Dst32, LONG Src32A, LONG Src32B );			/* IxLmtzReg32								*/
#define	IxLmtzImm16( Dst16, Imm16 )	IxLmtZImm16( &(Dst16), Imm16 )
#define	IxLmtzReg16( Dst16, Src16A, Src16B )	IxLmtZReg16( &(Dst16), Src16A, Src16B )
//#define	IxLmtzImm32( Dst32, Imm32 )	IxLmtZImm32( &(Dst32), Imm32 )
//#define	IxLmtzReg32( Dst32, Src32A, Src32B )	IxLmtZReg32( &(Dst32), Src32A, Src32B )
void	IxTblSin16( SHORT *Dst16, SHORT Src16 );						/* IxTblSin16								*/
void	IxTblAtan16( SHORT *Dst16, SHORT Src16 );						/* IxTblAtan16								*/
void	IxTblSqrt16( SHORT *Dst16, SHORT Src16 );						/* IxTblSqrt16								*/
void	IxTblVlmt16( SHORT *Dst16, SHORT Src16 );						/* IxTblVlmt16								*/
void	IxTblSpvect16( SHORT *Dst16, SHORT Src16 );						/* IxTblSpvect16							*/
void	IxTblMatcon16( SHORT *Dst16, SHORT Src16 );						/* IxTblMatcon16							*/
//void	IxLoadMpmem16( SHORT *Dst16, SHORT Src16, SHORT Imm16 );		/* IxLoadMpmem16							*/
#define	IxLoadMpmem16( Dst16, Src16, Imm16 )	(Dst16) = *((Src16) + (Imm16))		/* IxLoadMpmem16							*/
#endif	//#ifdef ASIP_CC



/******************************************* end of file ********************************************/
