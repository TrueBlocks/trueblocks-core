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
class CSetFormula : public CTransaction {
public:
    address_t _formula;

public:
    CSetFormula(void);
    CSetFormula(const CSetFormula& se);
    virtual ~CSetFormula(void);
    CSetFormula& operator=(const CSetFormula& se);

    DECLARE_NODE(CSetFormula);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSetFormula& item) const;
    bool operator!=(const CSetFormula& item) const { return !operator==(item); }
    friend bool operator<(const CSetFormula& v1, const CSetFormula& v2);
    friend ostream& operator<<(ostream& os, const CSetFormula& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSetFormula& se);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSetFormula::CSetFormula(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSetFormula::CSetFormula(const CSetFormula& se) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(se);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSetFormula::~CSetFormula(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetFormula::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetFormula::initialize(void) {
    CTransaction::initialize();

    _formula = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSetFormula::duplicate(const CSetFormula& se) {
    clear();
    CTransaction::duplicate(se);

    _formula = se._formula;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSetFormula& CSetFormula::operator=(const CSetFormula& se) {
    duplicate(se);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSetFormula::operator==(const CSetFormula& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSetFormula& v1, const CSetFormula& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSetFormula> CSetFormulaArray;
extern CArchive& operator>>(CArchive& archive, CSetFormulaArray& array);
extern CArchive& operator<<(CArchive& archive, const CSetFormulaArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

