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
#include "etherlib.h"
#include "configgroup.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfigFile : public CBaseNode {
public:
    string_q name;
    CConfigGroupArray groups;

public:
    CConfigFile(void);
    CConfigFile(const CConfigFile& co);
    virtual ~CConfigFile(void);
    CConfigFile& operator=(const CConfigFile& co);

    DECLARE_NODE(CConfigFile);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    explicit CConfigFile(const string_q& n) : name(n) { }
    // EXISTING_CODE
    bool operator==(const CConfigFile& item) const;
    bool operator!=(const CConfigFile& item) const { return !operator==(item); }
    friend bool operator<(const CConfigFile& v1, const CConfigFile& v2);
    friend ostream& operator<<(ostream& os, const CConfigFile& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfigFile& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfigFile::CConfigFile(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigFile::CConfigFile(const CConfigFile& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfigFile::~CConfigFile(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigFile::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigFile::initialize(void) {
    CBaseNode::initialize();

    name = "";
    groups.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfigFile::duplicate(const CConfigFile& co) {
    clear();
    CBaseNode::duplicate(co);

    name = co.name;
    groups = co.groups;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfigFile& CConfigFile::operator=(const CConfigFile& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfigFile::operator==(const CConfigFile& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfigFile& v1, const CConfigFile& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfigFile> CConfigFileArray;
extern CArchive& operator>>(CArchive& archive, CConfigFileArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfigFileArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CConfigFile& con);
extern CArchive& operator>>(CArchive& archive, CConfigFile& con);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

