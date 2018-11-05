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
class CEscapeHatch : public CTransaction {
public:
    address_t _token;

public:
    CEscapeHatch(void);
    CEscapeHatch(const CEscapeHatch& es);
    virtual ~CEscapeHatch(void);
    CEscapeHatch& operator=(const CEscapeHatch& es);

    DECLARE_NODE(CEscapeHatch);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEscapeHatch& item) const;
    bool operator!=(const CEscapeHatch& item) const { return !operator==(item); }
    friend bool operator<(const CEscapeHatch& v1, const CEscapeHatch& v2);
    friend ostream& operator<<(ostream& os, const CEscapeHatch& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEscapeHatch& es);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEscapeHatch::CEscapeHatch(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEscapeHatch::CEscapeHatch(const CEscapeHatch& es) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(es);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEscapeHatch::~CEscapeHatch(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEscapeHatch::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEscapeHatch::initialize(void) {
    CTransaction::initialize();

    _token = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEscapeHatch::duplicate(const CEscapeHatch& es) {
    clear();
    CTransaction::duplicate(es);

    _token = es._token;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CEscapeHatch& CEscapeHatch::operator=(const CEscapeHatch& es) {
    duplicate(es);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEscapeHatch::operator==(const CEscapeHatch& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEscapeHatch& v1, const CEscapeHatch& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CEscapeHatch> CEscapeHatchArray;
extern CArchive& operator>>(CArchive& archive, CEscapeHatchArray& array);
extern CArchive& operator<<(CArchive& archive, const CEscapeHatchArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

