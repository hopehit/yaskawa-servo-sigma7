/****************************************************************************************************/
/*																									*/
/*																									*/
/*		MprgSgdv.inc : �r�f�c�u�}�C�N���v���O����													*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/************** Copyright (C) Yaskawa Electric Corporation ******************************************/
/*																									*/
/*		Rev.0.00 : 2012.08.06  Y.Tanaka		�EJL-086�����d������C���ꉻ�p�Ƀo�[�W������蒼��		*/
/*		Rev.0.01 : 2012.08.17  Y.Tanaka		�E�\���́A���[�J���ϐ�������							*/
/*		Rev.0.02 : 2012.08.20  Y.Tanaka		�E�\���́A���[�J���ϐ�������							*/
/*																									*/
/****************************************************************************************************/


/****************************************************************************************************/
/*																									*/
/*		�}�C�N�����Z���\�t�g�o�[�W������`													<V720>	*/
/*																									*/
/****************************************************************************************************/
#pragma AsicReg begin
#define	MSW_VER		0x0002				/* ADR:0x0002: �\�t�g�o�[�W����(���i�p)								*/
#define	TST_VER		0x0000				/* ADR:0x0000: �e�X�g�o�[�W����(�]���p)								*/
#define	YSP_VER		0x0000				/* ADR:0x0000: �x�d�l�o�[�W����(���i�p)								*/

/****************************************************************************************************/
/*																									*/
/*		Function Prototype																			*/
/*																									*/
/****************************************************************************************************/
#ifdef ASIP_CC										/* JL-086�R���p�C���p							*/
void	main( void );								/* �X�^�[�g�A�b�v								*/
#elif defined(WIN32)								/* VC�V�~�����[�V��������p						*/
void	MpStart( void );							/* �X�^�[�g�A�b�v								*/
#endif
void	MpIntHost( void );							/* �z�X�g�����ݏ���								*/
void	MpIntAD( void );							/* �d�����䊄����								*/
void	MpIntEnc( void );							/* �G���R�[�_������								*/
void	MpUPDATE_DIVPOS( void );					/* �����ʒu�X�V									*/
void	MpDataClear( void );						/* �}�C�N���p�f�[�^�N���A						*/
USHORT	MpSQRT( ULONG src );						/* ���������Z����(����)							*/
void	MpOVMMODK( void );							/* �I�[�o���W���[�������H						*/
void	MpMlibPfbkxremNolim( void );				/* �]��t��FB���Z����							*/


/****************************************************************************************************/
/*																									*/
/*		H/W Register																				*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		READ register																				*/
/*--------------------------------------------------------------------------------------------------*/
/*���[�h���W�X�^�ǉ�(�b��)�A��`���͕ʓr�Ή�<6.06>		��������	*/
#define	MRD0 *(VUSHORT*)0x700		/* M-port�X���[�u��M�f�[�^�@���[�h0(0,1�o�C�g)					*/
#define	MRD4 *(VUSHORT*)0x704		/* M-port�X���[�u��M�f�[�^�@���[�h4(8,9�o�C�g)					*/
#define	MRD5 *(VUSHORT*)0x705		/* M-port�X���[�u��M�f�[�^�@���[�h5(10,11�o�C�g)				*/
/*--------------------------------------------------------------------------------------------------*/
#define	MRD47 *(VUSHORT*)0x72F		/* M-port�X���[�u��M�f�[�^�@���[�h47(94,95�o�C�g)				*/
/*--------------------------------------------------------------------------------------------------*/
#define	SFT1R0 *(VUSHORT*)0x780		/* RC���߃V�t�g�P�ێ����W�X�^0(bit0-4)							*/
#define	SFT1R1 *(VUSHORT*)0x781		/* RC���߃V�t�g�P�ێ����W�X�^1(bit0-4)							*/
#define	SFT1R2 *(VUSHORT*)0x782		/* RC���߃V�t�g�P�ێ����W�X�^2(bit0-4)							*/
#define	SFT1R3 *(VUSHORT*)0x783		/* RC���߃V�t�g�P�ێ����W�X�^3(bit0-4)							*/
#define	SFT1R4 *(VUSHORT*)0x784		/* RC���߃V�t�g�P�ێ����W�X�^4(bit0-4)							*/
#define	SFT1R5 *(VUSHORT*)0x785		/* RC���߃V�t�g�P�ێ����W�X�^5(bit0-4)							*/
#define	SFT1R6 *(VUSHORT*)0x786		/* RC���߃V�t�g�P�ێ����W�X�^6(bit0-4)							*/
#define	SFT1R7 *(VUSHORT*)0x787		/* RC���߃V�t�g�P�ێ����W�X�^7(bit0-4)  Round��p				*/
#define	SFT1R8 *(VUSHORT*)0x788		/* RC���߃V�t�g�P�ێ����W�X�^8(bit0-4)  Round��p				*/
#define	SFT1R9 *(VUSHORT*)0x789		/* RC���߃V�t�g�P�ێ����W�X�^9(bit0-4)							*/
#define	SFT1R10 *(VUSHORT*)0x78A	/* RC���߃V�t�g�P�ێ����W�X�^10(bit0-4)							*/
#define	SFT1R11 *(VUSHORT*)0x78B	/* RC���߃V�t�g�P�ێ����W�X�^11(bit0-4)							*/
#define	SFT1R12 *(VUSHORT*)0x78C	/* RC���߃V�t�g�P�ێ����W�X�^12(bit0-4)							*/
#define	SFT1R13 *(VUSHORT*)0x78D	/* RC���߃V�t�g�P�ێ����W�X�^13(bit0-4)							*/
#define	SFT1R14 *(VUSHORT*)0x78E	/* RC���߃V�t�g�P�ێ����W�X�^14(bit0-4)							*/
#define	SFT1R15 *(VUSHORT*)0x78F	/* RC���߃V�t�g�P�ێ����W�X�^15(bit0-4)							*/
/*--------------------------------------------------------------------------------------------------*/
#define	SFT2R0 *(VUSHORT*)0x790		/* RC���߃V�t�g�Q�ێ����W�X�^0(bit0-5)							*/
#define	SFT2R1 *(VUSHORT*)0x791		/* RC���߃V�t�g�Q�ێ����W�X�^1(bit0-5)							*/
#define	SFT2R2 *(VUSHORT*)0x792		/* RC���߃V�t�g�Q�ێ����W�X�^2(bit0-5)							*/
#define	SFT2R3 *(VUSHORT*)0x793		/* RC���߃V�t�g�Q�ێ����W�X�^3(bit0-5)							*/
#define	SFT2R4 *(VUSHORT*)0x794		/* RC���߃V�t�g�Q�ێ����W�X�^4(bit0-5)							*/
#define	SFT2R5 *(VUSHORT*)0x795		/* RC���߃V�t�g�Q�ێ����W�X�^5(bit0-5)							*/
#define	SFT2R6 *(VUSHORT*)0x796		/* RC���߃V�t�g�Q�ێ����W�X�^6(bit0-5)							*/
#define	SFT2R7 *(VUSHORT*)0x797		/* RC���߃V�t�g�Q�ێ����W�X�^7(bit0-5)  Round��p				*/
#define	SFT2R8 *(VUSHORT*)0x798		/* RC���߃V�t�g�Q�ێ����W�X�^8(bit0-5)  Round��p				*/
#define	SFT2R9 *(VUSHORT*)0x799		/* RC���߃V�t�g�Q�ێ����W�X�^9(bit0-5)							*/
#define	SFT2R10 *(VUSHORT*)0x79A	/* RC���߃V�t�g�Q�ێ����W�X�^10(bit0-5)							*/
#define	SFT2R11 *(VUSHORT*)0x79B	/* RC���߃V�t�g�Q�ێ����W�X�^11(bit0-5)							*/
#define	SFT2R12 *(VUSHORT*)0x79C	/* RC���߃V�t�g�Q�ێ����W�X�^12(bit0-5)							*/
#define	SFT2R13 *(VUSHORT*)0x79D	/* RC���߃V�t�g�Q�ێ����W�X�^13(bit0-5)							*/
#define	SFT2R14 *(VUSHORT*)0x79E	/* RC���߃V�t�g�Q�ێ����W�X�^14(bit0-5)							*/
#define	SFT2R15 *(VUSHORT*)0x79F	/* RC���߃V�t�g�Q�ێ����W�X�^15(bit0-5)							*/
/*--------------------------------------------------------------------------------------------------*/
#define	NCTCTM *(VUSHORT*)0x7AC		/* �w�ߗpNCT�^�C�}�J�E���^�l									*/
#define	NCTCFB *(VUSHORT*)0x7AD		/* �w�ߗpNCT�t�B�[�h�o�b�N�J�E���^�l							*/
#define	NCTCCD *(VUSHORT*)0x7AE		/* �w�ߗpNCT CD�E���^�l											*/
#define	NCTCTP *(VUSHORT*)0x7AF		/* �w�ߗpNCT TP�E���^�l											*/
/*--------------------------------------------------------------------------------------------------*/
#define	IREG_S *(VUSHORT*)0x7B0		/* �Ԑړ]���\�[�X�A�h���X�ێ����W�X�^�ݒ�						*/
#define	IREG_D *(VUSHORT*)0x7B1		/* �Ԑړ]������A�h���X�ێ����W�X�^�ݒ�							*/
#define	DBSTS *(VUSHORT*)0x7B4		/* �f�b�h�o���h�J�E���^�I�[�o�^�A���_�t���[							*/
#define	DBU1 *(VUSHORT*)0x7B5		/* PU1V�f�b�h�o���h�⏞�J�E���^���b�`�l							*/
#define	DBV1 *(VUSHORT*)0x7B6		/* PV1V�f�b�h�o���h�⏞�J�E���^���b�`�l							*/
#define	DBW1 *(VUSHORT*)0x7B7		/* PW1V�f�b�h�o���h�⏞�J�E���^���b�`�l							*/
#define	ACMC4 *(VUSHORT*)0x7B8		/* �w�߃}���`�v���N�X�`�����l��4								*/
#define	ACMC5 *(VUSHORT*)0x7B9		/* �w�߃}���`�v���N�X�`�����l��5								*/
#define	ACMC6 *(VUSHORT*)0x7BA		/* �w�߃}���`�v���N�X�`�����l��6								*/
#define	ACMC7 *(VUSHORT*)0x7BB		/* �w�߃}���`�v���N�X�`�����l��7								*/
#define	FCCFZRNL *(VUSHORT*)0x7BE	/* �t���N���[�Y�h�J�E���^FZRN���b�`�f�[�^							*/
#define	FCLDL *(VUSHORT*)0x7BF		/* �t���N���[�Y�h�J�E���^���[�h���b�`�f�[�^							*/
/*--------------------------------------------------------------------------------------------------*/
//#define	UPDO *(VUSHORT*)0x7C0		/* �g���h�n�o�̓f�[�^											*/
#define	UPDI *(VUSHORT*)0x7C1		/* �g���h�n���̓f�[�^											*/
#define	SADAT_A *(VUSHORT*)0x7C2	/* �G���R�[�_�`�c�f�[�^A										*/
#define	SADAT_AA *(VUSHORT*)0x7C3	/* �G���R�[�_�`�c�f�[�^AA										*/
#define	SADAT_B *(VUSHORT*)0x7C4	/* �G���R�[�_�`�c�f�[�^B										*/
#define	SADAT_BB *(VUSHORT*)0x7C5	/* �G���R�[�_�`�c�f�[�^BB										*/
#define	FBZRND *(VUSHORT*)0x7C8		/* �t�B�[�h�o�b�N�J�E���^ZRN���b�`�f�[�^						*/
#define	FBCLT *(VUSHORT*)0x7C9		/* �t�B�[�h�o�b�N�J�E���^���[�h���b�`�f�[�^						*/
#define	FBPUD *(VUSHORT*)0x7CA		/* �t�B�[�h�o�b�N�J�E���^PU���b�`�f�[�^							*/
#define	FBPCD *(VUSHORT*)0x7CB		/* �t�B�[�h�o�b�N�J�E���^PC���b�`�f�[�^							*/
#define	TCNT *(VUSHORT*)0x7CC		/* �t�B�[�h�o�b�N�^�C�}���ݒl									*/
#define	T1LT *(VUSHORT*)0x7CD		/* �t�B�[�h�o�b�N�^�C�}�`�^�a�p���X���b�`�l						*/
#define	T2LT *(VUSHORT*)0x7CE		/* �t�B�[�h�o�b�N�L�����A���b�`�l								*/
#define	T3LT *(VUSHORT*)0x7CF		/* �t�B�[�h�o�b�N�^�C�}�s�P���O�l���b�`�l						*/
/*���[�h���W�X�^�ǉ�(�b��)�A��`���͕ʓr�Ή�<6.06>		�����܂�	*/
/*--------------------------------------------------------------------------------------------------*/
#define	IuAD *(VUSHORT*)0x07d0					/* ADR:0x07d0: AD data U							*/
#define	IvAD *(VUSHORT*)0x07d1					/* ADR:0x07d1: AD data V							*/
//   	--;						/* ADR:0x07d2: 														*/
/*--------------------------------------------------------------------------------------------------*/
#define	IFLAG *(VUSHORT*)0x07d3					/* ADR:0x07d3: Flag status											*/
								//  bit.0 : Z(zero)
								//  bit.1 : S(sign'-')
								//  bit.2 : C(carry)
								//  bit.3 : B(bollow)
								//  bit.4 : LB(limit)
								//  bit.5 : ZD(zero div)
								//  bit.6 : DOVF(over-flow div)
								//  bit.8 : ILS0(interrupt0)
								//  bit.9 : ILS1(interrupt1)
								//  bit.A : ILS2(interrupt2)
/*--------------------------------------------------------------------------------------------------*/
#define	MACCL ((DWREG*)(VUSHORT*)0x07d4)->l					/* ADR:0x07d4: MPLY 0-31 bit										*/
#define	MACCLL ((DWREG*)(VUSHORT*)0x07d4)->s[0]				/* ADR:0x07d4: MPLY 0-15 bit										*/
#define	MACCLH ((DWREG*)(VUSHORT*)0x07d4)->s[1]				/* ADR:0x07d4: MPLY 16-31 bit										*/
	/*----------------------------------------------------------------------------------------------*/
#define	MACCH ((DWREG*)(VUSHORT*)0x07d6)->l					/* ADR:0x07d6: MPLY 32-63 bit										*/
#define	MACCHL ((DWREG*)(VUSHORT*)0x07d6)->s[0]				/* ADR:0x07d6: MPLY 32-47 bit										*/
#define	MACCHH ((DWREG*)(VUSHORT*)0x07d6)->s[1]				/* ADR:0x07d6: MPLY 48-63 bit										*/
	/*----------------------------------------------------------------------------------------------*/
#define	DivRem *(VUSHORT*)0x07d8					/* ADR:0x07d8: Divide modulo										*/
#define	HSUR0 *(VUSHORT*)0x07d9					/* ADR:0x07d9: �z�X�g�w�߃|�[�g�O									*/
#define	HSUR1 *(VUSHORT*)0x07da					/* ADR:0x07da: �z�X�g�w�߃|�[�g�P									*/
#define	HSUR2 *(VUSHORT*)0x07db					/* ADR:0x07db: �z�X�g�w�߃|�[�g�Q									*/
#define	HSUR3 *(VUSHORT*)0x07dc					/* ADR:0x07dc: �z�X�g�w�߃|�[�g�R									*/
/*--------------------------------------------------------------------------------------------------*/
#define	CTSTR *(VUSHORT*)0x07dd					/* ADR:0x07dd: controll status										*/
								//  bit.0 : '1'�Œ�
								//  bit.1 : CTRD1
								//  bit.2 : CTRD2
								//  bit.3 : CTRD3(RLOCK)
								//  bit.4 : CTRD4(BB)
								//  bit.5 : CTRD5
								//  bit.6 : CTRD6
								//  bit.7 : CTRD7
/*--------------------------------------------------------------------------------------------------*/
#define	INT1SET *(VUSHORT*)0x07de				/* ADR:0x07de: INT1 status read clr									*/
								//  bit.0 : EADINTAC
								//  bit.1 : MPINTAC
								//  bit.2 : CNVSTAC
								//  bit.3 : INT0AC
								//  bit.4 : CMDINTAC
								//  bit.5 : SPG0INTAC
								//  bit.6 : SPG1INTAC
								//  bit.7 : URTINTAC
/*--------------------------------------------------------------------------------------------------*/
#define	FLTSTAT *(VUSHORT*)0x07df					/* ADR:0x07df: fault status(FLTSTAT)								*/
								//  bit.0 : HBLK0(Sync-Serial bit0)
								//  bit.1 : HBLK1(Sync-Serial bit1)
								//  bit.2 : HBLK2(Sync-Serial bit2)
								//  bit.3 : '0'�Œ�
								//  bit.4 : OVDAT(OV)
								//  bit.5 : OCDAT(OC)
								//  bit.6 : OVLG(Degital filter fault:0)
								//  bit.7 : DBON1LT
								//  bit.8 : DBON2LT
								//  bit.A : BB(Base-Block)
								//  bit.B : GUP2(Give-up host:0)
								//  bit.C : OVLGCH1(fault:0)
								//  bit.D : OVLGCH2(fault:0)
								//  bit.E : OVLGCH3(fault:0)
								//  bit.F : CRUP(carrier counter status:up=1)
/*--------------------------------------------------------------------------------------------------*/
#define	FBCSET *(VUSHORT*)0x07e0					/* ADR:0x07e0: counter status										*/
								//  bit.0:PZD(test)
								//  bit.1 : DADAT(division out A)
								//  bit.2 : DBDAT(division out B)
								//  bit.3 : DCDAT(division out C)
								//  bit.4 : PADAT(PA)
								//  bit.5 : PBDAT(PB)
								//  bit.6 : PCDAT(PC)
								//  bit.7 : ZRE
								//  bit.8 : CADAT(CA input)
								//  bit.9 : CBDAT(CB input)
								//  bit.A : CCDAT(CC input)
								//  bit.B : CLE(CLM latch end)
								//  bit.C : PUI(U)
								//  bit.D : PVI(V)
								//  bit.E : PWI(W)
								//  bit.F : UE
/*--------------------------------------------------------------------------------------------------*/
#define	FCCST *(VUSHORT*)0x07e1					/* ADR:0x07e1: SDM status											*/
								//  bit.0 : SDM1TER(decimation1 over)
								//  bit.1 : SDM2TER(decimation2 over)
								//  bit.2 : SDM3TER(decimation3 over)
								//  bit.3 : SDM1TER(decimation1 mclk miss)
								//  bit.4 : SDM2TER(decimation2 mclk miss)
								//  bit.5 : SDM3TER(decimation3 mclk miss)
								//  bit.7 : Encoder AD Latch Complete flag)
								//  bit.8 : RXFLG0(Serial-Enc0 receive flag)
								//  bit.9 : RXFLG1(Serial-Enc1 receive flag)
/*--------------------------------------------------------------------------------------------------*/
#define	CRUDP *(VUSHORT*)0x07e2					/* ADR:0x07e2: Y-Tcounter 											*/
								//  bit.0-2 : for generic A/D
								//  bit.3   : '1'�Œ�
								//  bit.4-6 : for decimation1
								//  bit.8-A : for decimation2
/*--------------------------------------------------------------------------------------------------*/
//   	--;						/* ADR:0x07e3: 														*/
#define	OPCCLM *(VUSHORT*)0x07e4					/* ADR:0x07e4: OP counter CLM latch									*/
//   	--;						/* ADR:0x07e5: 														*/
//   	--;						/* ADR:0x07e6: 														*/
#define	OPCLT *(VUSHORT*)0x07e7					/* ADR:0x07e7: OP counter load latch								*/
//   	--;						/* ADR:0x07e8: 														*/
//   	--;						/* ADR:0x07e9: 														*/
#define	SRPG1RD2 *(VUSHORT*)0x07ea				/* ADR:0x07ea: Serial-PG1 receive word2								*/
#define	SRPG1RD3 *(VUSHORT*)0x07eb				/* ADR:0x07eb: Serial-PG1 receive word3								*/
#define	SRPG1RD4 *(VUSHORT*)0x07ec				/* ADR:0x07ec: Serial-PG1 receive word4								*/
#define	SRPG1RD5 *(VUSHORT*)0x07ed				/* ADR:0x07ed: Serial-PG1 receive word5								*/
#define	SRPG1RD6 *(VUSHORT*)0x07ee				/* ADR:0x07ee: Serial-PG1 receive word6								*/
#define	SRPG1RD7 *(VUSHORT*)0x07ef				/* ADR:0x07ef: Serial-PG1 receive word7								*/
/*--------------------------------------------------------------------------------------------------*/
#define	SFTMACCL ((DWREG*)(VUSHORT*)0x07f0)->l					/* ADR:0x07f0: BRL shifter 0-31 bit										*/
#define	SFTMACCLL ((DWREG*)(VUSHORT*)0x07f0)->s[0]				/* ADR:0x07f0: BRL shifter 0-15 bit										*/
#define	SFTMACCLH ((DWREG*)(VUSHORT*)0x07f0)->s[1]				/* ADR:0x07f0: BRL shifter 16-31 bit										*/
	/*----------------------------------------------------------------------------------------------*/
#define	SFTMACCH ((DWREG*)(VUSHORT*)0x07f2)->l					/* ADR:0x07f2: BRL shifter 0-31 bit										*/
#define	SFTMACCHL ((DWREG*)(VUSHORT*)0x07f2)->s[0]				/* ADR:0x07f2: BRL shifter 0-15 bit										*/
#define	SFTMACCHH ((DWREG*)(VUSHORT*)0x07f2)->s[1]				/* ADR:0x07f2: BRL shifter 16-31 bit										*/
	/*----------------------------------------------------------------------------------------------*/
#define	CAD0D *(VUSHORT*)0x07f4					/* ADR:0x00f4: Operation AD0 data									*/
#define	CAD1D *(VUSHORT*)0x07f5					/* ADR:0x00f5: Operation AD1 data Low								*/
#define	CAD1H *(VUSHORT*)0x07f6					/* ADR:0x00f6: Operation AD1 data high								*/
//   	--;						/* ADR:0x00f7: 														*/
/*--------------------------------------------------------------------------------------------------*/
#define	ILR *(VUSHORT*)0x07f8					/* ADR:0x07f8: interrupt level read									*/
								//  bit.0-2 : interrupt0
								//  bit.4-6 : interrupt1
								//  bit.8-A : interrupt2
								//  bit.C-E : interrupt3
/*--------------------------------------------------------------------------------------------------*/
//   	--;						/* ADR:0x07f9: 														*/
#define	SRPG0RD2 *(VUSHORT*)0x07fa				/* ADR:0x07fa: serial PG0 Rx word 2									*/
#define	SRPG0RD3 *(VUSHORT*)0x07fb				/* ADR:0x07fb: serial PG0 Rx word 3									*/
#define	SRPG0RD4 *(VUSHORT*)0x07fc				/* ADR:0x07fc: serial PG0 Rx word 4									*/
#define	SRPG0RD5 *(VUSHORT*)0x07fd				/* ADR:0x07fd: serial PG0 Rx word 5									*/
#define	SRPG0RD6 *(VUSHORT*)0x07fe				/* ADR:0x07fe: serial PG0 Rx word 6									*/
#define	SRPG0RD7 *(VUSHORT*)0x07ff				/* ADR:0x07ff: serial PG0 Rx word 7									*/
//;
/*--------------------------------------------------------------------------------------------------*/
/*		WRITE register																				*/
/*--------------------------------------------------------------------------------------------------*/
#define	UPDO *(VUSHORT*)0x07c0					/* ADR:0x07c0: �g��IO for debug											<V720>	*/
/*--------------------------------------------------------------------------------------------------*/
#define	OUTPT *(VUSHORT*)0x07d0					/* ADR:0x07d0: INT2 port											*/
#define	WDT1L *(VUSHORT*)0x07d1					/* ADR:0x07d1: WDT trigger port(WDT1L)								*/
/*--------------------------------------------------------------------------------------------------*/
#define	BBSET *(VUSHORT*)0x07d2					/* ADR:0x07d2: soft BB set											*/
								//  bit.2 : soft BB
								//  bit.8 : EADINTEN(�G���R�[�_AD�ϊ����������݃C�l�[�u��)
								//  bit.9 : MPINTEN(M-Port�����݃C�l�[�u��)
								//  bit.A : CNVSTEN(enable CONVSTL int)
								//  bit.B : INT0EN
								//  bit.C : CMDINTEN
								//  bit.D : SPG0INTEN(�V���A���G���R�[�_0�^C���G�b�W�����݃C�l�[�u��)
								//  bit.E : SPG1INTEN(�V���A���G���R�[�_1�����݃C�l�[�u��)
								//  bit.F : UTINTEN(�񓯊��V���A������M�������݃C�l�[�u��)
/*--------------------------------------------------------------------------------------------------*/
#define	CRST *(VUSHORT*)0x07d3					/* ADR:0x07d3: Carrer start											*/
								//  bit.0 : CRSTL(PWM�L�����A�J�n�w��)
								//  bit.1 : CC40M(�L�����A�J�E���^�N���b�N�ݒ�@0:80MHz�@1:40MHz)
#define	SOD0L *(VUSHORT*)0x07d4					/* ADR:0x07d4: Sync-Serial0 Low										*/
#define	SOD0H *(VUSHORT*)0x07d5					/* ADR:0x07d5: Sync-Serial0 High									*/
#define	SOD1L *(VUSHORT*)0x07d6					/* ADR:0x07d6: Sync-Serial1 Low										*/
#define	SOD1H *(VUSHORT*)0x07d7					/* ADR:0x07d7: Sync-Serial1 High									*/
/*--------------------------------------------------------------------------------------------------*/
#define	SDMECLR *(VUSHORT*)0x07d8				/* ADR:0x07d8: SDM status clr										*/
								//  bit.0 : Decimation1 error clear
								//  bit.1 : Decimation2 error clear
								//  bit.2 : Decimation3 error clear
								//  bit.3 : Decimation1-3 MCLK error clear
/*--------------------------------------------------------------------------------------------------*/
#define	ADSYNC *(VUSHORT*)0x07d9					/* ADR:0x07d9: AD sync												*/
//   	--;						/* ADR:0x07da: 														*/
/*--------------------------------------------------------------------------------------------------*/
#define	PWMOS *(VUSHORT*)0x07db					/* ADR:0x07db: PWM out sellect										*/
								//  bit.0 : PWMOSEL(0:Triangle,1:Space-Vec)
								//  bit.3 : LV2L(0:2level,1:3level)
								//  bit.4 : MBCMPH(0:Normal,1:Nx<--X2)
								//  bit.5 : LDTYPE(0:Carrier,1:T0(servo))
								//  bit.6 : FLDTYPE(0:Bottom,1:Top&Bottom)
								//  bit.7 : SAWL(0:SAW type,1:Normal)
								//  bit.8 : SAWDWL(0:Down,1:Up)
/*--------------------------------------------------------------------------------------------------*/
#define	CrSet1 *(VUSHORT*)0x07dc					/* ADR:0x07dc: Carrer setting										*/
								//  bit.0-2 : T&B counter clear set
/*--------------------------------------------------------------------------------------------------*/
#define	CTSTW *(VUSHORT*)0x07dd					/* ADR:0x07dd: controll status										*/
								//  bit.0 : STD0
								//  bit.1 : STD1
								//  bit.2 : STD2
								//  bit.3 : STD3(D/QLIM)
								//  bit.4 : STD4
								//  bit.5 : STD5(TLIM)
								//  bit.6 : STD6
								//  bit.7 : STD7
/*--------------------------------------------------------------------------------------------------*/
#define	ADT0 *(VUSHORT*)0x07de					/* ADR:0x07de: carrer 1/3 point set									*/
#define	CRFRQ *(VUSHORT*)0x07df					/* ADR:0x07df: Carrer frequency										*/
#define	PwmP0 *(VUSHORT*)0x07e0					/* ADR:0x07e0: PWM pattern 1 0										*/
#define	PwmP2 *(VUSHORT*)0x07e1					/* ADR:0x07e1: PWM pattern 3 2										*/
#define	PwmP4 *(VUSHORT*)0x07e2					/* ADR:0x07e2: PWM pattern 5 4										*/
#define	PwmP6 *(VUSHORT*)0x07e3					/* ADR:0x07e3: PWM pattern 7 6										*/
#define	PwmP8 *(VUSHORT*)0x07e4					/* ADR:0x07e4: PWM pattern 9 8										*/
#define	PwmP10 *(VUSHORT*)0x07e5					/* ADR:0x07e5: PWM pattern 11 10									*/
#define	PwmP12 *(VUSHORT*)0x07e6					/* ADR:0x07e6: PWM pattern 12										*/
#define	PwmT0 *(VUSHORT*)0x07e7					/* ADR:0x07e7: PWM timer 0											*/
#define	PwmT1 *(VUSHORT*)0x07e8					/* ADR:0x07e8: PWM timer 1											*/
#define	PwmT2 *(VUSHORT*)0x07e9					/* ADR:0x07e9: PWM timer 2											*/
#define	PwmT3 *(VUSHORT*)0x07ea					/* ADR:0x07ea: PWM timer 3											*/
#define	PwmT4 *(VUSHORT*)0x07eb					/* ADR:0x07eb: PWM timer 4											*/
#define	PwmT5 *(VUSHORT*)0x07ec					/* ADR:0x07ec: PWM timer 5											*/
#define	PwmT6 *(VUSHORT*)0x07ed					/* ADR:0x07ed: PWM timer 6											*/
#define	PwmT7 *(VUSHORT*)0x07ee					/* ADR:0x07ee: PWM timer 7											*/
#define	PwmT8 *(VUSHORT*)0x07ef					/* ADR:0x07ef: PWM timer 8											*/
#define	PwmT9 *(VUSHORT*)0x07f0					/* ADR:0x07f0: PWM timer 9											*/
#define	PwmT10 *(VUSHORT*)0x07f1					/* ADR:0x07f1: PWM timer 10											*/
#define	PwmT11 *(VUSHORT*)0x07f2					/* ADR:0x07f2: PWM timer 11											*/
//   	--;						/* ADR:0x07f3: 														*/
#define	ISA0 *(VUSHORT*)0x07f4					/* ADR:0x07f4: interrupt 0 address									*/
#define	ISA1 *(VUSHORT*)0x07f5					/* ADR:0x07f5: interrupt 1 address									*/
#define	ISA2 *(VUSHORT*)0x07f6					/* ADR:0x07f6: interrupt 2 address									*/
#define	ISA3 *(VUSHORT*)0x07f7					/* ADR:0x07f7: interrupt 3 address									*/
#define	INTLVWR *(VUSHORT*)0x07f8				/* ADR:0x07f8: interrupt level write								*/
/*--------------------------------------------------------------------------------------------------*/
#define	DivSet *(VUSHORT*)0x07f9					/* ADR:0x07f9: counter divide set									*/
								// bit.0   : PCMASK
								// bit.A-B : division C out select
								//		   (00:normal,01:div
								//		    10:normal,11:div(00|01))
/*--------------------------------------------------------------------------------------------------*/
#define	PCVS0 *(VUSHORT*)0x07fa					/* ADR:0x07fa: PWM-pulse convert set0								*/
#define	PCVS1 *(VUSHORT*)0x07fb					/* ADR:0x07fb: PWM-pulse convert set1								*/
#define	PCVS2 *(VUSHORT*)0x07fc					/* ADR:0x07fc: PWM-pulse convert set2								*/
#define	EIX *(VUSHORT*)0x07fd					/* ADR:0x07fd: interrupt enable										*/
#define	DIX *(VUSHORT*)0x07fe					/* ADR:0x07fe: interrupt disable									*/
//   	-;						/* ADR:0x07ff: 														*/
/*--------------------------------------------------------------------------------------------------*/
/*		Flags																						*/
/*--------------------------------------------------------------------------------------------------*/


/****************************************************************************************************/
/*																									*/
/*		�萔�}�N����`																				*/
/*																									*/
/****************************************************************************************************/
#define		ZEROR	0			/* ADR:0x0000: (WREGBASE+000) ; Zero register(Low)					*/
#define		ONER	1			/* ADR:0x01ec: one value register					ADDR = 080073D8	*/
#define		NONER	-1			/* ADR:0x01ee: -1 value register					ADDR = 080073DC	*/


/*--------------------------------------------------------------------------------------------------*/
/*		AMPTYPE : ��]�^/���j�A�^�I��														<V720>	*/
/*--------------------------------------------------------------------------------------------------*/
#pragma AsicReg begin
#define	ROTARY		0x0000		/* 0	: ��]�^										*/
#define LINEAR		0x0001		/* 1	: ���j�A�^													*/

/*--------------------------------------------------------------------------------------------------*/
/*		PAOSEQCMD : PaoSeqMcmd Interface Definition											<V720>	*/
/*--------------------------------------------------------------------------------------------------*/
#define PAONOCMD	0x0000		/* No Command              (Host CPU:KPX_PAOSEQ_NOCMD)				*/
#define PAOPLSOUT	0x0020		/* IncPulse Output Request (Host CPU:KPX_IPLSOUT_REQUEST)			*/

/*--------------------------------------------------------------------------------------------------*/
/*		POSETCMD : �p���X�o�͉�H�������v���@(��Βl����V�[�P���X�̃p���X�o�͉�H�������p)	<V720>	*/
/*--------------------------------------------------------------------------------------------------*/
#define POSETNOCMD	0x0000		/* �v���Ȃ�															*/
#define POSETCMD00	0x0001		/* �p���X�ϊ��ʒu��0000h�������Z�b�g�v��							*/
#define POSETCMDFF	0x0002		/* �p���X�ϊ��ʒu��FFFFh�������Z�b�g�v��							*/

/*--------------------------------------------------------------------------------------------------*/
/*		BITIPRM																				<V720>	*/
/*--------------------------------------------------------------------------------------------------*/
#define UPGDIVOUT	0x0001		/* bit0 : �}�C�N�������@�\�g�p�I��									*/
//      equ			0x0002		/* bit1 : ��														*/
//      equ			0x0004		/* bit2 : ��														*/
//      equ			0x0008		/* bit3 : ��														*/
//      equ			0x0010		/* bit4 : ��														*/
//      equ			0x0020		/* bit5 : ��														*/
//      equ			0x0040		/* bit6 : ��														*/
//      equ			0x0080		/* bit7 : ��														*/
//      equ			0x0100		/* bit8 : ��														*/
//      equ			0x0200		/* bit9 : ��														*/
//      equ			0x0400		/* bitA : ��														*/
//      equ			0x0800		/* bitB : ��														*/
//      equ			0x1000		/* bitC : ��														*/
//      equ			0x2000		/* bitD : ��														*/
//      equ			0x4000		/* bitE : ��														*/
//      equ			0x8000		/* bitF : ��														*/


/*--------------------------------------------------------------------------------------------------*/
/*		BITDAT 																				<V720>	*/
/*--------------------------------------------------------------------------------------------------*/
#define SERXERR		0x0001		/* bit0 : Senc Receive Error										*/
#define ACCCHKENA	0x0002		/* bit1 : Encoder Acc. Check Enable Flag							*/
//      equ			0x0004		/* bit2 : ��														*/
//      equ			0x0008		/* bit3 : ��														*/
//      equ			0x0010		/* bit4 : ��														*/
//      equ			0x0020		/* bit5 : ��														*/
//      equ			0x0040		/* bit6 : ��														*/
//      equ			0x0080		/* bit7 : ��														*/
//      equ			0x0100		/* bit8 : ��														*/
//      equ			0x0200		/* bit9 : ��														*/
//      equ			0x0400		/* bitA : ��														*/
//      equ			0x0800		/* bitB : ��														*/
//      equ			0x1000		/* bitC : ��														*/
//      equ			0x2000		/* bitD : ��														*/
//      equ			0x4000		/* bitE : ��														*/
//      equ			0x8000		/* bitF : ��														*/

/****************************************************************************************************/
/*																									*/
/*		SGDM immidiate Definition																	*/
/*																									*/
/****************************************************************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*		CSW ( control select switch ) 																*/
/*--------------------------------------------------------------------------------------------------*/
#define ICLR		0x0001		/* bit.0 : Integration clear bit									*/
#define ISEL		0x0002		/* bit.1 : Voltage compensation select bit							*/
//      --			0x0004		/* bit.2 : ----														*/
//      --			0x0008		/* bit.3 : ----														*/
#define OBSSEL		0x0008		/* bit.3 : Current Observer Select bit	; <V038>	<V076>			*/
#define F1DSABL		0x0010		/* bit.4 : Notch filter1 Disable									*/
#define F2DSABL		0x0020		/* bit.5 : Notch filter2 Disable									*/
#define LPFDSABL	0x0040		/* bit.6 : Low Pass filter1 Disable									*/
#define LPFCDSABL	0x0080		/* bit.7 : Low Pass filter2 Disable									*/
#define OVMSEL1		0x0100		/* bit.8 : Over modulation select(bit 8,9)							*/
#define OVMSEL2		0x0200		/* bit.9 : if bit8,9 = 10 or 11  then OVMOD2						*/
								//         if bit8,9 = 01        then OVMOD1
#define OVMMOD		0x0400		/* bit.A : modulation modification enable							*/
#define V_FB		0x0800		/* bit.B : �d��FB������ߊE���I��									*/
#define DIDTSEL		0x1000		/* bit.C : L di/dt cmp select(1:di/dt,0:def)						*/
//      ANTIWU		0x2000		/* bit.D : anti-windup test											*/
#define V_FB2		0x2000		/* bit.D : �d��FB������ߊE���I��2	<V531>							*/
#define INT_ST		0x4000		/* bit.E : �O�a���ϕ���~											*/
#define F3DSABL		0x8000		/* bit.F : Notch filter3 Disable									*/
/*--------------------------------------------------------------------------------------------------*/
/*		CPORT read data			  bit	address 4a0h												*/
/*--------------------------------------------------------------------------------------------------*/
//      --			0x0001		/* bit.0 :															*/
//      --			0x0002		/* bit.1 :															*/
//      --			0x0004		/* bit.2 :															*/
#define RLOCK		0x0008		/* bit.3 : Transer register lock status								*/
#define BB			0x0010		/* bit.4 : Base block bit											*/
/*--------------------------------------------------------------------------------------------------*/
/*		status set				  bit																*/
/*--------------------------------------------------------------------------------------------------*/
//      --			0x0004		/* bit.2 :															*/
#define DLIM		0x0008		/* bit.3 : D-axis integration limit status							*/
#define QLIM		0x0008		/* bit.3 : Q-axis integration limit status							*/
//      --			0x0010		/* bit.4 :															*/
#define TLIM		0x0020		/* bit.5 : Torque Limit status										*/
#define IHOLD		0x0040		/* bit.6 : Anti-windup test											*/
//      --			0x0080		/* bit.7 :															*/
/*--------------------------------------------------------------------------------------------------*/
/*		status clear			  bit																*/
/*--------------------------------------------------------------------------------------------------*/
//      --			0xfffb		/* bit.2 :															*/
#define DLIMI		0xfff7		/* bit.3 :															*/
#define QLIMI		0xfff7		/* bit.3 :															*/
//      --			0xffef		/* bit.4 :															*/
#define TLIMI		0xffdf		/* bit.5 :															*/
#define IHOLDI		0xffbf		/* bit.6 : Anti-windup test											*/
//      NON			0xff7f		/* bit.7 :															*/
/*--------------------------------------------------------------------------------------------------*/
/*		static value																				*/
/*--------------------------------------------------------------------------------------------------*/
#define PI2			0x4000		/* 90deg															*/
#define PI23		0x5555		/* 120deg															*/










/***************************************** end of file **********************************************/
