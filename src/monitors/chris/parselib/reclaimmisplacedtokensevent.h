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
class CReclaimMisplacedTokensEvent : public CLogEntry {
public:
    address_t _from;
    biguint_t _tokenAmount;

public:
    CReclaimMisplacedTokensEvent(void);
    CReclaimMisplacedTokensEvent(const CReclaimMisplacedTokensEvent& re);
    virtual ~CReclaimMisplacedTokensEvent(void);
    CReclaimMisplacedTokensEvent& operator=(const CReclaimMisplacedTokensEvent& re);

    DECLARE_NODE(CReclaimMisplacedTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CReclaimMisplacedTokensEvent& item) const;
    bool operator!=(const CReclaimMisplacedTokensEvent& item) const { return !operator==(item); }
    friend bool operator<(const CReclaimMisplacedTokensEvent& v1, const CReclaimMisplacedTokensEvent& v2);
    friend ostream& operator<<(ostream& os, const CReclaimMisplacedTokensEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CReclaimMisplacedTokensEvent& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokensEvent::CReclaimMisplacedTokensEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokensEvent::CReclaimMisplacedTokensEvent(const CReclaimMisplacedTokensEvent& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokensEvent::~CReclaimMisplacedTokensEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReclaimMisplacedTokensEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReclaimMisplacedTokensEvent::initialize(void) {
    CLogEntry::initialize();

    _from = "";
    _tokenAmount = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReclaimMisplacedTokensEvent::duplicate(const CReclaimMisplacedTokensEvent& re) {
    clear();
    CLogEntry::duplicate(re);

    _from = re._from;
    _tokenAmount = re._tokenAmount;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CReclaimMisplacedTokensEvent& CReclaimMisplacedTokensEvent::operator=(const CReclaimMisplacedTokensEvent& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CReclaimMisplacedTokensEvent::operator==(const CReclaimMisplacedTokensEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CReclaimMisplacedTokensEvent& v1, const CReclaimMisplacedTokensEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CReclaimMisplacedTokensEvent> CReclaimMisplacedTokensEventArray;
extern CArchive& operator>>(CArchive& archive, CReclaimMisplacedTokensEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CReclaimMisplacedTokensEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

