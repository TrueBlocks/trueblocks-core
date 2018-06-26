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
#include <vector>
#include "abilib.h"
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNewReceipt : public CBaseNode {
public:
    SFAddress contractAddress;
    SFGas gasUsed;
    CLogEntryArray logs;
    SFBloom logsBloom;
    bool isError;

public:
    CNewReceipt(void);
    CNewReceipt(const CNewReceipt& ne);
    virtual ~CNewReceipt(void);
    CNewReceipt& operator=(const CNewReceipt& ne);

    DECLARE_NODE(CNewReceipt);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    friend class CTransaction;
    // EXISTING_CODE
    friend bool operator<(const CNewReceipt& v1, const CNewReceipt& v2);
    friend ostream& operator<<(ostream& os, const CNewReceipt& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CNewReceipt& ne);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNewReceipt::CNewReceipt(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNewReceipt::CNewReceipt(const CNewReceipt& ne) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ne);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNewReceipt::~CNewReceipt(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewReceipt::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewReceipt::initialize(void) {
    CBaseNode::initialize();

    contractAddress = "";
    gasUsed = 0;
    logs.clear();
    logsBloom = 0;
    isError = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNewReceipt::duplicate(const CNewReceipt& ne) {
    clear();
    CBaseNode::duplicate(ne);

    contractAddress = ne.contractAddress;
    gasUsed = ne.gasUsed;
    logs = ne.logs;
    logsBloom = ne.logsBloom;
    isError = ne.isError;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNewReceipt& CNewReceipt::operator=(const CNewReceipt& ne) {
    duplicate(ne);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const CNewReceipt& v1, const CNewReceipt& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CNewReceipt> CNewReceiptArray;
extern SFArchive& operator>>(SFArchive& archive, CNewReceiptArray& array);
extern SFArchive& operator<<(SFArchive& archive, const CNewReceiptArray& array);

//---------------------------------------------------------------------------
extern SFArchive& operator<<(SFArchive& archive, const CNewReceipt& newp);
extern SFArchive& operator>>(SFArchive& archive, CNewReceipt& newp);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

