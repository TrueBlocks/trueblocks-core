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
#include "cache.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CStatus : public CBaseNode {
public:
    string_q client_version;
    string_q trueblocks_version;
    string_q rpc_provider;
    string_q api_provider;
    string_q balance_provider;
    string_q cache_location;
    string_q index_location;
    string_q host;
    bool is_scraping;

public:
    CStatus(void);
    CStatus(const CStatus& st);
    virtual ~CStatus(void);
    CStatus& operator=(const CStatus& st);

    DECLARE_NODE(CStatus);

    // EXISTING_CODE
    CCachePtrArray caches;
    // EXISTING_CODE
    bool operator==(const CStatus& item) const;
    bool operator!=(const CStatus& item) const { return !operator==(item); }
    friend bool operator<(const CStatus& v1, const CStatus& v2);
    friend ostream& operator<<(ostream& os, const CStatus& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStatus& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStatus::CStatus(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStatus::CStatus(const CStatus& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStatus::~CStatus(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatus::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatus::initialize(void) {
    CBaseNode::initialize();

    client_version = "";
    trueblocks_version = "";
    rpc_provider = "";
    api_provider = "";
    balance_provider = "";
    cache_location = "";
    index_location = "";
    host = "";
    is_scraping = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatus::duplicate(const CStatus& st) {
    clear();
    CBaseNode::duplicate(st);

    client_version = st.client_version;
    trueblocks_version = st.trueblocks_version;
    rpc_provider = st.rpc_provider;
    api_provider = st.api_provider;
    balance_provider = st.balance_provider;
    cache_location = st.cache_location;
    index_location = st.index_location;
    host = st.host;
    is_scraping = st.is_scraping;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStatus& CStatus::operator=(const CStatus& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStatus::operator==(const CStatus& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStatus& v1, const CStatus& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStatus> CStatusArray;
extern CArchive& operator>>(CArchive& archive, CStatusArray& array);
extern CArchive& operator<<(CArchive& archive, const CStatusArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CStatus& sta);
extern CArchive& operator>>(CArchive& archive, CStatus& sta);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

