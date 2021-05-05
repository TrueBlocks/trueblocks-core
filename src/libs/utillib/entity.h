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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "basetypes.h"
#include "basenode.h"
#include "conversions.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CEntity : public CBaseNode {
  public:
    string_q cid;
    string_q tags;
    string_q name;
    string_q client;
    bool monitored;
    bool deleted;
    uint64_t sizeInBytes;
    CAddressArray addresses;
    string_q addressList;

  public:
    CEntity(void);
    CEntity(const CEntity& en);
    virtual ~CEntity(void);
    CEntity& operator=(const CEntity& en);

    DECLARE_NODE(CEntity);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CEntity& it) const;
    bool operator!=(const CEntity& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CEntity& v1, const CEntity& v2);
    friend ostream& operator<<(ostream& os, const CEntity& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CEntity& en);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CEntity::CEntity(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEntity::CEntity(const CEntity& en) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(en);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CEntity::~CEntity(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntity::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CEntity::initialize(void) {
    CBaseNode::initialize();

    cid = "";
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
inline void CEntity::duplicate(const CEntity& en) {
    clear();
    CBaseNode::duplicate(en);

    cid = en.cid;
    tags = en.tags;
    name = en.name;
    client = en.client;
    monitored = en.monitored;
    deleted = en.deleted;
    sizeInBytes = en.sizeInBytes;
    addresses = en.addresses;
    addressList = en.addressList;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CEntity& CEntity::operator=(const CEntity& en) {
    duplicate(en);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CEntity::operator==(const CEntity& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return addressList % it.addressList;
}

//-------------------------------------------------------------------------
inline bool operator<(const CEntity& v1, const CEntity& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.name < v2.name;
}

//---------------------------------------------------------------------------
typedef vector<CEntity> CEntityArray;
extern CArchive& operator>>(CArchive& archive, CEntityArray& array);
extern CArchive& operator<<(CArchive& archive, const CEntityArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ENTITY;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
