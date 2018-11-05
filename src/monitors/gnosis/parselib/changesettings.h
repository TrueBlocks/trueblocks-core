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
class CChangeSettings : public CTransaction {
public:
    biguint_t _ceiling;
    biguint_t _priceFactor;

public:
    CChangeSettings(void);
    CChangeSettings(const CChangeSettings& ch);
    virtual ~CChangeSettings(void);
    CChangeSettings& operator=(const CChangeSettings& ch);

    DECLARE_NODE(CChangeSettings);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeSettings& item) const;
    bool operator!=(const CChangeSettings& item) const { return !operator==(item); }
    friend bool operator<(const CChangeSettings& v1, const CChangeSettings& v2);
    friend ostream& operator<<(ostream& os, const CChangeSettings& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeSettings& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeSettings::CChangeSettings(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeSettings::CChangeSettings(const CChangeSettings& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeSettings::~CChangeSettings(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeSettings::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeSettings::initialize(void) {
    CTransaction::initialize();

    _ceiling = 0;
    _priceFactor = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeSettings::duplicate(const CChangeSettings& ch) {
    clear();
    CTransaction::duplicate(ch);

    _ceiling = ch._ceiling;
    _priceFactor = ch._priceFactor;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeSettings& CChangeSettings::operator=(const CChangeSettings& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeSettings::operator==(const CChangeSettings& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeSettings& v1, const CChangeSettings& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeSettings> CChangeSettingsArray;
extern CArchive& operator>>(CArchive& archive, CChangeSettingsArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeSettingsArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

