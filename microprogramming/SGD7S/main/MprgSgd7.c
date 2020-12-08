/****************************************************************************************************/
/*																									*/
/*																									*/
/*		MprgSgd7.c : ��-7S/7W �d������v���O����													*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		- Main Functions and Interrupts -															*/
/*		  main( )		: ���������� & Round Loop													*/
/*		  MpIntHost( )	: �z�X�g����(CPU��ScanA�������N���^�C�~���O�Ƃ���Round����R�[��)			*/
/*		  MpIntAD( )	: �d�����[�v���Z����(�d�����o�������荞�݂ɂ��N��)						*/
/*		  MpIntEnc( )	: Safety�@�\�Ή��p�����o�͏���(�G���R�[�_�ʐM���荞�݂ɂ��N��)			*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		�����ӎ�����																				*/
/*																									*/
/*		JL-086�p�ʃv���O������C����x�[�X�ŋL�q���A��p�R���p�C���ɂ��R���p�C�����s�����ƂŁA		*/
/*		�@�B��(���߃e�[�u��)�ł���uMprgJL086.c�v�𐶐�����B										*/
/*		�ڍׂ́A�y900-***-***�zIP Designer�C���X�g�[���A�o�[�W�����A�b�v�菇.doc���Q�ƁB			*/
/*																									*/
/*		�ʃv���O��������A�N�Z�X����H/W���W�X�^�d�l�A�A�Z���u�����ߎd�l�A�C���g�����V�b�N�֐�		*/
/*		�d�l�ɂ��ẮA�ȉ��ɏ�����B																*/
/*			�yRB1400590�z�}�C�N�����ZIP�\�t�g�E�F�A�}�j���A��										*/
/*			�yRB1400592�zMercury�T�[�{SoC(JL086A)_�T�[�{IP������									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		���R�[�f�B���O���[����																		*/
/*																									*/
/*		�@���W�X�^��`��ҏW(�ǉ��A�폜��)����ꍇ�́A�K���uMakeJL086Reg.xls�v�ɂčs���B			*/
/*		�A�L���X�g�͖����I�ɍs��(�C���g�����V�b�N�֐��̏ꍇ�A�\�����Ȃ�����ɂȂ�\����)�B		*/
/*		�B�V�t�g���Z�̓R�����g�ɘ_���V�t�g���Z�p�V�t�g���𖾋L����B�u>>�v��u<<�v�𒼐ڋL�q		*/
/*		  �����ꍇ�A�R���p�C���͘_���V�t�g�Ƃ��ăR���p�C������B�Z�p�V�t�g�̓C���g�����V�b�N		*/
/*		  �֐����g�p����K�v������B���d�v��														*/
/*		�C�ʏ�̎l�����Z�ł����Ă��C���g�����V�b�N�֐���D�悵�Ďg�p����B							*/
/*		�D�R�����g�͔��p�p���A�S�p�����A���ȁA�J�^�J�i�ŋL�q����B�S�p�p���A���p�J�^�J�i�֎~�B		*/
/*																									*/
/************** Copyright (C) Yaskawa Electric Corporation ******************************************/
/*																									*/
/*		Rev.1.00 : 2014.01.05  Y.Oka		�E��-�X-SD Rev.0.0A�A��-�X Rev3.15���x�[�X�ɐV�K�쐬	*/
/*											  (��-7��MpgDebug_024++++++++���x�[�X)					*/
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
#define	MSW_VER			0x0001					/* �\�t�g�o�[�W�����ݒ�								*/
#define	TST_VER			0x0000					/* �e�X�g�o�[�W�����ݒ�								*/
#define	YSP_VER			0x0000					/* Y�d�l�o�[�W�����ݒ�								*/


/****************************************************************************************************/
/*																									*/
/*		Multi Axis Select Switch for SGD7W															*/
/*																									*/
/****************************************************************************************************/
#define	xxx_MULTI_AXIS							/* ���������L��(SGD7W�̏ꍇ�uxxx_�v���폜)			*/
#ifdef	MULTI_AXIS
#define	MAX_AXIS_NUM	2						/* �ő吧�䎲����`									*/
#endif


/****************************************************************************************************/
/*																									*/
/*		H/W Access resister definitions																*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* Standerd definitions																				*/
/*--------------------------------------------------------------------------------------------------*/
int chess_storage(ISA0)	ISA0;					/* ���荞��0�W�����v��A�h���X						*/
int chess_storage(ISA1)	ISA1;					/* ���荞��1�W�����v��A�h���X						*/
int chess_storage(IL) 	INTLVWR;				/* �����݃��x���ݒ�									*/
int chess_storage(EIX)	EIX;					/* ���荞�݃C�l�[�u��								*/
int chess_storage(DIX)	DIX;					/* ���荞�݃f�B�X�G�[�u��							*/

/*--------------------------------------------------------------------------------------------------*/
/* Extern definitions																				*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6D0) OUTPT;	/* INT2 Output Port(����)							*/
extern int chess_storage(PFREG:0x6D1) WDT1L;	/* WDT Trigger Port(����)							*/
extern int chess_storage(PFREG:0x6D9) HSUR0;	/* �z�X�g�w�߃|�[�g0(����)							*/
extern int chess_storage(PFREG:0x6DA) HSUR1;	/* �z�X�g�w�߃|�[�g1(����)							*/
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
extern int chess_storage(PFREG:0x6D0) IuAD;		/* U���d���t�B�[�h�o�b�NADC���ݒl(Axis1)			*/
extern int chess_storage(PFREG:0x6D1) IvAD;		/* V���d���t�B�[�h�o�b�NADC���ݒl(Axis1)			*/
extern int chess_storage(PFREG:0x7D0) IuAD_2;	/* U���d���t�B�[�h�o�b�NADC���ݒl(Axis2)			*/
extern int chess_storage(PFREG:0x7D1) IvAD_2;	/* V���d���t�B�[�h�o�b�NADC���ݒl(Axis2)			*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6DB) PWMOS;	/* PWM�o�͑I�� (Axis1)								*/
extern int chess_storage(PFREG:0x6DF) CRFRQ;	/* PWM�L�����A���g��16�r�b�g�J�E���^�ݒ� (Axis1)	*/
extern int chess_storage(PFREG:0x7DB) PWMOS_2;	/* PWM�o�͑I�� (Axis2)								*/
extern int chess_storage(PFREG:0x6DF) CRFRQ_2;	/* PWM�L�����A���g��16�r�b�g�J�E���^�ݒ� (Axis2)	*/
extern int chess_storage(PFREG:0x6E7) PwmT0;	/* PWM�O�p�g��r�l 0 (Axis1)						*/
extern int chess_storage(PFREG:0x6E8) PwmT1;	/* PWM�O�p�g��r�l 1 (Axis1)						*/
extern int chess_storage(PFREG:0x6E9) PwmT2;	/* PWM�O�p�g��r�l 2 (Axis1)						*/
extern int chess_storage(PFREG:0x7E7) PwmT0_2;	/* PWM�O�p�g��r�l 0 (Axis2)						*/
extern int chess_storage(PFREG:0x7E8) PwmT1_2;	/* PWM�O�p�g��r�l 1 (Axis2)						*/
extern int chess_storage(PFREG:0x7E9) PwmT2_2;	/* PWM�O�p�g��r�l 2 (Axis2)						*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6DF) FLTSTAT;	/* �̏���� (Axis1)									*/
extern int chess_storage(PFREG:0x6E1) FCCDAT;	/* SVIP�ُ�ُ��� (Axis1)							*/
extern int chess_storage(PFREG:0x7DF) FLTSTAT_2;/* �̏���� (Axis2)									*/
extern int chess_storage(PFREG:0x7E1) FCCDAT_2;	/* SVIP�ُ�ُ��� (Axis2)							*/
/*--------------------------------------------------------------------------------------------------*/
extern int chess_storage(PFREG:0x6F9) DIVSET;	/* �����@�\�ݒ� (Axis1)								*/
extern int chess_storage(PFREG:0x6FA) PCVS0;	/* PWM-�p���X�ϊ��ʒu�ݒ� (Axis1)					*/
extern int chess_storage(PFREG:0x6FB) PCVS1;	/* PWM-�p���X�ϊ����_�␳1�ݒ� (Axis1)				*/
extern int chess_storage(PFREG:0x6BC) PCVS2;	/* PWM-�p���X�ϊ����_�␳2�ݒ� (Axis1)				*/

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
void	MpIntHost( void );										/* �z�X�g�����݃v���g�^�C�v�錾		*/
void	MpIntAD( void ) property(isr);							/* �d�����䊄���݃v���g�^�C�v�錾	*/
void	MpIntEnc( void );										/* �G���R�[�_�����݃v���g�^�C�v�錾	*/
void	MpDataClear( MICRO_AXIS_HANDLE *AxisRsc );				/* �}�C�N���p�f�[�^�N���A			*/

/*--------------------------------------------------------------------------------------------------*/
/* Inline Functions																					*/
/*--------------------------------------------------------------------------------------------------*/
inline	USHORT	MpSQRT( ULONG );								/* ���������Z����(����)				*/
inline	SHORT	MpOVMMODK( LONG, SHORT, SHORT, CSHORT* );		/* �I�[�o���W�����[�V��������		*/
inline	void	InitSbb( SHORT );								/* soft BB & INT1L�ݒ菉��������	*/
inline	void	InitPWM( MICRO_AXIS_HANDLE* );					/* PWM�ݒ菉��������				*/
inline	void	StartPWM( MICRO_AXIS_HANDLE*, SHORT );			/* PWM�o�͊J�n����					*/
inline	void	SetPWM( MICRO_AXIS_HANDLE* );					/* PWM�O�p�g��r�l�ݒ菈��			*/
inline	void	ChangeCarrierFreq( MICRO_AXIS_HANDLE*, SHORT );	/* �L�����A���g���ݒ菈��(�e��)		*/
inline	void	ChangeCarrierFreqAll( MICRO_AXIS_HANDLE* );		/* �L�����A���g���ݒ菈��(�S��)		*/
inline	void	SdmErrClrRequest( USHORT );						/* Decimation Filter Error Clear	*/
inline	void	CurAdSyncRequest( void );						/* �d��AD�T�C�N�������v��			*/
inline	void	ADConvDataLoad( MICRO_AXIS_HANDLE* );			/* �d�����o�l�擾����				*/
inline	void	GetSvipErrorSts( MICRO_AXIS_HANDLE* );			/* SVIP�ُ��Ԏ擾����				*/

/*--------------------------------------------------------------------------------------------------*/
/* ���Z���C�u����																					*/
/*--------------------------------------------------------------------------------------------------*/
/* MlibMulgain32 */
inline	void	IxMulgain32( LONG *x, LONG u, LONG k, DWREG *wk );
/* MlibMulgainNolim */
inline	void	IxMulgainNolim( LONG *x, LONG u, LONG k, DWREG *wk );
/* 1�����[�p�X�t�B���^ */
inline	void	IxLpfilter1( LONG *x, LONG u, LONG k, DWREG *wk );
/* 2���m�b�`�t�B���^ */
inline	void	IxNxfilter2( LONG *x, LONG u, LONG k[5], LONG z[4], DWREG wk[4] );
/* �ϕ����Z */
inline	void	IxIntegral( LONG *x, LONG u, LONG k, LONG iu[2], DWREG *wk );
/* ���a���Z */
inline	void	IxSquareSum( DLREG *x, LONG a, LONG b, DWREG *wk );/* <S18E> */


/****************************************************************************************************/
/*																									*/
/*		����������																					*/
/*																									*/
/****************************************************************************************************/
#ifndef IPD_SIM					/* IPDesigner�p�V�~�����[�V�����X�C�b�`								*/
void	main( void )			/* JL-086�ɓ��ڂ���v���O�������쐬����ꍇ�͂�����Œ�`����		*/
#else
void	MpStart( void )			/* �R���p�C���݂̂ŃV�~�����[�V�������s�Ȃ��ꍇ�͂�����Œ�`����	*/
#endif
{
	USHORT				ax_noR;
	MICRO_AXIS_HANDLE	*AxisRscR;
	LONG				*BlkTrAdr;

	SHORT	BbSetW;											/* Soft BB and INT1L Enable Status		*/
	SHORT	DivSetW;										/* �����@�\�ݒ� 						*/
	SHORT	PoSet1W;										/* �p���X�ϊ����_�␳1 					*/
	SHORT	PoSet2W;										/* �p���X�ϊ����_�␳2 					*/
	USHORT	uswk;											/* ���[�N���W�X�^ 						*/
	
/*--------------------------------------------------------------------------------------------------*/
/*		�o�[�W�����ݒ�																				*/
/*--------------------------------------------------------------------------------------------------*/
	VerInfo.MswVer = MSW_VER; 								/* �\�t�g�o�[�W�����ݒ�					*/
	VerInfo.TstVer = TST_VER;								/* �e�X�g�o�[�W�����ݒ�					*/
	VerInfo.YspVer = YSP_VER;								/* Y�d�l�o�[�W�����ݒ�					*/

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
#ifdef	MULTI_AXIS											/* ���������L��							*/
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
	{ /* �ʃv���O�����ɂ�镪���o�͗L��(0���ڂ̂ݏ���) */
		/* level(AD=3, INT1=4, HOST=0) */
		INTLVWR = 0x0043;
		ISA0 = (int)MpIntAD;
		ISA1 = (int)MpIntEnc;
		BbSetW = 0x2004;
		InitSbb( BbSetW );

/*--------------------------------------------------------------------------------------------------*/
/*		�����o�͊֘A���W�X�^�ݒ�																	*/
/*--------------------------------------------------------------------------------------------------*/
		PCVS0 = AxisHdl[0].EncIfV.DivPls.s[0];	/* �p���X�ϊ��ʒu (bit15-0)							*/

		PoSet1W = AxisHdl[0].DivPlsV.PoSet1In;	/* MpUPDATE_DIVPOS()�Ŕ�r���������邽�ߎc���Ă���	*/
		PoSet2W = AxisHdl[0].DivPlsV.PoSet2In;	/* MpUPDATE_DIVPOS()�Ŕ�r���������邽�ߎc���Ă���	*/
		PCVS1 = PoSet1W;						/* �p���X�ϊ����_�␳1 (bit15-0)					*/
		PCVS2 = PoSet2W;						/* �p���X�ϊ����_�␳2 (bit15-0)					*/

		DivSetW = AxisHdl[0].DivPlsV.DivSetIn;	/* MpUPDATE_DIVPOS()�Ŕ�r���������邽�ߎc���Ă���	*/
		DIVSET = DivSetW;						/* �����@�\�ݒ� 									*/
	}
	else
	{ /* �ʃv���O�����ɂ�镪���o�͖��� */
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
#ifdef	MULTI_AXIS														/* ���������L��				*/
	for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
	ax_noR = 0;
#endif
	{
		AxisRscR = &AxisHdl[ax_noR];

/*--------------------------------------------------------------------------------------------------*/
/*		Initialize Sin and Cos Table																*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscR->SinTbl.Sin1.l = 0x0000;		/* sin(��)			sin(0)		=  0.000 �� 0000h	*/
		AxisRscR->SinTbl.Cos1.l = 0x4000;		/* cos(��)			cos(0)		=  1.000 �� 4000h	*/
		AxisRscR->SinTbl.Sin2.l = 0xC893;		/* sin(��+2��/3)	sin(2��/3)	= -0.866 �� C893h	*/
		AxisRscR->SinTbl.Cos2.l = 0xE000;		/* cos(��+2��/3)	cos(2��/3)	= -0.500 �� E000h	*/
		AxisRscR->SinTbl.Sin3.l = 0x376D;		/* sin(��-2��/3)	sin(-2��/3)	=  0.866 �� 376Dh	*/
		AxisRscR->SinTbl.Cos3.l = 0xE000;		/* cos(��-2��/3)	cos(-2��/3)	= -0.500 �� E000h	*/

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
#ifndef IPD_SIM										/* IPDesigner�p�V�~�����[�V�����X�C�b�`			*/
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
			MpIntHost( );									/* �z�X�g�����ݏ������s					*/
		}

/*--------------------------------------------------------------------------------------------------*/
/*		Host port check for host INT2																*/
/*--------------------------------------------------------------------------------------------------*/
		if ( HSUR1 != 0x0 )
		{
			DIX = 0x0;										/* disable interupt						*/

#ifdef	MULTI_AXIS											/* ���������L��							*/
			for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
			ax_noR = 0;
#endif
			{
				AxisRscR = &AxisHdl[ax_noR];

				/* �ʑ� & ���[�^���x */
				AxisRscR->PhaseV.Phase = AxisRscR->MicroIf.PhaseIn;
				AxisRscR->PhaseV.PhaseBuf = AxisRscR->MicroIf.PhaseIn;
				AxisRscR->Vcmp.MotSpd = AxisRscR->MicroIf.MotSpdIn;
				/* �g���N�����l[2^24/MaxTrq] */
				AxisRscR->Trqctrl.TrqLimPlus = AxisRscR->MicroIf.TrqLimPlusIn;
				AxisRscR->Trqctrl.TrqLimMinus = AxisRscR->MicroIf.TrqLimMinusIn;
				/* �O���g���N */
				AxisRscR->Trqctrl.TrqDistAftFil = AxisRscR->MicroIf.TrqDistAftFilIn;
				/* AVR�Q�C�� */
				AxisRscR->Curctrl.AVRGain = AxisRscR->MicroIf.AVRGainIn;
			}

			EIX = 0x0;										/* enable interupt						*/
		}

/*--------------------------------------------------------------------------------------------------*/
/*		CPU_Round����̏������݃f�[�^�󂯎�菈��													*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS											/* ���������L��							*/
		for( ax_noR = 0; (SHORT)ax_noR < AxisNum; ax_noR++ )
#else
		ax_noR = 0;
#endif
		{
			AxisRscR = &AxisHdl[ax_noR];

			/* �펞�ύX�\ */
			AxisRscR->CurDet.IuOffset = AxisRscR->MicroIf.IuOffsetIn;
			AxisRscR->CurDet.IvOffset = AxisRscR->MicroIf.IvOffsetIn;
			AxisRscR->CurDet.IuGain = AxisRscR->MicroIf.IuGainIn;
			AxisRscR->CurDet.IvGain = AxisRscR->MicroIf.IvGainIn;

			/* �I�����C���f�[�^�󂯎�� */
			if( AxisRscR->BlockTr.TxNumRToAsic != (LONG)ZeroR )
			{	/* 0�łȂ��ꍇ�A�f�[�^��荞��OK */
				BlkTrAdr = AxisRscR->BlockTr.TxDstRToAsic;
				block_cp( (LONG*)BlkTrAdr,									/* �f�[�^�]������		*/
						  (LONG*)&AxisRscR->BlockTr.TxDataRToAsic0,
						  (unsigned int)AxisRscR->BlockTr.TxNumRToAsic );
				AxisRscR->BlockTr.TxNumRToAsic = (LONG)ZeroR;				/* �]���f�[�^���N���A	*/
			}

			/* BB���̂ݕύX�\ */
			if( AxisRscR->StsFlg.FltStsW & 0x0400 )
			{ /* BB���̏ꍇ */
				/* d��q�� ���Q�C��, �ϕ��Q�C�� */
				AxisRscR->Curctrl.GainKpd = AxisRscR->MicroIf.GainKpdIn;
				AxisRscR->Curctrl.GainKpq = AxisRscR->MicroIf.GainKpqIn;
				AxisRscR->Curctrl.GainKid = AxisRscR->MicroIf.GainKidIn;
				AxisRscR->Curctrl.GainKiq = AxisRscR->MicroIf.GainKiqIn;

				/* �ϒ������~�b�g	*/
				AxisRscR->Vltctrl.Vmax = AxisRscR->MicroIf.VmaxIn;

				/* ��ߊE�� */
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
	INT64				dlwk;					/* ���[�N���W�X�^(64)								*/
	MICRO_AXIS_HANDLE	*AxisRscH;

	SHORT	swk0;								/* �z�X�g���荞�݃��[�N���W�X�^0 SHORT				*/
	SHORT	swk1;								/* �z�X�g���荞�݃��[�N���W�X�^2 SHORT				*/
	LONG	lwk1;								/* �z�X�g���荞�݃��[�N���W�X�^1 LONG				*/
	LONG	lwk2;								/* �z�X�g���荞�݃��[�N���W�X�^2 LONG				*/
	LONG	lwk3;								/* �z�X�g���荞�݃��[�N���W�X�^3 LONG				*/
	DWREG	Ixlwk[4];							/* �z�X�g���荞�݉��Z���C�u�����p���W�X�^			*/

	IniWk.IN_WK0++;		/* for debug counter */
	
	WDT1L = 0x1;								/* Watch dog set									*/
//	OUTPT = 0x1;								/* �f�o�b�O�p										*/

/*--------------------------------------------------------------------------------------------------*/
/*		�L�����A���g���؂�ւ�����																	*/
/*--------------------------------------------------------------------------------------------------*/
	ChangeCarrierFreqAll( &AxisHdl[0] );

/*--------------------------------------------------------------------------------------------------*/
/*		input from host																				*/
/*--------------------------------------------------------------------------------------------------*/
	DIX = 0x0;									/* disable interupt									*/

#ifdef	MULTI_AXIS								/* ���������L��										*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else
	ax_noH = 0;
#endif
	{
		AxisRscH = &AxisHdl[ax_noH];

		/* �ʑ� & ���[�^���x */
		AxisRscH->PhaseV.Phase = AxisRscH->MicroIf.PhaseIn;
		AxisRscH->PhaseV.PhaseBuf = AxisRscH->MicroIf.PhaseIn;
		AxisRscH->Vcmp.MotSpd = AxisRscH->MicroIf.MotSpdIn;
		/* �g���N�����l[2^24/MaxTrq] */
		AxisRscH->Trqctrl.TrqLimPlus = AxisRscH->MicroIf.TrqLimPlusIn;
		AxisRscH->Trqctrl.TrqLimMinus = AxisRscH->MicroIf.TrqLimMinusIn;
		/* �O���g���N, �g���NFF, ���b�v���⏞�g���N[2^24/MaxTrq] */
		AxisRscH->Trqctrl.TrqDistAftFil = AxisRscH->MicroIf.TrqDistAftFilIn;
		AxisRscH->Trqctrl.TrqFF = AxisRscH->MicroIf.TrqFFIn;
		AxisRscH->Trqctrl.RippleComp = AxisRscH->MicroIf.RippleCompIn;
		/* AVR�Q�C�� */
		AxisRscH->Curctrl.AVRGain = AxisRscH->MicroIf.AVRGainIn;
		/* d��, q���d���w��(�ʏ�s�g�p) */
//		AxisRscH->VcmpV.VdRef = AxisRscH->AdinV.VdRefIn; /* �폜 */
//		AxisRscH->VcmpV.VqRef = AxisRscH->AdinV.VqRefIn; /* �폜 */
	}

	EIX = 0x0;									/* enable interupt									*/


/*--------------------------------------------------------------------------------------------------*/
/*		Carrier Freq Change check : if( status & BB ) Carrier Freq. change							*/
/*--------------------------------------------------------------------------------------------------*/
#ifdef	MULTI_AXIS								/* ���������L��										*/
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
#ifdef	MULTI_AXIS								/* ���������L��										*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else		/* ��#ifdef	MULTI_AXIS */
	ax_noH = 0;
#endif		/* ��#ifdef	MULTI_AXIS */
	{
		AxisRscH = &AxisHdl[ax_noH];
		if( AxisRscH->MicroIf.CtrlStsIn & BB )
		{ /* BB���̏ꍇ */
			DIX = 0x0;							/* disable interupt									*/

			/* ����p�ϐ������� */
			MpDataClear( AxisRscH );
			/* �L�����A���g���ύX */
			ChangeCarrierFreq( AxisRscH, ax_noH );

			EIX = 0x0;							/* enable interupt									*/
		}
		else
		{ /* BE���̏ꍇ */
/*--------------------------------------------------------------------------------------------------*/
/*		�g���NFF���͑�������																		*/
/*--------------------------------------------------------------------------------------------------*/
			AxisRscH->Trqctrl.TrqBfrComp = AxisRscH->MicroIf.TrqrefIn + AxisRscH->Trqctrl.TrqFF;

/*--------------------------------------------------------------------------------------------------*/
/*		���b�v���␳�g���N��������																	*/
/*--------------------------------------------------------------------------------------------------*/
			AxisRscH->Trqctrl.TrqAftComp = AxisRscH->Trqctrl.TrqBfrComp + AxisRscH->Trqctrl.RippleComp;

/*--------------------------------------------------------------------------------------------------*/
/* 		notch filter 1st																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL1) == 0 )
			{ /* �t�B���^�����Ȃ� */
				/* �t�B���^�f�ʂ� */
				AxisRscH->NotchFil.Notch1Out = AxisRscH->Trqctrl.TrqAftComp;
				/* �f�[�^�N���A */
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
/*		�m�b�`�t�B���^2�i��																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL2) == 0 )
			{ /* �t�B���^�f�ʂ� */
				AxisRscH->NotchFil.Notch2Out = AxisRscH->NotchFil.Notch1Out;
				/* �f�[�^�N���A */
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
/*		�m�b�`�t�B���^3�i��																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL3) == 0 )
			{ /* �t�B���^�f�ʂ� */
				AxisRscH->NotchFil.Notch3Out = AxisRscH->NotchFil.Notch2Out;
				/* �f�[�^�N���A */
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
/*		�m�b�`�t�B���^4�i��																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL4) == 0 )
			{ /* �t�B���^�f�ʂ� */
				AxisRscH->NotchFil.Notch4Out = AxisRscH->NotchFil.Notch3Out;
				/* �f�[�^�N���A */
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
/*		�m�b�`�t�B���^5�i��																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL5) == 0 )
			{ /* �t�B���^�f�ʂ� */
				AxisRscH->NotchFil.Notch5Out = AxisRscH->NotchFil.Notch4Out;
				/* �f�[�^�N���A */
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
/*		�m�b�`�t�B���^6�i��(���[�^���U�}���p)														*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.FilterSwitch & NOTCHFIL6) == 0 )
			{ /* �t�B���^�f�ʂ� */
				AxisRscH->NotchFil.Notch6Out = AxisRscH->NotchFil.Notch5Out;
				/* �f�[�^�N���A */
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
/*		�O���g���N���Z																				*/
/*--------------------------------------------------------------------------------------------------*/
			AxisRscH->Trqctrl.TrqAddDist = AxisRscH->Trqctrl.TrqAftFil + AxisRscH->Trqctrl.TrqDistAftFil;
		}

/*--------------------------------------------------------------------------------------------------*/
/*		����X�e�[�^�X����(�g���N�������t���O�N���A)												*/
/*--------------------------------------------------------------------------------------------------*/
		DIX = 0x0;											/* disable interupt						*/
		AxisRscH->StsFlg.CtrlStsRW = AxisRscH->MicroIf.CtrlStsIn;
		AxisRscH->StsFlg.CtrlStsRW = ( AxisRscH->StsFlg.CtrlStsRW & TLIMI );
		EIX = 0x0;											/* enable interupt						*/


/****************************************************************************************************/
/*		�p�r�I��																					*/
/****************************************************************************************************/
		if( AxisRscH->MotInfo.MotUse == SERVO  )
		{
/****************************************************************************************************/
/*		�p�r�F�T�[�{																				*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		�g���N���~�b�g																				*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscH->Trqctrl.TrqAddDist >= ZeroR )
			{	/* �����g���N���~�b�g */
				AxisRscH->Trqctrl.TrqAftLim = limit( AxisRscH->Trqctrl.TrqAddDist,
													 AxisRscH->Trqctrl.TrqLimPlus );
				if( AxisRscH->Trqctrl.TrqAftLim == AxisRscH->Trqctrl.TrqLimPlus )
				{
					AxisRscH->StsFlg.CtrlStsRW |= TLIM;					/* �g���N�������t���O�Z�b�g	*/
				}
			}
			else
			{	/* �����g���N���~�b�g */
				AxisRscH->Trqctrl.TrqAftLim = limit( AxisRscH->Trqctrl.TrqAddDist,
													 AxisRscH->Trqctrl.TrqLimMinus );
				if( (AxisRscH->Trqctrl.TrqAftLim + AxisRscH->Trqctrl.TrqLimMinus) == 0 )
				{
					AxisRscH->StsFlg.CtrlStsRW |= TLIM;					/* �g���N�������t���O�Z�b�g	*/
				}
			}
		}
		else
		{
/****************************************************************************************************/
/*		�p�r�F�厲																					*/
/****************************************************************************************************/
			// ���Ή�
		}

/****************************************************************************************************/
/*		���[�^��ޑI��																				*/
/****************************************************************************************************/
		if( AxisRscH->MotInfo.MotTypeSMIM == IM )
		{
/****************************************************************************************************/
/*		�C���_�N�V�������[�^�p����																	*/
/****************************************************************************************************/
			// ���Ή�
		}
		else
		{
/****************************************************************************************************/
/*		�����@�p����																				*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		CPU_ScanC����̏������݃f�[�^�󂯎�菈��													*/
/*--------------------------------------------------------------------------------------------------*/
//IPMM�̏ꍇ
//IdrefLim(LONG), IqrefLim(LONG), CtrlMode(SHORT), MaxCurLimInput(LONG)
			AxisRscH->Systems.DebugCtr1 = AxisRscH->BlockTr.TxNumCToAsic;
			AxisRscH->Systems.DebugCtr2 = (SHORT)ZeroR;
			if( AxisRscH->BlockTr.TxNumCToAsic != (SHORT)ZeroR )
			{	/* CPU�����f�[�^�������݊����̂��߁A�}�C�N�����W�X�^�ɓn�� */
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
/*		d,q���d���w�߃��~�b�g���[�p�X�t�B���^����													*/
/*		��ߊE������d�����~�b�g���[�p�X�t�B���^����												*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscH->MicroIf.CtrlSwitch & V_FB) != ZeroR )
			{
				/* WfIdrefLimLpf = �t�B���^�o�� */
				IxLpfilter1( &AxisRscH->WeakFV.WfIdrefLimLpf,
							 AxisRscH->MicroIf.IdrefLimIn, 
							 AxisRscH->WeakFV.WfILimLpfGain,
							 Ixlwk );

				/* WfIqrefLimLpf = �t�B���^�o�� */
//				IxLpfilter1( &AxisRscH->WeakFV.WfIqrefLimLpf,
//							 AxisRscH->MicroIf.IqrefLimIn,
//							 AxisRscH->WeakFV.WfILimLpfGain,
//							 Ixlwk );

				/* MaxCurLimOutput = �t�B���^�o�� */
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
/*		d���d���w�ߌv�Z																				*/
/*--------------------------------------------------------------------------------------------------*/
			lwk1 = limit( AxisRscH->Trqctrl.TrqAftLim, (LONG)1 );						/* wkIntHost9 = sign(TrqAftLim) */
			lwk1 = AxisRscH->Trqctrl.TrqAftLim * lwk1;									/* wkIntHost9 = |Trqref|	*/

			lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq3;
			if( lwk1 >= AxisRscH->IntAdV.MaxTrq3 )
			{	/* |Trqref| >= MaxTrq[3] �̎� */
				IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdE, Ixlwk );				/* wkIntHost0 = IdE*(|Trqref|-MaxTrq[3])	*/
				lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt3 );					/* wkIntHost0 = IdE*(|Trqref|-MaxTrq[3]) + IdrefOpt[3]	*/
				lwk3 = limitz( lwk2, (LONG)No24bitM );									/* wkIntHost1 = limit( wkIntHost0, -2^24�`0 )	*/
			}
			else
			{
				lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq2;
				if( lwk1 >= AxisRscH->IntAdV.MaxTrq2 )
				{	/* |Trqref| >= MaxTrq[2] �̎� */
					IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdD, Ixlwk );			/* wkIntHost0 = IdD*(|Trqref|-MaxTrq[2])	*/
					lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt2 );				/* wkIntHost0 = IdD*(|Trqref|-MaxTrq[2]) + IdrefOpt[2]	*/
					lwk3 = limitz( lwk2, (LONG)No24bitM );								/* wkIntHost1 = limit( wkIntHost0, -2^24�`0 )	*/
				}
				else
				{
					lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq1;
					if( lwk1 >= AxisRscH->IntAdV.MaxTrq1 )
					{	/* |Trqref| >= MaxTrq[1] �̎� */
						IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdC, Ixlwk );		/* wkIntHost0 = IdC*(|Trqref|-MaxTrq[1])	*/
						lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt1 );			/* wkIntHost0 = IdC*(|Trqref|-MaxTrq[1]) + IdrefOpt[1]	*/
						lwk3 = limitz( lwk2, (LONG)No24bitM );							/* wkIntHost1 = limit( wkIntHost0, -2^24�`0 )	*/
					}
					else
					{
						lwk3 = lwk1 - AxisRscH->IntAdV.MaxTrq0;
						if( lwk1 >= AxisRscH->IntAdV.MaxTrq0 )
						{	/* |Trqref| >= MaxTrq[0] �̎� */
							IxMulgain32( &lwk2, lwk3, AxisRscH->IntAdV.IdB, Ixlwk );	/* wkIntHost0 = IdB*(|Trqref|-MaxTrq[0])	*/
							lwk2 = add_limitf( lwk2, AxisRscH->IntAdV.IdrefOpt0 );		/* wkIntHost0 = IdB*(|Trqref|-MaxTrq[0]) + IdrefOpt[0]	*/
							lwk3 = limitz( lwk2, (LONG)No24bitM );						/* wkIntHost1 = limit( wkIntHost0, -2^24�`0 )	*/
						}
						else
						{	/* |Trqref| < MaxTrq[0] �̎� */
							IxMulgain32( &lwk2, lwk1, AxisRscH->IntAdV.IdA, Ixlwk );	/* wkIntHost0 = IdA*|Trqref|	*/
							lwk3 = limitz( lwk2, (LONG)No24bitM );						/* wkIntHost1 = limit( wkIntHost0, -2^24�`0 )	*/
						}
					}
				}
			}
/*--------------------------------------------------------------------------------------------------*/
/*		���Ɍ��o����d���d���w��																		*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscH->IntAdV.PhaseReady == False )
			{
				/* ���[�^�ʑ����o������(���Ɍ��o��)��d���d���w�߂��[���ɂ���B */
				lwk3 = (LONG)ZeroR;
			}
		}

/*--------------------------------------------------------------------------------------------------*/
/*		�U�N�d���v�Z																				*/
/*--------------------------------------------------------------------------------------------------*/
		IxMulgain32( &lwk1, AxisRscH->Vcmp.MotSpd, AxisRscH->Vcmp.Phi, Ixlwk );			/* wkIntHost5 = �փ�0				*/
	}

/*--------------------------------------------------------------------------------------------------*/
/*		�f�[�^��INT_AD�ɓ]��																		*/
/*--------------------------------------------------------------------------------------------------*/
	DIX = 0x0;												/* disable interupt						*/

#ifdef	MULTI_AXIS											/* ���������L��							*/
	for( ax_noH = 0; (SHORT)ax_noH < AxisNum; ax_noH++ )
#else
	ax_noH = 0;
#endif
	{
		AxisRscH = &AxisHdl[ax_noH];

		AxisRscH->Trqctrl.Trqref = AxisRscH->Trqctrl.TrqAftLim;		/* �g���N�w�߂���				*/
		AxisRscH->Curctrl.IdrefBeforeWF = lwk3;						/* Id*����					*/
		AxisRscH->Vcmp.OmegaPhi = lwk1;								/* �փӂ���					*/

		if( (AxisRscH->MicroIf.CtrlSwitch & V_FB) == 0 )
		{ /* ��ߊE������ => �ʑ��⏞�̏ꍇ(��-�V�ȑO�̃��[�^) */
			AxisRscH->WeakFV.WfIdref = AxisRscH->MicroIf.IdrefIn;	/* WeakFV.WfIdref(reference)	*/
		}
	}

	EIX = 0x0;												/* enable interupt						*/

/****************************************************************************************************/
/*		Data Output for CPU																			*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS											/* ���������L��							*/
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
	CSHORT*	pCtbl;								/* �e�[�u���|�C���^�p���[�N���W�X�^					*/
	SHORT	swk0;								/* 16bit���[�N���W�X�^0								*/
	SHORT	swk1;								/* 16bit���[�N���W�X�^1								*/
	SHORT	swk2;								/* 16bit���[�N���W�X�^2								*/
	SHORT	swk3;								/* 16bit���[�N���W�X�^3								*/
	SHORT	swk4;								/* 16bit���[�N���W�X�^4								*/
	SHORT	swk5;								/* 16bit���[�N���W�X�^5								*/
	SHORT	swk6;								/* 16bit���[�N���W�X�^6								*/
	SHORT	swk7;								/* 16bit���[�N���W�X�^7								*/
	SHORT	swk8;								/* 16bit���[�N���W�X�^8								*/
	SHORT	swk9;								/* 16bit���[�N���W�X�^9								*/
	SHORT	swk10;								/* 16bit���[�N���W�X�^10							*/
	SHORT	swk11;								/* 16bit���[�N���W�X�^11							*/
	DWREG	lwk0;								/* 32bit���[�N���W�X�^0								*/
	DWREG	lwk1;								/* 32bit���[�N���W�X�^1								*/
	DWREG	lwk2;								/* 32bit���[�N���W�X�^2								*/
	DWREG	lwk3;								/* 32bit���[�N���W�X�^2								*/
	DWREG	lwk4;								/* 32bit���[�N���W�X�^4								*/
	DWREG	lwk5;								/* 32bit���[�N���W�X�^2								*/
	DWREG	lwk6;								/* 32bit���[�N���W�X�^6								*/
	DWREG	lwk7;								/* 32bit���[�N���W�X�^7								*/
	DWREG	lwk8;								/* 32bit���[�N���W�X�^8								*/
	DWREG	lwk9;								/* 32bit���[�N���W�X�^9								*/
	DWREG	Ixlwk[4];							/* 32bit���Z���C�u�����p���W�X�^					*/
	DLREG	dlwk0;								/* 64bit���[�N���W�X�^0								*/
	OUTPT = 0x1;
	WDT1L = 0x0;								/* Watch dog reset									*/
	IniWk.IN_WK1++;								/* for debug counter								*/

/*--------------------------------------------------------------------------------------------------*/
/*		���Ȋ��荞�݋֎~�ݒ�																		*/
/*--------------------------------------------------------------------------------------------------*/
	INTLVWR &= 0x00F0;


/****************************************************************************************************/
/*		Current Control Input Procedure																*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS								/* ���������L��										*/
	for( ax_noI = 0; (SHORT)ax_noI < AxisNum; ax_noI++ )
#else
	ax_noI = 0;
#endif
	{
		AxisRscI = &AxisHdl[ax_noI];
/*--------------------------------------------------------------------------------------------------*/
/*		�ʑ���ԏ���																				*/
/*--------------------------------------------------------------------------------------------------*/
		if( AxisRscI->PhaseV.PhaseBuf == AxisRscI->PhaseV.PhaseBufLast )
		{ /* �ʑ���Ԃ��� */
			AxisRscI->PhaseV.Phase += AxisRscI->PhaseV.PhaseItplt;			/* �ʑ���ԗʂ𑫂�����	*/
		}
		else
		{ /* �ʑ���Ԃ��Ȃ� */
			IxMulgainNolim( &lwk0.l, AxisRscI->Vcmp.MotSpd, AxisRscI->PhaseV.CnvSpdToPhase, Ixlwk );
			AxisRscI->PhaseV.PhaseItplt = lwk0.s[0];						/* PhaseItplt = �ʑ���ԗ�	*/
			AxisRscI->PhaseV.PhaseBufLast = AxisRscI->PhaseV.PhaseBuf;
		}

/*--------------------------------------------------------------------------------------------------*/
/*		sin, cos�v�Z����																			*/
/*--------------------------------------------------------------------------------------------------*/
/*		sin1 = sin��, sin2 = sin(��-2��/3), sin3 = sin(��+2��/3) 									*/
/*		cos1 = cos��, cos2 = cos(��-2��/3), cos3 = cos(��+2��/3) 									*/
/*--------------------------------------------------------------------------------------------------*/
		lwk0.s[0] = AxisRscI->PhaseV.Phase;						/* lwk0.s[0] = Phase				*/
		lwk0.s[0] += 32;
		lwk1.s[0] = (SHORT)PI23;
		lwk2.s[0] = lwk1.s[0] + lwk0.s[0];						/* lwk2.s[0] = Phase + 2��/3		*/
		lwk3.s[0] = lwk0.s[0] - lwk1.s[0];						/* lwk3.s[0] = Phase - 2��/3		*/

		lwk4.s[0] = lwk0.us[0] >> 6;							/* lwk4.s[0] = lwk0.s[0] >> 6(�_��)	*/
		IxTblSin16( AxisRscI->SinTbl.Sin1.s[0], lwk4.s[0] );	/* sin1.sr = SinTbl[ lwk4.s[0] ]	*/
		lwk0.s[0] += (SHORT)PI2;								/* lwk0.s[0] = lwk0.s[0] + ��/2		*/
		lwk4.s[0] = lwk0.us[0] >> 6;							/* lwk4.s[0] = lwk0.s[0] >> 6(�_��)	*/
		IxTblSin16( AxisRscI->SinTbl.Cos1.s[0], lwk4.s[0] );	/* cos1.sr = SinTbl[ lwk4.s[0] ]	*/

		lwk4.s[0] = lwk3.us[0] >> 6;							/* lwk4.s[0] = lwk3.s[0] >> 6�_��)	*/
		IxTblSin16( AxisRscI->SinTbl.Sin2.s[0], lwk4.s[0] );	/* sin2.sr = SinTbl[ lwk4.s[0] ]	*/
		lwk3.s[0] += (SHORT)PI2;								/* lwk3.s[0] = lwk3.s[0] + ��/2	*/
		lwk4.s[0] = lwk3.us[0] >> 6;							/* lwk4.s[0] = lwk3.s[0] >> 6�_��)	*/
		IxTblSin16( AxisRscI->SinTbl.Cos2.s[0], lwk4.s[0] );	/* cos2.sr = SinTbl[ lwk4.s[0] ]	*/

		lwk4.s[0] = lwk2.us[0] >> 6;							/* lwk4.s[0] = lwk2.s[0] >> 6�_��)	*/
		IxTblSin16( AxisRscI->SinTbl.Sin3.s[0], lwk4.s[0] );	/* sin3.sr = SinTbl[ lwk4.s[0] ]	*/
		lwk2.s[0] += (SHORT)PI2;								/* lwk2.s[0] = lwk2.s[0] + ��/2	*/
		lwk4.s[0] = lwk2.s[0] >> 6;								/* lwk4.s[0] = lwk2.s[0] >> 6�_��)	*/
		IxTblSin16( AxisRscI->SinTbl.Cos3.s[0], lwk4.s[0] );	/* cos3.sr = SinTbl[ lwk4.s[0] ]	*/

		AxisRscI->SinTbl.Sin1.l = AxisRscI->SinTbl.Sin1.s[0];	/* 16bit��32bit(�����t)				*/
		AxisRscI->SinTbl.Sin2.l = AxisRscI->SinTbl.Sin2.s[0];	/* 16bit��32bit(�����t)				*/
		AxisRscI->SinTbl.Sin3.l = AxisRscI->SinTbl.Sin3.s[0];	/* 16bit��32bit(�����t)				*/
		AxisRscI->SinTbl.Cos1.l = AxisRscI->SinTbl.Cos1.s[0];	/* 16bit��32bit(�����t)				*/
		AxisRscI->SinTbl.Cos2.l = AxisRscI->SinTbl.Cos2.s[0];	/* 16bit��32bit(�����t)				*/
		AxisRscI->SinTbl.Cos3.l = AxisRscI->SinTbl.Cos3.s[0];	/* 16bit��32bit(�����t)				*/
	}

/*--------------------------------------------------------------------------------------------------*/
/*		A/D convert data loading																	*/
/*--------------------------------------------------------------------------------------------------*/
	ADConvDataLoad( &AxisHdl[0] );

#ifdef	MULTI_AXIS								/* ���������L��										*/
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
		/* lwk1.l = (cos - cos3) * IuDetTmp / 2^14 (�Z�p) */
		lwk1.l = mulshr( (AxisRscI->SinTbl.Cos1.l - AxisRscI->SinTbl.Cos3.l),
						 AxisRscI->CurDet.IuDetTmp,
						 14 );
		/* lwk2.l = (cos2 - cos3) * IvDetTmp / 2^14 (�Z�p) */
		lwk2.l = mulshr( (AxisRscI->SinTbl.Cos2.l - AxisRscI->SinTbl.Cos3.l),
						 AxisRscI->CurDet.IvDetTmp,
						 14 );
		/* IdDetect = (cos - cos3) * IuDetTmp / 2^14 + (cos2 - cos3) * IvDetTmp / 2^14 (�Z�p) */
		AxisRscI->Curctrl.IdDetect = lwk1.l + lwk2.l;
/*--------------------------------------------------------------------------------------------------*/
		/* lwk1.l = (sin3 - sin) * IuDetTmp / 2^14 (�Z�p) */
		lwk1.l = mulshr( (AxisRscI->SinTbl.Sin3.l - AxisRscI->SinTbl.Sin1.l),
						 AxisRscI->CurDet.IuDetTmp,
						 14 );
		/* lwk2.l = (sin3 - sin2) * IvDetTmp / 2^14 (�Z�p) */
		lwk2.l = mulshr( (AxisRscI->SinTbl.Sin3.l - AxisRscI->SinTbl.Sin2.l),
						 AxisRscI->CurDet.IvDetTmp,
						 14 );
		/* IqDetect = (sin3 - sin) * IuDetTmp / 2^14 + (sin3 - sin2) * IvDetTmp / 2^14 (�Z�p) */
		AxisRscI->Curctrl.IqDetect = lwk1.l + lwk2.l;
	}


/****************************************************************************************************/
/*		Current Control Main Procedure																*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS								/* ���������L��										*/
	for( ax_noI = 0; (SHORT)ax_noI < AxisNum; ax_noI++ )
#else
	ax_noI = 0;
#endif
	{
		AxisRscI = &AxisHdl[ax_noI];
/*--------------------------------------------------------------------------------------------------*/
/*		Current Observer																			*/
/*--------------------------------------------------------------------------------------------------*/
		/* �d���I�u�U�[�o���Ή� �� �d���ʑ��⏞�I�u�U�[�o�ɕύX�\��

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
/*		�C���_�N�V�������[�^�p����																	*/
/****************************************************************************************************/
				; // ���Ή�
			}
			else
			{
/****************************************************************************************************/
/*																									*/
/*		�����@�p����																				*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		�����ӖO�a�v�Z		KtPercent = (��Mg/��Mg0) * 2^14											*/
/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = limit( AxisRscI->Curctrl.Iqref, (LONG)OneR );		/* lwk0 = sign(Iqref)	*/
				lwk0.l = lwk0.l * AxisRscI->Curctrl.Iqref;					/* lwk0 = |Iqref|		*/

				lwk1.l = lwk0.l - AxisRscI->IntAdV.IKt2;
				if( lwk1.l >= 0 )
				{	/* |Iqref| >= IKt2 �̏ꍇ */
					/* KtPercent = limit( Kt2+(Kt3-Kt2) / (Imax-IKt2) * (Iq-IKt2), 0�`2^14 )		*/
					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.KtB, Ixlwk );
					lwk3.l = add_limitf( AxisRscI->IntAdV.Kt2, lwk2.l );
					AxisRscI->IntAdV.KtPercent = limitz( lwk3.l, (LONG)No14bit );
				}
				else
				{
					lwk1.l = lwk0.l - AxisRscI->IntAdV.IKt;
					if( lwk1.l >= 0 )
					{	/* |Iqref| >= IKt �̏ꍇ */
						/* KtPercent = limit( 100%+(Kt2-100%) / (IKt2-IKt) * (Iq-IKt), 0�`2^14 )	*/
						IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.KtA, Ixlwk );
						lwk3.l = add_limitf( (LONG)No14bit, lwk2.l );
						AxisRscI->IntAdV.KtPercent = limitz( lwk3.l, (LONG)No14bit );
					}
					else
					{	/* |Iqref| < IKt �̏ꍇ */
						AxisRscI->IntAdV.KtPercent = (LONG)No14bit;			/* KtPercent = 100%		*/
					}
				}
/*--------------------------------------------------------------------------------------------------*/
/*		Lq�O�a�v�Z			LqPercent = (Lq/Lq0) * 2^14												*/
/*--------------------------------------------------------------------------------------------------*/
				lwk1.l = lwk0.l - AxisRscI->IntAdV.ILq3;
				if( lwk1.l >= 0 )
				{	/* |Iqref| >= ILq3 �̏ꍇ */
					/* LqPercent = limit( Lq3+(Lq4-Lq3) / (Imax-ILq3) * (Iq-ILq3), 0�`2^14 )		*/
					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.LqC, Ixlwk );
					lwk3.l = add_limitf( AxisRscI->IntAdV.Lq3, lwk2.l );
					AxisRscI->IntAdV.LqPercent = limitz( lwk3.l, (LONG)No14bit );
				}
				else
				{
					lwk1.l = lwk0.l - AxisRscI->IntAdV.ILq2;
					if( lwk1.l >= 0 )
					{	/* |Iqref| >= ILq2 �̏ꍇ */
						/* LqPercent = limit( Lq2+(Lq3-Lq2) / (ILq3-ILq2) * (Iq-ILq2), 0�`2^14 )	*/
						IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.Lq2, Ixlwk );
						lwk3.l = add_limitf( AxisRscI->IntAdV.Lq3, lwk2.l );
						AxisRscI->IntAdV.LqPercent = limitz( lwk3.l, (LONG)No14bit );
					}
					else
					{	/* |Iqref| < ILq2 �̏ꍇ */
						/* LqPercent = limit( Lq+(Lq2-Lq) / ILq 2* Iq, 0�`2^14 )					*/
						IxMulgain32( &lwk2.l, lwk0.l, AxisRscI->IntAdV.LqA, Ixlwk );
						lwk3.l = add_limitf( (LONG)No14bit, lwk2.l );
						AxisRscI->IntAdV.LqPercent = limitz( lwk3.l, (LONG)No14bit );
					}
				}
/*--------------------------------------------------------------------------------------------------*/
/*		Ld�O�a�v�Z			LdPercent = (Ld/Ld0) * 2^14												*/
/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = limit( AxisRscI->Curctrl.Idref, (LONG)OneR );		/* lwk0 = sign(Idref)	*/
				lwk0.l = lwk0.l * AxisRscI->Curctrl.Idref;					/* lwk0 = |Idref|		*/

				lwk1.l = lwk0.l - AxisRscI->IntAdV.ILd3;
				if( lwk1.l >= 0 )
				{	/* |Idref| >= ILd3 �̏ꍇ */
					/* LdPercent = limit( Ld3+(Ld4-Ld3) / (Imax-ILd3) * (Id-ILd3), 0�`2^14 )		*/
					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.LdC, Ixlwk );
					lwk3.l = add_limitf( AxisRscI->IntAdV.Ld3, lwk2.l );
					AxisRscI->IntAdV.LdPercent = limitz( lwk3.l, (LONG)No14bit );
				}
				else
				{
					lwk1.l = lwk0.l - AxisRscI->IntAdV.ILd2;
					if( lwk1.l >= 0 )
					{	/* |Idref| >= ILd2 �̏ꍇ */
						/* LdPercent = limit( Ld2+(Ld3-Ld2) / (ILd3-ILd2) * (Id-ILd2), 0�`2^14 )	*/
						IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->IntAdV.LdB, Ixlwk );
						lwk3.l = add_limitf( AxisRscI->IntAdV.Ld2, lwk2.l );
						AxisRscI->IntAdV.LdPercent = limitz( lwk3.l, (LONG)No14bit );
					}
					else
					{	/* |Idref| < ILd2 �̏ꍇ */
						/* LdPercent = limit( Ld+(Ld2-Ld) / ILd2 * Id, 0�`2^14 )					*/
						IxMulgain32( &lwk2.l, lwk0.l, AxisRscI->IntAdV.LdA, Ixlwk );
						lwk3.l = add_limitf( (LONG)No14bit, lwk2.l );
						AxisRscI->IntAdV.LdPercent = limitz( lwk3.l, (LONG)No14bit );
					}
				}

/****************************************************************************************************/
/*																									*/
/*		��ߊE������																				*/
/*																									*/
/****************************************************************************************************//* <S18E>:�C�� */
				if( AxisRscI->MicroIf.CtrlSwitch & V_FB )
				{ /* ��ߊE���L���̏ꍇ */
/*--------------------------------------------------------------------------------------------------*/
/*		q���d�����~�b�g�v�Z																			*/
/*--------------------------------------------------------------------------------------------------*/
					dlwk0.dl = mul( AxisRscI->WeakFV.WfV1max, AxisRscI->WeakFV.WfV1max );
					lwk2.l = dlwk0.l[0];
					lwk3.l = dlwk0.l[1];								/* lwk3,2 = WFV1max^2		*/
					dlwk0.dl = mul( AxisRscI->Curctrl.VdBeforeLim,		/* dlwk0 = VdBeforeLim^2	*/
									 AxisRscI->Curctrl.VdBeforeLim );
/*--------------------------------------------------------------------------------------------------*/
					lwk2.l = (ULONG)lwk2.l >> 1;						/* lwk2 = lwk2 / 2(�_��)	*/
					lwk4.l = (ULONG)dlwk0.l[0] >> 1;					/* lwk4 = MacL / 2(�_��)	*/
					lwk4.l = lwk2.l - lwk4.l;							/* ����32bit���Z			*/
					lwk6.l = (ULONG)lwk4.l >> 31;						/* �{���[(�_��)				*/
					lwk4.l = (ULONG)lwk4.l << 1;						/* lwk4 = lwk4 * 2(�_��)	*/
					lwk5.l = lwk3.l - dlwk0.l[1];						/* ���32bit���Z			*/
					lwk5.l = lwk5.l - lwk6.l;							/* lwk5 = lwk5 - borrow		*/
/*--------------------------------------------------------------------------------------------------*/
					if( lwk5.l < (LONG)ZeroR )
					{	/* ���̏ꍇ */
						lwk0.l = (LONG)ZeroR;							/* lwk0 = 0							*/
					}
					else
					{
						if( lwk5.l == (LONG)ZeroR )
						{	/* ���32bit��0�̏ꍇ */
							lwk1.l = lwk4.l;
							lwk0.s[0] = MpSQRT( lwk1.l );				/* swk0 = ��(2^48 - Idref^2)		*/
							lwk0.l = (ULONG)lwk0.s[0] & No0000ffff;
						}
						else
						{
							if( lwk5.l < (LONG)No8bit )
							{	/* ���32bit��0x00000100��菬�����ꍇ */
								lwk8.l = (ULONG)lwk4.l >> 8;			/* �_���V�t�g						*/
								lwk8.l = lwk8.l & No00ffffff;
								lwk9.l = (ULONG)lwk5.l << 24;			/* �_���V�t�g						*/
								lwk9.l = lwk9.l & Noff000000;
								lwk1.l = lwk8.l + lwk9.l;				/* lwk1 = (2^48 - Idref^2)/2^8		*/
								lwk0.s[0] = MpSQRT( lwk1.l );			/* swk0 = ��(2^48 - Idref^2)/2^4	*/
								lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 4;
																		/* lwk0 = ��(2^48 - Idref^2) (�_��)	*/
							}
							else
							{
								if( lwk5.l < No16bit )
								{	/* ���32bit��0x00010000��菬�����ꍇ */
									lwk8.l = (ULONG)lwk4.l >> 16;		/* �_���V�t�g						*/
									lwk8.l = lwk8.l & No0000ffff;
									lwk9.l = (ULONG)lwk5.l << 16;		/* �_���V�t�g						*/
									lwk9.l = lwk9.l & Noffff0000;
									lwk1.l = lwk8.l + lwk9.l;			/* lwk1 = (2^48 - Idref^2)/2^16		*/
									lwk0.s[0] = MpSQRT( lwk1.l );		/* swk0 = ��(2^48 - Idref^2)/2^8	*/
									lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 8;
																		/* lwk0 = ��(2^48 - Idref^2) (�_��)	*/
								}
								else
								{	/* ���32bit��0x00010000�̏ꍇ */
									lwk0.l = (LONG)No24bit;				/* lwk0 = ��(2^48) = 2^24			*/
								}
							}
						}
					}
					AxisRscI->WeakFV.WfVqLim = lwk0.l;
	/*--------------------------------------------------------------------------------------------------*/
	/*		WFVqLim - |VqBeforeLim| �̌v�Z																*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk8.l = limit( AxisRscI->Curctrl.VqBeforeLim, (LONG)OneR );
					dlwk0.dl = mul ( AxisRscI->Curctrl.VqBeforeLim, lwk8.l );	/* MacL = |VqBeforeLim|	*/
					lwk0.l = AxisRscI->WeakFV.WfVqLim - dlwk0.l[0];				/* lwk0 = WFVqLim - |VqBeforeLim|	*/
					lwk0.l = limit( lwk0.l, (LONG)No23bit );					/* ���Z�̂��߂Ɂ}23bit�Ń��~�b�g���� */

					lwk2.l = limit( lwk0.l, (LONG)OneR );
					lwk2.l = lwk0.l * lwk2.l;									/* lwk2 = |lwk0|	*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		��ፀ(wkInTtAd0 * (Kpv / ��)) �̌v�Z														*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk9.l = limit( AxisRscI->Vcmp.MotSpd, (LONG)OneR );
					lwk9.l = AxisRscI->Vcmp.MotSpd * lwk9.l;					/* lwk9 = |MotSpd|	*/
																				/* Ld�̕ω������l��	*/
					lwk9.l = mulshr( lwk9.l, AxisRscI->IntAdV.LdPercent, 27 );	/* lwk9 = (|MotSpd|*LdPercent/16384)/2^13 (�Z�p)	*/
					lwk1.l = lwk0.l / lwk9.l;

					IxMulgain32( &lwk2.l, lwk1.l, AxisRscI->WeakFV.WfKpv, Ixlwk );
					lwk4.l = limit( lwk2.l, (LONG)No30bit );					/* �}30bit�Ń��~�b�g	*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		�ϕ����v�Z(�A���`���C���h�A�b�v�t��)														*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk7.l = AxisRscI->WeakFV.Idref0 - AxisRscI->Curctrl.Idref;	/* ���~�b�g�O - ���~�b�g�� */
					lwk8.l = lwk4.l - lwk7.l;									/* �ϕ����͂������		*/
					lwk8.l = limit( lwk8.l, (LONG)No30bit );					/* �}30bit�Ń��~�b�g	*/
					IxIntegral( &lwk5.l,										/* lwk5 = �ϕ���		*/
								lwk8.l,
								AxisRscI->WeakFV.WfKiv,
								AxisRscI->WeakFV.WfInteg.l,
								Ixlwk );

					if( lwk5.l >= (LONG)ZeroR )
					{	/* �ϕ��� >= 0 �̏ꍇ */
						lwk5.l = (LONG)ZeroR;
						AxisRscI->WeakFV.WfInteg.l[0] = ZeroR;					/* �ϕ��N���A			*/
						AxisRscI->WeakFV.WfInteg.l[1] = ZeroR;					/* �ϕ��N���A			*/
					}
	/*--------------------------------------------------------------------------------------------------*/
	/*		��ፀ + �ϕ���																				*/
	/*--------------------------------------------------------------------------------------------------*/
					lwk6.l = lwk4.l + lwk5.l;
	/*--------------------------------------------------------------------------------------------------*/
	/*		���[�p�X�t�B���^																			*/
	/*--------------------------------------------------------------------------------------------------*/
					IxLpfilter1( &AxisRscI->WeakFV.WfIdref, lwk6.l, AxisRscI->WeakFV.WfLpfGain, Ixlwk );
	/*--------------------------------------------------------------------------------------------------*/
	/*		WFIdref > 0 �Ȃ�΁AWFIdref = 0,�ϕ� = 0													*/
	/*			WFIdref�����ɂȂ邱�Ƃ͖����B���ɂȂ����ꍇ��0�ɂ���B									*/
	/*--------------------------------------------------------------------------------------------------*/
					if( AxisRscI->WeakFV.WfIdref >= (LONG)ZeroR )
					{	/* WFIdref >= 0 �̏ꍇ */
						AxisRscI->WeakFV.WfIdref = (LONG)ZeroR;
						AxisRscI->WeakFV.WfInteg.l[0] = (LONG)ZeroR;
						AxisRscI->WeakFV.WfInteg.l[1] = (LONG)ZeroR;
					}
				}
				else
				{	/* ��ߊE�����䖳���̏ꍇ */
					AxisRscI->WeakFV.WfIdref = (LONG)ZeroR;						/* ��ߊE��Idref=0 		*/
				}
	/*--------------------------------------------------------------------------------------------------*/
	/*		Idref = Idref1 + WFIdref (Idref = limit( lwk0, WFIdrefLim�`0 ))								*/
	/*--------------------------------------------------------------------------------------------------*/
				AxisRscI->WeakFV.Idref0 = AxisRscI->Curctrl.IdrefBeforeWF + AxisRscI->WeakFV.WfIdref;
				AxisRscI->Curctrl.Idref = limitz( AxisRscI->WeakFV.Idref0, AxisRscI->WeakFV.WfIdrefLim );
//				IxAndReg16( &FlagIdrefLim, IFLAG, FlagLimit );	/* ���~�b�g�t���O���o��(�s�g�p)		*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		�����ӂ̋t���v�Z																			*/
	/*--------------------------------------------------------------------------------------------------*/
				/* lwk0 = (��Mg / ��max) * 2^14 */
				lwk0.l = mulshr( AxisRscI->IntAdV.KtPercent, AxisRscI->IntAdV.RatioPhiMg, 14 );
				/* lwk1 = (Ld/Ld0 * Ld0/Lq0) * 2^28 */
				lwk1.l = AxisRscI->IntAdV.LdPercent * AxisRscI->IntAdV.RatioLdLq;
				/* lwk2 = (Lq/Lq0) * 2^28 */
				lwk2.l = (ULONG)AxisRscI->IntAdV.LqPercent << 14;
				/* lwk4 = ((Lq-Ld)/Lq0) * Idref */
				lwk3.l = lwk2.l - lwk1.l;
				lwk4.l = mulshr( lwk3.l, AxisRscI->Curctrl.Idref, 28 );
				/* lwk5 = ((Lq-Ld)/Lq0)*Idref * (Lq0*MaxCur/2^24)/��max*2^14 */
				IxMulgain32( &lwk5.l, lwk4.l, AxisRscI->IntAdV.CnvRatioPhi, Ixlwk );
				/* lwk6 = ((��Mg/��max)-(((Lq-Ld)/Lq0)*Idref*(Lq0*MaxCur/2^24)/��max))*2^14	*/
				lwk6.l = sub_limitf( lwk0.l, lwk5.l );
				/* lwk6 = limit( lwk6, 0�`32767 )	*/
				lwk6.l = limitz( lwk6.l, (LONG)No32767 );

				if( lwk6.s[0] >= (SHORT)1024 )
				{	/* lwk6.s[0] >= 1024 �̏ꍇ */
					lwk7.l = (LONG)No24bit / (LONG)lwk6.s[0];						/* lwk7.sr = 2^24 / lwk6.sr	*/
					lwk7.l = (ULONG)lwk7.s[0];										/* 16bit��32bit(�����Ȃ�)	*/
					/* PhiForTrq = 2^24/lwk6.sr * CnvToIqref = 2^10/�� */
					IxMulgain32( &AxisRscI->IntAdV.PhiForTrq, lwk7.l, AxisRscI->IntAdV.CnvToIqref, Ixlwk );	
				}
				else
				{
					if( lwk6.s[0] >= (SHORT)32 )
					{	/* lwk6.s[0] >= 32 �̏ꍇ */
						lwk7.l = (LONG)No19bit / (LONG)lwk6.s[0];					/* lwk7.sr = 2^19 / lwk6.sr	*/
						lwk7.l = (ULONG)lwk7.s[0];									/* 16bit��32bit(�����Ȃ�)	*/
						lwk7.l = (ULONG)lwk7.l << 5;								/* lwk7 = lwk7 << 5			*/
						/* PhiForTrq = 2^24/lwk6.sr * CnvToIqref = 2^10/�� */
						IxMulgain32( &AxisRscI->IntAdV.PhiForTrq, lwk7.l, AxisRscI->IntAdV.CnvToIqref, Ixlwk );
					}
					else
					{
						if( lwk6.s[0] >= (SHORT)OneR )
						{	/* lwk6.s[0] >= 1 �̏ꍇ */
							lwk7.l = (LONG)No14bit / (LONG)lwk6.s[0];				/* lwk7.sr = 2^14 / lwk6.sr	*/
							lwk7.l = (ULONG)lwk7.s[0];								/* 16bit��32bit(�����Ȃ�)	*/
							lwk7.l = (ULONG)lwk7.l << 10;							/* lwk7 = lwk7 << 10		*/
							/* PhiForTrq = 2^24/lwk6.sr * CnvToIqref = 2^10/��	*/
							IxMulgain32( &AxisRscI->IntAdV.PhiForTrq, lwk7.l, AxisRscI->IntAdV.CnvToIqref, Ixlwk );
						}
						else
						{	/* lwk6.s[0] = 0 �̏ꍇ */
							AxisRscI->IntAdV.PhiForTrq = (LONG)No24bit;
						}
					}
				}
	/*--------------------------------------------------------------------------------------------------*/
	/*		q���d���w�ߌv�Z																				*/
	/*--------------------------------------------------------------------------------------------------*/
				/* MacH,L = Trqref / �� * 2^10 */
				lwk0.l = mulshr_limitf( AxisRscI->Trqctrl.Trqref, AxisRscI->IntAdV.PhiForTrq, 10 );
				AxisRscI->Curctrl.IqrefBeforeLim = lwk0.l;		/* IqrefBeforeLim = Trqref / ��			*/
	/*--------------------------------------------------------------------------------------------------*/
	/*		�d�����~�b�g����																			*/
	/*--------------------------------------------------------------------------------------------------*/
				lwk9.l = (LONG)No24bit;									/* lwk9 = 2^24							*/
				lwk9.l = limitz( lwk9.l, AxisRscI->MotInfo.MaxCur );	/* lwk9 = limit( lwk9, 0�`MaxCur )		*/
				lwk9.l = limitz( lwk9.l, AxisRscI->MotInfo.MaxCurLim );	/* lwk9 = limit( lwk9, 0�`MaxCurLim )	*/

	/*--------------------------------------------------------------------------------------------------*/
	/*		q���d���w�߃��~�b�g�v�Z																		*/
	/*--------------------------------------------------------------------------------------------------*/
				dlwk0.dl = mul( lwk9.l, lwk9.l );							/* MacH,L = lwk9^2			*/
				lwk0.l = dlwk0.l[0];
				lwk1.l = dlwk0.l[1];										/* lwk1,0 = MaxCurLim^2		*/
				lwk0.l = (ULONG)lwk0.l >> 1;								/* lwk0 = lwk0 / 2 (�_��)	*/
	/*--------------------------------------------------------------------------------------------------*/
				dlwk0.dl = mul( AxisRscI->Curctrl.Idref, AxisRscI->Curctrl.Idref );/* MacH,L = Idref^2	*/
				lwk2.l = (ULONG)dlwk0.l[0] >> 1;							/* lwk2 = MacL / 2 (�_��)	*/
				lwk2.l = lwk0.l - lwk2.l;									/* ����32bit���Z			*/
				lwk4.l = (ULONG)lwk2.l >> 31;								/* �{���[  (�_��)			*/
				lwk2.l = (ULONG)lwk2.l << 1;								/* lwk2 = lwk2 * 2  (�_��)	*/
				lwk3.l = lwk1.l - dlwk0.l[1];								/* ���32bit���Z			*/
				lwk3.l = lwk3.l - lwk4.l;									/* lwk3 = lwk3 - borrow		*/
				if( lwk3.l < (LONG)ZeroR )
				{	/* ���̏ꍇ */
					lwk0.l = (LONG)ZeroR;									/* lwk0 = 0					*/
				}
				else
				{
					if( lwk3.l == (LONG)ZeroR )
					{	/* ���32bit��0�̏ꍇ */
						lwk0.s[0] = MpSQRT( lwk2.l );						/* lwk0 = ��(MaxCurLim^2 - Idref^2)	*/
						lwk0.l = (ULONG)lwk0.s[0] & No0000ffff;
					}
					else
					{
						if( lwk3.l < (LONG)No8bit )
						{	/* ���32bit��0x00000100��菬�����ꍇ */
							lwk8.l = (ULONG)lwk2.l >> 8;					/* �_���V�t�g						*/
							lwk8.l = lwk8.l & No00ffffff;
							lwk9.l = (ULONG)lwk3.l << 24;					/* �_���V�t�g						*/
							lwk9.l = lwk9.l & Noff000000;
							lwk1.l = lwk8.l + lwk9.l;					/* lwk1 = (MaxCurLim^2 - Idref^2)/2^8	*/
							lwk0.s[0] = MpSQRT( lwk1.l );				/* lwk0 = ��(MaxCurLim^2 - Idref^2)/2^4	*/
							lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 4;
																		/* lwk0 = ��(MaxCurLim^2 - Idref^2)		*/
						}
						else
						{
							if( lwk3.l < (LONG)No16bit )
							{	/* ���32bit��0x00010000��菬�����ꍇ */
								lwk8.l = (ULONG)lwk2.l >> 16;				/* �_���V�t�g						*/
								lwk8.l = lwk8.l & No0000ffff;
								lwk9.l = (ULONG)lwk3.l << 16;				/* �_���V�t�g						*/
								lwk9.l = lwk9.l & Noffff0000;
								lwk1.l = lwk8.l + lwk9.l;				/* lwk1 = (MaxCurLim^2 - Idref^2)/2^16	*/
								lwk0.s[0] = MpSQRT( lwk1.l );			/* lwk0 = ��(MaxCurLim^2 - Idref^2)/2^8	*/
								lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) << 8;
																		/* lwk0 = ��(MaxCurLim^2 - Idref^2)		*/
							}
							else
							{	/* ���32bit��0x00010000�̏ꍇ */
								lwk0.l = (LONG)No24bit;						/* lwk0 = ��(2^48) = 2^24			*/
							}
						}
					}
				}
	/*--------------------------------------------------------------------------------------------------*/
	/*		q���d���w�߃��~�b�g																			*/
	/*--------------------------------------------------------------------------------------------------*/
				if( AxisRscI->Curctrl.IqrefBeforeLim >= ZeroR )
				{	/* �������~�b�g���� */
					AxisRscI->Curctrl.Iqref = limit( AxisRscI->Curctrl.IqrefBeforeLim, lwk0.l );
					/* �g���N�������t���O���Z�b�g */
					swk10 = AxisRscI->StsFlg.CtrlStsRW | TLIM;
					AxisRscI->StsFlg.CtrlStsRW = cmove( (AxisRscI->Curctrl.Iqref == lwk0.l),
														swk10,
														AxisRscI->StsFlg.CtrlStsRW );
				}
				else
				{	/* �������~�b�g���� */
					AxisRscI->Curctrl.Iqref = limit( AxisRscI->Curctrl.IqrefBeforeLim, lwk0.l );
					/* �g���N�������t���O���Z�b�g */
					swk10 = AxisRscI->StsFlg.CtrlStsRW | TLIM;
					AxisRscI->StsFlg.CtrlStsRW = cmove( (AxisRscI->Curctrl.Iqref == lwk0.l),
														swk10,
														AxisRscI->StsFlg.CtrlStsRW );
				}
			}

/****************************************************************************************************/
/*																									*/
/*		ACRd(d���d������)																			*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		�΍��v�Z																					*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = AxisRscI->Curctrl.Idref - AxisRscI->Curctrl.IdDetect;
/*--------------------------------------------------------------------------------------------------*/
/*		��ፀ�v�Z																					*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk1.l, lwk0.l, AxisRscI->Curctrl.GainKpd, Ixlwk );	/* lwk1 = GainKpd * lwk0			*/
			lwk1.l = mulshr( lwk1.l, AxisRscI->IntAdV.LdPercent, 14 );			/* lwk1 = (GainKpd*Ld/Ld0) * lwk0	*/
			lwk1.l = limit( lwk1.l, (LONG)No30bit );							/* �}30bit�Ń��~�b�g				*/
			lwk7.l = lwk1.l;
/*--------------------------------------------------------------------------------------------------*/
/*		�ϕ����v�Z(�A���`���C���h�A�b�v�t��)														*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->MicroIf.CtrlSwitch & ANTIWUP )
			{	/* �A���`���C���h�A�b�v���� */
				lwk2.l = AxisRscI->Curctrl.VdBeforeLim - AxisRscI->Curctrl.Vdref;	/* lwk2 = �d�����~�b�g��		*/
				lwk7.l = lwk7.l - lwk2.l;											/* lwk7 = �␳��ϕ�����		*/
				lwk7.l = limit( lwk7.l, (LONG)No30bit );							/* �}30bit�Ń��~�b�g			*/
			}
			IxIntegral( &lwk3.l, lwk7.l, AxisRscI->Curctrl.GainKid, AxisRscI->Curctrl.IntegD.l, Ixlwk );
/*--------------------------------------------------------------------------------------------------*/
/*		��ፀ + �ϕ��� �v�Z																		*/
/*--------------------------------------------------------------------------------------------------*/
			lwk4.l = lwk1.l + lwk3.l;
/*--------------------------------------------------------------------------------------------------*/
/*		�d���t�B���^(���[�p�X�t�B���^)																*/
/*--------------------------------------------------------------------------------------------------*/
			IxLpfilter1( &AxisRscI->Vfil.VdLpfOut, lwk4.l, AxisRscI->Vfil.VoltFilGain, Ixlwk );	

/****************************************************************************************************/
/*																									*/
/*		ACRq(q���d������)																			*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		�΍��v�Z																					*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = AxisRscI->Curctrl.Iqref - AxisRscI->Curctrl.IqDetect;
/*--------------------------------------------------------------------------------------------------*/
/*		��ፀ�v�Z																					*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk1.l, lwk0.l, AxisRscI->Curctrl.GainKpq, Ixlwk );	/* lwk1 = GainKpq * lwk0			*/
			lwk1.l = mulshr( lwk1.l, AxisRscI->IntAdV.LqPercent, 14 );			/* lwk1 = (GainKpq*Lq/Lq0) * lwk0	*/
			lwk1.l = limit( lwk1.l, No30bit );									/* �}30bit�Ń��~�b�g				*/
			lwk7.l = lwk1.l;
/*--------------------------------------------------------------------------------------------------*/
/*		�ϕ����v�Z(�A���`���C���h�A�b�v�t��)														*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->MicroIf.CtrlSwitch & ANTIWUP )
			{
				lwk2.l = AxisRscI->Curctrl.VqBeforeLim - AxisRscI->Curctrl.Vqref;	/* lwk2 = �d�����~�b�g��		*/
				lwk7.l = lwk7.l - lwk2.l;											/* lwk7 = �␳��ϕ�����		*/
				lwk7.l = limit( lwk7.l, (LONG)No30bit );							/* �}30bit�Ń��~�b�g			*/
			}
			IxIntegral( &lwk3.l, lwk7.l, AxisRscI->Curctrl.GainKiq, AxisRscI->Curctrl.IntegQ.l, Ixlwk );
/*--------------------------------------------------------------------------------------------------*/
/*		��ፀ + �ϕ��� �v�Z																		*/
/*--------------------------------------------------------------------------------------------------*/
			lwk4.l = lwk1.l + lwk3.l;
/*--------------------------------------------------------------------------------------------------*/
/*		�d���t�B���^(���[�p�X�t�B���^)																*/
/*--------------------------------------------------------------------------------------------------*/
			IxLpfilter1( &AxisRscI->Vfil.VqLpfOut, lwk4.l, AxisRscI->Vfil.VoltFilGain, Ixlwk );

/****************************************************************************************************/
/*																									*/
/*		Voltage Compensation(�d���⏞)																*/
/*																									*/
/****************************************************************************************************/
			if( AxisRscI->MicroIf.CtrlSwitch & ISEL )
			{													/* �d���w�߂��g�p					*/
				lwk1.l = AxisRscI->Curctrl.Idref;				/* lwk1 = d���d���w��				*/
				lwk2.l = AxisRscI->Curctrl.Iqref;				/* lwk2 = q���d���w��				*/
			}
			else
			{													/* �d��FB���g�p						*/
				lwk1.l = AxisRscI->Curctrl.IdDetect;			/* lwk1 = d���d��FB					*/
				lwk2.l = AxisRscI->Curctrl.IqDetect;			/* lwk2 = q���d��FB					*/
			}
/*--------------------------------------------------------------------------------------------------*/
/*		RId - ��LqIq �̌v�Z																			*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk3.l, lwk1.l, AxisRscI->Vcmp.Resist, Ixlwk );	/* lwk3 = RId				*/

			lwk4.l = mulshr( AxisRscI->Vcmp.MotSpd, lwk2.l, 20 );			/* lwk4 = (�� * Iq) >> 20	*/
			IxMulgain32( &lwk4.l, lwk4.l,  AxisRscI->Vcmp.Lq, Ixlwk );		/* lwk4 = ��Lq0Iq			*/
			lwk4.l = mulshr( lwk4.l,  AxisRscI->IntAdV.LqPercent, 14 );		/* lwk4 = ��Lq0Iq * Lq/Lq0	*/

			lwk3.l = lwk3.l - lwk4.l;										/* lwk3 = RId - ��LqIq		*/
/*--------------------------------------------------------------------------------------------------*/
/*		RIq + ��LdId + �փ� �̌v�Z																	*/
/*--------------------------------------------------------------------------------------------------*/
			IxMulgain32( &lwk4.l, lwk2.l, AxisRscI->Vcmp.Resist, Ixlwk );	/* lwk4 = RIq				*/

			lwk5.l = mulshr( AxisRscI->Vcmp.MotSpd, lwk1.l, 20 );			/* lwk5 = (�� * Id) >> 20	*/
			IxMulgain32( &lwk5.l, lwk5.l, AxisRscI->Vcmp.Ld, Ixlwk );		/* lwk5 = ��Ld0Id			*/
			lwk5.l = mulshr( lwk5.l, AxisRscI->IntAdV.LdPercent, 14 );		/* lwk5 = ��Ld0Id * Ld/Ld0	*/

			lwk6.l = mulshr( AxisRscI->Vcmp.OmegaPhi, AxisRscI->IntAdV.KtPercent, 14 );
																			/* lwk6 = �փ�0 * ��/��0			*/

			lwk4.l = lwk4.l + lwk5.l;										/* lwk4 = RIq + ��LdId				*/
			lwk4.l = lwk4.l + lwk6.l;										/* lwk4 = RIq + ��LdId + �փ�		*/
/*--------------------------------------------------------------------------------------------------*/
/*		Ld*dId/dt, Lq*dIq/dt �̌v�Z (Ldi/dt�⏞)													*/
/*--------------------------------------------------------------------------------------------------*/
/* ���̂Ƃ���K�v�Ȃ��̂ŁA���Ȃ��B */
			AxisRscI->Vcmp.VoltageffD = lwk3.l;								/* VoltageffD = RId - ��LqIq		*/
			AxisRscI->Vcmp.VoltageffQ = lwk4.l;								/* VoltageffQ = RIq + ��LdId + �փ�	*/
/*--------------------------------------------------------------------------------------------------*/
/*		�d���w�߂ɓd��FF�⏞��������																*/
/*--------------------------------------------------------------------------------------------------*/
			lwk5.l = AxisRscI->Vfil.VdLpfOut + AxisRscI->Vcmp.VoltageffD;	/* lwk5 = VdLpfOut + VoltageffD		*/
			lwk6.l = AxisRscI->Vfil.VqLpfOut + AxisRscI->Vcmp.VoltageffQ;	/* lwk6 = VqLpfOut + VoltageffQ		*/
/*--------------------------------------------------------------------------------------------------*/
/*		AVR�Q�C���⏞																				*/
/*--------------------------------------------------------------------------------------------------*/
			lwk7.l = (ULONG)AxisRscI->Curctrl.AVRGain;					/* 16bit��32bit(�����Ȃ�)				*/
			lwk8.l = mulshr( lwk5.l, lwk7.l, 13 );						/* MacH,L = d���d���w�� * AVRGain		*/
																		/* lwk8 = MacH,L >> 13					*/
			AxisRscI->Curctrl.Vdref = limit( lwk8.l, (LONG)No30bit );	/* �}2^30�Ń��~�b�g						*/

			lwk9.l = mulshr( lwk6.l, lwk7.l, 13 );						/* MacH,L = q���d���w�� * AVRGain		*/
																		/* lwk9 = MacH,L >> 13					*/
			AxisRscI->Curctrl.Vqref = limit( lwk9.l, (LONG)No30bit );	/* �}2^30�Ń��~�b�g						*/

/****************************************************************************************************/
/*																									*/
/*		�d�����~�b�g����																			*/
/*																									*/
/****************************************************************************************************//* <S18E>:�C�� */
/*--------------------------------------------------------------------------------------------------*/
/*		�ϒ����v�Z																					*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = ZeroR;
			IxSquareSum( (DLREG*)&dlwk0,
						 AxisRscI->Curctrl.Vdref,
						 AxisRscI->Curctrl.Vqref,
						 (DWREG*)Ixlwk );

			if( dlwk0.l[1] >= (LONG)No24bit )
			{	/* ���32bit��0x01000000�ȏ�̏ꍇ */
				lwk0.s[0] = MpSQRT( dlwk0.l[1] );									/* lwk0 = ��(Vdref^2+Vqref^2)/2^16	*/
				AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 16;		/* V1 = ��(Vdref^2+Vqref^2)			*/
				lwk9.s[0] = 16;														/* �V�t�g���ۑ�						*/
			}
			else
			{
				if( dlwk0.l[1] >= (LONG)No16bit )
				{	/* ���32bit��0x00010000�ȏ�̏ꍇ */
					lwk8.l = (ULONG)dlwk0.l[0] >> 24;								/* �_���V�t�g						*/
					lwk8.l = lwk8.l & No000000ff;
					lwk9.l = (ULONG)dlwk0.l[1] << 8;								/* �_���V�t�g						*/
					lwk9.l = lwk9.l & Noffffff00;
					lwk1.l = lwk8.l + lwk9.l;										/* lwk1 = (Vdref^2+Vqref^2)/2^24	*/
					lwk0.s[0] = MpSQRT( lwk1.l );									/* lwk0 = ��(Vdref^2+Vqref^2)/2^12	*/
					AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 12;	/* V1 = ��(Vdref^2+Vqref^2)			*/
					lwk9.s[0] = 12;													/* �V�t�g���ۑ�						*/
				}
				else
				{
					if( dlwk0.l[1] >= (LONG)No8bit )
					{	/* ���32bit��0x00000100�ȏ�̏ꍇ */
						lwk8.l = (ULONG)dlwk0.l[0] >> 16;							/* �_���V�t�g						*/
						lwk8.l = lwk8.l & No0000ffff;
						lwk9.l = (ULONG)dlwk0.l[1] << 16;							/* �_���V�t�g						*/
						lwk9.l = lwk9.l & Noffff0000;
						lwk1.l = lwk8.l + lwk9.l;									/* lwk1 = (Vdref^2+Vqref^2)/2^16	*/
						lwk0.s[0] = MpSQRT( lwk1.l );								/* lwk0 = ��(Vdref^2+Vqref^2)/2^8	*/
						AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 8;/* V1 = ��(Vdref^2+Vqref^2)			*/
						lwk9.s[0] = 8;												/* �V�t�g���ۑ�						*/
					}
					else
					{
						if( dlwk0.l[1] != (LONG)ZeroR )
						{	/* ���32bit��0�łȂ��ꍇ */
							lwk8.l = (ULONG)dlwk0.l[0] >> 8;						/* �_���V�t�g						*/
							lwk8.l = lwk8.l & No00ffffff;
							lwk9.l = (ULONG)dlwk0.l[1] << 24;						/* �_���V�t�g						*/
							lwk9.l = lwk9.l & Noff000000;
							lwk1.l = lwk8.l + lwk9.l;								/* lwk1 = (Vdref^2+Vqref^2)/2^8		*/
							lwk0.s[0] = MpSQRT( lwk1.l );							/* lwk0 = ��(Vdref^2+Vqref^2)/2^4	*/
							AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff) << 4;
																					/* V1 = ��(Vdref^2+Vqref^2)			*/
							lwk9.s[0] = 4;											/* �V�t�g���ۑ�						*/
						}
						else
						{	/* ���32bit��0�̏ꍇ */
							lwk1.l = dlwk0.l[0];									/* lwk1 = (Vdref^2+Vqref^2)			*/
							lwk0.s[0] = MpSQRT( lwk1.l );							/* lwk0 = ��(Vdref^2+Vqref^2)		*/
							AxisRscI->Vltctrl.V1 = ((ULONG)lwk0.s[0] & No0000ffff);	/* V1 = ��(Vdref^2+Vqref^2)			*/
							lwk9.s[0] = (SHORT)ZeroR;								/* �V�t�g���ۑ�						*/
						}
					}
				}
			}

			AxisRscI->Curctrl.VdBeforeLim = AxisRscI->Curctrl.Vdref;				/* �ۑ�								*/
			AxisRscI->Curctrl.VqBeforeLim = AxisRscI->Curctrl.Vqref;				/* �ۑ�								*/
			AxisRscI->Vltctrl.V1BeforeLim = AxisRscI->Vltctrl.V1;					/* �ۑ�								*/

/*--------------------------------------------------------------------------------------------------*/
/*		�d�����~�b�g����																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( AxisRscI->Vltctrl.Vmax < AxisRscI->Vltctrl.V1 )
			{	/* �ϒ��������~�b�g�ȉ� */
				lwk1.l = AxisRscI->Vltctrl.Vmax;
				if( lwk0.s[0] < (SHORT)ZeroR )
				{	/* lwk0.s[0]�����̏ꍇ */
					/* lwk0.sr��32767�𒴂��ĕ��̒l�ɂȂ��Ă���̂ŁA2�Ŋ��� */
					lwk1.l = (ULONG)lwk1.l >> 1;									/* �_���V�t�g	*/
					lwk0.l = ((ULONG)lwk0.s[0] & No0000ffff) >> 1;					/* �_���V�t�g	*/
				}
				/* lwk2=Vmax/V1*2^(lwk9.sr), lwk0=4096�`65535, lwk1=4194304�`5340353 */
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
/*		Vuref = Vdref*cos�� - Vqref*sin��															*/
/*--------------------------------------------------------------------------------------------------*/
			lwk8.l = mulshr( AxisRscI->Curctrl.Vdref, AxisRscI->SinTbl.Cos1.l, 14 );	/* lwk8 = (Vderf * cos��) >> 14			*/
			lwk9.l = mulshr( AxisRscI->Curctrl.Vqref, AxisRscI->SinTbl.Sin1.l, 14 );	/* lwk9 = (Vqerf * sin��) >> 14			*/
			AxisRscI->Curctrl.Vuref = lwk8.l - lwk9.l;									/* Vuref = Vdref*cos�� - Vqref*sin��	*/
/*--------------------------------------------------------------------------------------------------*/
/*		Vvref = Vdref*cos(��-2��/3) - Vqref*sin(��-2��/3)											*/
/*--------------------------------------------------------------------------------------------------*/
			lwk8.l = mulshr( AxisRscI->Curctrl.Vdref, AxisRscI->SinTbl.Cos2.l, 14 );	/* lwk8 = (Vderf * ��-2��/3) >> 14						*/
			lwk9.l = mulshr( AxisRscI->Curctrl.Vqref, AxisRscI->SinTbl.Sin2.l, 14 );	/* lwk9 = (Vqerf * ��-2��/3) >> 14						*/
			AxisRscI->Curctrl.Vvref = lwk8.l - lwk9.l;									/* Vuref = Vdref*cos(��-2��/3) - Vqref*sin(��-2��/3)	*/
/*--------------------------------------------------------------------------------------------------*/
/*		Vwref = -Vuref - Vvref																		*/
/*--------------------------------------------------------------------------------------------------*/
			lwk5.l = (LONG)ZeroR - AxisRscI->Curctrl.Vuref;
			AxisRscI->Curctrl.Vwref = lwk5.l - AxisRscI->Curctrl.Vvref;					/* Vwref = -Vuref - Vvref				*/

/****************************************************************************************************/
/*																									*/
/*		�ϒ��␳���ߕϒ��␳																		*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		Over modulation type select																	*/
/*--------------------------------------------------------------------------------------------------*/
			if( (AxisRscI->MicroIf.CtrlSwitch & OVMSEL2) == ZeroR )
			{	/* �^�C�v2�L���ł͂Ȃ��ꍇ	*/
				if( AxisRscI->Vltctrl.V1 >= (LONG)No22bit )	
				{	/* �ϒ��� >= 100% �̏ꍇ	*/
					if( (AxisRscI->MicroIf.CtrlSwitch & OVMSEL1) != ZeroR )
					{	/* �^�C�v1�L���̏ꍇ	*/
/*--------------------------------------------------------------------------------------------------*/
/*		Over modulation1																			*/
/*--------------------------------------------------------------------------------------------------*/
						IxSetCtblAdr( pCtbl, &(OVMODTBLG[0][0]) );					/* gain type						*/
						lwk9.l = mulshr( AxisRscI->Vltctrl.V1, (LONG)OneR, 9 );
																					/* ���K����ύX(�}2^22���}8192)		*/
						AxisRscI->Vltctrl.ModulationComp = MpOVMMODK( AxisRscI->Vltctrl.V1,
																	  lwk9.s[0],
																	  AxisRscI->MicroIf.CtrlSwitch,
																	  pCtbl );
						lwk0.l = (ULONG)AxisRscI->Vltctrl.ModulationComp;			/* 16bit��32bit(�����Ȃ�)			*/
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
						{	/* |Vuref| >= |Vvref| �̏ꍇ */
							if( lwk1.l >= lwk3.l )
							{	/* U�����ł��傫�����̏��� */
								lwk1.l = lwk1.l - (LONG)No22bit;			/* lwk1 = |Vuref|-2^22	*/
								lwk1.l = limitz( lwk1.l, (LONG)No24bit );	/* zero limit			*/
								lwk0.l = lwk4.l * lwk1.l;
							}
							else
							{	/* W�����ł��傫�����̏��� */
								lwk3.l = lwk3.l - (LONG)No22bit;			/* lwk3 = |Vwref|-2^22	*/
								lwk3.l = limitz( lwk3.l, (LONG)No24bit );	/* zero limit			*/
								lwk0.l = lwk6.l * lwk3.l;
							}
						}
						else
						{
							if( lwk2.l >= lwk3.l )
							{	/* V�����ł��傫�����̏��� */
								lwk2.l = lwk2.l - (LONG)No22bit;			/* lwk2 = |Vvref|-2^22	*/
								lwk2.l = limitz( lwk2.l, (LONG)No24bit );	/* zero limit			*/
								lwk0.l = lwk5.l * lwk2.l;
							}
							else
							{	/* W�����ł��傫�����̏��� */
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
																			/* ���K����ύX(�}2^22���}8192)		*/
				AxisRscI->Vltctrl.ModulationComp = MpOVMMODK( AxisRscI->Vltctrl.V1,
															  lwk9.s[0],
															  AxisRscI->MicroIf.CtrlSwitch,
															  pCtbl );
/*--------------------------------------------------------------------------------------------------*/
/*		MAX = lwk1, MIN = lwk2																		*/
/*		OFS = (lwk1+lwk2)/2																			*/
/*--------------------------------------------------------------------------------------------------*/
				if( AxisRscI->Curctrl.Vuref >= AxisRscI->Curctrl.Vvref )
				{	/* Vuref >= Vref �̏ꍇ */
					lwk1.l = AxisRscI->Curctrl.Vuref;
					lwk2.l = AxisRscI->Curctrl.Vvref;
				}
				else
				{	/* Vuref < Vref �̏ꍇ */
					lwk1.l = AxisRscI->Curctrl.Vvref;
					lwk2.l = AxisRscI->Curctrl.Vuref;
				}

				if( lwk1.l < AxisRscI->Curctrl.Vwref )
				{	/* Vuref��Vvref�̑傫���� < Vwref �̏ꍇ */
					lwk1.l = AxisRscI->Curctrl.Vwref;
				}
				else
				{
					if( AxisRscI->Curctrl.Vwref < lwk2.l )
					{	/* Vuref��Vvref�̏������� < Vwref �̏ꍇ */
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
				lwk0.l = (ULONG)AxisRscI->Vltctrl.ModulationComp;			/* 16bit��32bit(�����Ȃ�)		*/
				lwk0.l = (ULONG)lwk0.l << 9;								/* �_���V�t�g					*/

				lwk1.l = limit( AxisRscI->Curctrl.Vuref, (LONG)OneR );		/* lwk1 = -1 / 0 / 1			*/
				lwk1.l = lwk1.l | (LONG)OneR;								/* lwk1 = -1 / 1(0��1�ɂ���)	*/
				lwk1.l = lwk1.l * lwk0.l;
				AxisRscI->Curctrl.Vuref = lwk1.l + AxisRscI->Curctrl.Vuref;

				lwk1.l = limit( AxisRscI->Curctrl.Vvref, (LONG)OneR );		/* lwk1 = -1 / 0 / 1			*/
				lwk1.l = lwk1.l | (LONG)OneR;								/* lwk1 = -1 / 1(0��1�ɂ���)	*/
				lwk1.l = lwk1.l * lwk0.l;
				AxisRscI->Curctrl.Vvref = lwk1.l + AxisRscI->Curctrl.Vvref;

				lwk1.l = limit( AxisRscI->Curctrl.Vwref, (LONG)OneR );		/* lwk1 = -1 / 0 / 1			*/
				lwk1.l = lwk1.l | (LONG)OneR;								/* lwk1 = -1 / 1(0��1�ɂ���)	*/
				lwk1.l = lwk1.l * lwk0.l;
				AxisRscI->Curctrl.Vwref = lwk1.l + AxisRscI->Curctrl.Vwref;
			}

/****************************************************************************************************/
/*																									*/
/*		On-Delay																					*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		Iuref, Ivref �̌v�Z																			*/
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*		Iuref = Idref*cos�� - Iqref*sin��															*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = mulshr( AxisRscI->Curctrl.Idref, AxisRscI->SinTbl.Cos1.l, 14 );	/* lwk0 = (Idref * cos��) >> 14							*/
			lwk1.l = mulshr( AxisRscI->Curctrl.Iqref, AxisRscI->SinTbl.Sin1.l, 14 );	/* lwk1 = (Iqref * sin��) >> 14							*/
			AxisRscI->OnDelay.Iuref = lwk0.l - lwk1.l;									/* Iuref = Idref*cos�� - Iqref*sin��					*/
/*--------------------------------------------------------------------------------------------------*/
/*		Ivref = Idref*cos(��-2��/3) - Iqref*sin(��-2��/3)											*/
/*--------------------------------------------------------------------------------------------------*/
			lwk0.l = mulshr( AxisRscI->Curctrl.Idref, AxisRscI->SinTbl.Cos2.l, 14 );	/* lwk0 = (Idref * cos(��-2��/3)) >> 14					*/
			lwk1.l = mulshr( AxisRscI->Curctrl.Iqref, AxisRscI->SinTbl.Sin2.l, 14 );	/* lwk1 = (Iqref * sin(��-2��/3)) >> 14					*/
			AxisRscI->OnDelay.Ivref = lwk0.l - lwk1.l;									/* Ivref = Idref*cos(��-2��/3) - Iqref*sin(��-2��/3)	*/
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
				lwk1.l = AxisRscI->CurDet.IuDetect;								/* FB�d�����g�p			*/
			}
			else
			{
				lwk1.l = AxisRscI->OnDelay.Iuref;								/* �d���w�߂��g�p		*/
			}

			if( LPX_ABS(AxisRscI->CurDet.IvDetect) > LPX_ABS(AxisRscI->OnDelay.OnDelayLevel ) )
			{
				lwk2.l = AxisRscI->CurDet.IvDetect;								/* FB�d�����g�p			*/
			}
			else
			{
				lwk2.l = AxisRscI->OnDelay.Ivref;								/* �d���w�߂��g�p		*/
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
/*		��`�g�⏞																					*/
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
/*		��`�g�⏞																					*/
/*--------------------------------------------------------------------------------------------------*/
				lwk0.l = mulshr( AxisRscI->OnDelay.OnDelaySlope, lwk1.l, 24 );		/* lwk0 = (Iu * SlopeOnDelay) >> 24			*/
				lwk0.l = limit( lwk0.l, (LONG)No14bit );							/* �}16384�Ń��~�b�g						*/
				lwk1.s[0] = mulshr( AxisRscI->OnDelay.OnDelayComp, lwk0.s[0], 14 );	/* lwk1.s = (lwk0.s * OnDelayComp) >> 14	*/

				lwk0.l = mulshr( AxisRscI->OnDelay.OnDelaySlope, lwk2.l, 24 );		/* lwk0 = (Iv * SlopeOnDelay) >> 24			*/
				lwk0.l = limit( lwk0.l, (LONG)No14bit );							/* �}16384�Ń��~�b�g						*/
				lwk2.s[0] = mulshr( AxisRscI->OnDelay.OnDelayComp, lwk0.s[0], 14 );	/* lwk2.s = (lwk0.s * OnDelayComp) >> 14	*/

				lwk0.l = mulshr( AxisRscI->OnDelay.OnDelaySlope, lwk3.l, 24 );		/* lwk0 = (Iw * SlopeOnDelay) >> 24			*/
				lwk0.l = limit( lwk0.l, (LONG)No14bit );							/* �}16384�Ń��~�b�g						*/
				lwk3.s[0] = mulshr( AxisRscI->OnDelay.OnDelayComp, lwk0.s[0], 14 );	/* lwk3.s = (lwk0.sr * OnDelayComp) >> 14	*/
			}
		}

/****************************************************************************************************/
/*																									*/
/*		�O�p�g��r�l�v�Z																			*/
/*																									*/
/****************************************************************************************************//* <S18E>:�C�� */
/*--------------------------------------------------------------------------------------------------*/
/*		-400000h..400000h ��  0h..800000h �� 0h..CRFRQ												*/
/*--------------------------------------------------------------------------------------------------*/
		AxisRscI->Curctrl.Vuref = limit( AxisRscI->Curctrl.Vuref, (LONG)No22bit );	/* �}400000h�Ń��~�b�g	*/
		AxisRscI->Curctrl.Vvref = limit( AxisRscI->Curctrl.Vvref, (LONG)No22bit );	/* �}400000h�Ń��~�b�g	*/
		AxisRscI->Curctrl.Vwref = limit( AxisRscI->Curctrl.Vwref, (LONG)No22bit );	/* �}400000h�Ń��~�b�g	*/
/*--------------------------------------------------------------------------------------------------*/
/*		�{���́A(Vxref-400000)�Ōv�Z����̂�(400000h-Vxref)�Ƃ��Ă���B								*/
/*		���R�F�d�����o�̌������t�̂���(�A���v�����[�^�ɗ��ꂽ�Ƃ��Ƀ}�C�i�X�ɂȂ�)�A�d���̂ݕ�����	*/
/*			  �t�]������΂悩�������A�o�͓d���̕������t�]�����B���̂��߁A�G���R�[�_�̎��t����	*/
/*			  180�K���炵�Ď��t���Ă���B															*/
/*--------------------------------------------------------------------------------------------------*/
		lwk0.l = (ULONG)AxisRscI->IntAdV.CarrierFreq;			/* 16bit��32bit(�����Ȃ�)			*/

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
			lwk7.s[0] = lwk7.s[0] - lwk1.s[0];								/* Vuref + �I���f�B���C�␳��	*/
			lwk7.s[0] = limit( lwk7.s[0], AxisRscI->IntAdV.CarrierFreq );	/* 0�`CarrierFreq�Ń��~�b�g		*/
		}

		if( ( lwk8.s[0] != (SHORT)ZeroR ) && (lwk8.s[0] != AxisRscI->IntAdV.CarrierFreq ) )
		{
			lwk8.s[0] = lwk8.s[0] - lwk2.s[0];								/* Vvref + �I���f�B���C�␳��	*/
			lwk8.s[0] = limit( lwk8.s[0], AxisRscI->IntAdV.CarrierFreq );	/* 0�`CarrierFreq�Ń��~�b�g		*/
		}

		if( ( lwk9.s[0] != (SHORT)ZeroR ) && (lwk9.s[0] != AxisRscI->IntAdV.CarrierFreq ) )
		{
			lwk9.s[0] = lwk9.s[0] - lwk3.s[0];								/* Vwref + �I���f�B���C�␳��	*/
			lwk9.s[0] = limit( lwk9.s[0], AxisRscI->IntAdV.CarrierFreq );	/* 0�`CarrierFreq�Ń��~�b�g		*/
		}
		AxisRscI->PwmV.PwmCntT2 = lwk9.s[0];
		AxisRscI->PwmV.PwmCntT1 = lwk8.s[0];
		AxisRscI->PwmV.PwmCntT0 = lwk7.s[0];
	}


/****************************************************************************************************/
/*		Current Control Output Procedure															*/
/****************************************************************************************************/
#ifdef	MULTI_AXIS								/* ���������L��										*/
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
/*		���Ȋ��荞�݋֎~����																		*/
/*--------------------------------------------------------------------------------------------------*/
	INTLVWR = 0x0003;

	OUTPT = 0x0;
	IniWk.IN_WK1H++;

	return;
}


/****************************************************************************************************/
/*																									*/
/*		Encoder(SPG0) Interrupt Procedure		; �ʏ�(�����C���N���p���X�o�͊�����):11clk			*/
/*																									*/
/*		[����]�D�揇�ʂ��ō��ʂ̊��������Ȃ̂ŁA�ł��邾���Z�������ɂ��邱�ƁB						*/
/****************************************************************************************************/
void	MpIntEnc( void )
{
#if 0		/* JL086�Ŏ��s���邽�߃R�����g�A�E�g		*/
/*--------------------------------------------------------------------------------------------------*/
		if( EncIfV.IncPlsReq == 1 )
		{
			PCVS0 = EncIfV.DivPls.s[0];			/* �p���X�ϊ��ʒu�Z�b�g								*/
		}
		else if( EncIfV.PAOSeqCmd != PAOPLSOUT )
		{
			PCVS0 = (SHORT)IHostWk.IncInitPls;	/* �p���X�ϊ��ʒu�Z�b�g								*/
		}
/*--------------------------------------------------------------------------------------------------*/
		IEncWk.RxFlg0 = FCCST;		/* SDM status bit8 : IEncWk.RxFlg0(Serial-Enc0 receive flag)	*/
/*--------------------------------------------------------------------------------------------------*/
/*		�������ԒZ�k�̂��߁A�g�p���Ȃ��f�[�^�̓Ǎ��݂͂��Ȃ��B										*/
/*--------------------------------------------------------------------------------------------------*/
		IEncWk.RxPos.s[0] = SRPG0RD5;			/* ����l�Ǎ��݁FPosition Low						*/
		IEncWk.RxPos.s[1] = SRPG0RD6;			/* ����l�Ǎ��݁FPosition High						*/
/*--------------------------------------------------------------------------------------------------*/
		IEncWk.EncWk0 = INT1SET;				/* INT1 Acknowledge									*/
/*--------------------------------------------------------------------------------------------------*/
#endif	//#if 0		/* JL086�Ŏ��s���邽�߃R�����g�A�E�g		*/
		return;									/* return											*/
}



#if 0		/* JL086�Ŏ��s���邽�߃R�����g�A�E�g		*/
/****************************************************************************************************/
/*																									*/
/*		�����p���X�X�V����						;	�ő�:???clk, �ʏ�:???clk						*/
/*																									*/
/****************************************************************************************************/
void	MpUPDATE_DIVPOS( void )
{
/*--------------------------------------------------------------------------------------------------*/
		IHostWk.Divuswk = INT1SET;				/* INT1 Acknowledge									*/
/*--------------------------------------------------------------------------------------------------*/
		IHostWk.LastRcvPosX = EncIfV.RcvPosX0.l;			/* �O��ʒu�f�[�^�X�V					*/
/*--------------------------------------------------------------------------------------------------*/
/*		�V���A���G���R�[�_��M�`�F�b�N			; IEncWk.RxFlg0�̒l��@INT_ENC�����ɂčX�V			*/
/*--------------------------------------------------------------------------------------------------*/
//		Divuswk = IEncWk.RxFlg0;				/* SDMSTS bit8 : SPG0 Recieve Completed Check		*/
		if( (IEncWk.RxFlg0 & 0x100 ) == 0 )
		{
			if( EncIfV.SPGFail >= IHostWk.EncMstErrCnt )
			{
				EncIfV.RcvPosX2.l = EncIfV.RcvPosX1.l;						/* �O�X��ʒu�f�[�^											*/
				EncIfV.RcvPosX1.l = EncIfV.RcvPosX0.l;						/* �O��ʒu�f�[�^											*/
				EncIfV.RcvPosX0.l = EncIfV.RcvPosX0.l + EncIfV.RcvPosX1.l;	/* ��ԉ��Z													*/
				EncIfV.RcvPosX0.l = EncIfV.RcvPosX0.l - EncIfV.RcvPosX2.l;	/* EncIfV.RcvPosX0 += (EncIfV.RcvPosX1 - EncIfV.RcvPosX2)	*/
				IHostWk.EncMstErrCnt++;										/* IHostWk.EncMstErrCnt++									*/
			}
		}
/*--------------------------------------------------------------------------------------------------*/
		else
		{
			IHostWk.RxPos0 = IEncWk.RxPos.l;	/* ����l�X�V : IEncWk.RxPos�̒l��@INT_ENC�����ɂčX�V			*/
/*--------------------------------------------------------------------------------------------------*/
/*		�ʒu���Z																					*/
/*		IHostWk.RcvPosX = MencP.MposSign * ((MencV.RxPosL[0].sl>>MencP.MposSftX)<<MencP.MposSftR);	*/
/*																									*/
/*		32bit��ʋl�߃f�[�^�̂��߁A�_���V�t�g�ɂČv�Z(�����r�b�g�̉e���Ȃ�)							*/
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
/*		�����x���Z�`�F�b�N																			*/
/*--------------------------------------------------------------------------------------------------*/
			if( DivPlsV.AccCntClrReq != 0 )
			{
				IHostWk.Divuswk = ~EncIfV.BitData;			/* DivWk0=~EncIfV.BitData							*/
				IHostWk.Divuswk = IHostWk.Divuswk | ACCCHKENA;			/* DivWk0.ACCCHKENA = TRUE				*/
				EncIfV.BitData = ~IHostWk.Divuswk;			/* EncIfV.BitData=~DivWk0							*/
				IHostWk.AccChkCnt = 0;						/* IHostWk.AccChkCnt = 0							*/
				DivPlsV.AccCntClrReq = 0;					/* �����x�`�F�b�N�J�n�J�E���g�N���A�v�����Z�b�g		*/
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
						IHostWk.DivWk0 = IHostWk.DivWk0 & 0xfffffffe;			/* �Z�p�E�V�t�g�̎l�̌ܓ��������̑΍�				*/
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
					IHostWk.DivWk0 = IHostWk.DivWk0 & 0xfffffffe;				/* �Z�p�E�V�t�g�̎l�̌ܓ��������̑΍�				*/
					IHostWk.DivWk0 = IlibASR32(IHostWk.DivWk0 , 1);				/* DivWk0  = (IHostWk.RcvPosX  - EncIfV.RcvPosX1) >> 1	*/
					IHostWk.DivWk1 = EncIfV.RcvPosX1.l - EncIfV.RcvPosX2.l;		/* DivWk1  =  EncIfV.RcvPosX1 - EncIfV.RcvPosX2		*/
					IHostWk.MotAcc = IHostWk.DivWk0 - IHostWk.DivWk1;			/* IHostWk.MotAcc  =  DivWk0 - DivWk1				*/
				}
			}
			if( EncIfV.AccErrLv.l >= IHostWk.MotAcc )
			{
/*--------------------------------------------------------------------------------------------------*/
/*		�����x�ُ펞																				*/
/*--------------------------------------------------------------------------------------------------*/
				if( EncIfV.SPGFail < IHostWk.EncMstErrCnt )
				{
					EncIfV.RcvPosX2.l = EncIfV.RcvPosX1.l;	/* �O�X��ʒu�f�[�^						*/
					EncIfV.RcvPosX1.l = EncIfV.RcvPosX0.l;	/* �O��ʒu�f�[�^						*/
					EncIfV.RcvPosX0.l = IHostWk.RcvPosX;	/* �����x�ُ펞�͕�Ԃ��Ȃ�				*/
					IHostWk.EncMstErrCnt++;					/* IHostWk.EncMstErrCnt++				*/
				}
			}
			else if( ( EncIfV.AccErrLv.l + IHostWk.MotAcc ) < 0 )
			{
/*--------------------------------------------------------------------------------------------------*/
/*		�����x���펞																				*/
/*--------------------------------------------------------------------------------------------------*/
				IHostWk.EncMstErrCnt = 0;					/* IHostWk.EncMstErrCnt=0				*/
				EncIfV.RcvPosX2.l = EncIfV.RcvPosX1.l;		/* �O�X��ʒu�f�[�^						*/
				EncIfV.RcvPosX1.l = EncIfV.RcvPosX0.l;		/* �O��ʒu�f�[�^						*/
				EncIfV.RcvPosX0.l = IHostWk.RcvPosX;		/* ����ʒu�f�[�^						*/
			}
/*--------------------------------------------------------------------------------------------------*/
		}
/*--------------------------------------------------------------------------------------------------*/
/*		dMotPos = RMX_dPosOfXpos( MencV.MotPosX[0], LastMotPosX );									*/
/*--------------------------------------------------------------------------------------------------*/
/*		�Z�p�E�V�t�g�ɂĐ؂�̂Ă��鉺�ʃr�b�g��0�̂��߁A�l�̌ܓ��̉e���Ȃ��B						*/
/*--------------------------------------------------------------------------------------------------*/
		IHostWk.DMotPos = EncIfV.RcvPosX0.l - IHostWk.LastRcvPosX;		/* IHostWk.DMotPos = EncIfV.RcvPosX0 - IHostWk.LastRcvPosX	*/
		IHostWk.DMotPos = IlibASR32(IHostWk.DMotPos , EncIfV.MotPosSftR);
/*--------------------------------------------------------------------------------------------------*/
		if( EncIfV.IncPlsReq == 1 )
		{
			EncIfV.PlsOSetCmd = DivPlsV.PlsOSetCmdIn;		/* �p���X�o�͉�H�������v���X�V from HostCPU		*/
			if( EncIfV.PlsOSetCmd == POSETCMD00 )
			{
				PCVS0 = 0x0000;
				DivPlsV.PlsOSetCmdIn = POSETNOCMD;			/* �������v���N���A						*/
			}
			else if( EncIfV.PlsOSetCmd == POSETCMDFF )
			{
				PCVS0 = 0xFFFF;
				DivPlsV.PlsOSetCmdIn = POSETNOCMD;			/* �������v���N���A						*/
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
				PCVS1 = IHostWk.PoSet1W;		/* �p���X�ϊ����_�␳1�Z�b�g (HostCPU�Ɠ�����Ԃɐݒ�)			*/
				PCVS2 = IHostWk.PoSet2W;		/* �p���X�ϊ����_�␳2�Z�b�g									*/
			}
		}
		if( IHostWk.DivSetW != DivPlsV.DivSetIn )
		{
			IHostWk.DivSetW = DivPlsV.DivSetIn;
			DivSet = IHostWk.DivSetW;			/* �����@�\�Z�b�g (HostCPU�Ɠ�����Ԃɐݒ�)			*/
		}
		if( EncIfV.IncPlsReq != 1 )
		{
			if( EncIfV.AmpType != LINEAR )
			{
/*--------------------------------------------------------------------------------------------------*/
//		�����p���X = (MencV.MotPosX[0] >> MencP.EncIfV.DivOutSft);									*
/*--------------------------------------------------------------------------------------------------*/
//		�Z�p�E�V�t�g�ɂĐ؂�̂Ă��鉺�ʃr�b�g��0�ɂ���(�l�̌ܓ��������΍�)						*
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
		EncIfV.IncPlsReq = DivPlsV.IncPlsReqIn;				/* �����C���N���p���X�o�͗v���X�V from HostCPU		*/
		EncIfV.PAOSeqCmd = DivPlsV.PAOSeqCmdIn;

		return;												/* return								*/
}
#endif	//#if 0		/* JL086�Ŏ��s���邽�߃R�����g�A�E�g		*/

			

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

	uswk0 = sqrt( src );									/* ���ʂ͏����_�ȉ��͐؂�̂�			*/
	ulwk2 = mul( (SHORT)uswk0, (SHORT)uswk0 );				/* �������̌��ʂ�����					*/
	ulwk2 = src - ulwk2;									/* ���͂Ǝ���̍������(�؎̂Č덷)		*/
	ulwk0 = (ULONG)uswk0;
	if( uswk0 < 0xffff )
	{ /* �ő�l�𒴂��Ȃ��ꍇ */
		if( ulwk0 < ulwk2 )
		{ /* �؎̂Č덷���������̌��ʂ��傫���ꍇ */
			/* �␳���� */
			uswk0 = uswk0 + 1;
		}
	}
	else
	{
		/* �ő�l�𒴂���ꍇ�͐؎̂Ă̕␳�Ȃ�	*/
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

	SHORT	swk0;											/* 16bit���[�N���W�X�^0					*/
	SHORT	swk1;											/* 16bit���[�N���W�X�^1					*/
	SHORT	swk2;											/* 16bit���[�N���W�X�^2					*/
	SHORT	swk3;											/* 16bit���[�N���W�X�^3					*/
	SHORT	swk4;											/* 16bit���[�N���W�X�^4					*/

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
			swk0 = (USHORT)swk0 >> 5;						/* high(�_���V�t�g)						*/
			swk1 = swk1 & 0x1F;								/* low									*/
			if( swk0 >= 32 )
			{	/* �ϒ���1.27�ȏ�ꍇ */
				pCtbl = pCtbl + 30;
				IxLoadMpmem16( swk4, pCtbl, 1 );
			}
			else
			{	/* �ϒ���1.27�ȉ��ꍇ */
				swk2 = swk0;
				if( ( swk2 & 1 ) == 0 )
				{
					/* �e�[�u��2�_���o�� */
					pCtbl = pCtbl + swk0;
 					IxLoadMpmem16( swk2, pCtbl, 0 );
					IxLoadMpmem16( swk3, pCtbl, 1 );
				}
				else
				{
					/* �e�[�u��2�_���o�� */
					pCtbl = pCtbl + swk0;
					IxLoadMpmem16( swk2, pCtbl, 1 );
					pCtbl = pCtbl + 2;
					IxLoadMpmem16( swk3, pCtbl, 0 );
				}
				/* ��ԏ��� */
				swk0 = swk3 - swk2;
				swk0 = mulshr(swk0, swk1, 5);
				swk4 = swk0 + swk2;
			}
		}
		else
		{	/* �ϒ���1.27�ȏ�ꍇ */
			pCtbl = pCtbl + 30;
			IxLoadMpmem16( swk4, pCtbl, 1 );
		}
	}
	return	swk4;
}


#if 0
/****************************************************************************************************/
/*																									*/
/*		���䉉�Z���C�u����																			*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		�]��t���ʒu�e�a�v�Z : rv = (kx*u+pfbrem)>>sx 	; ??clk										*/
/*																									*/
/****************************************************************************************************/
//LONG	MpMlibPfbkxremNolim(
/*		LONG u,									/* DivPlsV.Argu0    : ����							*/
/*		LONG k,									/* DivPlsV.Argu1    : �Q�C��						*/
/*		LONG *pfbrem )							/* DivPlsV.Iu0    : �]��ւ̃|�C���^				*/
/*--------------------------------------------------------------------------------------------------*/
/*												/* DivPlsV.Ret0     : �߂�l						*/
/*--------------------------------------------------------------------------------------------------*/
/*		LONG	kx								/* DivPlsV.Kx     : kx								*/
/*		LONG	sx								/* DivPlsV.Sx     : sx								*/
/*		LONG	rv								/* lswk10  : ���Z����								*/
/*		LONG	pfbrem							/* lswk11  : �]��									*/
/*		LONG	wk1								/* lswk1	  : ��Ɨp								*/
/*		LONG	wk2								/* lswk2   : ��Ɨp									*/
/*												/* lswk3   : ��Z���ʕێ��p(����32bit)				*/
/*												/* lswk4   : ��Z���ʕێ��p(���32bit)				*/
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
/*			 �Z�p�E�V�t�g�ɂĐ؂�̂Ă��鉺�ʃr�b�g��0�ɂ���(�l�̌ܓ��������΍�)					*/
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
				;										/* �����Ȃ�									*/
			}
		}
/*--------------------------------------------------------------------------------------------------*/
		return;
}


/****************************************************************************************************/
/*																									*/
/*		IxNxfilter2 : 2���m�b�`�t�B���^																*/
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
/*		���͉��Z																					*/
/*--------------------------------------------------------------------------------------------------*/
		dlwk0.dl = mul( u, wk[0].l );						/* dlwk0.dl = u * wk[0]					*/
		wk[2].l = asr( dlwk0.l[1], wk[1].s[0] );			/* wk[2] = dlwk0.l[1] >> wk[1] (�Z�p)	*/
		
/*--------------------------------------------------------------------------------------------------*/
/*		�ϕ����Z																					*/
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
/*		�ϕ��l���~�b�g																				*/
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
/*		�߂�l�v�Z(�l�̌ܓ�����)																	*/
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

	wk[3].l = (wk[0].l >> 31) & (wk[2].l >> 31);				/* �L�����[							*/
	x->l[1] = x->l[1] + (wk[3].l & (LONG)OneR);
}


/****************************************************************************************************/
/*																									*/
/*		Initialize Soft BB & INT1L Enable															*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		���l�F																						*/
/*		JL-086A�s�����̂��߁A5��A���������ݎ��{												*/
/*																									*/
/****************************************************************************************************/
inline	void	InitSbb( SHORT BbSet )						/* soft BB & INT1L�ݒ菉��������		*/
{
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
	BBSET = BbSet;
#ifdef	MULTI_AXIS							/* ���������L��											*/
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
	BBSET_2 = BbSet;
#endif /* MULTI_AXIS */						/* ���������L��											*/
}

/****************************************************************************************************/
/*																									*/
/*		PWM������																					*/
/*																									*/
/****************************************************************************************************/
inline void InitPWM( MICRO_AXIS_HANDLE *AxisRsc )
{
	USHORT	uswk;

	/* PWM�o�͑I��ݒ� */
	PWMOS = 0x0A0;							/* PWM�o�͑I��		: �L�����A�J�E���^��r�o��			*/
											/* ����r�I��		: ��r�l����PU1�ANU1�M���쐬		*/
											/* �����[�h���[�h	: �}�C�N���������ݎ��ɑ����[�h		*/
											/* ���g�����[�h�I��	: �L�����A�J�Ŏ��g�����[�h			*/
											/* �m�R�M���g�I��	: �O�p�g							*/

	/* �L�����A���g���擾 & �ݒ� */
	AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn;
	CRFRQ = AxisRsc->IntAdV.CarrierFreq;				/* �L�����A���g�����ݒn�ݒ�					*/

	/* �L�����A�J�E���^��r�l�ݒ� */
	uswk = ((USHORT)AxisRsc->IntAdV.CarrierFreq >> 1);	/* IntAdV.CrFreqW /2(50p duty)				*/
	AxisRsc->PwmV.PwmCntT2 = uswk;
	AxisRsc->PwmV.PwmCntT1 = uswk;
	AxisRsc->PwmV.PwmCntT0 = uswk;
	PwmT2 = AxisRsc->PwmV.PwmCntT2;
	PwmT1 = AxisRsc->PwmV.PwmCntT1;
	PwmT0 = AxisRsc->PwmV.PwmCntT0;

#ifdef	MULTI_AXIS							/* ���������L��											*/
	AxisRsc++;
	PWMOS_2 = 0x0A0;						/* PWM�o�͑I��		: �L�����A�J�E���^��r�o��			*/
											/* ����r�I��		: ��r�l����PU1�ANU1�M���쐬		*/
											/* �����[�h���[�h	: �}�C�N���������ݎ��ɑ����[�h		*/
											/* ���g�����[�h�I��	: �L�����A�J�Ŏ��g�����[�h			*/
											/* �m�R�M���g�I��	: �O�p�g							*/

	/* �L�����A���g���擾 & �ݒ� */
	AxisRsc->IntAdV.CarrierFreq = AxisRsc->MicroIf.CarrierFreqIn;
	CRFRQ_2 = AxisRsc->IntAdV.CarrierFreq;					/* �L�����A���g�����ݒn�ݒ�					*/

	/* �L�����A�J�E���^��r�l�ݒ� */
	uswk = ((USHORT)AxisRsc->IntAdV.CarrierFreq >> 1);	/* IntAdV.CrFreqW /2(50p duty)				*/
	AxisRsc->PwmV.PwmCntT2 = uswk;
	AxisRsc->PwmV.PwmCntT1 = uswk;
	AxisRsc->PwmV.PwmCntT0 = uswk;
	PwmT2_2 = AxisRsc->PwmV.PwmCntT2;
	PwmT1_2 = AxisRsc->PwmV.PwmCntT1;
	PwmT0_2 = AxisRsc->PwmV.PwmCntT0;
#endif /* MULTI_AXIS */						/* ���������L��											*/
}


/****************************************************************************************************/
/*																									*/
/*		PWM�o�͊J�n																					*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*		���l�F																						*/
/*		JL-086A�s�����̂��߁A5��A���������ݎ��{												*/
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
/*		PWM�o��																						*/
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
/*		�L�����A���g���ύX(�e��)																	*/
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
		; //�����Ȃ�
	}
}


/****************************************************************************************************/
/*																									*/
/*		�L�����A���g���ύX(�S��)																	*/
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
/*		�f�V���[�V�����t�B���^�G���[�N���A															*/
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
		; //�����Ȃ�
	}
}


/****************************************************************************************************/
/*																									*/
/*		�f�V���[�V�����t�B���^�G���[�N���A															*/
/*																									*/
/****************************************************************************************************/
inline	void	CurAdSyncRequest( void )					/* �d��AD�T�C�N�������v��				*/
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

	/* swk = IuAD >> 2(�Z�p) */
	swk = mulshr( IuAD, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IuOffset) * IuGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IuOffset), AxisRsc->CurDet.IuGain );
	/* lwk0.l = (swk + IuOffset) * IuGain * KnorCurrent / 2^14 (�Z�p) */
	AxisRsc->CurDet.IuDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IuDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (�Z�p) */
	AxisRsc->CurDet.IuDetect = mulshr( AxisRsc->CurDet.IuDetTmp, (LONG)No3, 1 );
	/*------------------------------------------------------------------------------*/
	/* swk = IvAD >> 2(�Z�p) */
	swk = mulshr( IvAD, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IvOffset) * IvGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IvOffset), AxisRsc->CurDet.IvGain );
	/* lwk0.l = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 (�Z�p�V�t�g) */
	AxisRsc->CurDet.IvDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IvDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (�Z�p) */
	AxisRsc->CurDet.IvDetect = mulshr( AxisRsc->CurDet.IvDetTmp, (LONG)No3, 1 );

#ifdef	MULTI_AXIS

	AxisRsc++;
	/* swk = IuAD >> 2(�Z�p) */
	swk = mulshr( IuAD_2, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IuOffset) * IuGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IuOffset), AxisRsc->CurDet.IuGain );
	/* lwk0.l = (swk + IuOffset) * IuGain * KnorCurrent / 2^14 (�Z�p) */
	AxisRsc->CurDet.IuDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IuDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (�Z�p) */
	AxisRsc->CurDet.IuDetect = mulshr( AxisRsc->CurDet.IuDetTmp, (LONG)No3, 1 );
	/*------------------------------------------------------------------------------*/
	/* swk = IvAD >> 2(�Z�p) */
	swk = mulshr( IvAD_2, (SHORT)OneR, 2 );
	/* lwk0.l = (swk + IvOffset) * IvGain */
	lwk0.l = mul( (swk + AxisRsc->CurDet.IvOffset), AxisRsc->CurDet.IvGain );
	/* lwk0.l = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 (�Z�p�V�t�g) */
	AxisRsc->CurDet.IvDetTmp = mulshr( lwk0.l, AxisRsc->CurDet.KnorCurrent, 14 );
	/* IvDetect = (swk + IvOffset) * IvGain * KnorCurrent / 2^14 * 3 / 2 (�Z�p) */
	AxisRsc->CurDet.IvDetect = mulshr( AxisRsc->CurDet.IvDetTmp, (LONG)No3, 1 );

#endif
}


/****************************************************************************************************/
/*																									*/
/*		SVIP�ُ��Ԏ擾																			*/
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

