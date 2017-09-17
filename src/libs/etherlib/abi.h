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
#include "function.h"
#include "parameter.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAbi;
typedef SFArrayBase<CAbi>         CAbiArray;
typedef SFList<CAbi*>             CAbiList;
typedef SFUniqueList<CAbi*>       CAbiListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbi : public CBaseNode {
public:
    CFunctionArray abiByName;
    CFunctionArray abiByEncoding;

public:
    CAbi(void);
    CAbi(const CAbi& ab);
    virtual ~CAbi(void);
    CAbi& operator=(const CAbi& ab);

    DECLARE_NODE(CAbi);

    // EXISTING_CODE
    void clearABI(void);
    bool loadABI(const SFString& addr);
    bool loadABIFromFile(const SFString& fileName);
    CFunction *findFunctionByName(const SFString& search);
    CFunction *findFunctionByEncoding(const SFString& search);
    friend class CAccount;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAbi& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAbi& ab);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbi::CAbi(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi::CAbi(const CAbi& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbi::~CAbi(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::Clear(void) {
    // EXISTING_CODE
    abiByName.Clear();
    abiByEncoding.Clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::Init(void) {
    CBaseNode::Init();

    abiByName.Clear();
    abiByEncoding.Clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::Copy(const CAbi& ab) {
    Clear();
    CBaseNode::Copy(ab);

    abiByName = ab.abiByName;
    abiByEncoding = ab.abiByEncoding;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAbi& CAbi::operator=(const CAbi& ab) {
    Copy(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAbiArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAbiArray);
IMPLEMENT_ARCHIVE_LIST(CAbiList);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFUint32 verbose;
#define REP_FREQ   11
#define REP_INFREQ 563
void clearAbis(void);
// EXISTING_CODE
}  // namespace qblocks

