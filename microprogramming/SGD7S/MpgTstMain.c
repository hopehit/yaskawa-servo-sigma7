/****************************************************************************************************/
/*																									*/
/*																									*/
/*		MpgTstMain.c : Micro-Program Test Main														*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*																									*/
/*		機 能 : マイクロプログラムをＶＣ＋＋環境で実行し、マイクロプログラムのデバッグを行う。		*/
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
/*		Rev.1.00 : 2007.03.03	T.Taniguchi															*/
/*																									*/
/*																									*/
/****************************************************************************************************/
#ifdef WIN32			/* VCのみ有効(ASIPコンパイラ対応)			*/
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
#else	//#ifdef WIN32			/* VCのみ有効(ASIPコンパイラ対応)			*/
#include "Basedef.h"
#endif	//#ifdef WIN32			/* VCのみ有効(ASIPコンパイラ対応)			*/
/*--------------------------------------------------------------------------------------------------*/
#include "MprgStruct.h"
//typedef  const UCHAR  CUCHAR;


/****************************************************************************************************/
/*		Disable Warninig																			*/
/****************************************************************************************************/
#ifdef WIN32			/* VCのみ有効(ASIPコンパイラ対応)			*/
#pragma	warning( disable : 4068 )				/* Disable VC++ Warning : Unknown #pragma			*/
#pragma	warning( disable : 4102 )				/* Disable VC++ Warning : Not Used Label			*/
#pragma	warning( disable : 4305 )				/* Disable VC++ Warning : const int --> short		*/
#pragma	warning( disable : 4996 )				/* Disable VC++ Warning : check 'fopen' declaration	*///110811tanaka21追加
#endif	//#ifdef WIN32			/* VCのみ有効(ASIPコンパイラ対応)			*/
/****************************************************************************************************/
/*		Return Code Definition																		*/
/****************************************************************************************************/
#define TSTOK	 0								/* Test OK											*/
#define TSTNG	-1								/* Test NG											*/
/****************************************************************************************************/
/*		Include MicroProgram Source File															*/
/****************************************************************************************************/
#define RomVlmtTableXX	RomVlmtTable02			/* Select RomVlmtTable01/02							*/
/*--------------------------------------------------------------------------------------------------*/
//#include "IxInst.h"						/* Include IxInst Function							*/
/****************************************************************************************************/
/*		Include Test MicroProgram File																*/
/****************************************************************************************************/
#ifdef WIN32
#include "MprgSgdv_VC.h"				/* Include MicroProgram Source File					*/
#elif defined(ASIP_CC)
#include "MprgSgdv_chessde.h"				/* Include MicroProgram Source File					*/
#endif
#include "RefTbl.h"
//#include "MprgSgdvStruct.h"
/*--------------------------------------------------------------------------------------------------*/



/****************************************************************************************************/
/*																									*/
/*		Test Functions																				*/
/*																									*/
/****************************************************************************************************/
LONG	TstMpStart( void );
LONG	TstMpIntAD( void );
LONG	TstMpEtcFunc( void );
void	TstSetDataAD( void );
extern	void MpStart( void );
extern	void MpIntAD( void );
/*--------------------------------------------------------------------------------------------------*/
void	SetInitPrm( void );
void	SetTrqRef( int i );
void	GetADOut( int i );
void	MakeADOutFile( int i );


MICRO_AXIS_HANDLE	*AxisRscT;		/* テスト用軸ハンドル		*/


/****************************************************************************************************/
/*																									*/
/*		Main Function																				*/
/*																									*/
/****************************************************************************************************/
void	main( void )
{
LONG	TstNgCnt = 0;

/*--------------------------------------------------------------------------------------------------*/
/*		Test ASIC Micro Program																		*/
/*--------------------------------------------------------------------------------------------------*/
		if( TstMpStart( )   != TSTOK ){ TstNgCnt++;}		/* Test MpStart() Function			*/
    while(1){
		if( TstMpIntAD( )   != TSTOK ){ TstNgCnt++;}		/* Test MpIntAD() Function			*/
    }
//		if( TstMpEtcFunc( ) != TSTOK ){ TstNgCnt++;}		/* Test MpEtcFunc() Function		*/
}



/****************************************************************************************************/
/*																									*/
/*		Test MpStart() Function																		*/
/*																									*/
/****************************************************************************************************/
LONG	TstMpStart( void )
{
		MpStart( );
		return( TSTOK );
}



/****************************************************************************************************/
/*																									*/
/*		Test MpIntAD() Function																		*/
/*																									*/
/****************************************************************************************************/
LONG	TstMpIntAD( void )
{
LONG	i;
USHORT	ax_noT;

	for( ax_noT = 0;ax_noT<2;ax_noT++)
	{
		AxisRscT = &AxisHdl[ax_noT];

/*--------------------------------------------------------------------------------------------------*/
/*		DataSet for IntAD																			*/
/*--------------------------------------------------------------------------------------------------*/
		SetInitPrm();
/*--------------------------------------------------------------------------------------------------*/
/*		Test Loop																					*/
/*--------------------------------------------------------------------------------------------------*/
		for( i=0; i < 5; i++ )
		{
			SetTrqRef(i);
			MpIntAD( );
			GetADOut(i);
#ifdef WIN32
			MakeADOutFile(i);
#endif
		}
	}
/*--------------------------------------------------------------------------------------------------*/
		return( TSTOK );
}



/****************************************************************************************************/
/*																									*/
/*		Test MpXxxYyyy() Function																	*/
/*																									*/
/****************************************************************************************************/
LONG	TstMpEtcFunc( void )
{
LONG	i;

/*--------------------------------------------------------------------------------------------------*/
/*		Test Loop																					*/
/*--------------------------------------------------------------------------------------------------*/
		for( i=0; i < 10; i++ )
		{
			;;;;
		}
/*--------------------------------------------------------------------------------------------------*/
		return( TSTOK );
}

/****************************************************************************************************/
/*																									*/
/*		Set Initial Pramater for IntAD																*/
/*																									*/
/****************************************************************************************************/
void	SetInitPrm( void )
{
	
	AxisRscT->IntAdV.IuOffset = 0xFEFF;
	AxisRscT->IntAdV.IvOffset = 0xFAFF;
	AxisRscT->IntAdP.Kcu = 0X0001;
	AxisRscT->IntAdP.Kcv = 0X0001;
	AxisRscT->VcmpV.VdRef = 0x0000;
	AxisRscT->VcmpV.VqRef = 0x0000;
	AxisRscT->IntAdV.TLimP = 0x983A;
	AxisRscT->IntAdV.TLimM = 0x983A;
	AxisRscT->IntAdV.KEangle = 0x0300;
	AxisRscT->IntAdP.Kf31 = 0X0000;
	AxisRscT->IntAdP.Kc = 0X7903;
	AxisRscT->IntAdP.Ld = 0xBA43;
	AxisRscT->IntAdP.Lq = 0xBA43;
	AxisRscT->IntAdP.Mag = 0x7048;
	AxisRscT->IntAdP.KdP = 0XED07;
	AxisRscT->IntAdP.KdI = 0X4D03;
	AxisRscT->IntAdP.KqI = 0X4D03;
	AxisRscT->IntAdP.VdLim = 0xBE28;
	AxisRscT->IntAdP.VqLim = 0xBE28;
	AxisRscT->IntAdP.OnDelayLvl = 0x983A;
	AxisRscT->IntAdP.Tfil = 0x0040;
	AxisRscT->IntAdP.OnDelayComp = 0x7800;
	AxisRscT->IntAdP.CtrlSw = 0XF28D;
	AxisRscT->IntAdP.CrFreq = 0XA60E;
	AxisRscT->IntAdP.Kf11 = 0X400F;
	AxisRscT->IntAdP.TLpf2 = 0x0040;
	AxisRscT->IntAdP.MotResist = 0x2A10;
	AxisRscT->IntAdP.OnDelaySlope = 0x9D36;
	AxisRscT->IntAdP.FccRst = 0X0000;
	AxisRscT->IntAdP.L_dIdt = 0X0000;
	AxisRscT->DobsP.TsPerL = 0XB505;
	AxisRscT->DobsP.Gobs = 0x0728;
	AxisRscT->DobsP.RLTs = 0xE90F;
	AxisRscT->DobsP.FilObsGain = 0xBD20;
	AxisRscT->PhaseV.PhaseIpF = 0x0000;
	AxisRscT->WeakFV.WfKiV.s[0] = 0x983A;
	AxisRscT->WeakFV.WfKpV.l= 0x0F0008F0;
//	WFKPVH = 0x0F00;
	AxisRscT->WeakFV.WfKiV.l = 0x000006F9;
//	WFKIVH = 0x0000;
	AxisRscT->WeakFV.WfV1Max = 0xCC24;
	AxisRscT->WeakFV.WfIdRefLim = 0x0000;

}


void	SetTrqRef( int i )
{
	AxisRscT->PhaseV.PhaseH = TrqRefIn[i].PHA;
	AxisRscT->PhaseV.PhaseIp = TrqRefIn[i].PHAIP;
	AxisRscT->WeakFV.IdOut = TrqRefIn[i].IDO;
	AxisRscT->IntAdV.IqOut1L.s[0]= TrqRefIn[i].IQO;
	AxisRscT->WeakFV.Vel = TrqRefIn[i].VEL;
	AxisRscT->IntAdP.Kvv = TrqRefIn[i].KVV;
}

typedef struct{
	SHORT	IDO;			/* q軸電流指令(IntAdV.IqOut1L.s[0])				*/
	LONG	IDIL;			/* 弱め界磁用速度(WeakFV.Vel)			*/
	LONG	VDFL;			/* ACRd Filter Output(IAcrV.VdFil.l)			*/
	LONG	VQFL;			/* ACRq Filter Output(AcrV.VqFil.l)			*/
	SHORT	WFVDREF;			/* d軸電圧指令(WeakFV.WfVdRef)			*/
	SHORT	WFVQREF;			/* q軸電圧指令(WeakFV.WfVqRef)			*/
}ADOUT;

//ADOUT ADOutTbl[1000];
ADOUT ADOutTbl[5];
void	GetADOut( int i )
{
	ADOutTbl[i].IDO = AxisRscT->WeakFV.IdOut;
	ADOutTbl[i].IDIL = AxisRscT->AcrV.IdIntgl.l;
	ADOutTbl[i].VDFL = AxisRscT->AcrV.VdFil.l;
	ADOutTbl[i].VQFL = AxisRscT->AcrV.VqFil.l;
	ADOutTbl[i].WFVDREF = AxisRscT->WeakFV.WfVdRef;
	ADOutTbl[i].WFVQREF = AxisRscT->WeakFV.WfVqRef;
}

#ifdef WIN32
void	MakeADOutFile( int i )
{
	FILE *fp;
	char *filename = "ADOut_C.csv";
	int count = 0;
	/* ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ((fp = fopen(filename, "a")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);	/* エラーの場合は通常、異常終了する */
	}
	
	/* ファイルの読み（書き）*/
	if(i==0)
	{
		count = fprintf(fp, "time,WeakFV.IdOut,AcrV.IdIntgl.l,AcrV.VdFil.l,AcrV.VqFil.l,WeakFV.WfVdRef,WeakFV.WfVqRef\n");
	}
	count |= fprintf(fp, "%f,%d,%d,%d,%d,%d,%d\n",
			i*0.125,ADOutTbl[i].IDO, ADOutTbl[i].IDIL, ADOutTbl[i].VDFL, ADOutTbl[i].VQFL, ADOutTbl[i].WFVDREF, ADOutTbl[i].WFVQREF);

	if(count <= 0)
	{
		fprintf(stderr,"write error\n");
		fclose(fp);
		return;
	}
	fclose(fp);	/* ファイルのクローズ */
	
	return ;

}
#endif

/**************************************** end of file ***********************************************/
