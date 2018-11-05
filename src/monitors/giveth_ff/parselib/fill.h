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
class CFill : public CTransaction {
public:
    CBigUintArray data;

public:
    CFill(void);
    CFill(const CFill& fi);
    virtual ~CFill(void);
    CFill& operator=(const CFill& fi);

    DECLARE_NODE(CFill);

    const string_q getStringAt(const string_q& name, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFill& item) const;
    bool operator!=(const CFill& item) const { return !operator==(item); }
    friend bool operator<(const CFill& v1, const CFill& v2);
    friend ostream& operator<<(ostream& os, const CFill& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFill& fi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFill::CFill(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFill::CFill(const CFill& fi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFill::~CFill(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFill::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFill::initialize(void) {
    CTransaction::initialize();

    data.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFill::duplicate(const CFill& fi) {
    clear();
    CTransaction::duplicate(fi);

    data = fi.data;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFill& CFill::operator=(const CFill& fi) {
    duplicate(fi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFill::operator==(const CFill& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFill& v1, const CFill& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFill> CFillArray;
extern CArchive& operator>>(CArchive& archive, CFillArray& array);
extern CArchive& operator<<(CArchive& archive, const CFillArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

