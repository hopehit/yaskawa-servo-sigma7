*********************************************************************************************************
*																										*
*																										*
*		Revision History of MpgDebug Software															*
*																										*
*																										*
*********************************************************************************************************
�ύX�����쐬(MpgDebu_006.5�ȍ~)

=========================================================================================================
[ Version = MpgDebug_006.87 ]  2012.10.09 Y.Tanaka
���ύX�_
�E�R�����g�C���i���s���ɕύX�Ȃ��j

< Update Files >
�E�ύX
	TstSrcC\MprgSgdv.c


=========================================================================================================
[ Version = MpgDebug_006.86 ]  2012.10.09 Y.Tanaka
���ύX�_
�EVC�p���~�b�g���Z�����ǉ�
�@�Z�p�E�V�t�g���Z�A�������Z�Ƀ��~�b�g���f������ǉ��B
�@�L��/�����̃R���p�C���X�C�b�`�Ƃ���DBG_LMT���g�p����B(�ʏ�͗L��)

�E�ڐA�~�X���C��
�@�V�t�g�����A�������򓙂Ň�-�X�}�C�N����������̈ڐA���Ԉ���Ă����ӏ����C���B


< Update Files >
�E�ύX
	SysSrc\IlibSvc.c
	SysSrc\IlibSvc.h
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdvStruct.h
	MpgTstMain.c

�E�ǉ�
	TstSrcC\MprgLmtChkVCMacro.h


=========================================================================================================
[ Version = MpgDebug_006.8 ]  2012.09.19 Y.Tanaka
���ύX�_
�EVC�p�̎Z�p�E�V�t�g���Z�����C�u�����ɕύX
�@076���߂̎Z�p�E�V�t�g�̓V�t�g���ɐ�Βl�ŃV�t�g���s�Ȃ��l�̌ܓ����s�Ȃ��B
�@VC�̒ʏ�̉E�V�t�g�ł͂����ɑΉ��ł��Ȃ����߁A���C�u�����������B
�@�܂�chessde�ł�asr(src,imm)�̃C���g�����V�b�N���߂ƂȂ�̂ŁA
�@�\�[�X��͋��ʂƂ��A�R���p�C���X�C�b�`�Œ�`���قȂ�悤�ȍ��ɍ���Ή�����B

�E�Z�p�E�V�t�g���Z���C�u�����ǉ�
�@��L�̂��߁A���C�u������ǉ��B���ۂ̉��Z�ɍ��킹��64bit�̉��Z�܂ŗp�ӁB

SHORT	IlibASR16( SHORT src , SHORT sft );	/* �Z�p�V�t�g���Z16bit									*/
LONG	IlibASR32( LONG src , SHORT sft);	/* �Z�p�V�t�g���Z32bit									*/
INT64	IlibASR64( INT64 src , SHORT sft);	/* �Z�p�V�t�g���Z64bit									*/

< Update Files >
�E�ύX
	SysSrc\IlibSvc.c
	SysSrc\IlibSvc.h
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	MpgTstMain.c


=========================================================================================================
[ Version = MpgDebug_006.7 ]  2012.09.18 Y.Tanaka
���ύX�_
�E�V�~�����[�V�����Ή�
�@SigmaWin+�̃f�[�^�g���[�X�Ŏ擾����PJOG���쎞�̓��̓f�[�^�ݒ菈����
�@�d�����䕔�̏o�̓f�[�^�擾������ǉ��B
void	SetInitPrm( void );					/* �����p�����[�^�ݒ菈��				*/
void	SetTrqRef( int i );					/* �X�L�������̓��̓p�����[�^�ݒ�		*/
void	GetADOut( int i );					/* �d������̏o�͌��ʎ擾				*/
void	MakeADOutFile( int i );				/* �o�͌��ʂ�csv�o��					*/

�E�e�[�u�����[�h�����̃R�����g�A�E�g���Ă����Ƃ����L���ɕύX


< Update Files >
�E�ύX
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdvStruct.h
	MpgTstMain.c

�E�ǉ�
	TstSrcC\MprgStruct.h
	TstSrcC\RefTbl.h

=========================================================================================================
[ Version = MpgDebug_006.6 ]  2012.09.10 Y.Tanaka
���ύX�_
�E�V�~�����[�V�������o�͑Ή�
�@�e�X�g�p���C���R�[�h�Ƀp�����[�^�Z�b�g�����iTstSetDataAD()�j��ǉ��B

�E�ϐ����ύX
�@�}�C�N���̖��c��Low/High������Ă������̂̕ϐ�������L/H���폜�B
�@���̑��X�y���~�X�����C���B
�ύX1.INTADP
	TfIl			-->		Tfil
	MotRegst		-->		MotRegst
	DeadTimeComp	-->		OnDelaySlope

�ύX2.INTADWK
	sqrt			-->		sqrtwk

�ύX3.WEAKFV
	WfKpVL			-->		WfKpV
	WfKiVL			-->		WfKiV


�E�s�v�ȃ��~�b�g���Z���폜
�@���~�b�g�����̕s�v�ȉӏ��Ń��~�b�g���Z�����Ă����Ƃ����ʏ�̑���ɕύX�B
���֘A�ύX�ԍ�/�����F<V543>/900-028-459

�Echessde/VC�؂蕪��
�@chessde�p��VC�p���R���p�C���X�C�b�`�Ő؂�ւ��B
�@chessde	�F#define	ASIP_CC
�@VC		�F#define	WIN32

�Echessde�p�̃��W�X�^��`�Ƀ��[�h���W�X�^��`��ǉ�
�@0x700�`0x7CF�܂ł̃��W�X�^��`��ǉ��B

�E�e�[�u���Z�b�g/���[�h�����iIxSetCtblAdr/IxLoadMpmem16�j���폜

�EVC�p�̃��~�b�g�����Ńt���O�ϐ��m�F�����Ȃ��悤�ɕύX
�@�t���O�ϐ�(Flag)�͋[��C���ꏈ���Ŏg���Ă������̂ŕs�v�Ȃ��߁B
�@�ύX�O�Ƌ�ʂ��邽�ߊ֐����ύX�B
	IxLmtzImm16		-->		IxLmtZImm16
	IxLmtzReg16		-->		IxLmtZReg16
	IxLmtzImm32		-->		IxLmtZImm32
	IxLmtzReg32		-->		IxLmtZReg32

�E���̑��ȗ��\�ȏ����̋L�q���ȗ�


< Update Files >
�E�ύX
	SysSrc\IxInst.c
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdv_chessde.h
	TstSrcC\MprgSgdv_VC.h
	TstSrcC\MprgSgdvStruct.h
	MpgTstMain.c

�E�폜
	TstSrcC\MprgSgdv_ini.c
	TstSrcC\MprgSgdv_ini.h

�E�ǉ�
	TstSrcC\MpConstTbl.h

=========================================================================================================
[ Version = MpgDebug_006.5 ]  2012.09.04 Y.Tanaka
���ύX�_
�EMpSQRT�Ή�
�@MpSQRT()��chessde�p�ɕύX���Ă��Ȃ��������ߕύX�Ή�
�@�}�C�N���ō��V�t�g�̏������E�V�t�g�ŋL�q���Ă����������������ߏC��

�E���p�̃����o�ϐ��̌^�ύX
�@DLREG(64bit���p��)�ADWREG(32bit���p��)�ɂ��ꂼ��unsigned�̃����o��ǉ�

typedef union {									/* Double Word Register								*/
		INT64	dl;								/* 64bit Register									*/
		UINT64	udl;								/* 64bit Register									*/
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

#signed, unsigned�z���g�ɂǂ�����K�v���v�����B

�E���������Z(MpSQRT)�p���[�N���O���[�o���ϐ��Ɉړ�
�@����ɔ���SQRTWK�\���̂�ǉ��B
�@���������Z�͓d�����䊄���݂݂̂Ŏ��s����Ă���̂ŁAINTADWK�̃����o�ϐ��Ƃ��Ēǉ��B

< Update Files >
�E�ύX
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdvStruct.h


