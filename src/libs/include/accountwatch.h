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
class CAccountWatch;
typedef SFArrayBase<CAccountWatch>         CAccountWatchArray;
typedef SFList<CAccountWatch*>             CAccountWatchList;
typedef SFUniqueList<CAccountWatch*>       CAccountWatchListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountWatch : public CBaseNode
{
public:
    SFUint32 index;
    SFAddress address;
    SFString name;
    SFString color;
    SFUint32 firstBlock;
    bool disabled;
    SFIntBN inFlows;
    SFIntBN outFlows;
    SFIntBN corrections;
    SFUintBN actBal;

public:
    CAccountWatch(void);
    CAccountWatch(const CAccountWatch& ac);
   ~CAccountWatch(void);
    CAccountWatch& operator=(const CAccountWatch& ac);

    DECLARE_NODE (CAccountWatch);

    // EXISTING_CODE
    CAccountWatch(uint32_t _id, const SFString& _addr, const SFString& _name, blknum_t fB, const SFString& _color)
        : index(_id), address(toLower(_addr)), name(_name), color(_color), firstBlock(fB),
          disabled(false), inFlows(0), outFlows(0), actBal(0) {}
    SFIntBN balance(void) const;
    SFIntBN correctedBal(void) const;
    void correctShun(SFIntBN cor) const;
    void flowIn(SFIntBN _amt) const;
    void flowOut(SFIntBN _amt) const;
    bool getWatch(const CToml& toml, uint32_t n);
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountWatch& ac);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const CAccountWatch& ac)
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountWatch::~CAccountWatch(void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Init(void)
{
    CBaseNode::Init();

    index = 0;
//    address = EMPTY;
//    name = EMPTY;
//    color = EMPTY;
    firstBlock = 0;
    disabled = 0;
    inFlows = 0;
    outFlows = 0;
    corrections = 0;
    actBal = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Copy(const CAccountWatch& ac)
{
    Clear();
    CBaseNode::Copy(ac);

    index = ac.index;
    address = ac.address;
    name = ac.name;
    color = ac.color;
    firstBlock = ac.firstBlock;
    disabled = ac.disabled;
    inFlows = ac.inFlows;
    outFlows = ac.outFlows;
    corrections = ac.corrections;
    actBal = ac.actBal;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountWatch& CAccountWatch::operator=(const CAccountWatch& ac)
{
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CAccountWatch::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountWatchArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountWatchArray);
IMPLEMENT_ARCHIVE_LIST(CAccountWatchList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
