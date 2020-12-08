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
	VUSHORT		MACCDLL;		/* 0x600 乗算結果ワード0											*/
	VUSHORT		MACCDLH;		/* 0x601 乗算結果上位ワード1										*/
	VUSHORT		MACCDHL;		/* 0x602 乗算結果ワード2											*/
	VUSHORT		MACCDHH;		/* 0x603 乗算結果上位ワード3										*/
	VUSHORT		SACCDLL;		/* 0x604 バレルシフタワード0										*/
	VUSHORT		SACCDLH;		/* 0x605 バレルシフタワード1										*/
	VUSHORT		SACCDHL;		/* 0x606 バレルシフタワード2										*/
	VUSHORT		SACCDHH;		/* 0x607 バレルシフタワード3										*/
	VUSHORT		MODL;			/* 0x608 剰余下位ワード												*/
	VUSHORT		MODH;			/* 0x609 剰余上位ワード												*/
	VUSHORT		IREG_S;			/* 0x60A 間接転送ソースアドレス保持レジスタ設定						*/
	VUSHORT		IREG_D;			/* 0x60B 間接転送宛先アドレス保持レジスタ設定						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy1[6];		/* 0x60C - 0x611  --Reserve--										*/
	VUSHORT		INTLVR;			/* 0x612 割込みレベル設定値											*/
								/*  bit.0 : 割り込み0レベル設定ビット0								*/
								/*  bit.1 : 割り込み0レベル設定ビット1								*/
								/*  bit.2 : 割り込み0レベル設定ビット2								*/
								/*  bit.3 : 割り込み1レベル設定ビット0								*/
								/*  bit.4 : 割り込み1レベル設定ビット1								*/
								/*  bit.5 : 割り込み1レベル設定ビット2								*/
								/*  bit.6 : 割り込み2レベル設定ビット0								*/
								/*  bit.7 : 割り込み2レベル設定ビット1								*/
								/*  bit.8 : 割り込み2レベル設定ビット2								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCTRL;			/* 0x613 浮動小数点演算制御レジスタ									*/
								/*  bit.0 : 丸めモード												*/
								/*  bit.1 : 無効演算例外発生許可									*/
								/*  bit.2 : 0除算例外発生許可										*/
								/*  bit.3 : オーバフロー例外発生許可								*/
								/*  bit.4 : アンダフロー例外発生許可								*/
								/*  bit.5 : 不正確例外発生許可										*/
								/*  bit.6 : FPU例外割り込み出力										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCC;			/* 0x614 浮動小数点コンディションコードレジスタ						*/
								/*  bit.0 : 浮動小数点演算結果コンディション1						*/
								/*  bit.1 : 浮動小数点演算結果コンディション2						*/
								/*  bit.2 : 浮動小数点演算結果コンディション3						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FTRP;			/* 0x615 浮動小数点トラップレジスタ									*/
								/*  bit.0 : 無効演算例外発生										*/
								/*  bit.1 : 0除算例外発生											*/
								/*  bit.2 : オーバフロー発生										*/
								/*  bit.3 : アンダフロー発生										*/
								/*  bit.4 : 不正確例外発生											*/
								/*  bit.5 : 無効演算種類1											*/
								/*  bit.6 : 無効演算種類2											*/
								/*  bit.7 : 無効演算種類3											*/
								/*  bit.8 : 以下浮動小数点例外発生時のオペコード					*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy2[106];	/* 0x616 - 0x67F  --Reserve--										*/
	VUSHORT		EncFlag;		/* 0x680 エンコーダ補正設定およびアラーム							*/
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
	VUSHORT		BaseTim;		/* 0x681 ベースタイマ値												*/
	VUSHORT		LastBaseTim;	/* 0x682 前回のベースタイマ値										*/
	VUSHORT		HenkaItiTim;	/* 0x683 変化位置タイマ												*/
	VUSHORT		SPGRXF;			/* 0x684 受信フラグ設定												*/
	VUSHORT		Dummy3;			/* 0x685  --Reserve--												*/
	VUSHORT		MotPos0_L;		/* 0x686 補正後位置データ(下位ワード)								*/
	VUSHORT		MotPos0_H;		/* 0x687 補正後位置データ(上位ワード)								*/
	VUSHORT		MotPos1_L;		/* 0x688 前回の補正後位置データ(下位ワード)							*/
	VUSHORT		MotPos1_H;		/* 0x689 前回の補正後位置データ(上位ワード)							*/
	VUSHORT		RcvPos0_L;		/* 0x68A 補正なし位置データ(下位ワード)								*/
	VUSHORT		RcvPos0_H;		/* 0x68B 補正なし位置データ(上位ワード)								*/
	VUSHORT		RcvPos1_L;		/* 0x68C 前回の補正なし位置データ(下位ワード)						*/
	VUSHORT		RcvPos1_H;		/* 0x68D 前回の補正なし位置データ(上位ワード)						*/
	VUSHORT		SPGD_1;			/* 0x68E エンコーダ受信データ設定(SPGD1)							*/
	VUSHORT		SPGD_2;			/* 0x68F エンコーダ受信データ設定(SPGD2)							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		SPGD_3;			/* 0x690 エンコーダ受信データ設定(SPGD3)							*/
	VUSHORT		SPGD_4;			/* 0x691 エンコーダ受信データ設定(SPGD4)							*/
	VUSHORT		SPGD_5;			/* 0x692 エンコーダ受信データ設定(SPGD5)							*/
	VUSHORT		RxPos0_LL;		/* 0x693 レジスタラッチデータ 0-15									*/
	VUSHORT		RxPos0_LH;		/* 0x694 レジスタラッチデータ 16-31									*/
	VUSHORT		RxPos0_H;		/* 0x695 レジスタラッチデータ 32-47									*/
	VUSHORT		RxPos1_LL;		/* 0x696 前回の受信データ レジスタラッチデータ 0-15					*/
	VUSHORT		RxPos1_LH;		/* 0x697 前回の受信データ レジスタラッチデータ 16-31				*/
	VUSHORT		RxPos1_H;		/* 0x698 前回の受信データ レジスタラッチデータ 32-47				*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy4[7];		/* 0x699 - 0x69F  --Reserve--										*/
	VUSHORT		AVU;			/* 0x6A0 電圧検出用ADデータ											*/
	VUSHORT		Dummy5[11];		/* 0x6A1 - 0x6AB  --Reserve--										*/
	VUSHORT		NCTCTM;			/* 0x6AC 指令用NCTタイマカウンタ値									*/
	VUSHORT		NCTCFB;			/* 0x6AD 指令用NCTフィードバックカウンタ値							*/
	VUSHORT		NCTCCD;			/* 0x6AE 指令用NCT CDウンタ値										*/
	VUSHORT		NCTCTP;			/* 0x6AF 指令用NCT TPウンタ値										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy6[4];		/* 0x6B0 - 0x6B3  --Reserve--										*/
	VUSHORT		DBSTS;			/* 0x6B4 デッドバンドカウンタオーバ/アンダフロー					*/
								/*  bit.0 : DBCU1OV													*/
								/*  bit.1 : DBCU1UN													*/
								/*  bit.2 : DBCV1OV													*/
								/*  bit.3 : DBCV1UN													*/
								/*  bit.4 : DBCW1OV													*/
								/*  bit.5 : DBCW1UN													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		DBU1;			/* 0x6B5 PU1Vデッドバンド補償カウンタラッチ値						*/
	VUSHORT		DBV1;			/* 0x6B6 PV1Vデッドバンド補償カウンタラッチ値						*/
	VUSHORT		DBW1;			/* 0x6B7 PW1Vデッドバンド補償カウンタラッチ値						*/
	VUSHORT		ACMC4;			/* 0x6B8 指令マルチプレクスチャンネル4								*/
	VUSHORT		ACMC5;			/* 0x6B9 指令マルチプレクスチャンネル5								*/
	VUSHORT		ACMC6;			/* 0x6BA 指令マルチプレクスチャンネル6								*/
	VUSHORT		ACMC7;			/* 0x6BB 指令マルチプレクスチャンネル7								*/
	VUSHORT		Dummy7;			/* 0x6BC  --Reserve--												*/
	VUSHORT		FCCST;			/* 0x6BD フルクローズドカウンタステータス							*/
	VUSHORT		FCCFZRNL;		/* 0x6BE フルクローズドカウンタFZRNラッチデータ						*/
	VUSHORT		FCLDL;			/* 0x6BF フルクローズドカウンタロードラッチデータ					*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy8[2];		/* 0x6C0 - 0x6C1  --Reserve--										*/
	VUSHORT		SADATA;			/* 0x6C2 エンコーダＡＤデータA										*/
	VUSHORT		SAADATA;		/* 0x6C3 エンコーダＡＤデータAA										*/
	VUSHORT		SBDATA;			/* 0x6C4 エンコーダＡＤデータB										*/
	VUSHORT		SBBDATA;		/* 0x6C5 エンコーダＡＤデータBB										*/
	VUSHORT		Dummy9[2];		/* 0x6C6 - 0x6C7  --Reserve--										*/
	VUSHORT		FBZRND;			/* 0x6C8 フィードバックカウンタZRNラッチデータ						*/
	VUSHORT		FBCLT;			/* 0x6C9 フィードバックカウンタロードラッチデータ					*/
	VUSHORT		FBPUD;			/* 0x6CA フィードバックカウンタPUラッチデータ						*/
	VUSHORT		FBPCD;			/* 0x6CB フィードバックカウンタPCラッチデータ						*/
	VUSHORT		TCNT;			/* 0x6CC フィードバックタイマ現在値									*/
	VUSHORT		T1LT;			/* 0x6CD フィードバックタイマA/Bパルスラッチ値						*/
	VUSHORT		T2LT;			/* 0x6CE フィードバックキャリアラッチ値								*/
	VUSHORT		T3LT;			/* 0x6CF フィードバックタイマT1直前値ラッチ値						*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		IuAD;			/* 0x6D0 U相電流フィードバックADC現在値								*/
	VUSHORT		IvAD;			/* 0x6D1 V相電流フィードバックADC現在値								*/
	VUSHORT		CRFRQI;			/* 0x6D2 PWMキャリアカウンタ現在値									*/
	VUSHORT		Dummy10[6];		/* 0x6D3 - 0x6D8  --Reserve--										*/
	VUSHORT		HSUR0;			/* 0x6D9 ホスト指令ポート０											*/
	VUSHORT		HSUR1;			/* 0x6DA ホスト指令ポート１											*/
	VUSHORT		HSUR2;			/* 0x6DB ホスト指令ポート２											*/
	VUSHORT		HSUR3;			/* 0x6DC ホスト指令ポート３											*/
	VUSHORT		CTSTR;			/* 0x6DD controll status											*/
								/*  bit.0 : '1'固定													*/
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
	VUSHORT		YTCOUT;			/* 0x6E2 山谷カウンタ現在値											*/
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
	VUSHORT		IREG_S;			/* 0x60A 間接転送ソースアドレス保持レジスタ設定						*/
	VUSHORT		IREG_D;			/* 0x60B 間接転送宛先アドレス保持レジスタ設定						*/
	VUSHORT		EIX;			/* 0x60C 割り込みイネーブル											*/
	VUSHORT		DIX;			/* 0x60D 割り込みディスエーブル										*/
	VUSHORT		ISA0;			/* 0x60E 割り込み０ジャンプ先アドレス								*/
	VUSHORT		ISA1;			/* 0x60F 割り込み１ジャンプ先アドレス								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		ISA2;			/* 0x610 割り込み２ジャンプ先アドレス								*/
	VUSHORT		ISA3;			/* 0x611 割り込み３ジャンプ先アドレス								*/
	VUSHORT		INTLVWR;		/* 0x612 割込みレベル設定											*/
								/*  bit.0 : 割り込み0レベル設定ビット0								*/
								/*  bit.1 : 割り込み0レベル設定ビット1								*/
								/*  bit.2 : 割り込み0レベル設定ビット2								*/
								/*  bit.3 : 割り込み1レベル設定ビット0								*/
								/*  bit.4 : 割り込み1レベル設定ビット1								*/
								/*  bit.5 : 割り込み1レベル設定ビット2								*/
								/*  bit.6 : 割り込み2レベル設定ビット0								*/
								/*  bit.7 : 割り込み2レベル設定ビット1								*/
								/*  bit.8 : 割り込み2レベル設定ビット2								*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		FCTRL;			/* 0x613 浮動小数点演算制御レジスタ									*/
								/*  bit.0 : 丸めモード												*/
								/*  bit.1 : 無効演算例外発生許可									*/
								/*  bit.2 : 0除算例外発生許可										*/
								/*  bit.3 : オーバフロー例外発生許可								*/
								/*  bit.4 : アンダフロー例外発生許可								*/
								/*  bit.5 : 不正確例外発生許可										*/
								/*  bit.6 : FPU例外割り込み出力										*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy1[112];	/* 0x614 - 0x683  --Reserve--										*/
	VUSHORT		SPGRXF;			/* 0x684 受信フラグ設定												*/
								/*  bit.0 : SPGRXF													*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy2[8];		/* 0x685 - 0x68C  --Reserve--										*/
	VUSHORT		SPGD_1;			/* 0x68D エンコーダ受信データ設定(SPGD1)							*/
	VUSHORT		SPGD_2;			/* 0x68E エンコーダ受信データ設定(SPGD2)							*/
	VUSHORT		SPGD_3;			/* 0x68F エンコーダ受信データ設定(SPGD3)							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		SPGD_4;			/* 0x690 エンコーダ受信データ設定(SPGD4)							*/
	VUSHORT		SPGD_5;			/* 0x691 エンコーダ受信データ設定(SPGD5)							*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		Dummy3[47];		/* 0x692 - 0x6C0  --Reserve--										*/
	VUSHORT		POSET0H;		/* 0x6C1 シリアルPG-パルス変換位置設定ビット16-23					*/
	VUSHORT		POSET12H;		/* 0x6C2 シリアルPG-パルス変換原点補正H								*/
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
								/*  bit.7 : HINT2EN(ホストCPU割込み2)								*/
								/*  bit.8 : EADINTEN(エンコーダAD変換完了割込みイネーブル)			*/
								/*  bit.9 : MPINTEN(M-Port割込みイネーブル)							*/
								/*  bit.A : CNVSTEN(キャリア山谷カウンタ割込みイネーブル)			*/
								/*  bit.B : INT0EN(INT0割込みイネーブル)							*/
								/*  bit.C : CMDINTEN(指令パルスカウント割込みイネーブル)			*/
								/*  bit.D : SPG0INTEN(シリアルエンコーダ0／C相エッジ割込みイネーブル)	*/
								/*  bit.E : SPG1INTEN(シリアルエンコーダ1割込みイネーブル)			*/
								/*  bit.F : UTINTEN(非同期シリアル送受信完割込みイネーブル)			*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		CRST;			/* 0x6D3 Carrer start												*/
								/*  bit.0 : CRSTL(PWMキャリア開始指令)								*/
								/*  bit.1 : CC40M(キャリアカウンタクロック設定L)					*/
								/*  bit.2 : CC40M(キャリアカウンタクロック設定H)					*/
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
	VUSHORT		PwmT0;			/* 0x6E7 PWM三角波比較値 0											*/
	VUSHORT		PwmT1;			/* 0x6E8 PWM三角波比較値 1											*/
	VUSHORT		PwmT2;			/* 0x6E9 PWM三角波比較値 2											*/
	VUSHORT		PwmT3;			/* 0x6EA PWM三角波比較値 3											*/
	VUSHORT		PwmT4;			/* 0x6EB PWM三角波比較値 4											*/
	VUSHORT		PwmT5;			/* 0x6EC PWM三角波比較値 5											*/
	VUSHORT		PwmT6;			/* 0x6ED PWM三角波比較値 6											*/
	VUSHORT		PwmT7;			/* 0x6EE PWM三角波比較値 7											*/
	VUSHORT		PwmT8;			/* 0x6EF PWM三角波比較値 8											*/
/*--------------------------------------------------------------------------------------------------*/
	VUSHORT		PwmT9;			/* 0x6F0 PWM三角波比較値 9											*/
	VUSHORT		PwmT10;			/* 0x6F1 PWM三角波比較値 10											*/
	VUSHORT		PwmT11;			/* 0x6F2 PWM三角波比較値 11											*/
	VUSHORT		Dummy8[6];		/* 0x6F3 - 0x6F8  --Reserve--										*/
	VUSHORT		DIVSET;			/* 0x6F9 counter divide set											*/
	VUSHORT		PCVS0;			/* 0x6FA PWM-pulse convert set0										*/
	VUSHORT		PCVS1;			/* 0x6FB PWM-pulse convert set1										*/
	VUSHORT		PCVS2;			/* 0x6FC PWM-pulse convert set2										*/
} SVIP_WRITE_REG;



/*--------------------------------------------------------------------------------------------------*/







/***************************************** end of file **********************************************/
