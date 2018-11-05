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
class CCancelExistingRequest : public CTransaction {
public:
    biguint_t _key;

public:
    CCancelExistingRequest(void);
    CCancelExistingRequest(const CCancelExistingRequest& ca);
    virtual ~CCancelExistingRequest(void);
    CCancelExistingRequest& operator=(const CCancelExistingRequest& ca);

    DECLARE_NODE(CCancelExistingRequest);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCancelExistingRequest& item) const;
    bool operator!=(const CCancelExistingRequest& item) const { return !operator==(item); }
    friend bool operator<(const CCancelExistingRequest& v1, const CCancelExistingRequest& v2);
    friend ostream& operator<<(ostream& os, const CCancelExistingRequest& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCancelExistingRequest& ca);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCancelExistingRequest::CCancelExistingRequest(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCancelExistingRequest::CCancelExistingRequest(const CCancelExistingRequest& ca) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ca);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCancelExistingRequest::~CCancelExistingRequest(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelExistingRequest::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelExistingRequest::initialize(void) {
    CTransaction::initialize();

    _key = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCancelExistingRequest::duplicate(const CCancelExistingRequest& ca) {
    clear();
    CTransaction::duplicate(ca);

    _key = ca._key;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CCancelExistingRequest& CCancelExistingRequest::operator=(const CCancelExistingRequest& ca) {
    duplicate(ca);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCancelExistingRequest::operator==(const CCancelExistingRequest& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCancelExistingRequest& v1, const CCancelExistingRequest& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CCancelExistingRequest> CCancelExistingRequestArray;
extern CArchive& operator>>(CArchive& archive, CCancelExistingRequestArray& array);
extern CArchive& operator<<(CArchive& archive, const CCancelExistingRequestArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

