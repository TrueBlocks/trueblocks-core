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
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class COnAffiliatePayoutEvent : public CLogEntry {
public:
    biguint_t affiliateID;
    address_t affiliateAddress;
    string_q affiliateName;
    biguint_t roundID;
    biguint_t buyerID;
    biguint_t amount;
    biguint_t timeStamp;

public:
    COnAffiliatePayoutEvent(void);
    COnAffiliatePayoutEvent(const COnAffiliatePayoutEvent& on);
    virtual ~COnAffiliatePayoutEvent(void);
    COnAffiliatePayoutEvent& operator=(const COnAffiliatePayoutEvent& on);

    DECLARE_NODE(COnAffiliatePayoutEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const COnAffiliatePayoutEvent& item) const;
    bool operator!=(const COnAffiliatePayoutEvent& item) const { return !operator==(item); }
    friend bool operator<(const COnAffiliatePayoutEvent& v1, const COnAffiliatePayoutEvent& v2);
    friend ostream& operator<<(ostream& os, const COnAffiliatePayoutEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const COnAffiliatePayoutEvent& on);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline COnAffiliatePayoutEvent::COnAffiliatePayoutEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline COnAffiliatePayoutEvent::COnAffiliatePayoutEvent(const COnAffiliatePayoutEvent& on) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(on);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline COnAffiliatePayoutEvent::~COnAffiliatePayoutEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnAffiliatePayoutEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnAffiliatePayoutEvent::initialize(void) {
    CLogEntry::initialize();

    affiliateID = 0;
    affiliateAddress = "";
    affiliateName = "";
    roundID = 0;
    buyerID = 0;
    amount = 0;
    timeStamp = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void COnAffiliatePayoutEvent::duplicate(const COnAffiliatePayoutEvent& on) {
    clear();
    CLogEntry::duplicate(on);

    affiliateID = on.affiliateID;
    affiliateAddress = on.affiliateAddress;
    affiliateName = on.affiliateName;
    roundID = on.roundID;
    buyerID = on.buyerID;
    amount = on.amount;
    timeStamp = on.timeStamp;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline COnAffiliatePayoutEvent& COnAffiliatePayoutEvent::operator=(const COnAffiliatePayoutEvent& on) {
    duplicate(on);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool COnAffiliatePayoutEvent::operator==(const COnAffiliatePayoutEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const COnAffiliatePayoutEvent& v1, const COnAffiliatePayoutEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<COnAffiliatePayoutEvent> COnAffiliatePayoutEventArray;
extern CArchive& operator>>(CArchive& archive, COnAffiliatePayoutEventArray& array);
extern CArchive& operator<<(CArchive& archive, const COnAffiliatePayoutEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

