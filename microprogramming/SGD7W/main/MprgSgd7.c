/****************************************************************************************************/
/*																									*/
/*																									*/
/*		MprgSgd7.c : Σ-7S/7W 電流制御プログラム													*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		- Main Functions and Interrupts -															*/
/*		  main( )		: 初期化処理 & Round Loop													*/
/*		  MpIntHost( )	: ホスト処理(CPU側ScanA完了を起動タイミングとしてRoundからコール)			*/
/*		  MpIntAD( )	: 電流ループ演算処理(電流検出完了割り込みにより起動)						*/
/*		  MpIntEnc( )	: Safety機能対応用分周出力処理(エンコーダ通信割り込みにより起動)			*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		※注意事項※																				*/
/*																									*/
/*		JL-086用μプログラムはC言語ベースで記述し、専用コンパイラによりコンパイルを行うことで、		*/
/*		機械語(命令テーブル)である「MprgJL086.c」を生成する。										*/
/*		詳細は、【900-***-***】IP Designerインストール、バージョンアップ手順.docを参照。			*/
/*																									*/
/*		μプログラムからアクセスするH/Wレジスタ仕様、アセンブラ命令仕様、イントリンシック関数		*/
/*		仕様については、以下に準ずる。																*/
/*			【RB1400590】マイクロ演算IPソフトウェアマニュアル										*/
/*			【RB1400592】MercuryサーボSoC(JL086A)_サーボIP説明書									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		☆コーディングルール☆																		*/
/*																									*/
/*		①レジスタ定義を編集(追加、削除等)する場合は、必ず「MakeJL086Reg.xls」にて行う。			*/
/*		②キャストは明示的に行う(イントリンシック関数の場合、予期しない動作になる可能性大)。		*/
/*		③シフト演算はコメントに論理シフトか算術シフトかを明記する。「>>」や「<<」を直接記述		*/
/*		  した場合、コンパイラは論理シフトとしてコンパイルする。算術シフトはイントリンシック		*/
/*		  関数を使用する必要がある。※重要※														*/
/*		④通常の四則演算であってもイントリンシック関数を優先して使用する。							*/
/*		⑤コメントは半角英数、全角漢字、かな、カタカナで記述する。全角英数、半角カタカナ禁止。		*/
/*																									*/
/************** Copyright (C) Yaskawa Electric Corporation ******************************************/
/*																									*/
/*		Rev.1.00 : 2014.01.05  Y.Oka		・Σ-Ⅴ-SD Rev.0.0A、Σ-Ⅴ Rev3.15をベースに新規作成	*/
/*											  (Σ-7はMpgDebug_024++++++++をベース)					*/
/*																									*/
/*																									*/
/*																									*/
/****************************************************************************************************/
#include "IxInst.h"
#include "MprgStruct.h"
#include "MpConstTbl.h"


/****************************************************************************************************/
/*																									*/
/*		Version Infomation																			*/
/*																									*/
/****************************************************************************************************/
#define	MSW_VER			0x0001					/* ソフトバージョン設定								*/
#define	TST_VER			0x0000					/* テストバージョン設定								*/
#define	YSP_VER			0x0000					/* Y仕様バージョン設定								*/


/****************************************************************************************************/
/*																									*/
/*		Multi Axis Select Switch for SGD7W															*/
/*																									*/
/****************************************************************************************************/
#define	xxx_MULTI_AXIS							/* 多軸処理有効(SGD7Wの場合「xxx_」を削除)			*/
#ifdef	MULTI_AXIS
#define	MAX_AXIS_NUM	2						/* 最大制御軸数定義									*/
#endif


/****************************************************************************************************/
/*																									*/
/*		H/W Access resister definitions																*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* Standerd definitions																				*/
/*--------------------------------------------------------------------------------------------------*/
int chess_storage(ISA0)	ISA0;					/* 割り込み0ジャンプ先アドレス						*/
int chess_storage(ISA1)	ISA1;					/* 割り込み1ジャンプ先アドレス						*/
int chess_storage(IL) 	INTLVWR;				/* 割込みレベル設定									*/
int chess_storage(EIX)	EIX;					/* 割り込みイネーブル								*/
int chess_storage(DIX)	DIX;					/* 割り込みディスエーブル							*/

/*--------------------------------------------------------------------------------------------------*/
/* Extern definitions																				*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6D0) OUTPT;	/* INT2 Output Port(共通)							*/
extern int chess_storage(PFREG:0x6D1) WDT1L;	/* WDT Trigger Port(共通)							*/
extern int chess_storage(PFREG:0x6D9) HSUR0;	/* ホスト指令ポート0(共通)							*/
extern int chess_storage(PFREG:0x6DA) HSUR1;	/* ホスト指令ポート1(共通)							*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6D2) BBSET;	/* Soft BB and INT1L Enable Setting(Axis1)			*/
extern int chess_storage(PFREG:0x6D3) CRST;		/* PWM Carrier Start & Clock Setting(Axis1)			*/
extern int chess_storage(PFREG:0x6D8) SDMECLR;	/* Decimation filter alarm clear(Axis1)				*/
extern int chess_storage(PFREG:0x6D9) ADSYNC;	/* Sycn current AD cycle(Axis1)						*/
extern int chess_storage(PFREG:0x7D2) BBSET_2;	/* Soft BB and INT1L Enable Setting(Axis2)			*/
extern int chess_storage(PFREG:0x7D3) CRST_2;	/* PWM Carrier Start & Clock Setting(Axis2)			*/
extern int chess_storage(PFREG:0x7D8) SDMECLR_2;/* Decimation filter alarm clear(Axis2)				*/
extern int chess_storage(PFREG:0x7D9) ADSYNC_2;	/* Sycn current AD cycle(Axis2)						*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6D0) IuAD;		/* U相電流フィードバックADC現在値(Axis1)			*/
extern int chess_storage(PFREG:0x6D1) IvAD;		/* V相電流フィードバックADC現在値(Axis1)			*/
extern int chess_storage(PFREG:0x7D0) IuAD_2;	/* U相電流フィードバックADC現在値(Axis2)			*/
extern int chess_storage(PFREG:0x7D1) IvAD_2;	/* V相電流フィードバックADC現在値(Axis2)			*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6DB) PWMOS;	/* PWM出力選択 (Axis1)								*/
extern int chess_storage(PFREG:0x6DF) CRFRQ;	/* PWMキャリア周波数16ビットカウンタ設定 (Axis1)	*/
extern int chess_storage(PFREG:0x7DB) PWMOS_2;	/* PWM出力選択 (Axis2)								*/
extern int chess_storage(PFREG:0x6DF) CRFRQ_2;	/* PWMキャリア周波数16ビットカウンタ設定 (Axis2)	*/
extern int chess_storage(PFREG:0x6E7) PwmT0;	/* PWM三角波比較値 0 (Axis1)						*/
extern int chess_storage(PFREG:0x6E8) PwmT1;	/* PWM三角波比較値 1 (Axis1)						*/
extern int chess_storage(PFREG:0x6E9) PwmT2;	/* PWM三角波比較値 2 (Axis1)						*/
extern int chess_storage(PFREG:0x7E7) PwmT0_2;	/* PWM三角波比較値 0 (Axis2)						*/
extern int chess_storage(PFREG:0x7E8) PwmT1_2;	/* PWM三角波比較値 1 (Axis2)						*/
extern int chess_storage(PFREG:0x7E9) PwmT2_2;	/* PWM三角波比較値 2 (Axis2)						*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6DF) FLTSTAT;	/* 故障入力 (Axis1)									*/
extern int chess_storage(PFREG:0x6E1) FCCDAT;	/* SVIP異常異常状態 (Axis1)							*/
extern int chess_storage(PFREG:0x7DF) FLTSTAT_2;/* 故障入力 (Axis2)									*/
extern int chess_storage(PFREG:0x7E1) FCCDAT_2;	/* SVIP異常異常状態 (Axis2)							*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6F9) DIVSET;	/* 分周機能設定 (Axis1)								*/
extern int chess_storage(PFREG:0x6FA) PCVS0;	/* PWM-パルス変換位置設定 (Axis1)					*/
extern int chess_storage(PFREG:0x6FB) PCVS1;	/* PWM-パルス変換原点補正1設定 (Axis1)				*/
extern int chess_storage(PFREG:0x6BC) PCVS2;	/* PWM-パルス変換原点補正2設定 (Axis1)				*/

/****************************************************************************************************/
/*																									*/
/*		static variable definitions for debug														*/
/*																									*/
/****************************************************************************************************/
static	INITWK		IniWk;
static	COMWORKS	ComWk;

/****************************************************************************************************/
/*																									*/
/*		ProtoType Definitions																		*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* Standerd Functions and Interrupts																*/
/*--------------------------------------------------------------------------------------------------*/
void	MpIntHost( void );										/* ホスト割込みプロトタイプ宣言		*/
void	MpIntAD( void ) property(isr);							/* 電流制御割込みプロトタイプ宣言	*/
void	MpIntEnc( void );										/* エンコーダ割込みプロトタイプ宣言	*/
void	MpDataClear( MICRO_AXIS_HANDLE *AxisRsc );				/* マイクロ用データクリア			*/

/*--------------------------------------------------------------------------------------------------*/
/* Inline Functions																					*/
/*--------------------------------------------------------------------------------------------------*/
inline	USHORT	MpSQRT( ULONG );								/* 平方根演算処理(整数)				*/
inline	SHORT	MpOVMMODK( LONG, SHORT, SHORT, CSHORT* );		/* オーバモジュレーション処理		*/
inline	void	InitSbb( SHORT );								/* soft BB & INT1L設定初期化処理	*/
inline	void	InitPWM( MICRO_AXIS_HANDLE* );					/* PWM設定初期化処理				*/
inline	void	StartPWM( MICRO_AXIS_HANDLE*, SHORT );			/* PWM出力開始処理					*/
inline	void	SetPWM( MICRO_AXIS_HANDLE* );					/* PWM三角波比較値設定処理			*/
inline	void	ChangeCarrierFreq( MICRO_AXIS_HANDLE*, SHORT );	/* キャリア周波数設定処理(各軸)		*/
inline	void	ChangeCarrierFreqAll( MICRO_AXIS_HANDLE* );		/* キャリア周波数設定処理(全軸)		*/
inline	void	SdmErrClrRequest( USHORT );						/* Decimation Filter Error Clear	*/
inline	void	CurAdSyncRequest( void );						/* 電流ADサイクル同期要求			*/
inline	void	ADConvDataLoad( MICRO_AXIS_HANDLE* );			/* 電流検出値取得処理				*/
inline	void	GetSvipErrorSts( MICRO_AXIS_HANDLE* );			/* SVIP異常状態取得処理				*/

/*--------------------------------------------------------------------------------------------------*/
/* 演算ライブラリ																					*/
/*--------------------------------------------------------------------------------------------------*/
/* MlibMulgain32 */
inline	void	IxMulgain32( LONG *x, LONG u, LONG k, DWREG *wk );
/* MlibMulgainNolim */
inline	void	IxMulgainNolim( LONG *x, LONG u, LONG k, DWREG *wk );
/* 1次ローパスフィルタ */
inline	void	IxLpfilter1( LONG *x, LONG u, LONG k, DWREG *wk );
/* 2次ノッチフィルタ */
inline	void	IxNxfilter2( LONG *x, LONG u, LONG k[5], LONG z[4], DWREG wk[4] );
/* 積分演算 */
inline	void	IxIntegral( LONG *x, LONG u, LONG k, LONG iu[2], DWREG *wk );
/* 二乗和演算 */
inline	void	IxSquareSum( DLREG *x, LONG a, LONG b, DWREG *wk );/* <S18E> */


/****************************************************************************************************/
/*																									*/
/*		初期化処理																					*/
/*																									*/
/****************************************************************************************************/
#ifndef IPD_SIM					/* IPDesigner用シミュレーションスイッチ								*/
void	main( void )			/* JL-086に搭載するプログラムを作成する場合はこちらで定義する		*/
#else
void	MpStart( void )			/* コンパイラのみでシミュレーションを行なう場合はこちらで定義する	*/
#endif
{
	USHORT				ax_noR;
	MICRO_AXIS_HANDLE	*AxisRscR;
	LONG				*BlkTrAdr;

	SHORT	BbSetW;											/* Soft BB and INT1L Enable Status		*/
	SHORT	DivSetW;										/* 分周機能設定 						*/
	SHORT	PoSet1W;										/* パルス変換原点補正1 					*/
	SHORT	PoSet2W;										/* パルス変換原点補正2 					*/
	USHORT	uswk;											/* ワークレジスタ 						*/
	
/*--------------------------------------------------------------------------------------------------*/
/*		バージョン設定																				*/
/*--------------------------------------------------------------------------------------------------*/
	VerInfo.MswVer = MSW_VER; 								/* ソフトバージョン設定					*/
	VerInfo.TstVer = TST_VER;								/* テストバージョン設定					*/
	VerInfo.YspVer = YSP_VER;								/* Y仕様バージョン設定					*/

/*--------------------------------------------------------------------------------------------------*/
/*		Initialize Const Variables																	*/
/*--------------------------------------------------------------------------------------------------*/
	True = 0x00000001;										/* True = 1								*/
	False = 0x00000000;										/* False = 0							*/

/*--------------------------------------------------------------------------------------------------*/
/*		Get Axis Number from CPU																	*/
/*--------------------------------------------------------------------------------------------------*/
	AxisNum = AxisHdl[0].AxisInfo.AxisNum;

/*--------------------------------------------------------------------------------------------------*/
/*		Set H/W Register Address Pointer															*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS											/* 多軸処理有効							*/
	for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
	ax_noR = 0;
#endif
	{
		AxisRscR = &(AxisHdl[ax_noR]);
		if( ax_noR == 0 )
		{
			AxisRscR->SvIpRegR = (SVIP_READ_REG*)(0x600);
			AxisRscR->SvIpRegW = (SVIP_WRITE_REG*)(0x600);
		}
		else if( ax_noR == 1 )
		{
			AxisRscR->SvIpRegR = (SVIP_READ_REG*)(0x700);
			AxisRscR->SvIpRegW = (SVIP_WRITE_REG*)(0x700);
		}
	}

/*--------------------------------------------------------------------------------------------------*/
/*		Set Interrupts and Divide Pulse Output Setting												*/
/*--------------------------------------------------------------------------------------------------*/
	if( AxisHdl[0].EncIfV.BitIprm & UPGDIVOUT )
	{ /* μプログラムによる分周出力有効(0軸目のみ処理) */
		/* level(AD=3, INT1=4, HOST=0) */
		INTLVWR = 0x0043;
		ISA0 = (int)MpIntAD;
		ISA1 = (int)MpIntEnc;
		BbSetW = 0x2004;
		InitSbb( BbSetW );

/*--------------------------------------------------------------------------------------------------*/
/*		分周出力関連レジスタ設定																	*/
/*--------------------------------------------------------------------------------------------------*/
		PCVS0 = AxisHdl[0].EncIfV.DivPls.s[0];	/* パルス変換位置 (bit15-0)							*/

		PoSet1W = AxisHdl[0].DivPlsV.PoSet1In;	/* MpUPDATE_DIVPOS()で比較処理があるため残しておく	*/
		PoSet2W = AxisHdl[0].DivPlsV.PoSet2In;	/* MpUPDATE_DIVPOS()で比較処理があるため残しておく	*/
		PCVS1 = PoSet1W;						/* パルス変換原点補正1 (bit15-0)					*/
		PCVS2 = PoSet2W;						/* パルス変換原点補正2 (bit15-0)					*/

		DivSetW = AxisHdl[0].DivPlsV.DivSetIn;	/* MpUPDATE_DIVPOS()で比較処理があるため残しておく	*/
		DIVSET = DivSetW;						/* 分周機能設定 									*/
	}
	else
	{ /* μプログラムによる分周出力無効 */
		/* level(AD=3, INT1=0, HOST=0) */
		INTLVWR = 0x0003;
		ISA0 = (int)MpIntAD;
		BbSetW = 0x0004;
		InitSbb( BbSetW );
	}

/*--------------------------------------------------------------------------------------------------*/
/*		Initilize PWM 																				*/
/*--------------------------------------------------------------------------------------------------*/
	InitPWM( &AxisHdl[0] );

/*--------------------------------------------------------------------------------------------------*/
/*		Initialize variables																		*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS														/* 多軸処理有効				*/
	for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
	ax_noR = 0;
#endif
	{
		AxisRscR = &AxisHdl[ax_noR];

/*--------------------------------------------------------------------------------------------------*/
/*		Initialize Sin and Cos Table																*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscR->SinTbl.Sin1.l = 0x0000;		/* sin(θ)			sin(0)		=  0.000 → 0000h	*/
		AxisRscR->SinTbl.Cos1.l = 0x4000;		/* cos(θ)			cos(0)		=  1.000 → 4000h	*/
		AxisRscR->SinTbl.Sin2.l = 0xC893;		/* sin(θ+2π/3)	sin(2π/3)	= -0.866 → C893h	*/
		AxisRscR->SinTbl.Cos2.l = 0xE000;		/* cos(θ+2π/3)	cos(2π/3)	= -0.500 → E000h	*/
		AxisRscR->SinTbl.Sin3.l = 0x376D;		/* sin(θ-2π/3)	sin(-2π/3)	=  0.866 → 376Dh	*/
		AxisRscR->SinTbl.Cos3.l = 0xE000;		/* cos(θ-2π/3)	cos(-2π/3)	= -0.500 → E000h	*/

/*--------------------------------------------------------------------------------------------------*/
/*		Clear Register																				*/
/*--------------------------------------------------------------------------------------------------*/
		MpDataClear( AxisRscR );

/*--------------------------------------------------------------------------------------------------*/
/*		input CtrlStsIn, DLIM = QLIM = 0, output CtrlStsOut											*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscR->StsFlg.CtrlStsRW = AxisRscR->MicroIf.CtrlStsIn;
		AxisRscR->StsFlg.CtrlStsRW = ( AxisRscR->StsFlg.CtrlStsRW & DLIMI );
		AxisRscR->MicroIf.CtrlStsOut = AxisRscR->StsFlg.CtrlStsRW;
	}

/*--------------------------------------------------------------------------------------------------*/
/*		Start PWM																					*/
/*--------------------------------------------------------------------------------------------------*/
	BbSetW = BbSetW & 0xFFFB;								/* Release Soft BB						*/
	StartPWM( &AxisHdl[0], BbSetW );

/*--------------------------------------------------------------------------------------------------*/
/*		Start Interrupts																			*/
/*--------------------------------------------------------------------------------------------------*/
	EIX = 0x0;												/* Interuput start						*/



/****************************************************************************************************/
/*																									*/
/*		ROUND Procedure																				*/
/*																									*/
/****************************************************************************************************/
#ifndef IPD_SIM										/* IPDesigner用シミュレーションスイッチ			*/
	while (1)
#endif
	{

/*--------------------------------------------------------------------------------------------------*/
/*		A/D error check and clear																	*/
/*--------------------------------------------------------------------------------------------------*/
		GetSvipErrorSts( &AxisHdl[0] );

/*--------------------------------------------------------------------------------------------------*/
/*		Host port check for host INT																*/
/*--------------------------------------------------------------------------------------------------*/
		if ( HSUR0 != 0x0 )
		{
			MpIntHost( );									/* ホスト割込み処理実行					*/
		}

/*--------------------------------------------------------------------------------------------------*/
/*		Host port check for host INT2																*/
/*--------------------------------------------------------------------------------------------------*/
		if ( HSUR1 != 0x0 )
		{
			DIX = 0x0;										/* disable interupt						*/

#ifdef	MULTI_AXIS											/* 多軸処理有効							*/
			for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
			ax_noR = 0;
#endif
			{
				AxisRscR = &AxisHdl[ax_noR];

				/* 位相 & モータ速度 */
				AxisRscR->PhaseV.Phase = AxisRscR->MicroIf.PhaseIn;
				AxisRscR->PhaseV.PhaseBuf = AxisRscR->MicroIf.PhaseIn;
				AxisRscR->Vcmp.MotSpd = AxisRscR->MicroIf.MotSpdIn;
				/* トルク制限値[2^24/MaxTrq] */
				AxisRscR->Trqctrl.TrqLimPlus = AxisRscR->MicroIf.TrqLimPlusIn;
				AxisRscR->Trqctrl.TrqLimMinus = AxisRscR->MicroIf.TrqLimMinusIn;
				/* 外乱トルク */
				AxisRscR->Trqctrl.TrqDistAftFil = AxisRscR->MicroIf.TrqDistAftFilIn;
				/* AVRゲイン */
				AxisRscR->Curctrl.AVRGain = AxisRscR->MicroIf.AVRGainIn;
			}

			EIX = 0x0;										/* enable interupt						*/
		}

/*--------------------------------------------------------------------------------------------------*/
/*		CPU_Roundからの書き込みデータ受け取り処理													*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS											/* 多軸処理有効							*/
		for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
		ax_noR = 0;
#endif
		{
			AxisRscR = &AxisHdl[ax_noR];

			/* 常時変更可能 */
			AxisRscR->CurDet.IuOffset = AxisRscR->MicroIf.IuOffsetIn;
			AxisRscR->CurDet.IvOffset = AxisRscR->MicroIf.IvOffsetIn;
			AxisRscR->CurDet.IuGain = AxisRscR->MicroIf.IuGainIn;
			AxisRscR->CurDet.IvGain = AxisRscR->MicroIf.IvGainIn;

			/* オンラインデータ受け取り */
			if( AxisRscR->BlockTr.TxNumRToAsic != (LONG)ZeroR )
			{	/* 0でない場合、データ取り込みOK */
				BlkTrAdr = AxisRscR->BlockTr.TxDstRToAsic;
				block_cp( (LONG*)BlkTrAdr,									/* データ転送命令		*/
						  (LONG*)&AxisRscR->BlockTr.TxDataRToAsic0,
						  (unsigned int)AxisRscR->BlockTr.TxNumRToAsic );
				AxisRscR->BlockTr.TxNumRToAsic = (LONG)ZeroR;				/* 転送データ数クリア	*/
			}

			/* BB中のみ変更可能 */
			if( AxisRscR->StsFlg.FltStsW & 0x0400 )
			{ /* BB中の場合 */
				/* d軸q軸 比例ゲイン, 積分ゲイン */
				AxisRscR->Curctrl.GainKpd = AxisRscR->MicroIf.GainKpdIn;
				AxisRscR->Curctrl.GainKpq = AxisRscR->MicroIf.GainKpqIn;
				AxisRscR->Curctrl.GainKid = AxisRscR->MicroIf.GainKidIn;
				AxisRscR->Curctrl.GainKiq = AxisRscR->MicroIf.GainKiqIn;

				/* 変調率リミット	*/
				AxisRscR->Vltctrl.Vmax = AxisRscR->MicroIf.VmaxIn;

				/* 弱め界磁 */
				AxisRscR->WeakFV.WfKpv = AxisRscR->MicroIf.WfKpvIn;
				AxisRscR->WeakFV.WfKiv = AxisRscR->MicroIf.WfKivIn;
				AxisRscR->WeakFV.WfV1max = AxisRscR->MicroIf.WfV1maxIn;
				AxisRscR->WeakFV.WfLpfGain = AxisRscR->MicroIf.WfLpfGainIn;
				AxisRscR->WeakFV.WfILimLpfGain = AxisRscR->MicroIf.WfILimLpfGainIn;
			}
		}
	}


	return;	/* Unreachable */
}


/****************************************************************************************************/
/*																									*/
/*		HOST Interupt Procedure																		*/
/*																									*/
/****************************************************************************************************/
void	MpIntHost( void )
{
	USHORT				ax_noH;
	INT64				dlwk;					/* ワークレジスタ(64)								*/
	MICRO_AXIS_HANDLE	*AxisRscH;

	SHORT	swk0;								/* ホスト割り込みワークレジスタ0 SHORT				*/
	SHORT	swk1;								/* ホスト割り込みワークレジスタ2 SHORT				*/
	LONG	lwk1;								/* ホスト割り込みワークレジスタ1 LONG				*/
	LONG	lwk2;								/* ホスト割り込みワークレジスタ2 LONG				*/
	LONG	lwk3;								/* ホスト割り込みワークレジスタ3 LONG				*/
	DWREG	Ixlwk[4];							/* ホスト割り込み演算ライブラリ用レジスタ			*/

	IniWk.IN_WK0++;		/* for debug counter */
	
	WDT1L = 0x1;								/* Watch dog set									*/
//	OUTPT = 0x1;								/* デバッグ用										*/

/*--------------------------------------------------------------------------------------------------*/
/*		キャリア周波数切り替え処理																	*/
/*--------------------------------------------------------------------------------------------------*/
	ChangeCarrierFreqAll( &AxisHdl[0] );

/*--------------------------------------------------------------------------------------------------*/
/*		input from host																				*/
/*--------------------------------------------------------------------------------------------------*/
	DIX = 0x0;									/* disable interupt									*/

#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else
	ax_noH = 0;
#endif
	{
		AxisRscH = &AxisHdl[ax_noH];

		/* 位相 & モータ速度 */
		AxisRscH->PhaseV.Phase = AxisRscH->MicroIf.PhaseIn;
		AxisRscH->PhaseV.PhaseBuf = AxisRscH->MicroIf.PhaseIn;
		AxisRscH->Vcmp.MotSpd = AxisRscH->MicroIf.MotSpdIn;
		/* トルク制限値[2^24/MaxTrq] */
		AxisRscH->Trqctrl.TrqLimPlus = AxisRscH->MicroIf.TrqLimPlusIn;
		AxisRscH->Trqctrl.TrqLimMinus = AxisRscH->MicroIf.TrqLimMinusIn;
		/* 外乱トルク, トルクFF, リップル補償トルク[2^24/MaxTrq] */
		AxisRscH->Trqctrl.TrqDistAftFil = AxisRscH->MicroIf.TrqDistAftFilIn;
		AxisRscH->Trqctrl.TrqFF = AxisRscH->MicroIf.TrqFFIn;
		AxisRscH->Trqctrl.RippleComp = AxisRscH->MicroIf.RippleCompIn;
		/* AVRゲイン */
		AxisRscH->Curctrl.AVRGain = AxisRscH->MicroIf.AVRGainIn;
		/* d軸, q軸電圧指令(通常不使用) */
//		AxisRscH->VcmpV.VdRef = AxisRscH->AdinV.VdRefIn; /* 削除 */
//		AxisRscH->VcmpV.VqRef = AxisRscH->AdinV.VqRefIn; /* 削除 */
	}

	EIX = 0x0;									/* enable interupt									*/


/*--------------------------------------------------------------------------------------------------*/
/*		Carrier Freq Change check : if( status & BB ) Carrier Freq. change							*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else
	ax_noH = 0;
#endif
	{
		AxisRscH = &AxisHdl[ax_noH];
		if ( AxisRscH->MicroIf.FccRst != 0)
		{
			SdmErrClrRequest( ax_noH );
			AxisRscH->StsFlg.ADRst = AxisRscH->MicroIf.FccRst;
			AxisRscH->MicroIf.FccRst = 0;
			IniWk.IN_WK0H++;		/* for debug counter */
		}
	}

/*--------------------------------------------------------------------------------------------------*/
/*		Sync current A/D Cycle																		*/
/*--------------------------------------------------------------------------------------------------*/
	CurAdSyncRequest( );

/*--------------------------------------------------------------------------------------------------*/
/*		data clear while BB																			*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else		/* ↑#ifdef	MULTI_AXIS */
	ax_noH = 0;
#endif		/* ↑#ifdef	MULTI_AXIS */
	{
		AxisRscH = &AxisHdl[ax_noH];
		if( AxisRscH->MicroIf.CtrlStsIn & BB )
		{ /* BB中の場合 */
			DIX = 0x0;							/* disable interupt									*/

			/* 制御用変数初期化 */
			MpDataClear( AxisRscH );
			/* キャリア周波数変更 */
			ChangeCarrierFreq( AxisRscH, ax_noH );

			EIX = 0x0;							/* enable interupt									*/
		}
		else
		{ /* BE中の場合 */
/*--------------------------------------------------------------------------------------------------*/
/*		トルクFF入力足し込み																		*/
/*--------------------------------------------------------------------------------------------------*/
			AxisRscH->Trqctrl.TrqBfrComp = AxisRscH->MicroIf.TrqrefIn + AxisRscH->Trqctrl.TrqFF;

/*--------------------------------------------------------------------------------------------------*/
/*		リップル補正トルク足し込み																	*/
/*--------------------------------------------------------------------------------------------------*/
			AxisRscH->Trqctrl.TrqAftComp = AxisRscH->Trqctrl.TrqBfrComp + AxisRscH->Trqctrl.RippleComp;

/*--------------------------------------------------------------------------------------------------*/
/* 		notch filter 1st																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL1) == 0 )
			{ /* フィルタ処理なし */
				/* フィルタ素通り */
				AxisRscH->NotchFil.Notch1Out = AxisRscH->Trqctrl.TrqAftComp;
				/* データクリア */
				AxisRscH->NotchFil.Notch1Value0 = ZeroR;
				AxisRscH->NotchFil.Notch1Value1 = ZeroR;
				AxisRscH->NotchFil.Notch1Value2 = ZeroR;
				AxisRscH->NotchFil.Notch1Value3 = ZeroR;
			}
			else
			{
				IxNxfilter2( &AxisRscH->NotchFil.Notch1Out,
							 AxisRscH->Trqctrl.TrqAftComp,
							 &AxisRscH->NotchFil.Notch1Gain0,
							 &AxisRscH->NotchFil.Notch1Value0,
							 Ixlwk );
			}

/*--------------------------------------------------------------------------------------------------*/
/*		ノッチフィルタ2段目																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL2) == 0 )
			{ /* フィルタ素通り */
				AxisRscH->NotchFil.Notch2Out = AxisRscH->NotchFil.Notch1Out;
				/* データクリア */
				AxisRscH->NotchFil.Notch2Value0 = ZeroR;
				AxisRscH->NotchFil.Notch2Value1 = ZeroR;
				AxisRscH->NotchFil.Notch2Value2 = ZeroR;
				AxisRscH->NotchFil.Notch2Value3 = ZeroR;
			}
			else
			{
				IxNxfilter2( &AxisRscH->NotchFil.Notch2Out,
							 AxisRscH->NotchFil.Notch1Out,
							 &AxisRscH->NotchFil.Notch2Gain0,
							 &AxisRscH->NotchFil.Notch2Value0,
							 Ixlwk );
			}

/*--------------------------------------------------------------------------------------------------*/
/*		ノッチフィルタ3段目																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL3) == 0 )
			{ /* フィルタ素通り */
				AxisRscH->NotchFil.Notch3Out = AxisRscH->NotchFil.Notch2Out;
				/* データクリア */
				AxisRscH->NotchFil.Notch3Value0 = ZeroR;
				AxisRscH->NotchFil.Notch3Value1 = ZeroR;
				AxisRscH->NotchFil.Notch3Value2 = ZeroR;
				AxisRscH->NotchFil.Notch3Value3 = ZeroR;
			}
			else
			{
				IxNxfilter2( &AxisRscH->NotchFil.Notch3Out,
							 AxisRscH->NotchFil.Notch2Out,
							 &AxisRscH->NotchFil.Notch3Gain0,
							 &AxisRscH->NotchFil.Notch3Value0,
							 Ixlwk );
			}

/*--------------------------------------------------------------------------------------------------*/
/*		ノッチフィルタ4段目																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL4) == 0 )
			{ /* フィルタ素通り */
				AxisRscH->NotchFil.Notch4Out = AxisRscH->NotchFil.Notch3Out;
				/* データクリア */
				AxisRscH->NotchFil.Notch4Value0 = ZeroR;
				AxisRscH->NotchFil.Notch4Value1 = ZeroR;
				AxisRscH->NotchFil.Notch4Value2 = ZeroR;
				AxisRscH->NotchFil.Notch4Value3 = ZeroR;
			}
			else
			{
				IxNxfilter2( &AxisRscH->NotchFil.Notch4Out,
							 AxisRscH->NotchFil.Notch3Out,
							 &AxisRscH->NotchFil.Notch4Gain0,
							 &AxisRscH->NotchFil.Notch4Value0,
							 Ixlwk );
			}

/*--------------------------------------------------------------------------------------------------*/
/*		ノッチフィルタ5段目																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL5) == 0 )
			{ /* フィルタ素通り */
				AxisRscH->NotchFil.Notch5Out = AxisRscH->NotchFil.Notch4Out;
				/* データクリア */
				AxisRscH->NotchFil.Notch5Value0 = ZeroR;
				AxisRscH->NotchFil.Notch5Value1 = ZeroR;
				AxisRscH->NotchFil.Notch5Value2 = ZeroR;
				AxisRscH->NotchFil.Notch5Value3 = ZeroR;
			}
			else
			{
				IxNxfilter2( &AxisRscH->NotchFil.Notch5Out,
							 AxisRscH->NotchFil.Notch4Out,
							 &AxisRscH->NotchFil.Notch5Gain0,
							 &AxisRscH->NotchFil.Notch5Value0,
							 Ixlwk );
			}

/*--------------------------------------------------------------------------------------------------*/
/*		ノッチフィルタ6段目(モータ共振抑制用)														*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL6) == 0 )
			{ /* フィルタ素通り */
				AxisRscH->NotchFil.Notch6Out = AxisRscH->NotchFil.Notch5Out;
				/* データクリア */
				AxisRscH->NotchFil.Notch6Value0 = ZeroR;
				AxisRscH->NotchFil.Notch6Value1 = ZeroR;
				AxisRscH->NotchFil.Notch6Value2 = ZeroR;
				AxisRscH->NotchFil.Notch6Value3 = ZeroR;
			}
			else
			{
				IxNxfilter2( &AxisRscH->NotchFil.Notch6Out,
							 AxisRscH->NotchFil.Notch5Out,
							 &AxisRscH->NotchFil.Notch6Gain0,
							 &AxisRscH->NotchFil.Notch6Value0,
							 Ixlwk );
			}
			AxisRscH->Trqctrl.TrqAftFil = AxisRscH->NotchFil.Notch6Out;

/*--------------------------------------------------------------------------------------------------*/
/*		外乱トルク加算																				*/
/*--------------------------------------------------------------------------------------------------*/
			AxisRscH->Trqctrl.TrqAddDist = AxisRscH->Trqctrl.TrqAftFil + AxisRscH->Trqctrl.TrqDistAftFil;
		}

/*--------------------------------------------------------------------------------------------------*/
/*		制御ステータス処理(トルク制限中フラグクリア)												*/
/*--------------------------------------------------------------------------------------------------*/
		DIX = 0x0;											/* disable interupt						*/
		AxisRscH->StsFlg.CtrlStsRW = AxisRscH->MicroIf.CtrlStsIn;
		AxisRscH->StsFlg.CtrlStsRW = ( AxisRscH->StsFlg.CtrlStsRW & TLIMI );
		EIX = 0x0;											/* enable interupt						*/


/****************************************************************************************************/
/*		用途選択																					*/
/****************************************************************************************************/
		if( AxisRscH->MotInfo.MotUse == SERVO  )
		{
/****************************************************************************************************/
/*		用途：サーボ																				*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		トルクリミット																				*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscH->Trqctrl.TrqAddDist >= ZeroR )
			{	/* 正側トルクリミット */
				AxisRscH->Trqctrl.TrqAftLim = limit( AxisRscH->Trqctrl.TrqAddDist,
													 AxisRscH->Trqctrl.TrqLimPlus );
				if( AxisRscH->Trqctrl.TrqAftLim == AxisRscH->Trqctrl.TrqLimPlus )
				{
					AxisRscH->StsFlg.CtrlStsRW |= TLIM;					/* トルク制限中フラグセット	*/
				}
			}
			else
			{	/* 負側トルクリミット */
				AxisRscH->Trqctrl.TrqAftLim = limit( AxisRscH->Trqctrl.TrqAddDist,
													 AxisRscH->Trqctrl.TrqLimMinus );
				if( (AxisRscH->Trqctrl.TrqAftLim + AxisRscH->Trqctrl.TrqLimMinus) == 0 )
				{
					AxisRscH->StsFlg.CtrlStsRW |= TLIM;					/* トルク制限中フラグセット	*/
				}
			}
		}
		else
		{
/****************************************************************************************************/
/*		用途：主軸																					*/
/****************************************************************************************************/
			// 未対応
		}

/****************************************************************************************************/
/*		モータ種類選択																				*/
/****************************************************************************************************/
		if( AxisRscH->MotInfo.MotTypeSMIM == IM )
		{
/****************************************************************************************************/
/*		インダクションモータ用処理																	*/
/****************************************************************************************************/
			// 未対応
		}
		else
		{
/****************************************************************************************************/
/*		同期機用処理																				*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		CPU_ScanCからの書き込みデータ受け取り処理													*/
/*--------------------------------------------------------------------------------------------------*/
//IPMMの場合
//IdrefLim(LONG), IqrefLim(LONG), CtrlMode(SHORT), MaxCurLimInput(LONG)
			AxisRscH->Systems.DebugCtr1 = AxisRscH->BlockTr.TxNumCToAsic;
			AxisRscH->Systems.DebugCtr2 = (SHORT)ZeroR;
			if( AxisRscH->BlockTr.TxNumCToAsic != (SHORT)ZeroR )
			{	/* CPU側がデータ書き込み完了のため、マイクロレジスタに渡す */
				DIX = 0x0;											/* disable interupt				*/

				AxisRscH->MicroIf.IdrefLimIn = AxisRscH->BlockTr.TxDataCToAsic0.l;
				AxisRscH->MicroIf.IqrefLimIn = AxisRscH->BlockTr.TxDataCToAsic1.l;
				AxisRscH->MicroIf.WfCtrlModeIn = AxisRscH->BlockTr.TxDataCToAsic2.s[0];
				AxisRscH->MicroIf.MaxCurLimIn = AxisRscH->BlockTr.TxDataCToAsic3.l;
				AxisRscH->BlockTr.TxNumCToAsic = (SHORT)ZeroR;

				AxisRscH->Systems.DebugCtr1++;

				EIX = 0x0;											/* enable interupt				*/
			}

/*--------------------------------------------------------------------------------------------------*/
/*		d,q軸電流指令リミットローパスフィルタ処理													*/
/*		弱め界磁制御電流リミットローパスフィルタ処理												*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.CtrlSwitch & V_FB) != ZeroR )
			{
				/* WfIdrefLimLpf = フィルタ出力 */
				IxLpfilter1( &AxisRscH->WeakFV.WfIdrefLimLpf,
							 AxisRscH->MicroIf.IdrefLimIn, 
							 AxisRscH->WeakFV.WfILimLpfGain,
							 Ixlwk );

				/* WfIqrefLimLpf = フィルタ出力 */
//				IxLpfilter1( &AxisRscH->WeakFV.WfIqrefLimLpf,
//							 AxisRscH->MicroIf.IqrefLimIn,
//							 AxisRscH->WeakFV.WfILimLpfGain,
//							 Ixlwk );

				/* MaxCurLimOutput = フィルタ出力 */
				IxLpfilter1( &AxisRscH->WeakFV.MaxCurLimLpf,
							 AxisRscH->MicroIf.MaxCurLimIn,
							 AxisRscH->WeakFV.WfILimLpfGain,
							 Ixlwk );

				DIX = 0x0;											/* disable interupt				*//* <S18E> */
				AxisRscH->WeakFV.WfIdrefLim = AxisRscH->WeakFV.WfIdrefLimLpf;
//				AxisRscH->WeakFV.WfIqrefLim, AxisRscH->WeakFV.WfIqrefLimLpf );
				AxisRscH->MotInfo.MaxCurLim = AxisRscH->WeakFV.MaxCurLimLpf;
				EIX = 0x0;											/* enable interupt				*//* <S18E> */
			}

/*--------------------------------------------------------------------------------------------------*/
/*		d軸電流指令計算																				*/
/*--------------------------------------------------------------------------------------------------*/
			lwk1 = limit( AxisRscH->Trqctrl.TrqAftLim, (LONG)1 );						/* wkIntHost9 = sign(TrqAftLim) */
			lwk1 = AxisRscH->Trqctrl.TrqAftLim * lwk1;									/* wkIntHost9 = |Trqref|	*/

			lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq3;
			if( lwk1 >= AxisRscH->IntAdV.MaxTrq3 )
			{	/* |Trqref| >= MaxTrq[3] の時 */
				IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdE, Ixlwk );				/* wkIntHost0 = IdE*(|Trqref|-MaxTrq[3])	*/
				lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt3 );					/* wkIntHost0 = IdE*(|Trqref|-MaxTrq[3]) + IdrefOpt[3]	*/
				lwk3 = limitz( lwk2, (LONG)No24bitM );									/* wkIntHost1 = limit( wkIntHost0, -2^24～0 )	*/
			}
			else
			{
				lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq2;
				if( lwk1 >= AxisRscH->IntAdV.MaxTrq2 )
				{	/* |Trqref| >= MaxTrq[2] の時 */
					IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdD, Ixlwk );			/* wkIntHost0 = IdD*(|Trqref|-MaxTrq[2])	*/
					lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt2 );				/* wkIntHost0 = IdD*(|Trqref|-MaxTrq[2]) + IdrefOpt[2]	*/
					lwk3 = limitz( lwk2, (LONG)No24bitM );								/* wkIntHost1 = limit( wkIntHost0, -2^24～0 )	*/
				}
				else
				{
					lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq1;
					if( lwk1 >= AxisRscH->IntAdV.MaxTrq1 )
					{	/* |Trqref| >= MaxTrq[1] の時 */
						IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdC, Ixlwk );		/* wkIntHost0 = IdC*(|Trqref|-MaxTrq[1])	*/
						lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt1 );			/* wkIntHost0 = IdC*(|Trqref|-MaxTrq[1]) + IdrefOpt[1]	*/
						lwk3 = limitz( lwk2, (LONG)No24bitM );							/* wkIntHost1 = limit( wkIntHost0, -2^24～0 )	*/
					}
					else
					{
						lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq0;
						if( lwk1 >= AxisRscH->IntAdV.MaxTrq0 )
						{	/* |Trqref| >= MaxTrq[0] の時 */
							IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdB, Ixlwk );	/* wkIntHost0 = IdB*(|Trqref|-MaxTrq[0])	*/
							lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt0 );		/* wkIntHost0 = IdB*(|Trqref|-MaxTrq[0]) + IdrefOpt[0]	*/
							lwk3 = limitz( lwk2, (LONG)No24bitM );						/* wkIntHost1 = limit( wkIntHost0, -2^24～0 )	*/
						}
						else
						{	/* |Trqref| < MaxTrq[0] の時 */
							IxMulgain32( &lwk2, lwk1, AxisRscH->IntAdV.IdA, Ixlwk );	/* wkIntHost0 = IdA*|Trqref|	*/
							lwk3 = limitz( lwk2, (LONG)No24bitM );						/* wkIntHost1 = limit( wkIntHost0, -2^24～0 )	*/
						}
					}
				}
			}
/*--------------------------------------------------------------------------------------------------*/
/*		磁極検出中のd軸電流指令																		*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscH->IntAdV.PhaseReady == False )
			{
				/* モータ位相検出未完中(磁極検出中)はd軸電流指令をゼロにする。 */
				lwk3 = (LONG)ZeroR;
			}
		}

/*--------------------------------------------------------------------------------------------------*/
/*		誘起電圧計算																				*/
/*--------------------------------------------------------------------------------------------------*/
		IxMulgain32( &lwk1, AxisRscH->Vcmp.MotSpd, AxisRscH->Vcmp.Phi, Ixlwk );			/* wkIntHost5 = ωφ0				*/
	}

/*--------------------------------------------------------------------------------------------------*/
/*		データをINT_ADに転送																		*/
/*--------------------------------------------------------------------------------------------------*/
	DIX = 0x0;												/* disable interupt						*/

#ifdef	MULTI_AXIS											/* 多軸処理有効							*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else
	ax_noH = 0;
#endif
	{
		AxisRscH = &AxisHdl[ax_noH];

		AxisRscH->Trqctrl.Trqref = AxisRscH->Trqctrl.TrqAftLim;		/* トルク指令を代入				*/
		AxisRscH->Curctrl.IdrefBeforeWF = lwk3;						/* Id*を代入					*/
		AxisRscH->Vcmp.OmegaPhi = lwk1;								/* ωφを代入					*/

		if( (AxisRscH->MicroIf.CtrlSwitch & V_FB) == 0 )
		{ /* 弱め界磁無効 => 位相補償の場合(Σ-Ⅲ以前のモータ) */
			AxisRscH->WeakFV.WfIdref = AxisRscH->MicroIf.IdrefIn;	/* WeakFV.WfIdref(reference)	*/
		}
	}

	EIX = 0x0;												/* enable interupt						*/

/****************************************************************************************************/
/*		Data Output for CPU																			*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS											/* 多軸処理有効							*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else
	ax_noH = 0;
#endif
	{
		AxisRscH = &AxisHdl[ax_noH];

		AxisRscH->MicroIf.TrqMon = AxisRscH->Trqctrl.Trqref;
		AxisRscH->MicroIf.TrqMonAftFil = AxisRscH->Trqctrl.TrqAftFil;
		AxisRscH->MicroIf.TrqMonBfrComp = AxisRscH->Trqctrl.TrqBfrComp;
		AxisRscH->MicroIf.IdrefMon = AxisRscH->Curctrl.Idref;
		AxisRscH->MicroIf.IqrefMon = AxisRscH->Curctrl.Iqref;
		AxisRscH->MicroIf.IdDetectMon = AxisRscH->Curctrl.IdDetect;
		AxisRscH->MicroIf.IqDetectMon = AxisRscH->Curctrl.IqDetect;
		AxisRscH->MicroIf.VdrefMon = AxisRscH->Curctrl.Vdref;
		AxisRscH->MicroIf.VqrefMon = AxisRscH->Curctrl.Vqref;

		AxisRscH->Systems.IntHostCtr++;
	}

	return;
}



/****************************************************************************************************/
/*																									*/
/*		AD Interupt Procedure																		*/
/*																									*/
/****************************************************************************************************/
void	MpIntAD( void ) property(isr)
{
	USHORT				ax_noI;
	INT64				dlwk;
	MICRO_AXIS_HANDLE	*AxisRscI;

	/* Local Variables */
	CSHORT*	pCtbl;								/* テーブルポインタ用ワークレジスタ					*/
	SHORT	swk0;								/* 16bitワークレジスタ0								*/
	SHORT	swk1;								/* 16bitワークレジスタ1								*/
	SHORT	swk2;								/* 16bitワークレジスタ2								*/
	SHORT	swk3;								/* 16bitワークレジスタ3								*/
	SHORT	swk4;								/* 16bitワークレジスタ4								*/
	SHORT	swk5;								/* 16bitワークレジスタ5								*/
	SHORT	swk6;								/* 16bitワークレジスタ6								*/
	SHORT	swk7;								/* 16bitワークレジスタ7								*/
	SHORT	swk8;								/* 16bitワークレジスタ8								*/
	SHORT	swk9;								/* 16bitワークレジスタ9								*/
	SHORT	swk10;								/* 16bitワークレジスタ10							*/
	SHORT	swk11;								/* 16bitワークレジスタ11							*/
	DWREG	lwk0;								/* 32bitワークレジスタ0								*/
	DWREG	lwk1;								/* 32bitワークレジスタ1								*/
	DWREG	lwk2;								/* 32bitワークレジスタ2								*/
	DWREG	lwk3;								/* 32bitワークレジスタ2								*/
	DWREG	lwk4;								/* 32bitワークレジスタ4								*/
	DWREG	lwk5;								/* 32bitワークレジスタ2								*/
	DWREG	lwk6;								/* 32bitワークレジスタ6								*/
	DWREG	lwk7;								/* 32bitワークレジスタ7								*/
	DWREG	lwk8;								/* 32bitワークレジスタ8								*/
	DWREG	lwk9;								/* 32bitワークレジスタ9								*/
	DWREG	Ixlwk[4];							/* 32bit演算ライブラリ用レジスタ					*/
	DLREG	dlwk0;								/* 64bitワークレジスタ0								*/
	OUTPT = 0x1;
	WDT1L = 0x0;								/* Watch dog reset									*/
	IniWk.IN_WK1++;								/* for debug counter								*/

/*--------------------------------------------------------------------------------------------------*/
/*		自己割り込み禁止設定																		*/
/*--------------------------------------------------------------------------------------------------*/
	INTLVWR &= 0x00F0;


/****************************************************************************************************/
/*		Current Control Input Procedure																*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noI = 0; (SHORT)ax_noI < AxisNum; ax_noI++ )
#else
	ax_noI = 0;
#endif
	{
		AxisRscI = &AxisHdl[ax_noI];
/*--------------------------------------------------------------------------------------------------*/
/*		位相補間処理																				*/
/*--------------------------------------------------------------------------------------------------*/
		if( AxisRscI->PhaseV.PhaseBuf == AxisRscI->PhaseV.PhaseBufLast )
		{ /* 位相補間する */
			AxisRscI->PhaseV.Phase += AxisRscI->PhaseV.PhaseItplt;			/* 位相補間量を足し込む	*/
		}
		else
		{ /* 位相補間しない */
			IxMulgainNolim( &lwk0.l, AxisRscI->Vcmp.MotSpd, AxisRscI->PhaseV.CnvSpdToPhase, Ixlwk );
			AxisRscI->PhaseV.PhaseItplt = lwk0.s[0];						/* PhaseItplt = 位相補間量	*/
			AxisRscI->PhaseV.PhaseBufLast = AxisRscI->PhaseV.PhaseBuf;
		}

/*--------------------------------------------------------------------------------------------------*/
/*		sin, cos計算処理																			*/
/*--------------------------------------------------------------------------------------------------*/
/*		sin1 = sinθ, sin2 = sin(θ-2π/3), sin3 = sin(θ+2π/3) 									*/
/*		cos1 = cosθ, cos2 = cos(θ-2π/3), cos3 = cos(θ+2π/3) 									*/
/*--------------------------------------------------------------------------------------------------*/
		lwk0.s[0] = AxisRscI->PhaseV.Phase;						/* lwk0.s[0] = Phase				*/
		lwk0.s[0] += 32;
		lwk1.s[0] = (SHORT)PI23;
		lwk2.s[0] = lwk1.s[0] + lwk0.s[0];						/* lwk2.s[0] = Phase + 2π/3		*/
		lwk3.s[0] = lwk0.s[0] - lwk1.s[0];						/* lwk3.s[0] = Phase - 2π/3		*/

		lwk4.s[0] = lwk0.us[0] >> 6;							/* lwk4.s[0] = lwk0.s[0] >> 6(論理)	*/
		IxTblSin16( AxisRscI->SinTbl.Sin1.s[0], lwk4.s[0] );	/* sin1.sr = SinTbl[ lwk4.s[0] ]	*/
		lwk0.s[0] += (SHORT)PI2;								/* lwk0.s[0] = lwk0.s[0] + π/2		*/
		lwk4.s[0] = lwk0.us[0] >> 6;							/* lwk4.s[0] = lwk0.s[0] >> 6(論理)	*/
		IxTblSin16( AxisRscI->SinTbl.Cos1.s[0], lwk4.s[0] );	/* cos1.sr = SinTbl[ lwk4.s[0] ]	*/

		lwk4.s[0] = lwk3.us[0] >> 6;							/* lwk4.s[0] = lwk3.s[0] >> 6論理)	*/
		IxTblSin16( AxisRscI->SinTbl.Sin2.s[0], lwk4.s[0] );	/* sin2.sr = SinTbl[ lwk4.s[0] ]	*/
		lwk3.s[0] += (SHORT)PI2;								/* lwk3.s[0] = lwk3.s[0] + π/2	*/
		lwk4.s[0] = lwk3.us[0] >> 6;							/* lwk4.s[0] = lwk3.s[0] >> 6論理)	*/
		IxTblSin16( AxisRscI->SinTbl.Cos2.s[0], lwk4.s[0] );	/* cos2.sr = SinTbl[ lwk4.s[0] ]	*/

		lwk4.s[0] = lwk2.us[0] >> 6;							/* lwk4.s[0] = lwk2.s[0] >> 6論理)	*/
		IxTblSin16( AxisRscI->SinTbl.Sin3.s[0], lwk4.s[0] );	/* sin3.sr = SinTbl[ lwk4.s[0] ]	*/
		lwk2.s[0] += (SHORT)PI2;								/* lwk2.s[0] = lwk2.s[0] + π/2	*/
		lwk4.s[0] = lwk2.s[0] >> 6;								/* lwk4.s[0] = lwk2.s[0] >> 6論理)	*/
		IxTblSin16( AxisRscI->SinTbl.Cos3.s[0], lwk4.s[0] );	/* cos3.sr = SinTbl[ lwk4.s[0] ]	*/

		AxisRscI->SinTbl.Sin1.l = AxisRscI->SinTbl.Sin1.s[0];	/* 16bit→32bit(符号付)				*/
		AxisRscI->SinTbl.Sin2.l = AxisRscI->SinTbl.Sin2.s[0];	/* 16bit→32bit(符号付)				*/
		AxisRscI->SinTbl.Sin3.l = AxisRscI->SinTbl.Sin3.s[0];	/* 16bit→32bit(符号付)				*/
		AxisRscI->SinTbl.Cos1.l = AxisRscI->SinTbl.Cos1.s[0];	/* 16bit→32bit(符号付)				*/
		AxisRscI->SinTbl.Cos2.l = AxisRscI->SinTbl.Cos2.s[0];	/* 16bit→32bit(符号付)				*/
		AxisRscI->SinTbl.Cos3.l = AxisRscI->SinTbl.Cos3.s[0];	/* 16bit→32bit(符号付)				*/
	}

/*--------------------------------------------------------------------------------------------------*/
/*		A/D convert data loading																	*/
/*--------------------------------------------------------------------------------------------------*/
	ADConvDataLoad( &AxisHdl[0] );

#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noI = 0; (SHORT)ax_noI < AxisNum; ax_noI++ )
#else
	ax_noI = 0;
#endif
	{
		AxisRscI = &AxisHdl[ax_noI];
/*--------------------------------------------------------------------------------------------------*/
/*		dq-trans(UVW to DQ)																			*/
/*--------------------------------------------------------------------------------------------------*/
/*		IdDetect = ((cos - cos3) * IuDetect / 2^14 + (cos2 - cos3) * IvDetect / 2^14) * 2 / 3		*/
/*		IqDetect = ((sin3 - sin) * IuDetect / 2^14 + (sin3 - sin2) * IvDetect / 2^14) * 2 / 3		*/
/*		IdDetect = (cos - cos3) * IuDetTmp / 2^14 + (cos2 - cos3) * IvDetTmp / 2^14					*/
/*		IqDetect = (sin3 - sin) * IuDetTmp / 2^14 + (sin3 - sin2) * IvDetTmp / 2^14					*/
/*--------------------------------------------------------------------------------------------------*/
		/* lwk1.l = (cos - cos3) * IuDetTmp / 2^14 (算術) */
		lwk1.l = mulshr( (AxisRscI->SinTbl.Cos1.l - AxisRscI->SinTbl.Cos3.l),
						 AxisRscI->CurDet.IuDetTmp,
						 14 );
		/* lwk2.l = (cos2 - cos3) * IvDetTmp / 2^14 (算術) */
		lwk2.l = mulshr( (AxisRscI->SinTbl.Cos2.l - AxisRscI->SinTbl.Cos3.l),
						 AxisRscI->CurDet.IvDetTmp,
						 14 );
		/* IdDetect = (cos - cos3) * IuDetTmp / 2^14 + (cos2 - cos3) * IvDetTmp / 2^14 (算術) */
		AxisRscI->Curctrl.IdDetect = lwk1.l + lwk2.l;
/*--------------------------------------------------------------------------------------------------*/
		/* lwk1.l = (sin3 - sin) * IuDetTmp / 2^14 (算術) */
		lwk1.l = mulshr( (AxisRscI->SinTbl.Sin3.l - AxisRscI->SinTbl.Sin1.l),
						 AxisRscI->CurDet.IuDetTmp,
						 14 );
		/* lwk2.l = (sin3 - sin2) * IvDetTmp / 2^14 (算術) */
		lwk2.l = mulshr( (AxisRscI->SinTbl.Sin3.l - AxisRscI->SinTbl.Sin2.l),
						 AxisRscI->CurDet.IvDetTmp,
						 14 );
		/* IqDetect = (sin3 - sin) * IuDetTmp / 2^14 + (sin3 - sin2) * IvDetTmp / 2^14 (算術) */
		AxisRscI->Curctrl.IqDetect = lwk1.l + lwk2.l;
	}


/****************************************************************************************************/
/*		Current Control Main Procedure																*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noI = 0; (SHORT)ax_noI < AxisNum; ax_noI++ )
#else
	ax_noI = 0;
#endif
	{
		AxisRscI = &AxisHdl[ax_noI];
/*--------------------------------------------------------------------------------------------------*/
/*		Current Observer																			*/
/*--------------------------------------------------------------------------------------------------*/
		/* 電流オブザーバ未対応 ⇒ 電流位相補償オブザーバに変更予定

/*--------------------------------------------------------------------------------------------------*/
/*		Base Block Check																			*/
/*--------------------------------------------------------------------------------------------------*/
		if( AxisRscI->StsFlg.ADRst != False )
		{
			AxisRscI->StsFlg.ADRst = 0;
			AxisRscI->PwmV.PwmCntT2 = (USHORT)AxisRscI->IntAdV.CarrierFreq >> 1;
			AxisRscI->PwmV.PwmCntT1 = AxisRscI->PwmV.PwmCntT2;
			AxisRscI->PwmV.PwmCntT0 = AxisRscI->PwmV.PwmCntT2;
/*--------------------------------------------------------------------------------------------------*/
		}
		else if( (AxisRscI->StsFlg.CtrlStsRW & BB) != False )
		{
			AxisRscI->PwmV.PwmCntT2 = (USHORT)AxisRscI->IntAdV.CarrierFreq >> 1;
			AxisRscI->PwmV.PwmCntT1 = AxisRscI->PwmV.PwmCntT2;
			AxisRscI->PwmV.PwmCntT0 = AxisRscI->PwmV.PwmCntT2;
/*--------------------------------------------------------------------------------------------------*/
		}
		else
		{
			if( AxisRscI->MotInfo.MotTypeSMIM == IM )
			{
/****************************************************************************************************/
/*		インダクションモータ用処理																	*/
/****************************************************************************************************/
				; // 未対応
			}
			else
			{
/****************************************************************************************************/
/*																									*/
/*		同期機用処理																				*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		磁束φ飽和計算		KtPercent = (φMg/φMg0) * 2^14											*/
/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = limit( AxisRscI->Curctrl.Iqref, (LONG)OneR );		/* lwk0 = sign(Iqref)	*/
				lwk0.l = lwk0.l * AxisRscI->Curctrl.Iqref;					/* lwk0 = |Iqref|		*/

				lwk1.l = lwk0.l - AxisRscI->IntAdV.IKt2;
				if( lwk1.l >= 0 )
				{	/* |Iqref| >= IKt2 の場合 */
					/* KtPercent = limit( Kt2+(Kt3-Kt2) / (Imax-IKt2) * (Iq-IKt2), 0～2^14 )		*/
					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.KtB, Ixlwk );
					lwk3.l = add_limitf( AxisRscI->IntAdV.Kt2, lwk2.l );
					AxisRscI->IntAdV.KtPercent = limitz( lwk3.l, (LONG)No14bit );
				}
				else
				{
					lwk1.l = lwk0.l - AxisRscI->IntAdV.IKt;
					if( lwk1.l >= 0 )
					{	/* |Iqref| >= IKt の場合 */
						/* KtPercent = limit( 100%+(Kt2-100%) / (IKt2-IKt) * (Iq-IKt), 0～2^14 )	*/
						IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.KtA, Ixlwk );
						lwk3.l = add_limitf( (LONG)No14bit, lwk2.l );
						AxisRscI->IntAdV.KtPercent = limitz( lwk3.l, (LONG)No14bit );
					}
					else
					{	/* |Iqref| < IKt の場合 */
						AxisRscI->IntAdV.KtPercent = (LONG)No14bit;			/* KtPercent = 100%		*/
					}
				}
/*--------------------------------------------------------------------------------------------------*/
/*		Lq飽和計算			LqPercent = (Lq/Lq0) * 2^14												*/
/*--------------------------------------------------------------------------------------------------*/
				lwk1.l = lwk0.l - AxisRscI->IntAdV.ILq3;
				if( lwk1.l >= 0 )
				{	/* |Iqref| >= ILq3 の場合 */
					/* LqPercent = limit( Lq3+(Lq4-Lq3) / (Imax-ILq3) * (Iq-ILq3), 0～2^14 )		*/
					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.LqC, Ixlwk );
					lwk3.l = add_limitf( AxisRscI->IntAdV.Lq3, lwk2.l );
					AxisRscI->IntAdV.LqPercent = limitz( lwk3.l, (LONG)No14bit );
				}
				else
				{
					lwk1.l = lwk0.l - AxisRscI->IntAdV.ILq2;
					if( lwk1.l >= 0 )
					{	/* |Iqref| >= ILq2 の場合 */
						/* LqPercent = limit( Lq2+(Lq3-Lq2) / (ILq3-ILq2) * (Iq-ILq2), 0～2^14 )	*/
						IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.Lq2, Ixlwk );
						lwk3.l = add_limitf( AxisRscI->IntAdV.Lq3, lwk2.l );
						AxisRscI->IntAdV.LqPercent = limitz( lwk3.l, (LONG)No14bit );
					}
					else
					{	/* |Iqref| < ILq2 の場合 */
						/* LqPercent = limit( Lq+(Lq2-Lq) / ILq 2* Iq, 0～2^14 )					*/
						IxMulgain32( &lwk2.l, lwk0.l, AxisRscI->IntAdV.LqA, Ixlwk );
						lwk3.l = add_limitf( (LONG)No14bit, lwk2.l );
						AxisRscI->IntAdV.LqPercent = limitz( lwk3.l, (LONG)No14bit );
					}
				}
/*--------------------------------------------------------------------------------------------------*/
/*		Ld飽和計算			LdPercent = (Ld/Ld0) * 2^14												*/
/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = limit( AxisRscI->Curctrl.Idref, (LONG)OneR );		/* lwk0 = sign(Idref)	*/
				lwk0.l = lwk0.l * AxisRscI->Curctrl.Idref;					/* lwk0 = |Idref|		*/

				lwk1.l = lwk0.l - AxisRscI->IntAdV.ILd3;
				if( lwk1.l >= 0 )
				{	/* |Idref| >= ILd3 の場合 */
					/* LdPercent = limit( Ld3+(Ld4-Ld3) / (Imax-ILd3) * (Id-ILd3), 0～2^14 )		*/
					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.LdC, Ixlwk );
					lwk3.l = add_limitf( AxisRscI->IntAdV.Ld3, lwk2.l );
					AxisRscI->IntAdV.LdPercent = limitz( lwk3.l, (LONG)No14bit );
				}
				else
				{
					lwk1.l = lwk0.l - AxisRscI->IntAdV.ILd2;
					if( lwk1.l >= 0 )
					{	/* |Idref| >= ILd2 の場合 */
						/* LdPercent = limit( Ld2+(Ld3-Ld2) / (ILd3-ILd2) * (Id-ILd2), 0～2^14 )	*/
						IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.LdB, Ixlwk );
						lwk3.l = add_limitf( AxisRscI->IntAdV.Ld2, lwk2.l );
						AxisRscI->IntAdV.LdPercent = limitz( lwk3.l, (LONG)No14bit );
					}
					else
					{	/* |Idref| < ILd2 の場合 */
						/* LdPercent = limit( Ld+(Ld2-Ld) / ILd2 * Id, 0～2^14 )					*/
						IxMulgain32( &lwk2.l, lwk0.l, AxisRscI->IntAdV.LdA, Ixlwk );
						lwk3.l = add_limitf( (LONG)No14bit, lwk2.l );
						AxisRscI->IntAdV.LdPercent = limitz( lwk3.l, (LONG)No14bit );
					}
				}

/****************************************************************************************************/
/*																									*/
/*		弱め界磁制御																				*/
/*																									*/
/****************************************************************************************************//* <S18E>:修正 */
				if( AxisRscI->MicroIf.CtrlSwitch & V_FB )
				{ /* 弱め界磁有効の場合 */
/*--------------------------------------------------------------------------------------------------*/
/*		q軸電圧リミット計算																			*/
/*--------------------------------------------------------------------------------------------------*/
					dlwk0.dl = mul( AxisRscI->WeakFV.WfV1max, AxisRscI->WeakFV.WfV1max );
					lwk2.l = dlwk0.l[0];
					lwk3.l = dlwk0.l[1];								/* lwk3,2 = WFV1max^2		*/
					dlwk0.dl = mul( AxisRscI->Curctrl.VdBeforeLim,		/* dlwk0 = VdBeforeLim^2	*/
									 AxisRscI->Curctrl.VdBeforeLim );
/*--------------------------------------------------------------------------------------------------*/
					lwk2.l = (ULONG)lwk2.l >> 1;						/* lwk2 = lwk2 / 2(論理)	*/
					lwk4.l = (ULONG)dlwk0.l[0] >> 1;					/* lwk4 = MacL / 2(論理)	*/
					lwk4.l = lwk2.l - lwk4.l;							/* 下位32bit演算			*/
					lwk6.l = (ULONG)lwk4.l >> 31;						/* ボロー(論理)				*/
					lwk4.l = (ULONG)lwk4.l << 1;						/* lwk4 = lwk4 * 2(論理)	*/
					lwk5.l = lwk3.l - dlwk0.l[1];						/* 上位32bit演算			*/
					lwk5.l = lwk5.l - lwk6.l;							/* lwk5 = lwk5 - borrow		*/
/*--------------------------------------------------------------------------------------------------*/
					if( lwk5.l < (LONG)ZeroR )
					{	/* 負の場合 */
						lwk0.l = (LONG)ZeroR;							/* lwk0 = 0							*/
					}
					else
					{
						if( lwk5.l == (LONG)ZeroR )
						{	/* 上位32bitが0の場合 */
							lwk1.l = lwk4.l;
							lwk0.s[0] = MpSQRT( lwk1.l );				/* swk0 = √(2^48 - Idref^2)		*/
							lwk0.l = (ULONG)lwk0.s[0] & No0000ffff;
						}
						else
						{
							if( lwk5.l < (LONG)No8bit )
							{	/* 上位32bitが0x00000100より小さい場合 */
								lwk8.l = (ULONG)lwk4.l >> 8;			/* 論理シフト						*/
								lwk8.l = lwk8.l & No00ffffff;
								lwk9.l = (ULONG)lwk5.l << 24;			/* 論理シフト						*/
								lwk9.l = lwk9.l & Noff000000;
								lwk1.l = lwk8.l + lwk9.l;				/* lwk1 = (2^48 - Idref^2)/2^8		*/
								lwk0.s[0] = MpSQRT( lwk1.l );			/* swk0 = √(2^48 - Idref^2)/2^4	*/
								lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 4;
																		/* lwk0 = √(2^48 - Idref^2) (論理)	*/
							}
							else
							{
								if( lwk5.l < No16bit )
								{	/* 上位32bitが0x00010000より小さい場合 */
									lwk8.l = (ULONG)lwk4.l >> 16;		/* 論理シフト						*/
									lwk8.l = lwk8.l & No0000ffff;
									lwk9.l = (ULONG)lwk5.l << 16;		/* 論理シフト						*/
									lwk9.l = lwk9.l & Noffff0000;
									lwk1.l = lwk8.l + lwk9.l;			/* lwk1 = (2^48 - Idref^2)/2^16		*/
									lwk0.s[0] = MpSQRT( lwk1.l );		/* swk0 = √(2^48 - Idref^2)/2^8	*/
									lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 8;
																		/* lwk0 = √(2^48 - Idref^2) (論理)	*/
								}
								else
								{	/* 上位32bitが0x00010000の場合 */
									lwk0.l = (LONG)No24bit;				/* lwk0 = √(2^48) = 2^24			*/
								}
							}
						}
					}
					AxisRscI->WeakFV.WfVqLim = lwk0.l;
	/*--------------------------------------------------------------------------------------------------*/
	/*		WFVqLim - |VqBeforeLim| の計算																*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk8.l = limit( AxisRscI->Curctrl.VqBeforeLim, (LONG)OneR );
					dlwk0.dl = mul ( AxisRscI->Curctrl.VqBeforeLim, lwk8.l );	/* MacL = |VqBeforeLim|	*/
					lwk0.l = AxisRscI->WeakFV.WfVqLim - dlwk0.l[0];				/* lwk0 = WFVqLim - |VqBeforeLim|	*/
					lwk0.l = limit( lwk0.l, (LONG)No23bit );					/* 除算のために±23bitでリミットする */

					lwk2.l = limit( lwk0.l, (LONG)OneR );
					lwk2.l = lwk0.l * lwk2.l;									/* lwk2 = |lwk0|	*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		比例項(wkInTtAd0 * (Kpv / ω)) の計算														*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk9.l = limit( AxisRscI->Vcmp.MotSpd, (LONG)OneR );
					lwk9.l = AxisRscI->Vcmp.MotSpd * lwk9.l;					/* lwk9 = |MotSpd|	*/
																				/* Ldの変化分を考慮	*/
					lwk9.l = mulshr( lwk9.l, AxisRscI->IntAdV.LdPercent, 27 );	/* lwk9 = (|MotSpd|*LdPercent/16384)/2^13 (算術)	*/
					lwk1.l = lwk0.l / lwk9.l;

					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->WeakFV.WfKpv, Ixlwk );
					lwk4.l = limit( lwk2.l, (LONG)No30bit );					/* ±30bitでリミット	*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		積分項計算(アンチワインドアップ付き)														*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk7.l = AxisRscI->WeakFV.Idref0 - AxisRscI->Curctrl.Idref;	/* リミット前 - リミット後 */
					lwk8.l = lwk4.l - lwk7.l;									/* 積分入力から引く		*/
					lwk8.l = limit( lwk8.l, (LONG)No30bit );					/* ±30bitでリミット	*/
					IxIntegral( &lwk5.l,										/* lwk5 = 積分項		*/
								lwk8.l,
								AxisRscI->WeakFV.WfKiv,
								AxisRscI->WeakFV.WfInteg.l,
								Ixlwk );

					if( lwk5.l >= (LONG)ZeroR )
					{	/* 積分項 >= 0 の場合 */
						lwk5.l = (LONG)ZeroR;
						AxisRscI->WeakFV.WfInteg.l[0] = ZeroR;					/* 積分クリア			*/
						AxisRscI->WeakFV.WfInteg.l[1] = ZeroR;					/* 積分クリア			*/
					}
	/*--------------------------------------------------------------------------------------------------*/
	/*		比例項 + 積分項																				*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk6.l = lwk4.l + lwk5.l;
	/*--------------------------------------------------------------------------------------------------*/
	/*		ローパスフィルタ																			*/
	/*--------------------------------------------------------------------------------------------------*/
					IxLpfilter1( &AxisRscI->WeakFV.WfIdref, lwk6.l, AxisRscI->WeakFV.WfLpfGain, Ixlwk );
	/*--------------------------------------------------------------------------------------------------*/
	/*		WFIdref > 0 ならば、WFIdref = 0,積分 = 0													*/
	/*			WFIdrefが正になることは無い。正になった場合は0にする。									*/
	/*--------------------------------------------------------------------------------------------------*/
					if( AxisRscI->WeakFV.WfIdref >= (LONG)ZeroR )
					{	/* WFIdref >= 0 の場合 */
						AxisRscI->WeakFV.WfIdref = (LONG)ZeroR;
						AxisRscI->WeakFV.WfInteg.l[0] = (LONG)ZeroR;
						AxisRscI->WeakFV.WfInteg.l[1] = (LONG)ZeroR;
					}
				}
				else
				{	/* 弱め界磁制御無効の場合 */
					AxisRscI->WeakFV.WfIdref = (LONG)ZeroR;						/* 弱め界磁Idref=0 		*/
				}
	/*--------------------------------------------------------------------------------------------------*/
	/*		Idref = Idref1 + WFIdref (Idref = limit( lwk0, WFIdrefLim～0 ))								*/
	/*--------------------------------------------------------------------------------------------------*/
				AxisRscI->WeakFV.Idref0 = AxisRscI->Curctrl.IdrefBeforeWF + AxisRscI->WeakFV.WfIdref;
				AxisRscI->Curctrl.Idref = limitz( AxisRscI->WeakFV.Idref0, AxisRscI->WeakFV.WfIdrefLim );
//				IxAndReg16( &FlagIdrefLim, IFLAG, FlagLimit );	/* リミットフラグ取り出し(不使用)		*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		磁束φの逆数計算																			*/
	/*--------------------------------------------------------------------------------------------------*/
				/* lwk0 = (φMg / φmax) * 2^14 */
				lwk0.l = mulshr( AxisRscI->IntAdV.KtPercent, AxisRscI->IntAdV.RatioPhiMg, 14 );
				/* lwk1 = (Ld/Ld0 * Ld0/Lq0) * 2^28 */
				lwk1.l = AxisRscI->IntAdV.LdPercent * AxisRscI->IntAdV.RatioLdLq;
				/* lwk2 = (Lq/Lq0) * 2^28 */
				lwk2.l = (ULONG)AxisRscI->IntAdV.LqPercent << 14;
				/* lwk4 = ((Lq-Ld)/Lq0) * Idref */
				lwk3.l = lwk2.l - lwk1.l;
				lwk4.l = mulshr( lwk3.l, AxisRscI->Curctrl.Idref, 28 );
				/* lwk5 = ((Lq-Ld)/Lq0)*Idref * (Lq0*MaxCur/2^24)/φmax*2^14 */
				IxMulgain32( &lwk5.l, lwk4.l, AxisRscI->IntAdV.CnvRatioPhi, Ixlwk );
				/* lwk6 = ((φMg/φmax)-(((Lq-Ld)/Lq0)*Idref*(Lq0*MaxCur/2^24)/φmax))*2^14	*/
				lwk6.l = sub_limitf( lwk0.l, lwk5.l );
				/* lwk6 = limit( lwk6, 0～32767 )	*/
				lwk6.l = limitz( lwk6.l, (LONG)No32767 );

				if( lwk6.s[0] >= (SHORT)1024 )
				{	/* lwk6.s[0] >= 1024 の場合 */
					lwk7.l = (LONG)No24bit / (LONG)lwk6.s[0];						/* lwk7.sr = 2^24 / lwk6.sr	*/
					lwk7.l = (ULONG)lwk7.s[0];										/* 16bit→32bit(符号なし)	*/
					/* PhiForTrq = 2^24/lwk6.sr * CnvToIqref = 2^10/φ */
					IxMulgain32( &AxisRscI->IntAdV.PhiForTrq, lwk7.l, AxisRscI->IntAdV.CnvToIqref, Ixlwk );	
				}
				else
				{
					if( lwk6.s[0] >= (SHORT)32 )
					{	/* lwk6.s[0] >= 32 の場合 */
						lwk7.l = (LONG)No19bit / (LONG)lwk6.s[0];					/* lwk7.sr = 2^19 / lwk6.sr	*/
						lwk7.l = (ULONG)lwk7.s[0];									/* 16bit→32bit(符号なし)	*/
						lwk7.l = (ULONG)lwk7.l << 5;								/* lwk7 = lwk7 << 5			*/
						/* PhiForTrq = 2^24/lwk6.sr * CnvToIqref = 2^10/φ */
						IxMulgain32( &AxisRscI->IntAdV.PhiForTrq, lwk7.l, AxisRscI->IntAdV.CnvToIqref, Ixlwk );
					}
					else
					{
						if( lwk6.s[0] >= (SHORT)OneR )
						{	/* lwk6.s[0] >= 1 の場合 */
							lwk7.l = (LONG)No14bit / (LONG)lwk6.s[0];				/* lwk7.sr = 2^14 / lwk6.sr	*/
							lwk7.l = (ULONG)lwk7.s[0];								/* 16bit→32bit(符号なし)	*/
							lwk7.l = (ULONG)lwk7.l << 10;							/* lwk7 = lwk7 << 10		*/
							/* PhiForTrq = 2^24/lwk6.sr * CnvToIqref = 2^10/φ	*/
							IxMulgain32( &AxisRscI->IntAdV.PhiForTrq, lwk7.l, AxisRscI->IntAdV.CnvToIqref, Ixlwk );
						}
						else
						{	/* lwk6.s[0] = 0 の場合 */
							AxisRscI->IntAdV.PhiForTrq = (LONG)No24bit;
						}
					}
				}
	/*--------------------------------------------------------------------------------------------------*/
	/*		q軸電流指令計算																				*/
	/*--------------------------------------------------------------------------------------------------*/
				/* MacH,L = Trqref / φ * 2^10 */
				lwk0.l = mulshr_limitf( AxisRscI->Trqctrl.Trqref, AxisRscI->IntAdV.PhiForTrq, 10 );
				AxisRscI->Curctrl.IqrefBeforeLim = lwk0.l;		/* IqrefBeforeLim = Trqref / φ			*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		電流リミット処理																			*/
	/*--------------------------------------------------------------------------------------------------*/
				lwk9.l = (LONG)No24bit;									/* lwk9 = 2^24							*/
				lwk9.l = limitz( lwk9.l, AxisRscI->MotInfo.MaxCur );	/* lwk9 = limit( lwk9, 0～MaxCur )		*/
				lwk9.l = limitz( lwk9.l, AxisRscI->MotInfo.MaxCurLim );	/* lwk9 = limit( lwk9, 0～MaxCurLim )	*/

	/*--------------------------------------------------------------------------------------------------*/
	/*		q軸電流指令リミット計算																		*/
	/*--------------------------------------------------------------------------------------------------*/
				dlwk0.dl = mul( lwk9.l, lwk9.l );							/* MacH,L = lwk9^2			*/
				lwk0.l = dlwk0.l[0];
				lwk1.l = dlwk0.l[1];										/* lwk1,0 = MaxCurLim^2		*/
				lwk0.l = (ULONG)lwk0.l >> 1;								/* lwk0 = lwk0 / 2 (論理)	*/
	/*--------------------------------------------------------------------------------------------------*/
				dlwk0.dl = mul( AxisRscI->Curctrl.Idref, AxisRscI->Curctrl.Idref );/* MacH,L = Idref^2	*/
				lwk2.l = (ULONG)dlwk0.l[0] >> 1;							/* lwk2 = MacL / 2 (論理)	*/
				lwk2.l = lwk0.l - lwk2.l;									/* 下位32bit演算			*/
				lwk4.l = (ULONG)lwk2.l >> 31;								/* ボロー  (論理)			*/
				lwk2.l = (ULONG)lwk2.l << 1;								/* lwk2 = lwk2 * 2  (論理)	*/
				lwk3.l = lwk1.l - dlwk0.l[1];								/* 上位32bit演算			*/
				lwk3.l = lwk3.l - lwk4.l;									/* lwk3 = lwk3 - borrow		*/
				if( lwk3.l < (LONG)ZeroR )
				{	/* 負の場合 */
					lwk0.l = (LONG)ZeroR;									/* lwk0 = 0					*/
				}
				else
				{
					if( lwk3.l == (LONG)ZeroR )
					{	/* 上位32bitが0の場合 */
						lwk0.s[0] = MpSQRT( lwk2.l );						/* lwk0 = √(MaxCurLim^2 - Idref^2)	*/
						lwk0.l = (ULONG)lwk0.s[0] & No0000ffff;
					}
					else
					{
						if( lwk3.l < (LONG)No8bit )
						{	/* 上位32bitが0x00000100より小さい場合 */
							lwk8.l = (ULONG)lwk2.l >> 8;					/* 論理シフト						*/
							lwk8.l = lwk8.l & No00ffffff;
							lwk9.l = (ULONG)lwk3.l << 24;					/* 論理シフト						*/
							lwk9.l = lwk9.l & Noff000000;
							lwk1.l = lwk8.l + lwk9.l;					/* lwk1 = (MaxCurLim^2 - Idref^2)/2^8	*/
							lwk0.s[0] = MpSQRT( lwk1.l );				/* lwk0 = √(MaxCurLim^2 - Idref^2)/2^4	*/
							lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 4;
																		/* lwk0 = √(MaxCurLim^2 - Idref^2)		*/
						}
						else
						{
							if( lwk3.l < (LONG)No16bit )
							{	/* 上位32bitが0x00010000より小さい場合 */
								lwk8.l = (ULONG)lwk2.l >> 16;				/* 論理シフト						*/
								lwk8.l = lwk8.l & No0000ffff;
								lwk9.l = (ULONG)lwk3.l << 16;				/* 論理シフト						*/
								lwk9.l = lwk9.l & Noffff0000;
								lwk1.l = lwk8.l + lwk9.l;				/* lwk1 = (MaxCurLim^2 - Idref^2)/2^16	*/
								lwk0.s[0] = MpSQRT( lwk1.l );			/* lwk0 = √(MaxCurLim^2 - Idref^2)/2^8	*/
								lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 8;
																		/* lwk0 = √(MaxCurLim^2 - Idref^2)		*/
							}
							else
							{	/* 上位32bitが0x00010000の場合 */
								lwk0.l = (LONG)No24bit;						/* lwk0 = √(2^48) = 2^24			*/
							}
						}
					}
				}
	/*--------------------------------------------------------------------------------------------------*/
	/*		q軸電流指令リミット																			*/
	/*--------------------------------------------------------------------------------------------------*/
				if( AxisRscI->Curctrl.IqrefBeforeLim >= ZeroR )
				{	/* 正側リミット処理 */
					AxisRscI->Curctrl.Iqref = limit( AxisRscI->Curctrl.IqrefBeforeLim, lwk0.l );
					/* トルク制限中フラグをセット */
					swk10 = AxisRscI->StsFlg.CtrlStsRW | TLIM;
					AxisRscI->StsFlg.CtrlStsRW = cmove( (AxisRscI->Curctrl.Iqref == lwk0.l),
														swk10,
														AxisRscI->StsFlg.CtrlStsRW );
				}
				else
				{	/* 負側リミット処理 */
					AxisRscI->Curctrl.Iqref = limit( AxisRscI->Curctrl.IqrefBeforeLim, lwk0.l );
					/* トルク制限中フラグをセット */
					swk10 = AxisRscI->StsFlg.CtrlStsRW | TLIM;
					AxisRscI->StsFlg.CtrlStsRW = cmove( (AxisRscI->Curctrl.Iqref == lwk0.l),
														swk10,
														AxisRscI->StsFlg.CtrlStsRW );
				}
			}

/****************************************************************************************************/
/*																									*/
/*		ACRd(d軸電流制御)																			*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		偏差計算																					*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = AxisRscI->Curctrl.Idref - AxisRscI->Curctrl.IdDetect;
/*--------------------------------------------------------------------------------------------------*/
/*		比例項計算																					*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk1.l, lwk0.l, AxisRscI->Curctrl.GainKpd, Ixlwk );	/* lwk1 = GainKpd * lwk0			*/
			lwk1.l = mulshr( lwk1.l, AxisRscI->IntAdV.LdPercent, 14 );			/* lwk1 = (GainKpd*Ld/Ld0) * lwk0	*/
			lwk1.l = limit( lwk1.l, (LONG)No30bit );							/* ±30bitでリミット				*/
			lwk7.l = lwk1.l;
/*--------------------------------------------------------------------------------------------------*/
/*		積分項計算(アンチワインドアップ付き)														*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->MicroIf.CtrlSwitch & ANTIWUP )
			{	/* アンチワインドアップ処理 */
				lwk2.l = AxisRscI->Curctrl.VdBeforeLim - AxisRscI->Curctrl.Vdref;	/* lwk2 = 電圧リミット分		*/
				lwk7.l = lwk7.l - lwk2.l;											/* lwk7 = 補正後積分入力		*/
				lwk7.l = limit( lwk7.l, (LONG)No30bit );							/* ±30bitでリミット			*/
			}
			IxIntegral( &lwk3.l, lwk7.l, AxisRscI->Curctrl.GainKid, AxisRscI->Curctrl.IntegD.l, Ixlwk );
/*--------------------------------------------------------------------------------------------------*/
/*		比例項 + 積分項 計算																		*/
/*--------------------------------------------------------------------------------------------------*/
			lwk4.l = lwk1.l + lwk3.l;
/*--------------------------------------------------------------------------------------------------*/
/*		電圧フィルタ(ローパスフィルタ)																*/
/*--------------------------------------------------------------------------------------------------*/
			IxLpfilter1( &AxisRscI->Vfil.VdLpfOut, lwk4.l, AxisRscI->Vfil.VoltFilGain, Ixlwk );	

/****************************************************************************************************/
/*																									*/
/*		ACRq(q軸電流制御)																			*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		偏差計算																					*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = AxisRscI->Curctrl.Iqref - AxisRscI->Curctrl.IqDetect;
/*--------------------------------------------------------------------------------------------------*/
/*		比例項計算																					*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk1.l, lwk0.l, AxisRscI->Curctrl.GainKpq, Ixlwk );	/* lwk1 = GainKpq * lwk0			*/
			lwk1.l = mulshr( lwk1.l, AxisRscI->IntAdV.LqPercent, 14 );			/* lwk1 = (GainKpq*Lq/Lq0) * lwk0	*/
			lwk1.l = limit( lwk1.l, No30bit );									/* ±30bitでリミット				*/
			lwk7.l = lwk1.l;
/*--------------------------------------------------------------------------------------------------*/
/*		積分項計算(アンチワインドアップ付き)														*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->MicroIf.CtrlSwitch & ANTIWUP )
			{
				lwk2.l = AxisRscI->Curctrl.VqBeforeLim - AxisRscI->Curctrl.Vqref;	/* lwk2 = 電圧リミット分		*/
				lwk7.l = lwk7.l - lwk2.l;											/* lwk7 = 補正後積分入力		*/
				lwk7.l = limit( lwk7.l, (LONG)No30bit );							/* ±30bitでリミット			*/
			}
			IxIntegral( &lwk3.l, lwk7.l, AxisRscI->Curctrl.GainKiq, AxisRscI->Curctrl.IntegQ.l, Ixlwk );
/*--------------------------------------------------------------------------------------------------*/
/*		比例項 + 積分項 計算																		*/
/*--------------------------------------------------------------------------------------------------*/
			lwk4.l = lwk1.l + lwk3.l;
/*--------------------------------------------------------------------------------------------------*/
/*		電圧フィルタ(ローパスフィルタ)																*/
/*--------------------------------------------------------------------------------------------------*/
			IxLpfilter1( &AxisRscI->Vfil.VqLpfOut, lwk4.l, AxisRscI->Vfil.VoltFilGain, Ixlwk );

/****************************************************************************************************/
/*																									*/
/*		Voltage Compensation(電圧補償)																*/
/*																									*/
/****************************************************************************************************/
			if( AxisRscI->MicroIf.CtrlSwitch & ISEL )
			{													/* 電流指令を使用					*/
				lwk1.l = AxisRscI->Curctrl.Idref;				/* lwk1 = d軸電流指令				*/
				lwk2.l = AxisRscI->Curctrl.Iqref;				/* lwk2 = q軸電流指令				*/
			}
			else
			{													/* 電流FBを使用						*/
				lwk1.l = AxisRscI->Curctrl.IdDetect;			/* lwk1 = d軸電流FB					*/
				lwk2.l = AxisRscI->Curctrl.IqDetect;			/* lwk2 = q軸電流FB					*/
			}
/*--------------------------------------------------------------------------------------------------*/
/*		RId - ωLqIq の計算																			*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk3.l, lwk1.l, AxisRscI->Vcmp.Resist, Ixlwk );	/* lwk3 = RId				*/

			lwk4.l = mulshr( AxisRscI->Vcmp.MotSpd, lwk2.l, 20 );			/* lwk4 = (ω * Iq) >> 20	*/
			IxMulgain32( &lwk4.l, lwk4.l,  AxisRscI->Vcmp.Lq, Ixlwk );		/* lwk4 = ωLq0Iq			*/
			lwk4.l = mulshr( lwk4.l,  AxisRscI->IntAdV.LqPercent, 14 );		/* lwk4 = ωLq0Iq * Lq/Lq0	*/

			lwk3.l = lwk3.l - lwk4.l;										/* lwk3 = RId - ωLqIq		*/
/*--------------------------------------------------------------------------------------------------*/
/*		RIq + ωLdId + ωφ の計算																	*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk4.l, lwk2.l, AxisRscI->Vcmp.Resist, Ixlwk );	/* lwk4 = RIq				*/

			lwk5.l = mulshr( AxisRscI->Vcmp.MotSpd, lwk1.l, 20 );			/* lwk5 = (ω * Id) >> 20	*/
			IxMulgain32( &lwk5.l, lwk5.l, AxisRscI->Vcmp.Ld, Ixlwk );		/* lwk5 = ωLd0Id			*/
			lwk5.l = mulshr( lwk5.l, AxisRscI->IntAdV.LdPercent, 14 );		/* lwk5 = ωLd0Id * Ld/Ld0	*/

			lwk6.l = mulshr( AxisRscI->Vcmp.OmegaPhi, AxisRscI->IntAdV.KtPercent, 14 );
																			/* lwk6 = ωφ0 * φ/φ0			*/

			lwk4.l = lwk4.l + lwk5.l;										/* lwk4 = RIq + ωLdId				*/
			lwk4.l = lwk4.l + lwk6.l;										/* lwk4 = RIq + ωLdId + ωφ		*/
/*--------------------------------------------------------------------------------------------------*/
/*		Ld*dId/dt, Lq*dIq/dt の計算 (Ldi/dt補償)													*/
/*--------------------------------------------------------------------------------------------------*/
/* 今のところ必要ないので、作らない。 */
			AxisRscI->Vcmp.VoltageffD = lwk3.l;								/* VoltageffD = RId - ωLqIq		*/
			AxisRscI->Vcmp.VoltageffQ = lwk4.l;								/* VoltageffQ = RIq + ωLdId + ωφ	*/
/*--------------------------------------------------------------------------------------------------*/
/*		電圧指令に電圧FF補償を加える																*/
/*--------------------------------------------------------------------------------------------------*/
			lwk5.l = AxisRscI->Vfil.VdLpfOut + AxisRscI->Vcmp.VoltageffD;	/* lwk5 = VdLpfOut + VoltageffD		*/
			lwk6.l = AxisRscI->Vfil.VqLpfOut + AxisRscI->Vcmp.VoltageffQ;	/* lwk6 = VqLpfOut + VoltageffQ		*/
/*--------------------------------------------------------------------------------------------------*/
/*		AVRゲイン補償																				*/
/*--------------------------------------------------------------------------------------------------*/
			lwk7.l = (ULONG)AxisRscI->Curctrl.AVRGain;					/* 16bit→32bit(符号なし)				*/
			lwk8.l = mulshr( lwk5.l, lwk7.l, 13 );						/* MacH,L = d軸電圧指令 * AVRGain		*/
																		/* lwk8 = MacH,L >> 13					*/
			AxisRscI->Curctrl.Vdref = limit( lwk8.l, (LONG)No30bit );	/* ±2^30でリミット						*/

			lwk9.l = mulshr( lwk6.l, lwk7.l, 13 );						/* MacH,L = q軸電圧指令 * AVRGain		*/
																		/* lwk9 = MacH,L >> 13					*/
			AxisRscI->Curctrl.Vqref = limit( lwk9.l, (LONG)No30bit );	/* ±2^30でリミット						*/

/****************************************************************************************************/
/*																									*/
/*		電圧リミット処理																			*/
/*																									*/
/****************************************************************************************************//* <S18E>:修正 */
/*--------------------------------------------------------------------------------------------------*/
/*		変調率計算																					*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = ZeroR;
			IxSquareSum( (DLREG*)&dlwk0,
						 AxisRscI->Curctrl.Vdref,
						 AxisRscI->Curctrl.Vqref,
						 (DWREG*)Ixlwk );

			if( dlwk0.l[1] >= (LONG)No24bit )
			{	/* 上位32bitが0x01000000以上の場合 */
				lwk0.s[0] = MpSQRT( dlwk0.l[1] );									/* lwk0 = √(Vdref^2+Vqref^2)/2^16	*/
				AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 16;		/* V1 = √(Vdref^2+Vqref^2)			*/
				lwk9.s[0] = 16;														/* シフト数保存						*/
			}
			else
			{
				if( dlwk0.l[1] >= (LONG)No16bit )
				{	/* 上位32bitが0x00010000以上の場合 */
					lwk8.l = (ULONG)dlwk0.l[0] >> 24;								/* 論理シフト						*/
					lwk8.l = lwk8.l & No000000ff;
					lwk9.l = (ULONG)dlwk0.l[1] << 8;								/* 論理シフト						*/
					lwk9.l = lwk9.l & Noffffff00;
					lwk1.l = lwk8.l + lwk9.l;										/* lwk1 = (Vdref^2+Vqref^2)/2^24	*/
					lwk0.s[0] = MpSQRT( lwk1.l );									/* lwk0 = √(Vdref^2+Vqref^2)/2^12	*/
					AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 12;	/* V1 = √(Vdref^2+Vqref^2)			*/
					lwk9.s[0] = 12;													/* シフト数保存						*/
				}
				else
				{
					if( dlwk0.l[1] >= (LONG)No8bit )
					{	/* 上位32bitが0x00000100以上の場合 */
						lwk8.l = (ULONG)dlwk0.l[0] >> 16;							/* 論理シフト						*/
						lwk8.l = lwk8.l & No0000ffff;
						lwk9.l = (ULONG)dlwk0.l[1] << 16;							/* 論理シフト						*/
						lwk9.l = lwk9.l & Noffff0000;
						lwk1.l = lwk8.l + lwk9.l;									/* lwk1 = (Vdref^2+Vqref^2)/2^16	*/
						lwk0.s[0] = MpSQRT( lwk1.l );								/* lwk0 = √(Vdref^2+Vqref^2)/2^8	*/
						AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 8;/* V1 = √(Vdref^2+Vqref^2)			*/
						lwk9.s[0] = 8;												/* シフト数保存						*/
					}
					else
					{
						if( dlwk0.l[1] != (LONG)ZeroR )
						{	/* 上位32bitが0でない場合 */
							lwk8.l = (ULONG)dlwk0.l[0] >> 8;						/* 論理シフト						*/
							lwk8.l = lwk8.l & No00ffffff;
							lwk9.l = (ULONG)dlwk0.l[1] << 24;						/* 論理シフト						*/
							lwk9.l = lwk9.l & Noff000000;
							lwk1.l = lwk8.l + lwk9.l;								/* lwk1 = (Vdref^2+Vqref^2)/2^8		*/
							lwk0.s[0] = MpSQRT( lwk1.l );							/* lwk0 = √(Vdref^2+Vqref^2)/2^4	*/
							AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 4;
																					/* V1 = √(Vdref^2+Vqref^2)			*/
							lwk9.s[0] = 4;											/* シフト数保存						*/
						}
						else
						{	/* 上位32bitが0の場合 */
							lwk1.l = dlwk0.l[0];									/* lwk1 = (Vdref^2+Vqref^2)			*/
							lwk0.s[0] = MpSQRT( lwk1.l );							/* lwk0 = √(Vdref^2+Vqref^2)		*/
							AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff);	/* V1 = √(Vdref^2+Vqref^2)			*/
							lwk9.s[0] = (SHORT)ZeroR;								/* シフト数保存						*/
						}
					}
				}
			}

			AxisRscI->Curctrl.VdBeforeLim = AxisRscI->Curctrl.Vdref;				/* 保存								*/
			AxisRscI->Curctrl.VqBeforeLim = AxisRscI->Curctrl.Vqref;				/* 保存								*/
			AxisRscI->Vltctrl.V1BeforeLim = AxisRscI->Vltctrl.V1;					/* 保存								*/

/*--------------------------------------------------------------------------------------------------*/
/*		電圧リミット処理																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->Vltctrl.Vmax < AxisRscI->Vltctrl.V1 )
			{	/* 変調率がリミット以下 */
				lwk1.l = AxisRscI->Vltctrl.Vmax;
				if( lwk0.s[0] < (SHORT)ZeroR )
				{	/* lwk0.s[0]が負の場合 */
					/* lwk0.srが32767を超えて負の値になっているので、2で割る */
					lwk1.l = (ULONG)lwk1.l >> 1;									/* 論理シフト	*/
					lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) >> 1;					/* 論理シフト	*/
				}
				/* lwk2=Vmax/V1*2^(lwk9.sr), lwk0=4096～65535, lwk1=4194304～5340353 */
				lwk2.l = lwk1.l / lwk0.l;
				AxisRscI->Vltctrl.V1 = AxisRscI->Vltctrl.Vmax;		/* V1 = Vmax					*/
				AxisRscI->BlockTr.TxDataCToAsic3.l = lwk2.l;

				dlwk0.dl = 0;											/* Buffer Clear								*/
				dlwk0.dl = mul( AxisRscI->Curctrl.Vdref, lwk2.l );		/* MacH,L = Vdref * Vmax / V1 * 2^(lwk9.sr)	*/
				AxisRscI->Curctrl.Vdref = asr( dlwk0.dl, lwk9.s[0] );	/* Vdref = Vdref * Vmax / V1	*/

				dlwk0.dl = 0;											/* Buffer Clear								*/
				dlwk0.dl = mul( AxisRscI->Curctrl.Vqref, lwk2.l );		/* MacH,L = Vdref * Vmax / V1 * 2^(lwk9.sr)	*/
				AxisRscI->Curctrl.Vqref = asr( dlwk0.dl, lwk9.s[0] );	/* Vdref = Vdref * Vmax / V1	*/
			}

/****************************************************************************************************/
/*																									*/
/*		UVW transform : dq( 2phase ) to UVW( 3phase ) Transform										*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		Vuref = Vdref*cosθ - Vqref*sinθ															*/
/*--------------------------------------------------------------------------------------------------*/
			lwk8.l = mulshr( AxisRscI->Curctrl.Vdref, AxisRscI->SinTbl.Cos1.l, 14 );	/* lwk8 = (Vderf * cosθ) >> 14			*/
			lwk9.l = mulshr( AxisRscI->Curctrl.Vqref, AxisRscI->SinTbl.Sin1.l, 14 );	/* lwk9 = (Vqerf * sinθ) >> 14			*/
			AxisRscI->Curctrl.Vuref = lwk8.l - lwk9.l;									/* Vuref = Vdref*cosθ - Vqref*sinθ	*/
/*--------------------------------------------------------------------------------------------------*/
/*		Vvref = Vdref*cos(θ-2π/3) - Vqref*sin(θ-2π/3)											*/
/*--------------------------------------------------------------------------------------------------*/
			lwk8.l = mulshr( AxisRscI->Curctrl.Vdref, AxisRscI->SinTbl.Cos2.l, 14 );	/* lwk8 = (Vderf * θ-2π/3) >> 14						*/
			lwk9.l = mulshr( AxisRscI->Curctrl.Vqref, AxisRscI->SinTbl.Sin2.l, 14 );	/* lwk9 = (Vqerf * θ-2π/3) >> 14						*/
			AxisRscI->Curctrl.Vvref = lwk8.l - lwk9.l;									/* Vuref = Vdref*cos(θ-2π/3) - Vqref*sin(θ-2π/3)	*/
/*--------------------------------------------------------------------------------------------------*/
/*		Vwref = -Vuref - Vvref																		*/
/*--------------------------------------------------------------------------------------------------*/
			lwk5.l = (LONG)ZeroR - AxisRscI->Curctrl.Vuref;
			AxisRscI->Curctrl.Vwref = lwk5.l - AxisRscI->Curctrl.Vvref;					/* Vwref = -Vuref - Vvref				*/

/****************************************************************************************************/
/*																									*/
/*		変調補正＆過変調補正																		*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		Over modulation type select																	*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscI->MicroIf.CtrlSwitch & OVMSEL2) == ZeroR )
			{	/* タイプ2有効ではない場合	*/
				if( AxisRscI->Vltctrl.V1 >= (LONG)No22bit )	
				{	/* 変調率 >= 100% の場合	*/
					if( (AxisRscI->MicroIf.CtrlSwitch & OVMSEL1) != ZeroR )
					{	/* タイプ1有効の場合	*/
/*--------------------------------------------------------------------------------------------------*/
/*		Over modulation1																			*/
/*--------------------------------------------------------------------------------------------------*/
						IxSetCtblAdr( pCtbl, &(OVMODTBLG[0][0]) );					/* gain type						*/
						lwk9.l = mulshr( AxisRscI->Vltctrl.V1, (LONG)OneR, 9 );
																					/* 正規化を変更(±2^22→±8192)		*/
						AxisRscI->Vltctrl.ModulationComp = MpOVMMODK( AxisRscI->Vltctrl.V1,
																	  lwk9.s[0],
																	  AxisRscI->MicroIf.CtrlSwitch,
																	  pCtbl );
						lwk0.l = (ULONG)AxisRscI->Vltctrl.ModulationComp;			/* 16bit→32bit(符号なし)			*/
						AxisRscI->Curctrl.Vuref = mulshr( AxisRscI->Curctrl.Vuref, lwk0.l, 13 );
						AxisRscI->Curctrl.Vvref = mulshr( AxisRscI->Curctrl.Vvref, lwk0.l, 13 );
						AxisRscI->Curctrl.Vwref = mulshr( AxisRscI->Curctrl.Vwref, lwk0.l, 13 );
/*--------------------------------------------------------------------------------------------------*/
/*		lwk1 = |Vuref|,     lwk2 = |Vvref|,     lwk3 = |Vwref|										*/
/*		lwk4 = sign(Vuref), lwk5 = sign(Vvref), lwk6 = sign(Vwref)									*/
/*--------------------------------------------------------------------------------------------------*/
						lwk4.l = limit( AxisRscI->Curctrl.Vuref, (LONG)OneR );
						lwk1.l = lwk4.l * AxisRscI->Curctrl.Vuref;

						lwk5.l = limit( AxisRscI->Curctrl.Vvref, (LONG)OneR );
						lwk2.l = lwk5.l * AxisRscI->Curctrl.Vvref;

						lwk6.l = limit( AxisRscI->Curctrl.Vwref, (LONG)OneR );
						lwk3.l = lwk6.l * AxisRscI->Curctrl.Vwref;

						if( lwk1.l >= lwk2.l )
						{	/* |Vuref| >= |Vvref| の場合 */
							if( lwk1.l >= lwk3.l )
							{	/* U相が最も大きい時の処理 */
								lwk1.l = lwk1.l - (LONG)No22bit;			/* lwk1 = |Vuref|-2^22	*/
								lwk1.l = limitz( lwk1.l, (LONG)No24bit );	/* zero limit			*/
								lwk0.l = lwk4.l * lwk1.l;
							}
							else
							{	/* W相が最も大きい時の処理 */
								lwk3.l = lwk3.l - (LONG)No22bit;			/* lwk3 = |Vwref|-2^22	*/
								lwk3.l = limitz( lwk3.l, (LONG)No24bit );	/* zero limit			*/
								lwk0.l = lwk6.l * lwk3.l;
							}
						}
						else
						{
							if( lwk2.l >= lwk3.l )
							{	/* V相が最も大きい時の処理 */
								lwk2.l = lwk2.l - (LONG)No22bit;			/* lwk2 = |Vvref|-2^22	*/
								lwk2.l = limitz( lwk2.l, (LONG)No24bit );	/* zero limit			*/
								lwk0.l = lwk5.l * lwk2.l;
							}
							else
							{	/* W相が最も大きい時の処理 */
								lwk3.l = lwk3.l - (LONG)No22bit;			/* lwk3 = |Vwref|-2^22	*/
								lwk3.l = limitz( lwk3.l, (LONG)No24bit );	/* zero limit		*/
								lwk0.l = lwk6.l * lwk3.l;
							}
						}

						AxisRscI->Curctrl.Vuref = AxisRscI->Curctrl.Vuref - lwk0.l;
						AxisRscI->Curctrl.Vvref = AxisRscI->Curctrl.Vvref - lwk0.l;
						AxisRscI->Curctrl.Vwref = AxisRscI->Curctrl.Vwref - lwk0.l;
						AxisRscI->Vltctrl.Vcentral = lwk0.l;
					}
				}
			}
			else
			{
/*--------------------------------------------------------------------------------------------------*/
/*		Over modulation2																			*/
/*--------------------------------------------------------------------------------------------------*/
				IxSetCtblAdr( pCtbl, &(OVMODTBLO[0][0]) );					/* gain type						*/
				lwk9.l = mulshr( AxisRscI->Vltctrl.V1, (LONG)OneR, 9 );
																			/* 正規化を変更(±2^22→±8192)		*/
				AxisRscI->Vltctrl.ModulationComp = MpOVMMODK( AxisRscI->Vltctrl.V1,
															  lwk9.s[0],
															  AxisRscI->MicroIf.CtrlSwitch,
															  pCtbl );
/*--------------------------------------------------------------------------------------------------*/
/*		MAX = lwk1, MIN = lwk2																		*/
/*		OFS = (lwk1+lwk2)/2																			*/
/*--------------------------------------------------------------------------------------------------*/
				if( AxisRscI->Curctrl.Vuref >= AxisRscI->Curctrl.Vvref )
				{	/* Vuref >= Vref の場合 */
					lwk1.l = AxisRscI->Curctrl.Vuref;
					lwk2.l = AxisRscI->Curctrl.Vvref;
				}
				else
				{	/* Vuref < Vref の場合 */
					lwk1.l = AxisRscI->Curctrl.Vvref;
					lwk2.l = AxisRscI->Curctrl.Vuref;
				}

				if( lwk1.l < AxisRscI->Curctrl.Vwref )
				{	/* VurefとVvrefの大きい方 < Vwref の場合 */
					lwk1.l = AxisRscI->Curctrl.Vwref;
				}
				else
				{
					if( AxisRscI->Curctrl.Vwref < lwk2.l )
					{	/* VurefとVvrefの小さい方 < Vwref の場合 */
						lwk2.l = AxisRscI->Curctrl.Vwref;
					}
				}

				lwk0.l = lwk2.l + lwk1.l;
				lwk0.l = mulshr( lwk0.l, (LONG)OneR, 1 );

				AxisRscI->Curctrl.Vuref = AxisRscI->Curctrl.Vuref - lwk0.l;
				AxisRscI->Curctrl.Vvref = AxisRscI->Curctrl.Vvref - lwk0.l;
				AxisRscI->Curctrl.Vwref = AxisRscI->Curctrl.Vwref - lwk0.l;
				AxisRscI->Vltctrl.Vcentral = lwk0.l;
	/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = (ULONG)AxisRscI->Vltctrl.ModulationComp;			/* 16bit→32bit(符号なし)		*/
				lwk0.l = (ULONG)lwk0.l << 9;								/* 論理シフト					*/

				lwk1.l = limit( AxisRscI->Curctrl.Vuref, (LONG)OneR );		/* lwk1 = -1 / 0 / 1			*/
				lwk1.l = lwk1.l | (LONG)OneR;								/* lwk1 = -1 / 1(0を1にする)	*/
				lwk1.l = lwk1.l * lwk0.l;
				AxisRscI->Curctrl.Vuref = lwk1.l + AxisRscI->Curctrl.Vuref;

				lwk1.l = limit( AxisRscI->Curctrl.Vvref, (LONG)OneR );		/* lwk1 = -1 / 0 / 1			*/
				lwk1.l = lwk1.l | (LONG)OneR;								/* lwk1 = -1 / 1(0を1にする)	*/
				lwk1.l = lwk1.l * lwk0.l;
				AxisRscI->Curctrl.Vvref = lwk1.l + AxisRscI->Curctrl.Vvref;

				lwk1.l = limit( AxisRscI->Curctrl.Vwref, (LONG)OneR );		/* lwk1 = -1 / 0 / 1			*/
				lwk1.l = lwk1.l | (LONG)OneR;								/* lwk1 = -1 / 1(0を1にする)	*/
				lwk1.l = lwk1.l * lwk0.l;
				AxisRscI->Curctrl.Vwref = lwk1.l + AxisRscI->Curctrl.Vwref;
			}

/****************************************************************************************************/
/*																									*/
/*		On-Delay																					*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		Iuref, Ivref の計算																			*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*		Iuref = Idref*cosθ - Iqref*sinθ															*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = mulshr( AxisRscI->Curctrl.Idref, AxisRscI->SinTbl.Cos1.l, 14 );	/* lwk0 = (Idref * cosθ) >> 14							*/
			lwk1.l = mulshr( AxisRscI->Curctrl.Iqref, AxisRscI->SinTbl.Sin1.l, 14 );	/* lwk1 = (Iqref * sinθ) >> 14							*/
			AxisRscI->OnDelay.Iuref = lwk0.l - lwk1.l;									/* Iuref = Idref*cosθ - Iqref*sinθ					*/
/*--------------------------------------------------------------------------------------------------*/
/*		Ivref = Idref*cos(θ-2π/3) - Iqref*sin(θ-2π/3)											*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = mulshr( AxisRscI->Curctrl.Idref, AxisRscI->SinTbl.Cos2.l, 14 );	/* lwk0 = (Idref * cos(θ-2π/3)) >> 14					*/
			lwk1.l = mulshr( AxisRscI->Curctrl.Iqref, AxisRscI->SinTbl.Sin2.l, 14 );	/* lwk1 = (Iqref * sin(θ-2π/3)) >> 14					*/
			AxisRscI->OnDelay.Ivref = lwk0.l - lwk1.l;									/* Ivref = Idref*cos(θ-2π/3) - Iqref*sin(θ-2π/3)	*/
/*--------------------------------------------------------------------------------------------------*/
/*		if ( |IuDetect| <= OnDelayLevel )	lwk1 = Iuref											*/
/*		else								lwk1 = IuDetect											*/
/*		if ( |IvDetect| <= OnDelayLevel )	lwk2 = Ivref											*/
/*		else								lwk2 = IvDetect											*/
/*		if ( |IwDetect| <= OnDelayLevel )	lwk3 = Iwref											*/
/*		else								lwk3 = IwDetect											*/
/*--------------------------------------------------------------------------------------------------*/
			if( LPX_ABS(AxisRscI->CurDet.IuDetect) > LPX_ABS(AxisRscI->OnDelay.OnDelayLevel) )
			{
				lwk1.l = AxisRscI->CurDet.IuDetect;								/* FB電流を使用			*/
			}
			else
			{
				lwk1.l = AxisRscI->OnDelay.Iuref;								/* 電流指令を使用		*/
			}

			if( LPX_ABS(AxisRscI->CurDet.IvDetect) > LPX_ABS(AxisRscI->OnDelay.OnDelayLevel ) )
			{
				lwk2.l = AxisRscI->CurDet.IvDetect;								/* FB電流を使用			*/
			}
			else
			{
				lwk2.l = AxisRscI->OnDelay.Ivref;								/* 電流指令を使用		*/
			}

			lwk3.l = (LONG)ZeroR - AxisRscI->CurDet.IuDetect;
			lwk3.l = lwk3.l - AxisRscI->CurDet.IvDetect;				/* lwk3 = -IuDetect - IvDetect	*/

			if( LPX_ABS(lwk3.l) <= LPX_ABS(AxisRscI->OnDelay.OnDelayLevel) )
			{
				lwk3.l = (LONG)ZeroR - AxisRscI->OnDelay.Iuref;
				lwk3.l = lwk3.l - AxisRscI->OnDelay.Ivref;				/* lwk3 = -Iuref - Ivref		*/
			}
/*--------------------------------------------------------------------------------------------------*/
/*		if(SlopeOnDelay != 0) trapezoid type else rectangle type									*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->OnDelay.OnDelaySlope == (LONG)ZeroR )
			{
/*--------------------------------------------------------------------------------------------------*/
/*		矩形波補償																					*/
/*--------------------------------------------------------------------------------------------------*/
				lwk6.l = limit( lwk1.l, (LONG)OneR );					/* lwk6 = -1/0/+1					*/
				lwk1.s[0] = AxisRscI->OnDelay.OnDelayComp * lwk6.s[0];	/* lwk1.s = sign(Iu) * OnDelayComp	*/

				lwk6.l = limit( lwk2.l, (LONG)OneR );					/* lwk6 = -1/0/+1					*/
				lwk2.s[0] = AxisRscI->OnDelay.OnDelayComp * lwk6.s[0];	/* lwk2.s = sign(Iv) * OnDelayComp	*/

				lwk6.l = limit( lwk3.l, (LONG)OneR );					/* lwk6 = -1/0/+1					*/
				lwk3.s[0] = AxisRscI->OnDelay.OnDelayComp * lwk6.s[0];	/* lwk3.sr = sign(Iw) * OnDelayComp	*/
			}
			else
			{
/*--------------------------------------------------------------------------------------------------*/
/*		台形波補償																					*/
/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = mulshr( AxisRscI->OnDelay.OnDelaySlope, lwk1.l, 24 );		/* lwk0 = (Iu * SlopeOnDelay) >> 24			*/
				lwk0.l = limit( lwk0.l, (LONG)No14bit );							/* ±16384でリミット						*/
				lwk1.s[0] = mulshr( AxisRscI->OnDelay.OnDelayComp, lwk0.s[0], 14 );	/* lwk1.s = (lwk0.s * OnDelayComp) >> 14	*/

				lwk0.l = mulshr( AxisRscI->OnDelay.OnDelaySlope, lwk2.l, 24 );		/* lwk0 = (Iv * SlopeOnDelay) >> 24			*/
				lwk0.l = limit( lwk0.l, (LONG)No14bit );							/* ±16384でリミット						*/
				lwk2.s[0] = mulshr( AxisRscI->OnDelay.OnDelayComp, lwk0.s[0], 14 );	/* lwk2.s = (lwk0.s * OnDelayComp) >> 14	*/

				lwk0.l = mulshr( AxisRscI->OnDelay.OnDelaySlope, lwk3.l, 24 );		/* lwk0 = (Iw * SlopeOnDelay) >> 24			*/
				lwk0.l = limit( lwk0.l, (LONG)No14bit );							/* ±16384でリミット						*/
				lwk3.s[0] = mulshr( AxisRscI->OnDelay.OnDelayComp, lwk0.s[0], 14 );	/* lwk3.s = (lwk0.sr * OnDelayComp) >> 14	*/
			}
		}

/****************************************************************************************************/
/*																									*/
/*		三角波比較値計算																			*/
/*																									*/
/****************************************************************************************************//* <S18E>:修正 */
/*--------------------------------------------------------------------------------------------------*/
/*		-400000h..400000h →  0h..800000h → 0h..CRFRQ												*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscI->Curctrl.Vuref = limit( AxisRscI->Curctrl.Vuref, (LONG)No22bit );	/* ±400000hでリミット	*/
		AxisRscI->Curctrl.Vvref = limit( AxisRscI->Curctrl.Vvref, (LONG)No22bit );	/* ±400000hでリミット	*/
		AxisRscI->Curctrl.Vwref = limit( AxisRscI->Curctrl.Vwref, (LONG)No22bit );	/* ±400000hでリミット	*/
/*--------------------------------------------------------------------------------------------------*/
/*		本当は、(Vxref-400000)で計算するのを(400000h-Vxref)としている。								*/
/*		理由：電流検出の向きが逆のため(アンプ→モータに流れたときにマイナスになる)、電流のみ符号を	*/
/*			  逆転させればよかったが、出力電圧の符号を逆転した。そのため、エンコーダの取り付けも	*/
/*			  180゜ずらして取り付けている。															*/
/*--------------------------------------------------------------------------------------------------*/
		lwk0.l = (ULONG)AxisRscI->IntAdV.CarrierFreq;			/* 16bit→32bit(符号なし)			*/

		lwk4.l = (LONG)No22bit - AxisRscI->Curctrl.Vuref;
		lwk7.l = mulshr( lwk4.l, lwk0.l, 23 );

		lwk5.l = (LONG)No22bit - AxisRscI->Curctrl.Vvref;
		lwk8.l = mulshr( lwk5.l, lwk0.l, 23 );

		lwk6.l = (LONG)No22bit - AxisRscI->Curctrl.Vwref;
		lwk9.l = mulshr( lwk6.l, lwk0.l, 23 );
/*--------------------------------------------------------------------------------------------------*/
/*		Deat-time compensation (timer) : if(Vx == 0 || Vx == CarrierFreq) No compensation			*/
/*--------------------------------------------------------------------------------------------------*/
		if( ( lwk7.s[0] != (SHORT)ZeroR ) && (lwk7.s[0] != AxisRscI->IntAdV.CarrierFreq ) )
		{
			lwk7.s[0] = lwk7.s[0] - lwk1.s[0];								/* Vuref + オンディレイ補正分	*/
			lwk7.s[0] = limit( lwk7.s[0], AxisRscI->IntAdV.CarrierFreq );	/* 0～CarrierFreqでリミット		*/
		}

		if( ( lwk8.s[0] != (SHORT)ZeroR ) && (lwk8.s[0] != AxisRscI->IntAdV.CarrierFreq ) )
		{
			lwk8.s[0] = lwk8.s[0] - lwk2.s[0];								/* Vvref + オンディレイ補正分	*/
			lwk8.s[0] = limit( lwk8.s[0], AxisRscI->IntAdV.CarrierFreq );	/* 0～CarrierFreqでリミット		*/
		}

		if( ( lwk9.s[0] != (SHORT)ZeroR ) && (lwk9.s[0] != AxisRscI->IntAdV.CarrierFreq ) )
		{
			lwk9.s[0] = lwk9.s[0] - lwk3.s[0];								/* Vwref + オンディレイ補正分	*/
			lwk9.s[0] = limit( lwk9.s[0], AxisRscI->IntAdV.CarrierFreq );	/* 0～CarrierFreqでリミット		*/
		}
		AxisRscI->PwmV.PwmCntT2 = lwk9.s[0];
		AxisRscI->PwmV.PwmCntT1 = lwk8.s[0];
		AxisRscI->PwmV.PwmCntT0 = lwk7.s[0];
	}


/****************************************************************************************************/
/*		Current Control Output Procedure															*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS								/* 多軸処理有効										*/
	for( ax_noI = 0; (SHORT)ax_noI < AxisNum; ax_noI++ )
#else
	ax_noI = 0;
#endif
	{
		AxisRscI = &AxisHdl[ax_noI];

/*--------------------------------------------------------------------------------------------------*/
/*		Output status																				*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscI->MicroIf.CtrlStsOut = AxisRscI->StsFlg.CtrlStsRW;

/*--------------------------------------------------------------------------------------------------*/
/*		System Counter																				*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscI->Systems.IntHostCtr++;
	}
/*--------------------------------------------------------------------------------------------------*/
/*		PWM data set																				*/
/*--------------------------------------------------------------------------------------------------*/
	SetPWM( &AxisHdl[0] );

/*--------------------------------------------------------------------------------------------------*/
/*		自己割り込み禁止解除																		*/
/*--------------------------------------------------------------------------------------------------*/
	INTLVWR = 0x0003;

	OUTPT = 0x0;
	IniWk.IN_WK1H++;

	return;
}


/****************************************************************************************************/
/*																									*/
/*		Encoder(SPG0) Interrupt Procedure		; 通常(初期インクレパルス出力完了時):11clk			*/
/*																									*/
/*		[注意]優先順位が最高位の割込処理なので、できるだけ短い処理にすること。						*/
/****************************************************************************************************/
void	MpIntEnc( void )
{
#if 0		/* JL086で実行するためコメントアウト		*/
/*--------------------------------------------------------------------------------------------------*/
		if( EncIfV.IncPlsReq == 1 )
		{
			PCVS0 = EncIfV.DivPls.s[0];			/* パルス変換位置セット								*/
		}
		else if( EncIfV.PAOSeqCmd != PAOPLSOUT )
		{
			PCVS0 = (SHORT)IHostWk.IncInitPls;	/* パルス変換位置セット								*/
		}
/*--------------------------------------------------------------------------------------------------*/
		IEncWk.RxFlg0 = FCCST;		/* SDM status bit8 : IEncWk.RxFlg0(Serial-Enc0 receive flag)	*/
/*--------------------------------------------------------------------------------------------------*/
/*		処理時間短縮のため、使用しないデータの読込みはしない。										*/
/*--------------------------------------------------------------------------------------------------*/
		IEncWk.RxPos.s[0] = SRPG0RD5;			/* 今回値読込み：Position Low						*/
		IEncWk.RxPos.s[1] = SRPG0RD6;			/* 今回値読込み：Position High						*/
/*--------------------------------------------------------------------------------------------------*/
		IEncWk.EncWk0 = INT1SET;				/* INT1 Acknowledge									*/
/*--------------------------------------------------------------------------------------------------*/
#endif	//#if 0		/* JL086で実行するためコメントアウト		*/
		return;									/* return											*/
}



#if 0		/* JL086で実行するためコメントアウト		*/
/****************************************************************************************************/
/*																									*/
/*		分周パルス更新処理						;	最大:???clk, 通常:???clk						*/
/*																									*/
/****************************************************************************************************/
void	MpUPDATE_DIVPOS( void )
{
/*--------------------------------------------------------------------------------------------------*/
		IHostWk.Divuswk = INT1SET;				/* INT1 Acknowledge									*/
/*--------------------------------------------------------------------------------------------------*/
		IHostWk.LastRcvPosX = EncIfV.RcvPosX0.l;			/* 前回位置データ更新					*/
/*--------------------------------------------------------------------------------------------------*/
/*		シリアルエンコーダ受信チェック			; IEncWk.RxFlg0の値は@INT_ENC割込にて更新			*/
/*--------------------------------------------------------------------------------------------------*/
//		Divuswk = IEncWk.RxFlg0;				/* SDMSTS bit8 : SPG0 Recieve Completed Check		*/
		if( (IEncWk.RxFlg0 & 0x100 ) == 0 )
		{
			if( EncIfV.SPGFail >= IHostWk.EncMstErrCnt )
			{
				EncIfV.RcvPosX2.l = EncIfV.RcvPosX1.l;						/* 前々回位置データ											*/
				EncIfV.RcvPosX1.l = EncIfV.RcvPosX0.l;						/* 前回位置データ											*/
				EncIfV.RcvPosX0.l = EncIfV.RcvPosX0.l + EncIfV.RcvPosX1.l;	/* 補間演算													*/
				EncIfV.RcvPosX0.l = EncIfV.RcvPosX0.l - EncIfV.RcvPosX2.l;	/* EncIfV.RcvPosX0 += (EncIfV.RcvPosX1 - EncIfV.RcvPosX2)	*/
				IHostWk.EncMstErrCnt++;										/* IHostWk.EncMstErrCnt++									*/
			}
		}
/*--------------------------------------------------------------------------------------------------*/
		else
		{
			IHostWk.RxPos0 = IEncWk.RxPos.l;	/* 今回値更新 : IEncWk.RxPosの値は@INT_ENC割込にて更新			*/
/*--------------------------------------------------------------------------------------------------*/
/*		位置演算																					*/
/*		IHostWk.RcvPosX = MencP.MposSign * ((MencV.RxPosL[0].sl>>MencP.MposSftX)<<MencP.MposSftR);	*/
/*																									*/
/*		32bit上位詰めデータのため、論理シフトにて計算(符号ビットの影響なし)							*/
/*--------------------------------------------------------------------------------------------------*/
			IHostWk.RcvPosX = ( IHostWk.RxPos0 >> EncIfV.MotPosSftX ) << EncIfV.MotPosSftR;			/* IHostWk.RcvPosX = (ULONG)DivWk0  << EncIfV.MotPosSftR	*/
/*--------------------------------------------------------------------------------------------------*/
/*		IHostWk.RcvPosX = IHostWk.RcvPosX * EncIfV.MotPosSign										*/
/*--------------------------------------------------------------------------------------------------*/
			if( EncIfV.MotPosSign != 1 )
			{
				IHostWk.RcvPosX = ~IHostWk.RcvPosX;
				IHostWk.RcvPosX = IHostWk.RcvPosX + ONER;	/* IHostWk.RcvPosX = -IHostWk.RcvPosX	*/
			}
/*--------------------------------------------------------------------------------------------------*/
/*		加速度演算チェック																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( DivPlsV.AccCntClrReq != 0 )
			{
				IHostWk.Divuswk = ~EncIfV.BitData;			/* DivWk0=~EncIfV.BitData							*/
				IHostWk.Divuswk = IHostWk.Divuswk | ACCCHKENA;			/* DivWk0.ACCCHKENA = TRUE				*/
				EncIfV.BitData = ~IHostWk.Divuswk;			/* EncIfV.BitData=~DivWk0							*/
				IHostWk.AccChkCnt = 0;						/* IHostWk.AccChkCnt = 0							*/
				DivPlsV.AccCntClrReq = 0;					/* 加速度チェック開始カウントクリア要求リセット		*/
			}
//			Divuswk = EncIfV.BitData;
			if( ( EncIfV.BitData & ACCCHKENA ) == 0 )
			{
				IHostWk.MotAcc = ZEROR;						/* IHostWk.MotAcc = 0								*/
				IHostWk.AccChkCnt++;						/* IHostWk.AccChkCnt++								*/
				if( IHostWk.AccChkCnt >= 4 )
				{
					EncIfV.BitData = EncIfV.BitData | ACCCHKENA;		/* EncIfV.BitData.ACCCHKENA = TRUE		*/
				}
				EncIfV.RcvPosX0.l = IHostWk.RcvPosX;	/* EncIfV.RcvPosX0 = IHostWk.RcvPosX		*/
				EncIfV.RcvPosX1.l = IHostWk.RcvPosX;	/* EncIfV.RcvPosX1 = IHostWk.RcvPosX		*/
				EncIfV.RcvPosX2.l = IHostWk.RcvPosX;	/* EncIfV.RcvPosX2 = IHostWk.RcvPosX		*/
			}
			else
			{
				IHostWk.DivWk0 = IHostWk.RcvPosX - EncIfV.RcvPosX0.l;	/* DivWk0   = IHostWk.RcvPosX  - EncIfV.RcvPosX0	*/
				IHostWk.DivWk1 = EncIfV.RcvPosX0.l - EncIfV.RcvPosX1.l;	/* DivWk1   = EncIfV.RcvPosX0 - EncIfV.RcvPosX1		*/
				IHostWk.MotAcc = IHostWk.DivWk0 - IHostWk.DivWk1;		/* IHostWk.MotAcc   = DivWk0 - DivWk1				*/
				if( EncIfV.AccErrLv.l >= IHostWk.MotAcc )
				{
					if( ( EncIfV.AccErrLv.l + IHostWk.MotAcc ) < 0 )
					{
/*--------------------------------------------------------------------------------------------------*/
/*		DivWk0 = (IHostWk.RcvPosX - EncIfV.RcvPosX1) >> 1											*/
/*--------------------------------------------------------------------------------------------------*/
						IHostWk.DivWk0 = IHostWk.RcvPosX - EncIfV.RcvPosX1.l;	/* DivWk0 =  IHostWk.RcvPosX  - EncIfV.RcvPosX1		*/
						IHostWk.DivWk0 = IHostWk.DivWk0 & 0xfffffffe;			/* 算術右シフトの四捨五入無効化の対策				*/
						IHostWk.DivWk0 = IlibASR32(IHostWk.DivWk0 , 1);			/* DivWk0  = (IHostWk.RcvPosX  - EncIfV.RcvPosX1) >> 1	*/
						IHostWk.DivWk1 = EncIfV.RcvPosX1.l - EncIfV.RcvPosX2.l;	/* DivWk1  =  EncIfV.RcvPosX1 - EncIfV.RcvPosX2		*/
						IHostWk.MotAcc = IHostWk.DivWk0 - IHostWk.DivWk1;		/* IHostWk.MotAcc  =  DivWk0 - DivWk1				*/
					}
				}
				else
				{
/*--------------------------------------------------------------------------------------------------*/
/*		DivWk0 = (IHostWk.RcvPosX - EncIfV.RcvPosX1) >> 1											*/
/*--------------------------------------------------------------------------------------------------*/
					IHostWk.DivWk0 = IHostWk.RcvPosX - EncIfV.RcvPosX1.l;		/* DivWk0 =  IHostWk.RcvPosX  - EncIfV.RcvPosX1		*/
					IHostWk.DivWk0 = IHostWk.DivWk0 & 0xfffffffe;				/* 算術右シフトの四捨五入無効化の対策				*/
					IHostWk.DivWk0 = IlibASR32(IHostWk.DivWk0 , 1);				/* DivWk0  = (IHostWk.RcvPosX  - EncIfV.RcvPosX1) >> 1	*/
					IHostWk.DivWk1 = EncIfV.RcvPosX1.l - EncIfV.RcvPosX2.l;		/* DivWk1  =  EncIfV.RcvPosX1 - EncIfV.RcvPosX2		*/
					IHostWk.MotAcc = IHostWk.DivWk0 - IHostWk.DivWk1;			/* IHostWk.MotAcc  =  DivWk0 - DivWk1				*/
				}
			}
			if( EncIfV.AccErrLv.l >= IHostWk.MotAcc )
			{
/*--------------------------------------------------------------------------------------------------*/
/*		加速度異常時																				*/
/*--------------------------------------------------------------------------------------------------*/
				if( EncIfV.SPGFail < IHostWk.EncMstErrCnt )
				{
					EncIfV.RcvPosX2.l = EncIfV.RcvPosX1.l;	/* 前々回位置データ						*/
					EncIfV.RcvPosX1.l = EncIfV.RcvPosX0.l;	/* 前回位置データ						*/
					EncIfV.RcvPosX0.l = IHostWk.RcvPosX;	/* 加速度異常時は補間しない				*/
					IHostWk.EncMstErrCnt++;					/* IHostWk.EncMstErrCnt++				*/
				}
			}
			else if( ( EncIfV.AccErrLv.l + IHostWk.MotAcc ) < 0 )
			{
/*--------------------------------------------------------------------------------------------------*/
/*		加速度正常時																				*/
/*--------------------------------------------------------------------------------------------------*/
				IHostWk.EncMstErrCnt = 0;					/* IHostWk.EncMstErrCnt=0				*/
				EncIfV.RcvPosX2.l = EncIfV.RcvPosX1.l;		/* 前々回位置データ						*/
				EncIfV.RcvPosX1.l = EncIfV.RcvPosX0.l;		/* 前回位置データ						*/
				EncIfV.RcvPosX0.l = IHostWk.RcvPosX;		/* 今回位置データ						*/
			}
/*--------------------------------------------------------------------------------------------------*/
		}
/*--------------------------------------------------------------------------------------------------*/
/*		dMotPos = RMX_dPosOfXpos( MencV.MotPosX[0], LastMotPosX );									*/
/*--------------------------------------------------------------------------------------------------*/
/*		算術右シフトにて切り捨てられる下位ビットは0のため、四捨五入の影響なし。						*/
/*--------------------------------------------------------------------------------------------------*/
		IHostWk.DMotPos = EncIfV.RcvPosX0.l - IHostWk.LastRcvPosX;		/* IHostWk.DMotPos = EncIfV.RcvPosX0 - IHostWk.LastRcvPosX	*/
		IHostWk.DMotPos = IlibASR32(IHostWk.DMotPos , EncIfV.MotPosSftR);
/*--------------------------------------------------------------------------------------------------*/
		if( EncIfV.IncPlsReq == 1 )
		{
			EncIfV.PlsOSetCmd = DivPlsV.PlsOSetCmdIn;		/* パルス出力回路初期化要求更新 from HostCPU		*/
			if( EncIfV.PlsOSetCmd == POSETCMD00 )
			{
				PCVS0 = 0x0000;
				DivPlsV.PlsOSetCmdIn = POSETNOCMD;			/* 初期化要求クリア						*/
			}
			else if( EncIfV.PlsOSetCmd == POSETCMDFF )
			{
				PCVS0 = 0xFFFF;
				DivPlsV.PlsOSetCmdIn = POSETNOCMD;			/* 初期化要求クリア						*/
			}
			else
			{
				IHostWk.IncInitPls = DivPlsV.IncInitPlsIn.l;
				EncIfV.DivPls.l = DivPlsV.IncInitPlsIn.l;
				EncIfV.DivPos.l = DivPlsV.IncInitPlsIn.l;				/* for Linear				*/
				EncIfV.DivPlsRem.l = DivPlsV.IncInitRemIn.l;			/* for Linear				*/
			}
		}
		else
		{
			if( IHostWk.PoSet1W != DivPlsV.PoSet1In )
			{
				IHostWk.PoSet1W = DivPlsV.PoSet1In;
				IHostWk.PoSet2W = DivPlsV.PoSet2In;
				PCVS1 = IHostWk.PoSet1W;		/* パルス変換原点補正1セット (HostCPUと同じ状態に設定)			*/
				PCVS2 = IHostWk.PoSet2W;		/* パルス変換原点補正2セット									*/
			}
		}
		if( IHostWk.DivSetW != DivPlsV.DivSetIn )
		{
			IHostWk.DivSetW = DivPlsV.DivSetIn;
			DivSet = IHostWk.DivSetW;			/* 分周機能セット (HostCPUと同じ状態に設定)			*/
		}
		if( EncIfV.IncPlsReq != 1 )
		{
			if( EncIfV.AmpType != LINEAR )
			{
/*--------------------------------------------------------------------------------------------------*/
//		分周パルス = (MencV.MotPosX[0] >> MencP.EncIfV.DivOutSft);									*
/*--------------------------------------------------------------------------------------------------*/
//		算術右シフトにて切り捨てられる下位ビットを0にする(四捨五入無効化対策)						*
/*--------------------------------------------------------------------------------------------------*/
				IHostWk.DivWk1 = NONER << EncIfV.DivOutSft;						/* DivWk1=(FFFFFFFFh<<EncIfV.DivOutSft)												*/
				IHostWk.DivWk0 = EncIfV.RcvPosX0.l & IHostWk.DivWk1;			/* DivWk0=((EncIfV.RcvPosX0&(FFFFFFFFh<<EncIfV.DivOutSft))							*/
				EncIfV.DivPls.l = IlibASR32(IHostWk.DivWk0 , EncIfV.DivOutSft);	/* EncIfV.DivPls=((EncIfV.RcvPosX0&(FFFFFFFFh<<EncIfV.DivOutSft))>>EncIfV.DivOutSft	*/
			}
			else
			{
				DivPlsV.Argu0.l = IHostWk.DMotPos;			/* DivPlsV.Argu0 <-- IHostWk.DMotPos	*/
				DivPlsV.Argu1.l = EncIfV.DivOutGain.l;		/* DivPlsV.Argu1 <-- EncIfV.DivOutGain	*/
				DivPlsV.Iu0.l = EncIfV.DivPlsRem.l;			/* DivPlsV.Iu0 <-- EncIfV.DivPlsRem		*/
				MpMlibPfbkxremNolim( );						/* DivPlsV.Ret0 = MLIBPFBKXREMNOLIM()	*/
				EncIfV.DivPos.l = EncIfV.DivPos.l + DivPlsV.Ret0.l;		/* EncIfV.DivPos = EncIfV.DivPos + DivPlsV.Ret0		*/
				EncIfV.DivPlsRem.l = DivPlsV.Iu0.l;			/* EncIfV.DivPlsRem <-- DivPlsV.Iu0		*/
				EncIfV.DivPls.l = EncIfV.DivPos.l;			/* EncIfV.DivPls = EncIfV.DivPos		*/
			}
		}
		EncIfV.IncPlsReq = DivPlsV.IncPlsReqIn;				/* 初期インクレパルス出力要求更新 from HostCPU		*/
		EncIfV.PAOSeqCmd = DivPlsV.PAOSeqCmdIn;

		return;												/* return								*/
}
#endif	//#if 0		/* JL086で実行するためコメントアウト		*/

			

/****************************************************************************************************/
/*																									*/
/*		DATA clear subroutin																		*/
/*																									*/
/****************************************************************************************************/
void	MpDataClear( MICRO_AXIS_HANDLE *AxisRsc )
{
/*--------------------------------------------------------------------------------------------------*/
/*		HOST int clear																				*/
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->Curctrl.Idref = ZeroR;
	AxisRsc->Curctrl.Iqref = ZeroR;
	AxisRsc->Curctrl.IntegD.l[0] = ZeroR;
	AxisRsc->Curctrl.IntegD.l[1] = ZeroR;
	AxisRsc->Curctrl.VdBeforeLim = ZeroR;
	AxisRsc->Curctrl.Vdref = ZeroR;
	AxisRsc->Curctrl.IntegQ.l[0] = ZeroR;
	AxisRsc->Curctrl.IntegQ.l[1] = ZeroR;
	AxisRsc->Curctrl.VqBeforeLim = ZeroR;
	AxisRsc->Curctrl.Vqref = ZeroR;
	AxisRsc->Curctrl.Vuref = ZeroR;
	AxisRsc->Curctrl.Vvref = ZeroR;
	AxisRsc->Curctrl.Vwref = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->Vfil.VdLpfOut = ZeroR;
	AxisRsc->Vfil.VqLpfOut = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->OnDelay.Iuref = ZeroR;
	AxisRsc->OnDelay.Ivref = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->WeakFV.WfInteg.l[0] = ZeroR;
	AxisRsc->WeakFV.WfInteg.l[1] = ZeroR;
	AxisRsc->WeakFV.WfIdref = ZeroR;
	AxisRsc->WeakFV.Idref0 = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->Trqctrl.Trqref = ZeroR;
	AxisRsc->Trqctrl.TrqAftFil = ZeroR;
	AxisRsc->Trqctrl.TrqAddDist = ZeroR;
	AxisRsc->Trqctrl.TrqAftLim = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->MicroIf.TrqMon = ZeroR;
	AxisRsc->MicroIf.TrqMonAftFil = ZeroR;
	AxisRsc->MicroIf.IdrefMon = ZeroR;
	AxisRsc->MicroIf.IqrefMon = ZeroR;
	AxisRsc->MicroIf.IdDetectMon = ZeroR;
	AxisRsc->MicroIf.IqDetectMon = ZeroR;
	AxisRsc->MicroIf.VdrefMon = ZeroR;
	AxisRsc->MicroIf.VqrefMon = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	AxisRsc->NotchFil.Notch1Value0 = ZeroR;
	AxisRsc->NotchFil.Notch1Value1 = ZeroR;
	AxisRsc->NotchFil.Notch1Value2 = ZeroR;
	AxisRsc->NotchFil.Notch1Value3 = ZeroR;
	AxisRsc->NotchFil.Notch1Out = ZeroR;
	AxisRsc->NotchFil.Notch2Value0 = ZeroR;
	AxisRsc->NotchFil.Notch2Value1 = ZeroR;
	AxisRsc->NotchFil.Notch2Value2 = ZeroR;
	AxisRsc->NotchFil.Notch2Value3 = ZeroR;
	AxisRsc->NotchFil.Notch2Out = ZeroR;
	AxisRsc->NotchFil.Notch3Value0 = ZeroR;
	AxisRsc->NotchFil.Notch3Value1 = ZeroR;
	AxisRsc->NotchFil.Notch3Value2 = ZeroR;
	AxisRsc->NotchFil.Notch3Value3 = ZeroR;
	AxisRsc->NotchFil.Notch3Out = ZeroR;
	AxisRsc->NotchFil.Notch4Value0 = ZeroR;
	AxisRsc->NotchFil.Notch4Value1 = ZeroR;
	AxisRsc->NotchFil.Notch4Value2 = ZeroR;
	AxisRsc->NotchFil.Notch4Value3 = ZeroR;
	AxisRsc->NotchFil.Notch4Out = ZeroR;
	AxisRsc->NotchFil.Notch5Value0 = ZeroR;
	AxisRsc->NotchFil.Notch5Value1 = ZeroR;
	AxisRsc->NotchFil.Notch5Value2 = ZeroR;
	AxisRsc->NotchFil.Notch5Value3 = ZeroR;
	AxisRsc->NotchFil.Notch5Out = ZeroR;
/*--------------------------------------------------------------------------------------------------*/
	return;
}



/****************************************************************************************************/
/*																									*/
/*		 SQRT(TMP2(32)) Sub-routin																	*/
/*																									*/
/****************************************************************************************************/
/*		Input		ULONG src 	: High(16), Low(16)													*/
/*		Output		uswk0 		: SQRT(dat)															*/
/****************************************************************************************************/
inline	USHORT	MpSQRT( ULONG src )
{
	USHORT		uswk0;
	ULONG		ulwk0;
	ULONG		ulwk2;

	uswk0 = sqrt( src );									/* 結果は小数点以下は切り捨て			*/
	ulwk2 = mul( (SHORT)uswk0, (SHORT)uswk0 );				/* 平方根の結果を自乗					*/
	ulwk2 = src - ulwk2;									/* 入力と自乗の差を取る(切捨て誤差)		*/
	ulwk0 = (ULONG)uswk0;
	if( uswk0 < 0xffff )
	{ /* 最大値を超えない場合 */
		if( ulwk0 < ulwk2 )
		{ /* 切捨て誤差が平方根の結果より大きい場合 */
			/* 補正処理 */
			uswk0 = uswk0 + 1;
		}
	}
	else
	{
		/* 最大値を超える場合は切捨ての補正なし	*/
	}

	return ( uswk0 );
}


/****************************************************************************************************/
/*																									*/
/*		Over modulation composation calculation														*/
/*																									*/
/****************************************************************************************************/
/*		INPUT:   IntAdP: table address, V1:modulation, NormV1:modulation(=V1/2^9)					*/
/*		OUTPUT:  Kmod:   compensation gain/offset													*/
/*		work:    swk0,swk1,swk2,swk3,swk4															*/
/****************************************************************************************************/
inline	SHORT	MpOVMMODK( LONG V1, SHORT NormV1, SHORT CtrlSwitch, CSHORT* pCtbl )
{

	SHORT	swk0;											/* 16bitワークレジスタ0					*/
	SHORT	swk1;											/* 16bitワークレジスタ1					*/
	SHORT	swk2;											/* 16bitワークレジスタ2					*/
	SHORT	swk3;											/* 16bitワークレジスタ3					*/
	SHORT	swk4;											/* 16bitワークレジスタ4					*/

	if( V1 < (LONG)V115 )
	{
		IxLoadMpmem16( swk4, pCtbl, 0 );					/* IntAdP->Kmod = G[0];					*/
	}
	else if( (CtrlSwitch & OVMMOD) == 0 )
	{
		IxLoadMpmem16( swk4, pCtbl, 0 );					/* IntAdP->Kmod = G[0];					*/
	}
	else
	{
		if( V1 < (LONG)V127 )
		{
			swk0 = NormV1;
			swk0 = swk0 - 9443;								/* -9439-5(margin)						*/
			swk1 = swk0;
			swk0 = (USHORT)swk0 >> 5;						/* high(論理シフト)						*/
			swk1 = swk1 & 0x1F;								/* low									*/
			if( swk0 >= 32 )
			{	/* 変調率1.27以上場合 */
				pCtbl = pCtbl + 30;
				IxLoadMpmem16( swk4, pCtbl, 1 );
			}
			else
			{	/* 変調率1.27以下場合 */
				swk2 = swk0;
				if( ( swk2 & 1 ) == 0 )
				{
					/* テーブル2点取り出し */
					pCtbl = pCtbl + swk0;
 					IxLoadMpmem16( swk2, pCtbl, 0 );
					IxLoadMpmem16( swk3, pCtbl, 1 );
				}
				else
				{
					/* テーブル2点取り出し */
					pCtbl = pCtbl + swk0;
					IxLoadMpmem16( swk2, pCtbl, 1 );
					pCtbl = pCtbl + 2;
					IxLoadMpmem16( swk3, pCtbl, 0 );
				}
				/* 補間処理 */
				swk0 = swk3 - swk2;
				swk0 = mulshr(swk0, swk1, 5);
				swk4 = swk0 + swk2;
			}
		}
		else
		{	/* 変調率1.27以上場合 */
			pCtbl = pCtbl + 30;
			IxLoadMpmem16( swk4, pCtbl, 1 );
		}
	}
	return	swk4;
}


#if 0
/****************************************************************************************************/
/*																									*/
/*		制御演算ライブラリ																			*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		余り付き位置ＦＢ計算 : rv = (kx*u+pfbrem)>>sx 	; ??clk										*/
/*																									*/
/****************************************************************************************************/
//LONG	MpMlibPfbkxremNolim(
/*		LONG u,									/* DivPlsV.Argu0    : 入力							*/
/*		LONG k,									/* DivPlsV.Argu1    : ゲイン						*/
/*		LONG *pfbrem )							/* DivPlsV.Iu0    : 余りへのポインタ				*/
/*--------------------------------------------------------------------------------------------------*/
/*												/* DivPlsV.Ret0     : 戻り値						*/
/*--------------------------------------------------------------------------------------------------*/
/*		LONG	kx								/* DivPlsV.Kx     : kx								*/
/*		LONG	sx								/* DivPlsV.Sx     : sx								*/
/*		LONG	rv								/* lswk10  : 演算結果								*/
/*		LONG	pfbrem							/* lswk11  : 余り									*/
/*		LONG	wk1								/* lswk1	  : 作業用								*/
/*		LONG	wk2								/* lswk2   : 作業用									*/
/*												/* lswk3   : 乗算結果保持用(下位32bit)				*/
/*												/* lswk4   : 乗算結果保持用(上位32bit)				*/
/*--------------------------------------------------------------------------------------------------*/
void	MpMlibPfbkxremNolim( void )
{
/*--------------------------------------------------------------------------------------------------*/
		DivPlsV.Kx.l = DivPlsV.Argu1.l << 8;		/* DivPlsV.Kx = k<<8							*/
		DivPlsV.Sx.l = DivPlsV.Argu1.l >> 24;		/* DivPlsV.Sx = k>>24							*/
/*--------------------------------------------------------------------------------------------------*/
		IPfbwk.lswk1 = 24;							/* lswk1 = 24									*/
		if( IPfbwk.lswk1 >= DivPlsV.Sx.l )
		{
/*--------------------------------------------------------------------------------------------------*/
			IPfbwk.dlwk.l[0] = DivPlsV.Argu0.l * DivPlsV.Kx.l;  		/* provision				*/
			IPfbwk.lswk1 = IPfbwk.lswk1 - DivPlsV.Sx.l;					/* lswk1 = 24 - sx			*/
/*--------------------------------------------------------------------------------------------------*/
			IPfbwk.lswk2 = IPfbwk.dlwk.l[0] >> DivPlsV.Sx.s[0];			/* lswk2 = (xl>>sx)							*/
			IPfbwk.lswk2 = IPfbwk.lswk2 >> 8;							/* lswk2 =((xl>>sx)>>8)						*/
			IPfbwk.lswk10 = IPfbwk.dlwk.l[1] << IPfbwk.lswk1;			/* lswk10 = (xh<<(24-sx))					*/
			IPfbwk.lswk10 = IPfbwk.lswk10 + IPfbwk.lswk2;				/* lswk10 =((xh<<(24-sx)) + ((xl>>sx)>>8))	*/
/*--------------------------------------------------------------------------------------------------*/
			IPfbwk.lswk11 = IPfbwk.dlwk.l[0] << IPfbwk.lswk1;			/* lswk11 = (xl<<(24-sx))					*/
			IPfbwk.lswk11 = IPfbwk.lswk11 >> 8;							/* lswk11 =((xl<<(24-sx))>>8)				*/
			IPfbwk.lswk11 = IPfbwk.lswk11 + DivPlsV.Iu0.l;
		}
		else
		{
			IPfbwk.dlwk.l[0] = DivPlsV.Argu0.l * DivPlsV.Kx.l;  		/* provision				*/
			IPfbwk.lswk3 = IPfbwk.dlwk.l[0];							/* lswk3 = xl				*/
			IPfbwk.lswk4 = IPfbwk.dlwk.l[1];							/* lswk4 = xh				*/
			IPfbwk.lswk1 = DivPlsV.Sx.l - IPfbwk.lswk1;					/* lswk1 = sx - 24			*/
/*--------------------------------------------------------------------------------------------------*/
/*			 算術右シフトにて切り捨てられる下位ビットを0にする(四捨五入無効化対策)					*/
/*--------------------------------------------------------------------------------------------------*/
			IPfbwk.lswk2 = NONER << IPfbwk.lswk1;			/* lswk2 =(FFFFFFFFh<<(sx-24))			*/
			IPfbwk.lswk2 = IPfbwk.lswk4 & IPfbwk.lswk2;		/* lswk2 =(xh & (FFFFFFFFh<<(sx-24)))	*/
/*--------------------------------------------------------------------------------------------------*/
			IPfbwk.lswk11 = IPfbwk.lswk3 >> IPfbwk.lswk1;	/* lswk11 =   (xl>>(sx-24))							*/
			IPfbwk.lswk11 = IPfbwk.lswk11 >> 7;				/* lswk11 =  ((xl>>(sx-24))>>7)						*/
			IPfbwk.lswk11 = IPfbwk.lswk11 + ONER;			/* lswk11 = (((xl>>(sx-24))>>7)+1)					*/
			IPfbwk.lswk11 = IPfbwk.lswk11 >> 1;				/* lswk11 =((((xl>>(sx-24))>>7)+1)>>1)				*/
			IPfbwk.lswk11 = IPfbwk.lswk11 + DivPlsV.Iu0.l;	/* lswk11 = pfbrem + ((((xl>>(sx-24))>>7)+1)>>1)	*/
/*--------------------------------------------------------------------------------------------------*/
			IPfbwk.lswk1 = 56;								/* lswk1 = 56										*/
			IPfbwk.lswk1 = IPfbwk.lswk1 - DivPlsV.Sx.l;		/* lswk1 = 56 - sx									*/
			IPfbwk.lswk2 = IPfbwk.lswk4 << IPfbwk.lswk1;	/* lswk2 = (xh<<(56-sx))							*/
			IPfbwk.lswk2 = IPfbwk.lswk2 >> 8;				/* lswk2 =((xh<<(56-sx))>>8)						*/
			IPfbwk.lswk11 = IPfbwk.lswk11 + IPfbwk.lswk2;	/* lswk11= lswk11 + ((xh<<(56-sx))>>8)				*/
		}
		IPfbwk.lswk2 = 0x00800000;							/* lswk2 = 0x00800000								*/
#if 0
		if( IPfbwk.lswk11 >= IPfbwk.lswk2 )
		{
			IPfbwk.lswk11 = IPfbwk.lswk11 - ( IPfbwk.lswk2 << 1 );		/* lswk11 = pfbrem - 0x00800000 * 2		*/
			IPfbwk.lswk10 = IPfbwk.lswk10 + ONER;						/* lswk10 = lswk10 + 1					*/
		}
#endif
		DivPlsV.Iu0.l = IPfbwk.lswk11;						/* lswk11 --> pfbrem					*/
		DivPlsV.Ret0.l = IPfbwk.lswk10;						/* lswk10 --> DivPlsV.Ret0				*/
/*--------------------------------------------------------------------------------------------------*/
		return;
}
#endif


/****************************************************************************************************/
/*																									*/
/*		IxMulgain32																					*/
/*																									*/
/****************************************************************************************************/
inline	void	IxMulgain32( LONG *x, LONG u, LONG k, DWREG *wk )
{
		DLREG	dlwk0;
/*--------------------------------------------------------------------------------------------------*/
/*		k --> kx, sx																				*/
/*--------------------------------------------------------------------------------------------------*/
		wk[0].l = (ULONG)k << 8;							/* kx = (k <<  8)						*/
		wk[1].l = (ULONG)k >> 24;							/* sx = (k >> 24)						*/
		wk[1].s[0] = wk[1].s[0] + 8;						/* sx = (k >> 24) + 8					*/
/*--------------------------------------------------------------------------------------------------*/
/*		x = (((u * kx) >> (sx-1)) + 1) >> 1															*/
/*		Limit by 0x7FFFFFFF																			*/
/*--------------------------------------------------------------------------------------------------*/
//		*x = mulshr_limitf( u, wk[0].l, wk[1].s[0] );		/* Mac = (u * kx)						*/
		dlwk0.dl = mul( u, wk[0].l );
		*x = asr_limitf( dlwk0.l[1], dlwk0.l[0], wk[1].s[0] );
															/* wk[0] = (((u*kx)>>(sx-1)) + 1) >> 1	*/
															/* x = Limit( wk[0], 0x7FFFFFFF )		*/
/*--------------------------------------------------------------------------------------------------*/
		return;
}


/****************************************************************************************************/
/*																									*/
/*		IxMulgainNolim																				*/
/*																									*/
/****************************************************************************************************/
inline	void	IxMulgainNolim( LONG *x, LONG u, LONG k, DWREG *wk )
{
		DLREG	dlwk0;
/*--------------------------------------------------------------------------------------------------*/
/*		k --> kx, sx																				*/
/*--------------------------------------------------------------------------------------------------*/
		wk[0].l = (ULONG)k << 8;							/* kx = (k <<  8)						*/
		wk[1].l = (ULONG)k >> 24;							/* sx = (k >> 24)						*/
		wk[1].s[0] = wk[1].s[0] + 8;						/* sx = (k >> 24) + 8					*/
/*--------------------------------------------------------------------------------------------------*/
/*		x = (((u * kx) >> (sx-1)) + 1) >> 1															*/
/*--------------------------------------------------------------------------------------------------*/
//		*x = mulshr( u, wk[0].l, wk[1].s[0] );				/* Mac = (u * kx)						*/
//		*x = (u * wk[0].l) >> wk[1].s[0];					/* Mac = (u * kx)						*/
		dlwk0.dl = mul( u, wk[0].l );
		*x = asr( dlwk0.dl, wk[1].s[0] );					/* x = (((u*kx)>>(sx-1)) + 1) >> 1		*/
/*--------------------------------------------------------------------------------------------------*/
		return;
}


/****************************************************************************************************/
/*																									*/
/*		IxLpfilter1																					*/
/*																									*/
/****************************************************************************************************/
inline	void	IxLpfilter1( LONG *x, LONG u, LONG k, DWREG *wk )
{
/*--------------------------------------------------------------------------------------------------*/
/*		Check k																						*/
/*--------------------------------------------------------------------------------------------------*/
		if( k == (LONG)ZeroR )
		{												/* if( k == 0 )								*/
			*x = u;										/* x = u									*/
		}
		else
		{
/*--------------------------------------------------------------------------------------------------*/
/*		wk[1].dr = ((((u - x) * k) >> 23) + 1) >> 1													*/
/*--------------------------------------------------------------------------------------------------*/
			wk[0].l = u - *x;							/* wk[0] = (u - x)							*/
			wk[1].l = mulshr( wk[0].l, k, 24 );			/* wk[1] = ((((u-x)*k)>>23) + 1) >> 1		*/
/*--------------------------------------------------------------------------------------------------*/
/*		Check Zero and Set +1/-1																	*/
/*--------------------------------------------------------------------------------------------------*/
			if( wk[1].l != (LONG)ZeroR )				/* if( wk[1] != 0 )							*/
			{
				*x = *x + wk[1].l;						/* x = (((((u-x)*k)>>23) + 1) >> 1)			*/
			}
/*--------------------------------------------------------------------------------------------------*/
			else if( wk[0].l != (LONG)ZeroR )
			{											/* if( (u-x) != 0 )							*/
				if( wk[0].l > (LONG)ZeroR )
				{										/* if( (u-x) > 0 )							*/
					wk[1].l = (LONG)OneR;				/* wk[1] = 1								*/
					*x = *x + wk[1].l;					/* x = (((((u-x)*k)>>23) + 1) >> 1)			*/
				}
				else
				{
					wk[1].l = (LONG)NoneR;				/* wk[1] = -1								*/
					*x = *x + wk[1].l;					/* x = (((((u-x)*k)>>23) + 1) >> 1)			*/
				}
			}
			else
			{											/* if( (u-x) == 0 )							*/
				;										/* 処理なし									*/
			}
		}
/*--------------------------------------------------------------------------------------------------*/
		return;
}


/****************************************************************************************************/
/*																									*/
/*		IxNxfilter2 : 2次ノッチフィルタ																*/
/*																									*/
/****************************************************************************************************/
inline	void	IxNxfilter2( LONG *x, LONG u, LONG k[5], LONG z[4], DWREG wk[4] )
{
#define NXF_PRM_BITS	24										/* NxFilter2 Prameter Bits Number	*/
/*--------------------------------------------------------------------------------------------------*/
/*		wk[0].dr = k[4] * u																			*/
/*--------------------------------------------------------------------------------------------------*/
		wk[0].l = mulshr( k[4], u, (SHORT)NXF_PRM_BITS );		/* Mac   = k[4] * u					*/
																/* wk[0] = ((Mac>>23) + 1) >> 1		*/
/*--------------------------------------------------------------------------------------------------*/
/*		wk[0].dr = wk[0].dr + (k[0] * z[0]) + (k[1] * z[1])											*/
/*--------------------------------------------------------------------------------------------------*/
		wk[1].l = mulshr( k[0], z[0], (SHORT)NXF_PRM_BITS );	/* Mac   = k[0] * z[0]				*/
																/* wk[1] = ((Mac>>23) + 1) >> 1		*/
		wk[0].l = wk[0].l + wk[1].l;							/* wk[0] = wk[0] + wk[1]			*/
/*--------------------------------------------------------------------------------------------------*/
		wk[1].l = mulshr( k[1], z[1], (SHORT)NXF_PRM_BITS );	/* Mac   = k[1] * z[1]				*/
																/* wk[1] = ((Mac>>23) + 1) >> 1		*/
		wk[0].l = wk[0].l + wk[1].l;							/* wk[0] = wk[0] + wk[1]			*/
/*--------------------------------------------------------------------------------------------------*/
/*		wk[0].dr = wk[0].dr - (k[2] * z[2]) - (k[3] * z[3])											*/
/*--------------------------------------------------------------------------------------------------*/
		wk[1].l = mulshr( k[2], z[2], (SHORT)NXF_PRM_BITS );	/* Mac   = k[2] * z[2]				*/
																/* wk[1] = ((Mac>>23) + 1) >> 1		*/
		wk[0].l = wk[0].l - wk[1].l;							/* wk[0] = wk[0] - wk[1]			*/
/*--------------------------------------------------------------------------------------------------*/
		wk[1].l = mulshr( k[3], z[3], (SHORT)NXF_PRM_BITS );	/* Mac   = k[3] * z[3]				*/
																/* wk[1] = ((Mac>>23) + 1) >> 1		*/
		*x = wk[0].l - wk[1].l;									/* x     = wk[0] - wk[1]			*/
/*--------------------------------------------------------------------------------------------------*/
/*		Update z[i]																					*/
/*--------------------------------------------------------------------------------------------------*/
		z[1] = z[0];											/* z[1] = z[0]						*/
		z[0] = u;												/* z[0] = u							*/
		z[3] = z[2];											/* z[3] = z[2]						*/
		z[2] = *x;												/* z[2] = x							*/
/*--------------------------------------------------------------------------------------------------*/
		return;
}


/****************************************************************************************************/
/*																									*/
/*		IxIntegral																					*/
/*																									*/
/****************************************************************************************************/
inline	void	IxIntegral( LONG *x, LONG u, LONG k, LONG iu[2], DWREG *wk )
{
SHORT	swk0;
LONG	carry;
DLREG	dlwk0;

/*--------------------------------------------------------------------------------------------------*/
/*		k --> kx, sx																				*/
/*--------------------------------------------------------------------------------------------------*/
		wk[0].l = (ULONG)k << 8;							/* kx = (k <<  8)						*/
		wk[1].l = (ULONG)k >> 24;							/* sx = (k >> 24)						*/

/*--------------------------------------------------------------------------------------------------*/
/*		入力演算																					*/
/*--------------------------------------------------------------------------------------------------*/
		dlwk0.dl = mul( u, wk[0].l );						/* dlwk0.dl = u * wk[0]					*/
		wk[2].l = asr( dlwk0.l[1], wk[1].s[0] );			/* wk[2] = dlwk0.l[1] >> wk[1] (算術)	*/
		
/*--------------------------------------------------------------------------------------------------*/
/*		積分演算																					*/
/*--------------------------------------------------------------------------------------------------*/
		if( wk[2].l > (LONG)ZeroR )
		{
			iu[1] = iu[1] + (LONG)No25bit;
		}
		else if( wk[2].l < (LONG)NoneR )
		{
			iu[1] = iu[1] - (LONG)No25bit;
		}
		else
		{
/*--------------------------------------------------------------------------------------------------*/
/*			carry = ( (ULONG)(iu[0]+(xx[0]<<(25-sx))) < (ULONG)iu[0] );								*/
/*			iu[0] = iu[0] + (xx[0]<<(25-sx));														*/
/*			iu[1] = iu[1] + (xx[1]<<(25-sx)) + (((ULONG)xx[0]>>sx)>>7) + carry;						*/
/*--------------------------------------------------------------------------------------------------*/
			swk0 = (SHORT)25 - wk[1].s[0];
			wk[3].l = dlwk0.l[0] << swk0;
			wk[3].l += iu[0];
			if( (ULONG)wk[3].l < (ULONG)iu[0] )
			{
				carry = 1;
			}
			else
			{
				carry = 0;
			}
/*--------------------------------------------------------------------------------------------------*/
			iu[0] = wk[3].l;
			wk[2].l = dlwk0.l[1] << swk0;
			wk[3].l = (ULONG)dlwk0.l[0] >> wk[1].s[0];
			wk[3].l = asr( wk[3].l, 7 );
			iu[1] = wk[2].l + wk[3].l;
			iu[1] += carry;
		}
/*--------------------------------------------------------------------------------------------------*/
/*		積分値リミット																				*/
/*--------------------------------------------------------------------------------------------------*/
		if( iu[1] >= (LONG)No25bit )
		{
			iu[0] = 0;
			iu[1] = No25bit;
		}
		else if( iu[1] < (LONG)No25bitM )
		{
			iu[0] = 0;
			iu[1] = No25bitM;
		}
/*--------------------------------------------------------------------------------------------------*/
/*		戻り値計算(四捨五入処理)																	*/
/*--------------------------------------------------------------------------------------------------*/
		*x = asr( iu[1], (SHORT)OneR );						/* x = (iu[1] + 1) >> 1					*/
}


/****************************************************************************************************/
/*																									*/
/*		IxSquareSum																					*/
/*																									*/
/****************************************************************************************************/
inline	void	IxSquareSum( DLREG *x, LONG a, LONG b, DWREG *wk )
{
	volatile DLREG	dlwk0, dlwk1;

	dlwk0.dl = mul( a, a );
	wk[0].l = dlwk0.l[0];
	wk[1].l = dlwk0.l[1];

	dlwk1.dl = mul( b, b );
	wk[2].l = dlwk1.l[0];
	wk[3].l = dlwk1.l[1];

	x->l[0] = wk[0].l + wk[2].l;
	x->l[1] = wk[1].l + wk[3].l;

	wk[3].l = (wk[0].l >> 31) & (wk[2].l >> 31);				/* キャリー							*/
	x->l[1] = x->l[1] + (wk[3].l & (LONG)OneR);
}


/****************************************************************************************************/
/*																									*/
/*		Initialize Soft BB & INT1L Enable															*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		備考：																						*/
/*		JL-086A不具合回避のため、5回連続書き込み実施												*/
/*																									*/
/****************************************************************************************************/
inline	void	InitSbb( SHORT BbSet )						/* soft BB & INT1L設定初期化処理		*/
{
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
#ifdef	MULTI_AXIS							/* 多軸処理有効											*/
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
#endif /* MULTI_AXIS */						/* 多軸処理有効											*/
}

/****************************************************************************************************/
/*																									*/
/*		PWM初期化																					*/
/*																									*/
/****************************************************************************************************/
inline void InitPWM( MICRO_AXIS_HANDLE *AxisRsc )
{
	USHORT	uswk;

	/* PWM出力選択設定 */
	PWMOS = 0x0A0;							/* PWM出力選択		: キャリアカウンタ比較出力			*/
											/* 相比較選択		: 比較値からPU1、NU1信号作成		*/
											/* 即ロードモード	: マイクロ書き込み時に即ロード		*/
											/* 周波数ロード選択	: キャリア谷で周波数ロード			*/
											/* ノコギリ波選択	: 三角波							*/

	/* キャリア周波数取得 & 設定 */
	AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn;
	CRFRQ = AxisRsc->IntAdV.CarrierFreq;				/* キャリア周波数現在地設定					*/

	/* キャリアカウンタ比較値設定 */
	uswk = ((USHORT)AxisRsc->IntAdV.CarrierFreq >> 1);	/* IntAdV.CrFreqW /2(50p duty)				*/
	AxisRsc->PwmV.PwmCntT2 = uswk;
	AxisRsc->PwmV.PwmCntT1 = uswk;
	AxisRsc->PwmV.PwmCntT0 = uswk;
	PwmT2 = AxisRsc->PwmV.PwmCntT2;
	PwmT1 = AxisRsc->PwmV.PwmCntT1;
	PwmT0 = AxisRsc->PwmV.PwmCntT0;

#ifdef	MULTI_AXIS							/* 多軸処理有効											*/
	AxisRsc++;
	PWMOS_2 = 0x0A0;						/* PWM出力選択		: キャリアカウンタ比較出力			*/
											/* 相比較選択		: 比較値からPU1、NU1信号作成		*/
											/* 即ロードモード	: マイクロ書き込み時に即ロード		*/
											/* 周波数ロード選択	: キャリア谷で周波数ロード			*/
											/* ノコギリ波選択	: 三角波							*/

	/* キャリア周波数取得 & 設定 */
	AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn;
	CRFRQ_2 = AxisRsc->IntAdV.CarrierFreq;					/* キャリア周波数現在地設定					*/

	/* キャリアカウンタ比較値設定 */
	uswk = ((USHORT)AxisRsc->IntAdV.CarrierFreq >> 1);	/* IntAdV.CrFreqW /2(50p duty)				*/
	AxisRsc->PwmV.PwmCntT2 = uswk;
	AxisRsc->PwmV.PwmCntT1 = uswk;
	AxisRsc->PwmV.PwmCntT0 = uswk;
	PwmT2_2 = AxisRsc->PwmV.PwmCntT2;
	PwmT1_2 = AxisRsc->PwmV.PwmCntT1;
	PwmT0_2 = AxisRsc->PwmV.PwmCntT0;
#endif /* MULTI_AXIS */						/* 多軸処理有効											*/
}


/****************************************************************************************************/
/*																									*/
/*		PWM出力開始																					*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		備考：																						*/
/*		JL-086A不具合回避のため、5回連続書き込み実施												*/
/*																									*/
/****************************************************************************************************/
inline void StartPWM( MICRO_AXIS_HANDLE *AxisRsc, SHORT BbSet )
{
	/* Carrier Counter Clock Set and PWM start	*/
	CRST = AxisRsc->MicroIf.CarrierClk | 0x0001;

	/* Release Soft BB */
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;

#ifdef	MULTI_AXIS
	AxisRsc++;

	/* Carrier Counter Clock Set and PWM start	*/
	CRST_2 = AxisRsc->MicroIf.CarrierClk | 0x0001;

	/* Release Soft BB */
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
#endif
}


/****************************************************************************************************/
/*																									*/
/*		PWM出力																						*/
/*																									*/
/****************************************************************************************************/
inline void SetPWM( MICRO_AXIS_HANDLE *AxisRsc )
{
	PwmT2 = AxisRsc->PwmV.PwmCntT2;
	PwmT1 = AxisRsc->PwmV.PwmCntT1;
	PwmT0 = AxisRsc->PwmV.PwmCntT0;

#ifdef	MULTI_AXIS
	AxisRsc++;
	PwmT2_2 = AxisRsc->PwmV.PwmCntT2;
	PwmT1_2 = AxisRsc->PwmV.PwmCntT1;
	PwmT0_2 = AxisRsc->PwmV.PwmCntT0;
#endif
}


/****************************************************************************************************/
/*																									*/
/*		キャリア周波数変更(各軸)																	*/
/*																									*/
/****************************************************************************************************/
inline	void	ChangeCarrierFreq( MICRO_AXIS_HANDLE *AxisRsc, SHORT axno )
{
	if( axno == 0 )
	{
		if( AxisRsc->IntAdV.CarrierFreq != AxisRsc->MicroIf.CarrierFreqIn )
		{
			AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn; /* Carrier Buffer Change	*/
			CRFRQ = AxisRsc->IntAdV.CarrierFreq;						  /* Carrier Freq. Change	*/
		}
	}
#ifdef	MULTI_AXIS
	else if( axno == 1 )
	{
		if ( AxisRsc->IntAdV.CarrierFreq != AxisRsc->MicroIf.CarrierFreqIn )
		{
			AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn; /* Carrier Buffer Change	*/
			CRFRQ_2 = AxisRsc->IntAdV.CarrierFreq;						  /* Carrier Freq. Change	*/
		}
	}
#endif
	else
	{
		; //処理なし
	}
}


/****************************************************************************************************/
/*																									*/
/*		キャリア周波数変更(全軸)																	*/
/*																									*/
/****************************************************************************************************/
inline	void	ChangeCarrierFreqAll( MICRO_AXIS_HANDLE *AxisRsc )
{
	if( AxisRsc->IntAdV.CarrierFreq != AxisRsc->MicroIf.CarrierFreqIn )
	{
		AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn;	/* Carrier Buffer Change	*/
		CRFRQ = AxisRsc->IntAdV.CarrierFreq;							/* Carrier Freq. Change		*/
	}
#ifdef	MULTI_AXIS
	AxisRsc++;
	if ( AxisRsc->IntAdV.CarrierFreq != AxisRsc->MicroIf.CarrierFreqIn )
	{
		AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn;	/* Carrier Buffer Change	*/
		CRFRQ_2 = AxisRsc->IntAdV.CarrierFreq;							/* Carrier Freq. Change		*/
	}
#endif
}


/****************************************************************************************************/
/*																									*/
/*		デシメーションフィルタエラークリア															*/
/*																									*/
/****************************************************************************************************/
inline	void	SdmErrClrRequest( USHORT axno )
{
	if( axno == 0 )
	{
		SDMECLR = 0x000F;										/* Decimation filter Error Clear	*/
	}
#ifdef	MULTI_AXIS
	else if( axno == 1 )
	{
		SDMECLR_2 = 0x000F;										/* Decimation filter Error Clear	*/
	}
#endif
	else
	{
		; //処理なし
	}
}


/****************************************************************************************************/
/*																									*/
/*		デシメーションフィルタエラークリア															*/
/*																									*/
/****************************************************************************************************/
inline	void	CurAdSyncRequest( void )					/* 電流ADサイクル同期要求				*/
{
	ADSYNC = 0x0001;
#ifdef	MULTI_AXIS
	ADSYNC_2 = 0x0001;
#endif
}


/****************************************************************************************************/
/*																									*/
/*		A/D convert data loading																	*/
/*																									*/
/****************************************************************************************************/
/*		IuDetTmp = (IuGain * ( IuAD + IuOffset )) * KnorCurrent / 2^14								*/
/*		IvDetTmp = (IvGain * ( IuAD + IvOffset )) * KnorCurrent / 2^14								*/
/*		IuDetect = (IuGain * ( IuAD + IuOffset )) * KnorCurrent / 2^14 * 3 / 2						*/
/*		IvDetect = (IvGain * ( IuAD + IvOffset )) * KnorCurrent / 2^14 * 3 / 2						*/
/****************************************************************************************************/
inline void ADConvDataLoad( MICRO_AXIS_HANDLE *AxisRsc )
{
	SHORT	swk;
	DWREG	lwk0;

	/* swk = IuAD >> 2(算術) */
	swk = mulshr( IuAD, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IuOffset) * IuGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IuOffset), AxisRsc->CurDet.IuGain );
	/* lwk0.l = (swk + IuOffset) * IuGain * KnorCurrent / 2^14 (算術) */
	AxisRsc->CurDet.IuDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IuDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (算術) */
	AxisRsc->CurDet.IuDetect = mulshr( AxisRsc->CurDet.IuDetTmp, (LONG)No3, 1 );
	/*------------------------------------------------------------------------------*/
	/* swk = IvAD >> 2(算術) */
	swk = mulshr( IvAD, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IvOffset) * IvGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IvOffset), AxisRsc->CurDet.IvGain );
	/* lwk0.l = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 (算術シフト) */
	AxisRsc->CurDet.IvDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IvDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (算術) */
	AxisRsc->CurDet.IvDetect = mulshr( AxisRsc->CurDet.IvDetTmp, (LONG)No3, 1 );

#ifdef	MULTI_AXIS

	AxisRsc++;
	/* swk = IuAD >> 2(算術) */
	swk = mulshr( IuAD_2, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IuOffset) * IuGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IuOffset), AxisRsc->CurDet.IuGain );
	/* lwk0.l = (swk + IuOffset) * IuGain * KnorCurrent / 2^14 (算術) */
	AxisRsc->CurDet.IuDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IuDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (算術) */
	AxisRsc->CurDet.IuDetect = mulshr( AxisRsc->CurDet.IuDetTmp, (LONG)No3, 1 );
	/*------------------------------------------------------------------------------*/
	/* swk = IvAD >> 2(算術) */
	swk = mulshr( IvAD_2, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IvOffset) * IvGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IvOffset), AxisRsc->CurDet.IvGain );
	/* lwk0.l = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 (算術シフト) */
	AxisRsc->CurDet.IvDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IvDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (算術) */
	AxisRsc->CurDet.IvDetect = mulshr( AxisRsc->CurDet.IvDetTmp, (LONG)No3, 1 );

#endif
}


/****************************************************************************************************/
/*																									*/
/*		SVIP異常状態取得																			*/
/*																									*/
/****************************************************************************************************/
inline	void	GetSvipErrorSts( MICRO_AXIS_HANDLE *AxisRsc )
{
	AxisRsc->StsFlg.FccStsMon = FCCDAT;
	AxisRsc->StsFlg.FltStsW = FLTSTAT & 0x7FFF;
#ifdef	MULTI_AXIS
	AxisRsc++;
	AxisRsc->StsFlg.FccStsMon = FCCDAT_2;
	AxisRsc->StsFlg.FltStsW = FLTSTAT_2 & 0x7FFF;
#endif
}


/***************************************** end of file **********************************************/

