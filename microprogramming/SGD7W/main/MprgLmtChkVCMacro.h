#define	IxADDSUBLMTCHKRDY( SrcA, SrcB )		/* 加減算リミットチェック準備マクロ		*/\
	lmtBufsign[0] = (ULONG)(SrcA) >> 31;		/* 前項符号確認		*/\
	lmtBufsign[1] = (ULONG)(SrcB) >> 31;		/* 後項符号確認		*/\
	if((SrcA) > (SrcB) )	/* 大小比較	*/\
	{\
		lmtBuf.l = (SrcA);	/* 大きい方を保存	*/\
		lmtBufSw = 0;		/* 前項を保存	*/\
	}\
	else\
	{\
		lmtBuf.l = (SrcB);	/* 大きい方を保存	*/\
		lmtBufSw = 1;	/* 後項を保存	*/\
	}

#define	IxADDLMTCHK( result )	/* 加算リミット判別処理		*/\
	if( lmtBufsign[0] == lmtBufsign[1] )		/* 各項の符号比較	*/\
	{/* 同じ場合	*/\
		if( lmtBufsign[0] == 0 )\
		{/* 正の数		*/\
			if( lmtBuf.l > (result) )\
			{\
				Flag.Carry = 1;		/* キャリー発生		*/\
				Flag.Borrow = 0;	/* ボローリセット		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* キャリーリセット		*/\
				Flag.Borrow = 0;	/* ボローリセット		*/\
			}\
		}\
		else\
		{/* 負の数		*/\
			if( lmtBuf.l < (result) )\
			{\
				Flag.Carry = 0;		/* キャリーリセット		*/\
				Flag.Borrow = 1;	/* ボロー発生		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* キャリーリセット		*/\
				Flag.Borrow = 0;	/* ボローリセット		*/\
			}\
		}\
	}\
	else\
	{\
		Flag.Carry = 0;		/* キャリーリセット		*/\
		Flag.Borrow = 0;	/* ボローリセット		*/\
	}


#define	IxSUBLMTCHK( result )	/* 減算リミット判別処理		*/\
	if( lmtBufsign[0] != lmtBufsign[1] )		/* 各項の符号比較	*/\
	{/* 異符号の場合		*/\
		if( lmtBufSw == 0 )	/* 前項保存の場合	*/\
		{\
			if( lmtBuf.l > (result) )\
			{\
				Flag.Carry = 1;		/* キャリー発生		*/\
				Flag.Borrow = 0;	/* ボローリセット		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* キャリーリセット		*/\
				Flag.Borrow = 0;	/* ボローリセット		*/\
			}\
		}\
		else	/* 後項保存の場合	*/\
		{\
			if( (result) > 0 )\
			{\
				Flag.Carry = 0;		/* キャリーリセット		*/\
				Flag.Borrow = 1;	/* ボロー発生		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* キャリーリセット		*/\
				Flag.Borrow = 0;	/* ボローリセット		*/\
			}\
		}\
	}\
	else\
	{\
		Flag.Carry = 0;		/* キャリーリセット		*/\
		Flag.Borrow = 0;	/* ボローリセット		*/\
	}

#define	IxUSUBLMTCHK( result )	/* 減算リミット判別処理(unsignedのとき)		*/\
	if( lmtBufSw == 1 )	/* 後項保存の場合	*/\
	{\
		Flag.Carry = 0;		/* キャリーリセット		*/\
		Flag.Borrow = 1;	/* ボロー発生		*/\
	}\
	else\
	{\
		Flag.Carry = 0;		/* キャリーリセット		*/\
		Flag.Borrow = 0;	/* ボローリセット		*/\
	}


