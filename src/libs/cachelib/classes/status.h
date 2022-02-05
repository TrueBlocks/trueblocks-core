#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "cache.h"
#include "chain.h"

namespace qblocks {

// EXISTING_CODE
#include <ctime>
// EXISTING_CODE

//--------------------------------------------------------------------------
class CStatus : public CBaseNode {
  public:
    string_q clientVersion;
    string_q clientIds;
    string_q trueblocksVersion;
    string_q rpcProvider;
    string_q configPath;
    string_q cachePath;
    string_q indexPath;
    string_q host;
    bool isTesting;
    bool isApi;
    bool isDocker;
    bool isScraping;
    bool isArchive;
    bool isTracing;
    bool hasEskey;
    bool hasPinkey;
    timestamp_t ts;
    CCachePtrArray caches;
    CChainArray chains;

  public:
    CStatus(void);
    CStatus(const CStatus& st);
    virtual ~CStatus(void);
    CStatus& operator=(const CStatus& st);

    DECLARE_NODE(CStatus);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStatus& it) const;
    bool operator!=(const CStatus& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CStatus& v1, const CStatus& v2);
    friend ostream& operator<<(ostream& os, const CStatus& it);

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

    clientVersion = "";
    clientIds = "";
    trueblocksVersion = "";
    rpcProvider = "";
    configPath = "";
    cachePath = "";
    indexPath = "";
    host = "";
    isTesting = false;
    isApi = false;
    isDocker = false;
    isScraping = false;
    isArchive = false;
    isTracing = false;
    hasEskey = false;
    hasPinkey = false;
    ts = date_2_Ts(Now());
    caches.clear();
    chains.clear();

    // EXISTING_CODE
    // convert ts to UTC
    time_t lt = ts;
    tm unused;
    auto local_field = gmtime_r(&lt, &unused);
    local_field->tm_isdst = -1;
    ts = mktime(local_field);
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStatus::duplicate(const CStatus& st) {
    clear();
    CBaseNode::duplicate(st);

    clientVersion = st.clientVersion;
    clientIds = st.clientIds;
    trueblocksVersion = st.trueblocksVersion;
    rpcProvider = st.rpcProvider;
    configPath = st.configPath;
    cachePath = st.cachePath;
    indexPath = st.indexPath;
    host = st.host;
    isTesting = st.isTesting;
    isApi = st.isApi;
    isDocker = st.isDocker;
    isScraping = st.isScraping;
    isArchive = st.isArchive;
    isTracing = st.isTracing;
    hasEskey = st.hasEskey;
    hasPinkey = st.hasPinkey;
    ts = st.ts;
    caches = st.caches;
    chains = st.chains;

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
inline bool CStatus::operator==(const CStatus& it) const {
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
extern const char* STR_DISPLAY_STATUS;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
