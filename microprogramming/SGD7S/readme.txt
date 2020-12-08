*********************************************************************************************************
*																										*
*																										*
*		Revision History of MpgDebug Software															*
*																										*
*																										*
*********************************************************************************************************
変更履歴作成(MpgDebu_006.5以降)

=========================================================================================================
[ Version = MpgDebug_006.87 ]  2012.10.09 Y.Tanaka
◎変更点
・コメント修正（実行文に変更なし）

< Update Files >
・変更
	TstSrcC\MprgSgdv.c


=========================================================================================================
[ Version = MpgDebug_006.86 ]  2012.10.09 Y.Tanaka
◎変更点
・VC用リミット演算処理追加
　算術右シフト演算、加減演算にリミット判断処理を追加。
　有効/無効のコンパイルスイッチとしてDBG_LMTを使用する。(通常は有効)

・移植ミスを修正
　シフト処理、条件分岐等で∑-Ⅴマイクロ処理からの移植を間違っていた箇所を修正。


< Update Files >
・変更
	SysSrc\IlibSvc.c
	SysSrc\IlibSvc.h
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdvStruct.h
	MpgTstMain.c

・追加
	TstSrcC\MprgLmtChkVCMacro.h


=========================================================================================================
[ Version = MpgDebug_006.8 ]  2012.09.19 Y.Tanaka
◎変更点
・VC用の算術右シフト演算をライブラリに変更
　076命令の算術右シフトはシフト時に絶対値でシフトを行ない四捨五入も行なう。
　VCの通常の右シフトではそこに対応できないため、ライブラリ化した。
　またchessdeでもasr(src,imm)のイントリンシック命令となるので、
　ソース上は共通とし、コンパイルスイッチで定義が異なるような作りに今後対応する。

・算術右シフト演算ライブラリ追加
　上記のため、ライブラリを追加。実際の演算に合わせて64bitの演算まで用意。

SHORT	IlibASR16( SHORT src , SHORT sft );	/* 算術シフト演算16bit									*/
LONG	IlibASR32( LONG src , SHORT sft);	/* 算術シフト演算32bit									*/
INT64	IlibASR64( INT64 src , SHORT sft);	/* 算術シフト演算64bit									*/

< Update Files >
・変更
	SysSrc\IlibSvc.c
	SysSrc\IlibSvc.h
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	MpgTstMain.c


=========================================================================================================
[ Version = MpgDebug_006.7 ]  2012.09.18 Y.Tanaka
◎変更点
・シミュレーション対応
　SigmaWin+のデータトレースで取得したPJOG動作時の入力データ設定処理と
　電流制御部の出力データ取得処理を追加。
void	SetInitPrm( void );					/* 初期パラメータ設定処理				*/
void	SetTrqRef( int i );					/* スキャン毎の入力パラメータ設定		*/
void	GetADOut( int i );					/* 電流制御の出力結果取得				*/
void	MakeADOutFile( int i );				/* 出力結果のcsv出力					*/

・テーブルリード処理のコメントアウトしていたところを有効に変更


< Update Files >
・変更
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdvStruct.h
	MpgTstMain.c

・追加
	TstSrcC\MprgStruct.h
	TstSrcC\RefTbl.h

=========================================================================================================
[ Version = MpgDebug_006.6 ]  2012.09.10 Y.Tanaka
◎変更点
・シミュレーション入出力対応
　テスト用メインコードにパラメータセット処理（TstSetDataAD()）を追加。

・変数名変更
　マイクロの名残でLow/High分かれていたものの変数名からL/Hを削除。
　その他スペルミス等を修正。
変更1.INTADP
	TfIl			-->		Tfil
	MotRegst		-->		MotRegst
	DeadTimeComp	-->		OnDelaySlope

変更2.INTADWK
	sqrt			-->		sqrtwk

変更3.WEAKFV
	WfKpVL			-->		WfKpV
	WfKiVL			-->		WfKiV


・不要なリミット演算を削除
　リミット処理の不要な箇所でリミット演算をしていたところを通常の代入に変更。
※関連変更番号/文書：<V543>/900-028-459

・chessde/VC切り分け
　chessde用とVC用をコンパイルスイッチで切り替え。
　chessde	：#define	ASIP_CC
　VC		：#define	WIN32

・chessde用のレジスタ定義にリードレジスタ定義を追加
　0x700～0x7CFまでのレジスタ定義を追加。

・テーブルセット/リード処理（IxSetCtblAdr/IxLoadMpmem16）を削除

・VC用のリミット処理でフラグ変数確認をしないように変更
　フラグ変数(Flag)は擬似C言語処理で使っていたもので不要なため。
　変更前と区別するため関数名変更。
	IxLmtzImm16		-->		IxLmtZImm16
	IxLmtzReg16		-->		IxLmtZReg16
	IxLmtzImm32		-->		IxLmtZImm32
	IxLmtzReg32		-->		IxLmtZReg32

・その他省略可能な処理の記述を省略


< Update Files >
・変更
	SysSrc\IxInst.c
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdv_chessde.h
	TstSrcC\MprgSgdv_VC.h
	TstSrcC\MprgSgdvStruct.h
	MpgTstMain.c

・削除
	TstSrcC\MprgSgdv_ini.c
	TstSrcC\MprgSgdv_ini.h

・追加
	TstSrcC\MpConstTbl.h

=========================================================================================================
[ Version = MpgDebug_006.5 ]  2012.09.04 Y.Tanaka
◎変更点
・MpSQRT対応
　MpSQRT()をchessde用に変更していなかったため変更対応
　マイクロで左シフトの処理を右シフトで記述していた個所があったため修正

・共用体メンバ変数の型変更
　DLREG(64bit共用体)、DWREG(32bit共用体)にそれぞれunsignedのメンバを追加

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

#signed, unsignedホントにどちらも必要か要検討。

・平方根演算(MpSQRT)用ワークをグローバル変数に移動
　これに伴いSQRTWK構造体を追加。
　平方根演算は電流制御割込みのみで実行されているので、INTADWKのメンバ変数として追加。

< Update Files >
・変更
	SysSrc\IxInst.h
	TstSrcC\MprgSgdv.c
	TstSrcC\MprgSgdvStruct.h


