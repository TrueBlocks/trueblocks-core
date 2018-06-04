#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    void clearABI(void);
    bool loadABI(const SFString& addr);
    bool loadABIFromFile(const SFString& fileName);
    bool loadABIFromCSV(const SFString& fileName);
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
extern uint64_t verbose;
#define REP_FREQ   11
#define REP_INFREQ 563
void clearAbis(void);
void rebuildFourByteDB(void);
CFunction *findFunctionByEncoding(const SFString& encoding);
// EXISTING_CODE
}  // namespace qblocks

