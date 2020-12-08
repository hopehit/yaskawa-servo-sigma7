/****************************************************************************************************/
/*																									*/
/*																									*/
/*		SvIpAccess.h : Mercury JL-086 Servo IP H/W Registers definition								*/
/*																									*/
/*																									*/
/****************************************************************************************************/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/*																									*/
/************** Copyright (C) Yaskawa Electric Corporation ******************************************/
/*																									*/
/*		2013.10.21 : Made by Excel Macro															*/
/*																									*/
/*																									*/
/****************************************************************************************************/
#include "Basedef.h"



/****************************************************************************************************/
/*																									*/
/*		H/W Register																				*/
/*																									*/
/****************************************************************************************************/
/****************************************************************************************************/
/*		Read Registers																				*/
/****************************************************************************************************/
typedef struct _SVIP_READ_REG {
	VUSHORT		MACCDLL;		/* 0x600 ��Z���ʃ��[�h0											*/
	VUSHORT		MACCDLH;		/* 0x601 ��Z���ʏ�ʃ��[�h1										*/
	VUSHORT		MACCDHL;		/* 0x602 ��Z���ʃ��[�h2											*/
	VUSHORT		MACCDHH;		/* 0x603 ��Z���ʏ�ʃ��[�h3										*/
	VUSHORT		SACCDLL;		/* 0x604 �o�����V�t�^���[�h0										*/
	VUSHORT		SACCDLH;		/* 0x605 �o�����V�t�^���[�h1										*/
	VUSHORT		SACCDHL;		/* 0x606 �o�����V�t�^���[�h2										*/
	VUSHORT		SACCDHH;		/* 0x607 �o�����V�t�^���[�h3										*/
	VUSHORT		MODL;			/* 0x608 ��]���ʃ��[�h												*/
	VUSHORT		MODH;			/* 0x609 ��]��ʃ��[�h												*/
	VUSHORT		IREG_S;			/* 0x60A �Ԑړ]���\�[�X�A�h���X�ێ����W�X�^�ݒ�						*/
	VUSHORT		IREG_D;			/* 0x60B �Ԑړ]������A�h���X�ێ����W�X�^�ݒ�						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy1[6];		/* 0x60C - 0x611  --Reserve--										*/
	VUSHORT		INTLVR;			/* 0x612 �����݃��x���ݒ�l											*/
								/*  bit.0 : ���荞��0���x���ݒ�r�b�g0								*/
								/*  bit.1 : ���荞��0���x���ݒ�r�b�g1								*/
								/*  bit.2 : ���荞��0���x���ݒ�r�b�g2								*/
								/*  bit.3 : ���荞��1���x���ݒ�r�b�g0								*/
								/*  bit.4 : ���荞��1���x���ݒ�r�b�g1								*/
								/*  bit.5 : ���荞��1���x���ݒ�r�b�g2								*/
								/*  bit.6 : ���荞��2���x���ݒ�r�b�g0								*/
								/*  bit.7 : ���荞��2���x���ݒ�r�b�g1								*/
								/*  bit.8 : ���荞��2���x���ݒ�r�b�g2								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCTRL;			/* 0x613 ���������_���Z���䃌�W�X�^									*/
								/*  bit.0 : �ۂ߃��[�h												*/
								/*  bit.1 : �������Z��O��������									*/
								/*  bit.2 : 0���Z��O��������										*/
								/*  bit.3 : �I�[�o�t���[��O��������								*/
								/*  bit.4 : �A���_�t���[��O��������								*/
								/*  bit.5 : �s���m��O��������										*/
								/*  bit.6 : FPU��O���荞�ݏo��										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCC;			/* 0x614 ���������_�R���f�B�V�����R�[�h���W�X�^						*/
								/*  bit.0 : ���������_���Z���ʃR���f�B�V����1						*/
								/*  bit.1 : ���������_���Z���ʃR���f�B�V����2						*/
								/*  bit.2 : ���������_���Z���ʃR���f�B�V����3						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FTRP;			/* 0x615 ���������_�g���b�v���W�X�^									*/
								/*  bit.0 : �������Z��O����										*/
								/*  bit.1 : 0���Z��O����											*/
								/*  bit.2 : �I�[�o�t���[����										*/
								/*  bit.3 : �A���_�t���[����										*/
								/*  bit.4 : �s���m��O����											*/
								/*  bit.5 : �������Z���1											*/
								/*  bit.6 : �������Z���2											*/
								/*  bit.7 : �������Z���3											*/
								/*  bit.8 : �ȉ����������_��O�������̃I�y�R�[�h					*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy2[106];	/* 0x616 - 0x67F  --Reserve--										*/
	VUSHORT		EncFlag;		/* 0x680 �G���R�[�_�␳�ݒ肨��уA���[��							*/
								/*  bit.0 : ComAlarm												*/
								/*  bit.1 : AccAlarm												*/
								/*  bit.2 : --Reserve--												*/
								/*  bit.3 : --Reserve--												*/
								/*  bit.4 : --Reserve--												*/
								/*  bit.5 : AccChkEN												*/
								/*  bit.6 : SeRxErr													*/
								/*  bit.7 : SeRxComp												*/
								/*  bit.8 : EncDataNG												*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		BaseTim;		/* 0x681 �x�[�X�^�C�}�l												*/
	VUSHORT		LastBaseTim;	/* 0x682 �O��̃x�[�X�^�C�}�l										*/
	VUSHORT		HenkaItiTim;	/* 0x683 �ω��ʒu�^�C�}												*/
	VUSHORT		SPGRXF;			/* 0x684 ��M�t���O�ݒ�												*/
	VUSHORT		Dummy3;			/* 0x685  --Reserve--												*/
	VUSHORT		MotPos0_L;		/* 0x686 �␳��ʒu�f�[�^(���ʃ��[�h)								*/
	VUSHORT		MotPos0_H;		/* 0x687 �␳��ʒu�f�[�^(��ʃ��[�h)								*/
	VUSHORT		MotPos1_L;		/* 0x688 �O��̕␳��ʒu�f�[�^(���ʃ��[�h)							*/
	VUSHORT		MotPos1_H;		/* 0x689 �O��̕␳��ʒu�f�[�^(��ʃ��[�h)							*/
	VUSHORT		RcvPos0_L;		/* 0x68A �␳�Ȃ��ʒu�f�[�^(���ʃ��[�h)								*/
	VUSHORT		RcvPos0_H;		/* 0x68B �␳�Ȃ��ʒu�f�[�^(��ʃ��[�h)								*/
	VUSHORT		RcvPos1_L;		/* 0x68C �O��̕␳�Ȃ��ʒu�f�[�^(���ʃ��[�h)						*/
	VUSHORT		RcvPos1_H;		/* 0x68D �O��̕␳�Ȃ��ʒu�f�[�^(��ʃ��[�h)						*/
	VUSHORT		SPGD_1;			/* 0x68E �G���R�[�_��M�f�[�^�ݒ�(SPGD1)							*/
	VUSHORT		SPGD_2;			/* 0x68F �G���R�[�_��M�f�[�^�ݒ�(SPGD2)							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		SPGD_3;			/* 0x690 �G���R�[�_��M�f�[�^�ݒ�(SPGD3)							*/
	VUSHORT		SPGD_4;			/* 0x691 �G���R�[�_��M�f�[�^�ݒ�(SPGD4)							*/
	VUSHORT		SPGD_5;			/* 0x692 �G���R�[�_��M�f�[�^�ݒ�(SPGD5)							*/
	VUSHORT		RxPos0_LL;		/* 0x693 ���W�X�^���b�`�f�[�^ 0-15									*/
	VUSHORT		RxPos0_LH;		/* 0x694 ���W�X�^���b�`�f�[�^ 16-31									*/
	VUSHORT		RxPos0_H;		/* 0x695 ���W�X�^���b�`�f�[�^ 32-47									*/
	VUSHORT		RxPos1_LL;		/* 0x696 �O��̎�M�f�[�^ ���W�X�^���b�`�f�[�^ 0-15					*/
	VUSHORT		RxPos1_LH;		/* 0x697 �O��̎�M�f�[�^ ���W�X�^���b�`�f�[�^ 16-31				*/
	VUSHORT		RxPos1_H;		/* 0x698 �O��̎�M�f�[�^ ���W�X�^���b�`�f�[�^ 32-47				*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy4[7];		/* 0x699 - 0x69F  --Reserve--										*/
	VUSHORT		AVU;			/* 0x6A0 �d�����o�pAD�f�[�^											*/
	VUSHORT		Dummy5[11];		/* 0x6A1 - 0x6AB  --Reserve--										*/
	VUSHORT		NCTCTM;			/* 0x6AC �w�ߗpNCT�^�C�}�J�E���^�l									*/
	VUSHORT		NCTCFB;			/* 0x6AD �w�ߗpNCT�t�B�[�h�o�b�N�J�E���^�l							*/
	VUSHORT		NCTCCD;			/* 0x6AE �w�ߗpNCT CD�E���^�l										*/
	VUSHORT		NCTCTP;			/* 0x6AF �w�ߗpNCT TP�E���^�l										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy6[4];		/* 0x6B0 - 0x6B3  --Reserve--										*/
	VUSHORT		DBSTS;			/* 0x6B4 �f�b�h�o���h�J�E���^�I�[�o/�A���_�t���[					*/
								/*  bit.0 : DBCU1OV													*/
								/*  bit.1 : DBCU1UN													*/
								/*  bit.2 : DBCV1OV													*/
								/*  bit.3 : DBCV1UN													*/
								/*  bit.4 : DBCW1OV													*/
								/*  bit.5 : DBCW1UN													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		DBU1;			/* 0x6B5 PU1V�f�b�h�o���h�⏞�J�E���^���b�`�l						*/
	VUSHORT		DBV1;			/* 0x6B6 PV1V�f�b�h�o���h�⏞�J�E���^���b�`�l						*/
	VUSHORT		DBW1;			/* 0x6B7 PW1V�f�b�h�o���h�⏞�J�E���^���b�`�l						*/
	VUSHORT		ACMC4;			/* 0x6B8 �w�߃}���`�v���N�X�`�����l��4								*/
	VUSHORT		ACMC5;			/* 0x6B9 �w�߃}���`�v���N�X�`�����l��5								*/
	VUSHORT		ACMC6;			/* 0x6BA �w�߃}���`�v���N�X�`�����l��6								*/
	VUSHORT		ACMC7;			/* 0x6BB �w�߃}���`�v���N�X�`�����l��7								*/
	VUSHORT		Dummy7;			/* 0x6BC  --Reserve--												*/
	VUSHORT		FCCST;			/* 0x6BD �t���N���[�Y�h�J�E���^�X�e�[�^�X							*/
	VUSHORT		FCCFZRNL;		/* 0x6BE �t���N���[�Y�h�J�E���^FZRN���b�`�f�[�^						*/
	VUSHORT		FCLDL;			/* 0x6BF �t���N���[�Y�h�J�E���^���[�h���b�`�f�[�^					*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy8[2];		/* 0x6C0 - 0x6C1  --Reserve--										*/
	VUSHORT		SADATA;			/* 0x6C2 �G���R�[�_�`�c�f�[�^A										*/
	VUSHORT		SAADATA;		/* 0x6C3 �G���R�[�_�`�c�f�[�^AA										*/
	VUSHORT		SBDATA;			/* 0x6C4 �G���R�[�_�`�c�f�[�^B										*/
	VUSHORT		SBBDATA;		/* 0x6C5 �G���R�[�_�`�c�f�[�^BB										*/
	VUSHORT		Dummy9[2];		/* 0x6C6 - 0x6C7  --Reserve--										*/
	VUSHORT		FBZRND;			/* 0x6C8 �t�B�[�h�o�b�N�J�E���^ZRN���b�`�f�[�^						*/
	VUSHORT		FBCLT;			/* 0x6C9 �t�B�[�h�o�b�N�J�E���^���[�h���b�`�f�[�^					*/
	VUSHORT		FBPUD;			/* 0x6CA �t�B�[�h�o�b�N�J�E���^PU���b�`�f�[�^						*/
	VUSHORT		FBPCD;			/* 0x6CB �t�B�[�h�o�b�N�J�E���^PC���b�`�f�[�^						*/
	VUSHORT		TCNT;			/* 0x6CC �t�B�[�h�o�b�N�^�C�}���ݒl									*/
	VUSHORT		T1LT;			/* 0x6CD �t�B�[�h�o�b�N�^�C�}A/B�p���X���b�`�l						*/
	VUSHORT		T2LT;			/* 0x6CE �t�B�[�h�o�b�N�L�����A���b�`�l								*/
	VUSHORT		T3LT;			/* 0x6CF �t�B�[�h�o�b�N�^�C�}T1���O�l���b�`�l						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		IuAD;			/* 0x6D0 U���d���t�B�[�h�o�b�NADC���ݒl								*/
	VUSHORT		IvAD;			/* 0x6D1 V���d���t�B�[�h�o�b�NADC���ݒl								*/
	VUSHORT		CRFRQI;			/* 0x6D2 PWM�L�����A�J�E���^���ݒl									*/
	VUSHORT		Dummy10[6];		/* 0x6D3 - 0x6D8  --Reserve--										*/
	VUSHORT		HSUR0;			/* 0x6D9 �z�X�g�w�߃|�[�g�O											*/
	VUSHORT		HSUR1;			/* 0x6DA �z�X�g�w�߃|�[�g�P											*/
	VUSHORT		HSUR2;			/* 0x6DB �z�X�g�w�߃|�[�g�Q											*/
	VUSHORT		HSUR3;			/* 0x6DC �z�X�g�w�߃|�[�g�R											*/
	VUSHORT		CTSTR;			/* 0x6DD controll status											*/
								/*  bit.0 : '1'�Œ�													*/
								/*  bit.1 : CTRD1													*/
								/*  bit.2 : CTRD2													*/
								/*  bit.3 : CTRD3(RLOCK)											*/
								/*  bit.4 : CTRD4(BB)												*/
								/*  bit.5 : CTRD5													*/
								/*  bit.6 : CTRD6													*/
								/*  bit.7 : CTRD7													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		INT1SET;		/* 0x6DE INT1 status read clr										*/
								/*  bit.0 : EADINTAC												*/
								/*  bit.1 : INTMPEAC												*/
								/*  bit.2 : CNVSTAC													*/
								/*  bit.3 : INT0AC													*/
								/*  bit.4 : CMDINTAC												*/
								/*  bit.5 : SPG0INTAC												*/
								/*  bit.6 : SPG1INTAC												*/
								/*  bit.7 : URTINTAC												*/
								/*  bit.8 : HINT2AC													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FLTSTAT;		/* 0x6DF fault status(FLTSTAT)										*/
								/*  bit.0 : HBLK0(Sync-Serial bit0)									*/
								/*  bit.1 : HBLK1(Sync-Serial bit1)									*/
								/*  bit.2 : HBLK2(Sync-Serial bit2)									*/
								/*  bit.3 : OC2DAT													*/
								/*  bit.4 : OVDAT(OV)												*/
								/*  bit.5 : OCDAT(OC)												*/
								/*  bit.6 : OVLG(Degital filter fault:0)							*/
								/*  bit.7 : DBON1LT													*/
								/*  bit.8 : DBON2LT													*/
								/*  bit.9 : -- Reserve --											*/
								/*  bit.A : BB(Base-Block)											*/
								/*  bit.B : GUP2(Give-up host:0)									*/
								/*  bit.C : OVLGCH1(fault:0)										*/
								/*  bit.D : OVLGCH2(fault:0)										*/
								/*  bit.E : OVLGCH3(fault:0)										*/
								/*  bit.F : CRUP(carrier counter status:up=1)						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FBCSET;			/* 0x6E0 counter status												*/
								/*  bit.0 : PZD(test)												*/
								/*  bit.1 : DADAT(division out A)									*/
								/*  bit.2 : DBDAT(division out B)									*/
								/*  bit.3 : DCDAT(division out C)									*/
								/*  bit.4 : PADAT(PA)												*/
								/*  bit.5 : PBDAT(PB)												*/
								/*  bit.6 : PCDAT(PC)												*/
								/*  bit.7 : ZRE														*/
								/*  bit.8 : CADAT(CA input)											*/
								/*  bit.9 : CBDAT(CB input)											*/
								/*  bit.A : CCDAT(CC input)											*/
								/*  bit.B : CLE(CLM latch end)										*/
								/*  bit.C : PUI(U)													*/
								/*  bit.D : PVI(V)													*/
								/*  bit.E : PWI(W)													*/
								/*  bit.F : UE														*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCCDAT;			/* 0x6E1 SDM status													*/
								/*  bit.0 : SDM1TER(decimation1 over)								*/
								/*  bit.1 : SDM2TER(decimation2 over)								*/
								/*  bit.2 : SDM3TER(decimation3 over)								*/
								/*  bit.3 : SDM1TER(decimation1 mclk miss)							*/
								/*  bit.4 : SDM2TER(decimation2 mclk miss)							*/
								/*  bit.5 : SDM3TER(decimation3 mclk miss)							*/
								/*  bit.6 : --Reserve--												*/
								/*  bit.7 : Encoder AD Latch Complete flag							*/
								/*  bit.8 : RXFLG0(Serial-Enc0 receive flag)						*/
								/*  bit.9 : RXFLG1(Serial-Enc1 receive flag)						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		YTCOUT;			/* 0x6E2 �R�J�J�E���^���ݒl											*/
								/*  bit.0 : ADCRC0													*/
								/*  bit.1 : ADCRC1													*/
								/*  bit.2 : ADCRC2													*/
								/*  bit.3 : --Reserve--												*/
								/*  bit.4 : DF1CRC0													*/
								/*  bit.5 : DF1CRC1													*/
								/*  bit.6 : DF1CRC2													*/
								/*  bit.7 : --Reserve--												*/
								/*  bit.8 : DF2CRC0													*/
								/*  bit.9 : DF2CRC1													*/
								/*  bit.A : DF2CRC2													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy11;		/* 0x6E3  --Reserve--												*/
	VUSHORT		OPCCLM;			/* 0x6E4 OP counter CLM latch										*/
	VUSHORT		Dummy12[2];		/* 0x6E5 - 0x6E6  --Reserve--										*/
	VUSHORT		OPCLT;			/* 0x6E7 OP counter load latch										*/
	VUSHORT		Dummy13[2];		/* 0x6E8 - 0x6E9  --Reserve--										*/
	VUSHORT		SRPG1RD2;		/* 0x6EA Serial-PG1 receive word2									*/
	VUSHORT		SRPG1RD3;		/* 0x6EB Serial-PG1 receive word3									*/
	VUSHORT		SRPG1RD4;		/* 0x6EC Serial-PG1 receive word4									*/
	VUSHORT		SRPG1RD5;		/* 0x6ED Serial-PG1 receive word5									*/
	VUSHORT		SRPG1RD6;		/* 0x6EE Serial-PG1 receive word6									*/
	VUSHORT		SRPG1RD7;		/* 0x6EF Serial-PG1 receive word7									*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy14[10];	/* 0x6F0 - 0x6F9  --Reserve--										*/
	VUSHORT		SRPG0RD2;		/* 0x6FA serial PG0 Rx word 2										*/
	VUSHORT		SRPG0RD3;		/* 0x6FB serial PG0 Rx word 3										*/
	VUSHORT		SRPG0RD4;		/* 0x6FC serial PG0 Rx word 4										*/
	VUSHORT		SRPG0RD5;		/* 0x6FD serial PG0 Rx word 5										*/
	VUSHORT		SRPG0RD6;		/* 0x6FE serial PG0 Rx word 6										*/
	VUSHORT		SRPG0RD7;		/* 0x6FF serial PG0 Rx word 7										*/
} SVIP_READ_REG;



/****************************************************************************************************/
/*		Write Registers																				*/
/****************************************************************************************************/
typedef struct _SVIP_WRITE_REG {
	VUSHORT		Dummy0[10];		/* 0x600 - 0x609  --Reserve--										*/
	VUSHORT		IREG_S;			/* 0x60A �Ԑړ]���\�[�X�A�h���X�ێ����W�X�^�ݒ�						*/
	VUSHORT		IREG_D;			/* 0x60B �Ԑړ]������A�h���X�ێ����W�X�^�ݒ�						*/
	VUSHORT		EIX;			/* 0x60C ���荞�݃C�l�[�u��											*/
	VUSHORT		DIX;			/* 0x60D ���荞�݃f�B�X�G�[�u��										*/
	VUSHORT		ISA0;			/* 0x60E ���荞�݂O�W�����v��A�h���X								*/
	VUSHORT		ISA1;			/* 0x60F ���荞�݂P�W�����v��A�h���X								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		ISA2;			/* 0x610 ���荞�݂Q�W�����v��A�h���X								*/
	VUSHORT		ISA3;			/* 0x611 ���荞�݂R�W�����v��A�h���X								*/
	VUSHORT		INTLVWR;		/* 0x612 �����݃��x���ݒ�											*/
								/*  bit.0 : ���荞��0���x���ݒ�r�b�g0								*/
								/*  bit.1 : ���荞��0���x���ݒ�r�b�g1								*/
								/*  bit.2 : ���荞��0���x���ݒ�r�b�g2								*/
								/*  bit.3 : ���荞��1���x���ݒ�r�b�g0								*/
								/*  bit.4 : ���荞��1���x���ݒ�r�b�g1								*/
								/*  bit.5 : ���荞��1���x���ݒ�r�b�g2								*/
								/*  bit.6 : ���荞��2���x���ݒ�r�b�g0								*/
								/*  bit.7 : ���荞��2���x���ݒ�r�b�g1								*/
								/*  bit.8 : ���荞��2���x���ݒ�r�b�g2								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCTRL;			/* 0x613 ���������_���Z���䃌�W�X�^									*/
								/*  bit.0 : �ۂ߃��[�h												*/
								/*  bit.1 : �������Z��O��������									*/
								/*  bit.2 : 0���Z��O��������										*/
								/*  bit.3 : �I�[�o�t���[��O��������								*/
								/*  bit.4 : �A���_�t���[��O��������								*/
								/*  bit.5 : �s���m��O��������										*/
								/*  bit.6 : FPU��O���荞�ݏo��										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy1[112];	/* 0x614 - 0x683  --Reserve--										*/
	VUSHORT		SPGRXF;			/* 0x684 ��M�t���O�ݒ�												*/
								/*  bit.0 : SPGRXF													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy2[8];		/* 0x685 - 0x68C  --Reserve--										*/
	VUSHORT		SPGD_1;			/* 0x68D �G���R�[�_��M�f�[�^�ݒ�(SPGD1)							*/
	VUSHORT		SPGD_2;			/* 0x68E �G���R�[�_��M�f�[�^�ݒ�(SPGD2)							*/
	VUSHORT		SPGD_3;			/* 0x68F �G���R�[�_��M�f�[�^�ݒ�(SPGD3)							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		SPGD_4;			/* 0x690 �G���R�[�_��M�f�[�^�ݒ�(SPGD4)							*/
	VUSHORT		SPGD_5;			/* 0x691 �G���R�[�_��M�f�[�^�ݒ�(SPGD5)							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy3[47];		/* 0x692 - 0x6C0  --Reserve--										*/
	VUSHORT		POSET0H;		/* 0x6C1 �V���A��PG-�p���X�ϊ��ʒu�ݒ�r�b�g16-23					*/
	VUSHORT		POSET12H;		/* 0x6C2 �V���A��PG-�p���X�ϊ����_�␳H								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy4[13];		/* 0x6C3 - 0x6CF  --Reserve--										*/
	VUSHORT		OUTPT;			/* 0x6D0 INT2 port													*/
	VUSHORT		WDT1L;			/* 0x6D1 WDT trigger port											*/
	VUSHORT		BBSET;			/* 0x6D2 soft BB set												*/
								/*  bit.0 : --Reserve--												*/
								/*  bit.1 : --Reserve--												*/
								/*  bit.2 : soft BB													*/
								/*  bit.3 : --Reserve--												*/
								/*  bit.4 : --Reserve--												*/
								/*  bit.5 : --Reserve--												*/
								/*  bit.6 : --Reserve--												*/
								/*  bit.7 : HINT2EN(�z�X�gCPU������2)								*/
								/*  bit.8 : EADINTEN(�G���R�[�_AD�ϊ����������݃C�l�[�u��)			*/
								/*  bit.9 : MPINTEN(M-Port�����݃C�l�[�u��)							*/
								/*  bit.A : CNVSTEN(�L�����A�R�J�J�E���^�����݃C�l�[�u��)			*/
								/*  bit.B : INT0EN(INT0�����݃C�l�[�u��)							*/
								/*  bit.C : CMDINTEN(�w�߃p���X�J�E���g�����݃C�l�[�u��)			*/
								/*  bit.D : SPG0INTEN(�V���A���G���R�[�_0�^C���G�b�W�����݃C�l�[�u��)	*/
								/*  bit.E : SPG1INTEN(�V���A���G���R�[�_1�����݃C�l�[�u��)			*/
								/*  bit.F : UTINTEN(�񓯊��V���A������M�������݃C�l�[�u��)			*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		CRST;			/* 0x6D3 Carrer start												*/
								/*  bit.0 : CRSTL(PWM�L�����A�J�n�w��)								*/
								/*  bit.1 : CC40M(�L�����A�J�E���^�N���b�N�ݒ�L)					*/
								/*  bit.2 : CC40M(�L�����A�J�E���^�N���b�N�ݒ�H)					*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy5[4];		/* 0x6D4 - 0x6D7  --Reserve--										*/
	VUSHORT		SDMECLR;		/* 0x6D8 SDM status clr												*/
								/*  bit.0 : Decimation1 error clear									*/
								/*  bit.1 : Decimation2 error clear									*/
								/*  bit.2 : Decimation3 error clear									*/
								/*  bit.3 : Decimation1-3 MCLK error clear							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		ADSYNC;			/* 0x6D9 AD sync													*/
	VUSHORT		Dummy6;			/* 0x6DA  --Reserve--												*/
	VUSHORT		PWMOS;			/* 0x6DB PWM out sellect											*/
								/*  bit.0 : PWMOSEL(0:Triangle,1:Space-Vec)							*/
								/*  bit.1 : --Reserve--												*/
								/*  bit.2 : --Reserve--												*/
								/*  bit.3 : LV2L(0:2level,1:3level)									*/
								/*  bit.4 : MBCMPH(0:Normal,1:Nx<--X2)								*/
								/*  bit.5 : LDTYPE(0:Carrier,1:T0(servo))							*/
								/*  bit.6 : FLDTYPE(0:Bottom,1:Top&Bottom)							*/
								/*  bit.7 : SAWL(0:SAW type,1:Normal)								*/
								/*  bit.8 : SAWDWL(0:Down,1:Up)										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		CRSET1;			/* 0x6DC Carrer setting												*/
								/*  bit.0 : T&B counter clear set									*/
								/*  bit.1 : T&B counter clear set									*/
								/*  bit.2 : T&B counter clear set									*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		CTSTW;			/* 0x6DD controll status											*/
								/*  bit.0 : STD0													*/
								/*  bit.1 : STD1													*/
								/*  bit.2 : STD2													*/
								/*  bit.3 : STD3(D/QLIM)											*/
								/*  bit.4 : STD4													*/
								/*  bit.5 : STD5(TLIM)												*/
								/*  bit.6 : STD6													*/
								/*  bit.7 : STD7													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		ADT0;			/* 0x6DE carrer 1/3 point set										*/
	VUSHORT		CRFRQ;			/* 0x6DF Carrer frequency											*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		PwmUP1;			/* 0x6E0 PWM pattern UP1											*/
	VUSHORT		PwmVP1;			/* 0x6E1 PWM pattern VP1											*/
	VUSHORT		PwmWP1;			/* 0x6E2 PWM pattern WP1											*/
	VUSHORT		PwmUP2;			/* 0x6E3 PWM pattern UP2											*/
	VUSHORT		PwmVP2;			/* 0x6E4 PWM pattern VP2											*/
	VUSHORT		PwmWP2;			/* 0x6E5 PWM pattern WP2											*/
	VUSHORT		Dummy7;			/* 0x6E6  --Reserve--												*/
	VUSHORT		PwmT0;			/* 0x6E7 PWM�O�p�g��r�l 0											*/
	VUSHORT		PwmT1;			/* 0x6E8 PWM�O�p�g��r�l 1											*/
	VUSHORT		PwmT2;			/* 0x6E9 PWM�O�p�g��r�l 2											*/
	VUSHORT		PwmT3;			/* 0x6EA PWM�O�p�g��r�l 3											*/
	VUSHORT		PwmT4;			/* 0x6EB PWM�O�p�g��r�l 4											*/
	VUSHORT		PwmT5;			/* 0x6EC PWM�O�p�g��r�l 5											*/
	VUSHORT		PwmT6;			/* 0x6ED PWM�O�p�g��r�l 6											*/
	VUSHORT		PwmT7;			/* 0x6EE PWM�O�p�g��r�l 7											*/
	VUSHORT		PwmT8;			/* 0x6EF PWM�O�p�g��r�l 8											*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		PwmT9;			/* 0x6F0 PWM�O�p�g��r�l 9											*/
	VUSHORT		PwmT10;			/* 0x6F1 PWM�O�p�g��r�l 10											*/
	VUSHORT		PwmT11;			/* 0x6F2 PWM�O�p�g��r�l 11											*/
	VUSHORT		Dummy8[6];		/* 0x6F3 - 0x6F8  --Reserve--										*/
	VUSHORT		DIVSET;			/* 0x6F9 counter divide set											*/
	VUSHORT		PCVS0;			/* 0x6FA PWM-pulse convert set0										*/
	VUSHORT		PCVS1;			/* 0x6FB PWM-pulse convert set1										*/
	VUSHORT		PCVS2;			/* 0x6FC PWM-pulse convert set2										*/
} SVIP_WRITE_REG;



/*--------------------------------------------------------------------------------------------------*/







/***************************************** end of file **********************************************/
