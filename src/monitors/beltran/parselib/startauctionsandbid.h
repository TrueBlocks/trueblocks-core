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
class CStartAuctionsAndBid : public CTransaction {
public:
    CStringArray hashes;
    string_q sealedBid;

public:
    CStartAuctionsAndBid(void);
    CStartAuctionsAndBid(const CStartAuctionsAndBid& st);
    virtual ~CStartAuctionsAndBid(void);
    CStartAuctionsAndBid& operator=(const CStartAuctionsAndBid& st);

    DECLARE_NODE(CStartAuctionsAndBid);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStartAuctionsAndBid& item) const;
    bool operator!=(const CStartAuctionsAndBid& item) const { return !operator==(item); }
    friend bool operator<(const CStartAuctionsAndBid& v1, const CStartAuctionsAndBid& v2);
    friend ostream& operator<<(ostream& os, const CStartAuctionsAndBid& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStartAuctionsAndBid& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStartAuctionsAndBid::CStartAuctionsAndBid(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStartAuctionsAndBid::CStartAuctionsAndBid(const CStartAuctionsAndBid& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStartAuctionsAndBid::~CStartAuctionsAndBid(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuctionsAndBid::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuctionsAndBid::initialize(void) {
    CTransaction::initialize();

    hashes.clear();
    sealedBid = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStartAuctionsAndBid::duplicate(const CStartAuctionsAndBid& st) {
    clear();
    CTransaction::duplicate(st);

    hashes = st.hashes;
    sealedBid = st.sealedBid;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStartAuctionsAndBid& CStartAuctionsAndBid::operator=(const CStartAuctionsAndBid& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStartAuctionsAndBid::operator==(const CStartAuctionsAndBid& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStartAuctionsAndBid& v1, const CStartAuctionsAndBid& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStartAuctionsAndBid> CStartAuctionsAndBidArray;
extern CArchive& operator>>(CArchive& archive, CStartAuctionsAndBidArray& array);
extern CArchive& operator<<(CArchive& archive, const CStartAuctionsAndBidArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

