#if 0
#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "ethtypes.h"
#include "abilib.h"

//--------------------------------------------------------------------------
class CStructLog;
typedef SFArrayBase<CStructLog>         CStructLogArray;
typedef SFList<CStructLog*>             CStructLogList;
typedef SFUniqueList<CStructLog*>       CStructLogListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CStructLog : public CBaseNode
{
public:
	SFUint32 depth;
	SFUint32 error;
	SFUint32 gas;
	SFUint32 gasCost;
	SFStringArray memory;
	SFString op;
	SFUint32 pc;
	SFStringArray stack;
	SFStringArray storage;

public:
		CStructLog(void);
		CStructLog(const CStructLog& st);
	       ~CStructLog(void);
	CStructLog&	operator=(const CStructLog& st);

	DECLARE_NODE (CStructLog);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void Clear(void);
	void Init(void);
	void Copy(const CStructLog& st);
	bool readBackLevel(SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStructLog::CStructLog(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStructLog::CStructLog(const CStructLog& st)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStructLog::~CStructLog(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStructLog::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStructLog::Init(void)
{
	CBaseNode::Init();

	depth = 0;
	error = 0;
	gas = 0;
	gasCost = 0;
//	memory = ??; /* unknown type: SFStringArray */
//	op = EMPTY;
	pc = 0;
//	stack = ??; /* unknown type: SFStringArray */
//	storage = ??; /* unknown type: SFStringArray */

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStructLog::Copy(const CStructLog& st)
{
	Clear();
	CBaseNode::Copy(st);

	depth = st.depth;
	error = st.error;
	gas = st.gas;
	gasCost = st.gasCost;
	memory = st.memory;
	op = st.op;
	pc = st.pc;
	stack = st.stack;
	storage = st.storage;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CStructLog& CStructLog::operator=(const CStructLog& st)
{
	Copy(st);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CStructLog::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextStructlogChunk(const SFString& fieldIn, bool& force, const void *data);
extern SFString nextStructlogChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CStructLogArray);
IMPLEMENT_ARCHIVE_LIST(CStructLogList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#endif
