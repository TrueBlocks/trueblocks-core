#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CIpfshash : public CBaseNode {
  public:
    hash_t hash;

  public:
    CIpfshash(void);
    CIpfshash(const CIpfshash& ip);
    virtual ~CIpfshash(void);
    CIpfshash& operator=(const CIpfshash& ip);

    DECLARE_NODE(CIpfshash);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CIpfshash& item) const;
    bool operator!=(const CIpfshash& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CIpfshash& v1, const CIpfshash& v2);
    friend ostream& operator<<(ostream& os, const CIpfshash& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CIpfshash& ip);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CIpfshash::CIpfshash(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIpfshash::CIpfshash(const CIpfshash& ip) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ip);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CIpfshash::~CIpfshash(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIpfshash::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIpfshash::initialize(void) {
    CBaseNode::initialize();

    hash = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CIpfshash::duplicate(const CIpfshash& ip) {
    clear();
    CBaseNode::duplicate(ip);

    hash = ip.hash;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CIpfshash& CIpfshash::operator=(const CIpfshash& ip) {
    duplicate(ip);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CIpfshash::operator==(const CIpfshash& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CIpfshash& v1, const CIpfshash& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CIpfshash> CIpfshashArray;
extern CArchive& operator>>(CArchive& archive, CIpfshashArray& array);
extern CArchive& operator<<(CArchive& archive, const CIpfshashArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_IPFSHASH;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
