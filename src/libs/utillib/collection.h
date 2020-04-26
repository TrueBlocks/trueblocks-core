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
#include "basetypes.h"
#include "basenode.h"
#include "conversions.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CCollection : public CBaseNode {
  public:
    string_q id;
    string_q tags;
    string_q name;
    string_q client;
    bool monitored;
    bool deleted;
    uint64_t sizeInBytes;
    CAddressArray addresses;
    string_q addressList;

  public:
    CCollection(void);
    CCollection(const CCollection& co);
    virtual ~CCollection(void);
    CCollection& operator=(const CCollection& co);

    DECLARE_NODE(CCollection);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CCollection& item) const;
    bool operator!=(const CCollection& item) const {
        return !operator==(item);
    }
    friend bool operator<(const CCollection& v1, const CCollection& v2);
    friend ostream& operator<<(ostream& os, const CCollection& item);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CCollection& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCollection::CCollection(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCollection::CCollection(const CCollection& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCollection::~CCollection(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollection::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollection::initialize(void) {
    CBaseNode::initialize();

    id = "";
    tags = "";
    name = "";
    client = "";
    monitored = false;
    deleted = false;
    sizeInBytes = 0;
    addresses.clear();
    addressList = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCollection::duplicate(const CCollection& co) {
    clear();
    CBaseNode::duplicate(co);

    id = co.id;
    tags = co.tags;
    name = co.name;
    client = co.client;
    monitored = co.monitored;
    deleted = co.deleted;
    sizeInBytes = co.sizeInBytes;
    addresses = co.addresses;
    addressList = co.addressList;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCollection& CCollection::operator=(const CCollection& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CCollection::operator==(const CCollection& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return addressList % item.addressList;
}

//-------------------------------------------------------------------------
inline bool operator<(const CCollection& v1, const CCollection& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.name < v2.name;
}

//---------------------------------------------------------------------------
typedef vector<CCollection> CCollectionArray;
extern CArchive& operator>>(CArchive& archive, CCollectionArray& array);
extern CArchive& operator<<(CArchive& archive, const CCollectionArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_COLLECTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
