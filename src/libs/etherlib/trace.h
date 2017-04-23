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
#include "structlog.h"

//--------------------------------------------------------------------------
class CTrace;
typedef SFArrayBase<CTrace>         CTraceArray;
typedef SFList<CTrace*>             CTraceList;
typedef SFUniqueList<CTrace*>       CTraceListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTrace : public CBaseNode
{
public:
	SFUint32 gas;
	SFString returnValue;
	CStructLogArray structLogs;

public:
		CTrace(void);
		CTrace(const CTrace& tr);
	       ~CTrace(void);
	CTrace&	operator=(const CTrace& tr);

	DECLARE_NODE (CTrace);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void Clear(void);
	void Init(void);
	void Copy(const CTrace& tr);
	bool readBackLevel(SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTrace::CTrace(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTrace::CTrace(const CTrace& tr)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTrace::~CTrace(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Init(void)
{
	CBaseNode::Init();

	gas = 0;
//	returnValue = EMPTY;
//	structLogs = ??; /* unknown type: CStructLogArray */

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTrace::Copy(const CTrace& tr)
{
	Clear();
	CBaseNode::Copy(tr);

	gas = tr.gas;
	returnValue = tr.returnValue;
	structLogs = tr.structLogs;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CTrace& CTrace::operator=(const CTrace& tr)
{
	Copy(tr);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CTrace::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextTraceChunk(const SFString& fieldIn, bool& force, const void *data);
extern SFString nextTraceChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTraceArray);
IMPLEMENT_ARCHIVE_LIST(CTraceList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#endif
