#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include <map>
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBuyRecipient : public CTransaction {
public:
    address_t recipient;
    uint32_t v;
    string_q r;
    string_q s;

public:
    CBuyRecipient(void);
    CBuyRecipient(const CBuyRecipient& bu);
    virtual ~CBuyRecipient(void);
    CBuyRecipient& operator=(const CBuyRecipient& bu);

    DECLARE_NODE(CBuyRecipient);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CBuyRecipient& item) const;
    bool operator!=(const CBuyRecipient& item) const { return !operator==(item); }
    friend bool operator<(const CBuyRecipient& v1, const CBuyRecipient& v2);
    friend ostream& operator<<(ostream& os, const CBuyRecipient& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBuyRecipient& bu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBuyRecipient::CBuyRecipient(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBuyRecipient::CBuyRecipient(const CBuyRecipient& bu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBuyRecipient::~CBuyRecipient(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyRecipient::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyRecipient::initialize(void) {
    CTransaction::initialize();

    recipient = "";
    v = 0;
    r = "";
    s = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBuyRecipient::duplicate(const CBuyRecipient& bu) {
    clear();
    CTransaction::duplicate(bu);

    recipient = bu.recipient;
    v = bu.v;
    r = bu.r;
    s = bu.s;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CBuyRecipient& CBuyRecipient::operator=(const CBuyRecipient& bu) {
    duplicate(bu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBuyRecipient::operator==(const CBuyRecipient& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBuyRecipient& v1, const CBuyRecipient& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBuyRecipient> CBuyRecipientArray;
extern CArchive& operator>>(CArchive& archive, CBuyRecipientArray& array);
extern CArchive& operator<<(CArchive& archive, const CBuyRecipientArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

