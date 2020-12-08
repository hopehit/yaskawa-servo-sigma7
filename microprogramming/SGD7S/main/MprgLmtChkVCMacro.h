#define	IxADDSUBLMTCHKRDY( SrcA, SrcB )		/* �����Z���~�b�g�`�F�b�N�����}�N��		*/\
	lmtBufsign[0] = (ULONG)(SrcA) >> 31;		/* �O�������m�F		*/\
	lmtBufsign[1] = (ULONG)(SrcB) >> 31;		/* �㍀�����m�F		*/\
	if((SrcA) > (SrcB) )	/* �召��r	*/\
	{\
		lmtBuf.l = (SrcA);	/* �傫������ۑ�	*/\
		lmtBufSw = 0;		/* �O����ۑ�	*/\
	}\
	else\
	{\
		lmtBuf.l = (SrcB);	/* �傫������ۑ�	*/\
		lmtBufSw = 1;	/* �㍀��ۑ�	*/\
	}

#define	IxADDLMTCHK( result )	/* ���Z���~�b�g���ʏ���		*/\
	if( lmtBufsign[0] == lmtBufsign[1] )		/* �e���̕�����r	*/\
	{/* �����ꍇ	*/\
		if( lmtBufsign[0] == 0 )\
		{/* ���̐�		*/\
			if( lmtBuf.l > (result) )\
			{\
				Flag.Carry = 1;		/* �L�����[����		*/\
				Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
				Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
			}\
		}\
		else\
		{/* ���̐�		*/\
			if( lmtBuf.l < (result) )\
			{\
				Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
				Flag.Borrow = 1;	/* �{���[����		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
				Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
			}\
		}\
	}\
	else\
	{\
		Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
		Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
	}


#define	IxSUBLMTCHK( result )	/* ���Z���~�b�g���ʏ���		*/\
	if( lmtBufsign[0] != lmtBufsign[1] )		/* �e���̕�����r	*/\
	{/* �ٕ����̏ꍇ		*/\
		if( lmtBufSw == 0 )	/* �O���ۑ��̏ꍇ	*/\
		{\
			if( lmtBuf.l > (result) )\
			{\
				Flag.Carry = 1;		/* �L�����[����		*/\
				Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
				Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
			}\
		}\
		else	/* �㍀�ۑ��̏ꍇ	*/\
		{\
			if( (result) > 0 )\
			{\
				Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
				Flag.Borrow = 1;	/* �{���[����		*/\
			}\
			else\
			{\
				Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
				Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
			}\
		}\
	}\
	else\
	{\
		Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
		Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
	}

#define	IxUSUBLMTCHK( result )	/* ���Z���~�b�g���ʏ���(unsigned�̂Ƃ�)		*/\
	if( lmtBufSw == 1 )	/* �㍀�ۑ��̏ꍇ	*/\
	{\
		Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
		Flag.Borrow = 1;	/* �{���[����		*/\
	}\
	else\
	{\
		Flag.Carry = 0;		/* �L�����[���Z�b�g		*/\
		Flag.Borrow = 0;	/* �{���[���Z�b�g		*/\
	}


