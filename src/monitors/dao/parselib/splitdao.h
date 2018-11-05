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
class CSplitDAO : public CTransaction {
public:
    biguint_t _proposalID;
    address_t _newCurator;

public:
    CSplitDAO(void);
    CSplitDAO(const CSplitDAO& sp);
    virtual ~CSplitDAO(void);
    CSplitDAO& operator=(const CSplitDAO& sp);

    DECLARE_NODE(CSplitDAO);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSplitDAO& item) const;
    bool operator!=(const CSplitDAO& item) const { return !operator==(item); }
    friend bool operator<(const CSplitDAO& v1, const CSplitDAO& v2);
    friend ostream& operator<<(ostream& os, const CSplitDAO& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSplitDAO& sp);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSplitDAO::CSplitDAO(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSplitDAO::CSplitDAO(const CSplitDAO& sp) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(sp);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSplitDAO::~CSplitDAO(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSplitDAO::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSplitDAO::initialize(void) {
    CTransaction::initialize();

    _proposalID = 0;
    _newCurator = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSplitDAO::duplicate(const CSplitDAO& sp) {
    clear();
    CTransaction::duplicate(sp);

    _proposalID = sp._proposalID;
    _newCurator = sp._newCurator;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSplitDAO& CSplitDAO::operator=(const CSplitDAO& sp) {
    duplicate(sp);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSplitDAO::operator==(const CSplitDAO& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSplitDAO& v1, const CSplitDAO& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSplitDAO> CSplitDAOArray;
extern CArchive& operator>>(CArchive& archive, CSplitDAOArray& array);
extern CArchive& operator<<(CArchive& archive, const CSplitDAOArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

