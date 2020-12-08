//$Id: equ.h,v 1.1 2012/11/06 00:16:06 t-yamada Exp $
//
//----------------------------------------------------------
// Read registers
//----------------------------------------------------------
extern int chess_storage(PFREG:0x680) EncFlag;
extern int chess_storage(PFREG:0x681) BaseTim;
extern int chess_storage(PFREG:0x682) LastBaseTim;
extern int chess_storage(PFREG:0x683) HenkaItiTim;
extern int chess_storage(PFREG:0x684) SPGRXF;
//685h
extern int chess_storage(PFREG:0x686) MotPos0_L;
extern int chess_storage(PFREG:0x687) MotPos0_H;
extern int chess_storage(PFREG:0x688) MotPos1_L;
extern int chess_storage(PFREG:0x689) MotPos1_H;
extern int chess_storage(PFREG:0x68A) RcvPos0_L;
extern int chess_storage(PFREG:0x68B) RcvPos0_H;
extern int chess_storage(PFREG:0x68C) RcvPos1_L;
extern int chess_storage(PFREG:0x68D) RcvPos1_H;
extern int chess_storage(PFREG:0x68E) SPGD_1;
extern int chess_storage(PFREG:0x68F) SPGD_2;
extern int chess_storage(PFREG:0x690) SPGD_3;
extern int chess_storage(PFREG:0x691) SPGD_4;
extern int chess_storage(PFREG:0x692) SPGD_5;
extern int chess_storage(PFREG:0x693) RxPos0_LL;
extern int chess_storage(PFREG:0x694) RxPos0_LH;
extern int chess_storage(PFREG:0x695) RxPos0_H ;
extern int chess_storage(PFREG:0x696) RxPos1_LL;
extern int chess_storage(PFREG:0x697) RxPos1_LH;
extern int chess_storage(PFREG:0x698) RxPos1_H;
//699h-69Fh
extern int chess_storage(PFREG:0x6A0) AVU;
//6A1h-6ABh
extern int chess_storage(PFREG:0x6AC) NCTCTM;
extern int chess_storage(PFREG:0x6AD) NCTCFB;
extern int chess_storage(PFREG:0x6AE) NCTCCD;
extern int chess_storage(PFREG:0x6AF) NCTCTP;
//6B0h-6B3h
extern int chess_storage(PFREG:0x6B4) DBSTS;
extern int chess_storage(PFREG:0x6B5) DBU1;
extern int chess_storage(PFREG:0x6B6) DBV1;
extern int chess_storage(PFREG:0x6B7) DBW1;
extern int chess_storage(PFREG:0x6B8) ACMC4;
extern int chess_storage(PFREG:0x6B9) ACMC5;
extern int chess_storage(PFREG:0x6BA) ACMC6;
extern int chess_storage(PFREG:0x6BB) ACMC7;
//6BCh
extern int chess_storage(PFREG:0x6BD) FCCST;
extern int chess_storage(PFREG:0x6BE) FCCFZRNL;
extern int chess_storage(PFREG:0x6BF) FCLDL;
//6C0h-6C1h
extern int chess_storage(PFREG:0x6C2) SADATA;
extern int chess_storage(PFREG:0x6C3) SAADATA;
extern int chess_storage(PFREG:0x6C4) SBDATA;
extern int chess_storage(PFREG:0x6C5) SBBDATA;
//6C6h-6C7h
extern int chess_storage(PFREG:0x6C8) FBZRND;
extern int chess_storage(PFREG:0x6C9) FBCLT;
extern int chess_storage(PFREG:0x6CA) FBPUD;
extern int chess_storage(PFREG:0x6CB) FBPCD;
extern int chess_storage(PFREG:0x6CC) TCNT;
extern int chess_storage(PFREG:0x6CD) T1LT;
extern int chess_storage(PFREG:0x6CE) T2LT;
extern int chess_storage(PFREG:0x6CF) T3LT;
extern int chess_storage(PFREG:0x6D0) IuAD;
extern int chess_storage(PFREG:0x6D1) IvAD;
extern int chess_storage(PFREG:0x6D2) CRFRQI;
//6D3h-6D8h
extern int chess_storage(PFREG:0x6D9) HSUR0;
extern int chess_storage(PFREG:0x6DA) HSUR1;
extern int chess_storage(PFREG:0x6DB) HSUR2;
extern int chess_storage(PFREG:0x6DC) HSUR3;
extern int chess_storage(PFREG:0x6DD) CTSTR;
extern int chess_storage(PFREG:0x6DE) INT1SET;
extern int chess_storage(PFREG:0x6DF) FLTSTAT;
extern int chess_storage(PFREG:0x6E0) FBCSET;
extern int chess_storage(PFREG:0x6E1) FCCDAT;
extern int chess_storage(PFREG:0x6E2) YTCOUT;
//6E3h
extern int chess_storage(PFREG:0x6E4) OPCCLM;
extern int chess_storage(PFREG:0x6E5) FAD0DH;
extern int chess_storage(PFREG:0x6E6) FAD1DH;
extern int chess_storage(PFREG:0x6E7) OPCLT;
//6E8h-6E9h
extern int chess_storage(PFREG:0x6EA) SRPG1RD2;
extern int chess_storage(PFREG:0x6EB) SRPG1RD3;
extern int chess_storage(PFREG:0x6EC) SRPG1RD4;
extern int chess_storage(PFREG:0x6ED) SRPG1RD5;
extern int chess_storage(PFREG:0x6EE) SRPG1RD6;
extern int chess_storage(PFREG:0x6EF) SRPG1RD7;
//6F0h-6F3h
extern int chess_storage(PFREG:0x6F4) CAD0DAT;
extern int chess_storage(PFREG:0x6F5) CAD0DATH;
extern int chess_storage(PFREG:0x6F6) CAD1DATL;
extern int chess_storage(PFREG:0x6F7) CAD1DATH;
//6F8h-6F9h
extern int chess_storage(PFREG:0x6FA) SRPG0RD2;
extern int chess_storage(PFREG:0x6FB) SRPG0RD3;
extern int chess_storage(PFREG:0x6FC) SRPG0RD4;
extern int chess_storage(PFREG:0x6FD) SRPG0RD5;
extern int chess_storage(PFREG:0x6FE) SRPG0RD6;
extern int chess_storage(PFREG:0x6FF) SRPG0RD7;

//720h-77Fh
extern int chess_storage(PFREG:0x780) EncFlag_2;
extern int chess_storage(PFREG:0x781) BaseTim_2;
extern int chess_storage(PFREG:0x782) LastBaseTim_2;
extern int chess_storage(PFREG:0x783) HenkaItiTim_2;
extern int chess_storage(PFREG:0x784) SPGRXF_2;
//785h
extern int chess_storage(PFREG:0x786) MotPos0_L_2;
extern int chess_storage(PFREG:0x787) MotPos0_H_2;
extern int chess_storage(PFREG:0x788) MotPos1_L_2;
extern int chess_storage(PFREG:0x789) MotPos1_H_2;
extern int chess_storage(PFREG:0x78A) RcvPos0_L_2;
extern int chess_storage(PFREG:0x78B) RcvPos0_H_2;
extern int chess_storage(PFREG:0x78C) RcvPos1_L_2;
extern int chess_storage(PFREG:0x78D) RcvPos1_H_2;
extern int chess_storage(PFREG:0x78E) SPGD_1_2;
extern int chess_storage(PFREG:0x78F) SPGD_2_2;
extern int chess_storage(PFREG:0x790) SPGD_3_2;
extern int chess_storage(PFREG:0x791) SPGD_4_2;
extern int chess_storage(PFREG:0x792) SPGD_5_2;
extern int chess_storage(PFREG:0x793) RxPos0_LL_2;
extern int chess_storage(PFREG:0x794) RxPos0_LH_2;
extern int chess_storage(PFREG:0x795) RxPos0_H_2;
extern int chess_storage(PFREG:0x796) RxPos1_LL_2;
extern int chess_storage(PFREG:0x797) RxPos1_LH_2;
extern int chess_storage(PFREG:0x798) RxPos1_H_2;
//799h-79Fh
extern int chess_storage(PFREG:0x7A0) AVU_2;
//7A1h-7ABh
extern int chess_storage(PFREG:0x7AC) NCTCTM_2;
extern int chess_storage(PFREG:0x7AD) NCTCFB_2;
extern int chess_storage(PFREG:0x7AE) NCTCCD_2;
extern int chess_storage(PFREG:0x7AF) NCTCTP_2;
//7B0h-7B3h
extern int chess_storage(PFREG:0x7B4) DBSTS_2;
extern int chess_storage(PFREG:0x7B5) DBU1_2;
extern int chess_storage(PFREG:0x7B6) DBV1_2;
extern int chess_storage(PFREG:0x7B7) DBW1_2;
extern int chess_storage(PFREG:0x7B8) ACMC4_2;
extern int chess_storage(PFREG:0x7B9) ACMC5_2;
extern int chess_storage(PFREG:0x7BA) ACMC6_2;
extern int chess_storage(PFREG:0x7BB) ACMC7_2;
//7BCh
extern int chess_storage(PFREG:0x7BD) FCCST_2;
extern int chess_storage(PFREG:0x7BE) FCCFZRNL_2;
extern int chess_storage(PFREG:0x7BF) FCLDL_2;
//7C0h-7C1h
extern int chess_storage(PFREG:0x7C2) SADATA_2;
extern int chess_storage(PFREG:0x7C3) SAADATA_2;
extern int chess_storage(PFREG:0x7C4) SBDATA_2;
extern int chess_storage(PFREG:0x7C5) SBBDATA_2;
//7C6h-7C7h
extern int chess_storage(PFREG:0x7C8) FBZRND_2;
extern int chess_storage(PFREG:0x7C9) FBCLT_2;
extern int chess_storage(PFREG:0x7CA) FBPUD_2;
extern int chess_storage(PFREG:0x7CB) FBPCD_2;
extern int chess_storage(PFREG:0x7CC) TCNT_2;
extern int chess_storage(PFREG:0x7CD) T1LT_2;
extern int chess_storage(PFREG:0x7CE) T2LT_2;
extern int chess_storage(PFREG:0x7CF) T3LT_2;
extern int chess_storage(PFREG:0x7D0) IUD_2;
extern int chess_storage(PFREG:0x7D1) IVD_2;
extern int chess_storage(PFREG:0x7D2) CRFRQI_2;
//7D3h-7D8h
extern int chess_storage(PFREG:0x7D9) HSUR0_2;
extern int chess_storage(PFREG:0x7DA) HSUR1_2;
extern int chess_storage(PFREG:0x7DB) HSUR2_2;
extern int chess_storage(PFREG:0x7DC) HSUR3_2;
extern int chess_storage(PFREG:0x7DD) CTSTR_2;
extern int chess_storage(PFREG:0x7DE) INT1SET_2;
extern int chess_storage(PFREG:0x7DF) FLTSTAT_2;
extern int chess_storage(PFREG:0x7E0) FBCSET_2;
extern int chess_storage(PFREG:0x7E1) FCCDAT_2;
extern int chess_storage(PFREG:0x7E2) YTCOUT_2;
//7E3h
extern int chess_storage(PFREG:0x7E4) OPCCLM_2;
extern int chess_storage(PFREG:0x7E5) FAD0DH_2;
extern int chess_storage(PFREG:0x7E6) FAD1DH_2;
extern int chess_storage(PFREG:0x7E7) OPCLT_2;
//7E8h-7E9h
extern int chess_storage(PFREG:0x7EA) SRPG1RD2_2;
extern int chess_storage(PFREG:0x7EB) SRPG1RD3_2;
extern int chess_storage(PFREG:0x7EC) SRPG1RD4_2;
extern int chess_storage(PFREG:0x7ED) SRPG1RD5_2;
extern int chess_storage(PFREG:0x7EE) SRPG1RD6_2;
extern int chess_storage(PFREG:0x7EF) SRPG1RD7_2;
//7F0h-7F3h
extern int chess_storage(PFREG:0x7F4) CAD0DAT_2;
extern int chess_storage(PFREG:0x7F5) CAD0DATH_2;
extern int chess_storage(PFREG:0x7F6) CAD1DATL_2;
extern int chess_storage(PFREG:0x7F7) CAD1DATH_2;
//7F8h-7F9h
extern int chess_storage(PFREG:0x7FA) SRPG0RD2_2;
extern int chess_storage(PFREG:0x7FB) SRPG0RD3_2;
extern int chess_storage(PFREG:0x7FC) SRPG0RD4_2;
extern int chess_storage(PFREG:0x7FD) SRPG0RD5_2;
extern int chess_storage(PFREG:0x7FE) SRPG0RD6_2;
extern int chess_storage(PFREG:0x7FF) SRPG0RD7_2;

//----------------------------------------------------------
// Write registers
//----------------------------------------------------------
//620h-6C0h
extern int chess_storage(PFREG:0x6C1) POSET0H;
extern int chess_storage(PFREG:0x6C2) POSET12H;
//6C3h-6CFh
extern int chess_storage(PFREG:0x6D0) OUTPT;
extern int chess_storage(PFREG:0x6D1) WDT1L;
extern int chess_storage(PFREG:0x6D2) BBSET;
extern int chess_storage(PFREG:0x6D3) CRST;
//6D4h-6D7h
extern int chess_storage(PFREG:0x6D8) SDMECLR;
extern int chess_storage(PFREG:0x6D9) ADSYNC;
//6DAh
extern int chess_storage(PFREG:0x6DB) PWMOS;
extern int chess_storage(PFREG:0x6DC) CRSET1;
extern int chess_storage(PFREG:0x6DD) CTSTW;
extern int chess_storage(PFREG:0x6DE) ADT0;
extern int chess_storage(PFREG:0x6DF) CRFRQ;
extern int chess_storage(PFREG:0x6E0) PWMUP1;
extern int chess_storage(PFREG:0x6E1) PWMVP1;
extern int chess_storage(PFREG:0x6E2) PWMWP1;
extern int chess_storage(PFREG:0x6E3) PWMUP2;
extern int chess_storage(PFREG:0x6E4) PWMVP2;
extern int chess_storage(PFREG:0x6E5) PWMWP2;
//6E6h
extern int chess_storage(PFREG:0x6E7) PwmT0;
extern int chess_storage(PFREG:0x6E8) PwmT1;
extern int chess_storage(PFREG:0x6E9) PwmT2;
extern int chess_storage(PFREG:0x6EA) PWMT3;
extern int chess_storage(PFREG:0x6EB) PWMT4;
extern int chess_storage(PFREG:0x6EC) PWMT5;
extern int chess_storage(PFREG:0x6ED) PWMT6;
extern int chess_storage(PFREG:0x6EE) PWMT7;
extern int chess_storage(PFREG:0x6EF) PWMT8;
extern int chess_storage(PFREG:0x6F0) PWMT9;
extern int chess_storage(PFREG:0x6F1) PWMT10;
extern int chess_storage(PFREG:0x6F2) PWMT11;
//6F3h-6F8h
extern int chess_storage(PFREG:0x6F9) DIVSET;
extern int chess_storage(PFREG:0x6FA) PCVS0;
extern int chess_storage(PFREG:0x6FB) PCVS1;
extern int chess_storage(PFREG:0x6FC) PCVS2;

//720h-7C0h
extern int chess_storage(PFREG:0x7C1) POSET0H_2;
extern int chess_storage(PFREG:0x7C2) POSET12H_2;
//7C3h-7CFh
extern int chess_storage(PFREG:0x7D0) OUTPT_2;
extern int chess_storage(PFREG:0x7D1) WDT1L_2;
extern int chess_storage(PFREG:0x7D2) BBSET_2;
extern int chess_storage(PFREG:0x7D3) CRST_2;
//7D4h-7D7h
extern int chess_storage(PFREG:0x7D8) SDMECLR_2;
extern int chess_storage(PFREG:0x7D9) ADSYNC_2;
//7DAh
extern int chess_storage(PFREG:0x7DB) PWMOS_2;
extern int chess_storage(PFREG:0x7DC) CRSET1_2;
extern int chess_storage(PFREG:0x7DD) CTSTW_2;
extern int chess_storage(PFREG:0x7DE) ADT0_2;
extern int chess_storage(PFREG:0x7DF) CRFRQ_2;
extern int chess_storage(PFREG:0x7E0) PWMUP1_2;
extern int chess_storage(PFREG:0x7E1) PWMVP1_2;
extern int chess_storage(PFREG:0x7E2) PWMWP1_2;
extern int chess_storage(PFREG:0x7E3) PWMUP2_2;
extern int chess_storage(PFREG:0x7E4) PWMVP2_2;
extern int chess_storage(PFREG:0x7E5) PWMWP2_2;
//7E6h
extern int chess_storage(PFREG:0x7E7) PWMT0_2;
extern int chess_storage(PFREG:0x7E8) PWMT1_2;
extern int chess_storage(PFREG:0x7E9) PWMT2_2;
extern int chess_storage(PFREG:0x7EA) PWMT3_2;
extern int chess_storage(PFREG:0x7EB) PWMT4_2;
extern int chess_storage(PFREG:0x7EC) PWMT5_2;
extern int chess_storage(PFREG:0x7ED) PWMT6_2;
extern int chess_storage(PFREG:0x7EE) PWMT7_2;
extern int chess_storage(PFREG:0x7EF) PWMT8_2;
extern int chess_storage(PFREG:0x7F0) PWMT9_2;
extern int chess_storage(PFREG:0x7F1) PWMT10_2;
extern int chess_storage(PFREG:0x7F2) PWMT11_2;
//7F3h-7F8h
extern int chess_storage(PFREG:0x7F9) DIVSET_2;
extern int chess_storage(PFREG:0x7FA) PCVS0_2;
extern int chess_storage(PFREG:0x7FB) PCVS1_2;
extern int chess_storage(PFREG:0x7FC) PCVS2_2;
