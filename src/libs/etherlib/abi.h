#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    void clearABI(void);
    bool loadABI(const string_q& addr);
    bool loadABIFromFile(const string_q& fileName);
    bool loadABIFromCSV(const string_q& fileName);
    CFunction *findFunctionByName(const string_q& search);
    CFunction *findFunctionByEncoding(const string_q& search);
    friend class CAccount;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAbi& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAbi& ab);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbi::CAbi(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi::CAbi(const CAbi& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbi::~CAbi(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::clear(void) {
    // EXISTING_CODE
    abiByName.clear();
    abiByEncoding.clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::initialize(void) {
    CBaseNode::initialize();

    abiByName.clear();
    abiByEncoding.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::duplicate(const CAbi& ab) {
    clear();
    CBaseNode::duplicate(ab);

    abiByName = ab.abiByName;
    abiByEncoding = ab.abiByEncoding;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAbi& CAbi::operator=(const CAbi& ab) {
    duplicate(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
extern SFArchive& operator>>(SFArchive& archive, CAbiArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CAbiArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern uint64_t verbose;
#define REP_FREQ   11
#define REP_INFREQ 563
void clearAbis(void);
void rebuildFourByteDB(void);
CFunction *findFunctionByEncoding(const string_q& encoding);
// EXISTING_CODE
}  // namespace qblocks

