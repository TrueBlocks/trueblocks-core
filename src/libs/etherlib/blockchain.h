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
#include "block.h"
#include "etherlib.h"

//--------------------------------------------------------------------------
class CBlockChain;
typedef SFArrayBase<CBlockChain>         CBlockChainArray;
typedef SFList<CBlockChain*>             CBlockChainList;
typedef SFUniqueList<CBlockChain*>       CBlockChainListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBlockChain : public CBaseNode
{
public:
	CBlockArray blocks;

public:
		CBlockChain(void);
		CBlockChain(const CBlockChain& bl);
	       ~CBlockChain(void);
	CBlockChain&	operator=(const CBlockChain& bl);

	DECLARE_NODE (CBlockChain);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void Clear(void);
	void Init(void);
	void Copy(const CBlockChain& bl);
	bool readBackLevel(SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBlockChain::CBlockChain(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlockChain::CBlockChain(const CBlockChain& bl)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(bl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBlockChain::~CBlockChain(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockChain::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockChain::Init(void)
{
	CBaseNode::Init();

//	blocks = ??; /* unknown type: CBlockArray */

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockChain::Copy(const CBlockChain& bl)
{
	Clear();
	CBaseNode::Copy(bl);

	blocks = bl.blocks;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CBlockChain& CBlockChain::operator=(const CBlockChain& bl)
{
	Copy(bl);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CBlockChain::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextBlockchainChunk(const SFString& fieldIn, bool& force, const void *data);
extern SFString nextBlockchainChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CBlockChainArray);
IMPLEMENT_ARCHIVE_LIST(CBlockChainList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
#endif
