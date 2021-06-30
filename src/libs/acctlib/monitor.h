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
#include "transaction.h"
#include "reconciliation.h"
#include "ethstate.h"
#include "monitoredapp.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMonitor : public CAccountName {
  public:
    blknum_t nAppearances;
    blknum_t lastExport;
    blknum_t firstAppearance;
    blknum_t latestAppearance;
    blknum_t lastVisitedBlock;
    uint64_t sizeInBytes;

  public:
    CMonitor(void);
    CMonitor(const CMonitor& mo);
    virtual ~CMonitor(void);
    CMonitor& operator=(const CMonitor& mo);

    DECLARE_NODE(CMonitor);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
  public:
    bool isStaging;
    bloom_t bloom;
    CArchive* tx_cache;
    CAppearanceArray_mon apps;

    bool openForWriting(bool staging);
    void writeMonitorArray(const CAppearanceArray_mon& array);

    string_q getMonitorPath(const address_t& addr, bool staging) const;
    string_q getMonitorPathLast(const address_t& addr, bool staging) const;
    string_q getMonitorPathDels(const address_t& addr) const;

    blknum_t loadAppearances(MONAPPFUNC func, void* data);
    blknum_t getNextBlockToVisit(bool fresh = false) const;
    blknum_t getLastBlockInMonitorPlusOne(void) const;
    void writeLastBlockInMonitor(blknum_t bn, bool staging);

    size_t getFileSize(const string_q& path) const;
    size_t getRecordCnt(const string_q& path) const;

    bloom_t getBloom(void);

    bool monitorExists(void) const;
    bool isMonitorLocked(string_q& msg) const;
    bool clearMonitorLocks(void);
    void moveToProduction(bool staging);

    bool isDeleted(void) const;
    void deleteMonitor(void);
    void undeleteMonitor(void);
    void removeMonitor(void);
    // EXISTING_CODE
    bool operator==(const CMonitor& it) const;
    bool operator!=(const CMonitor& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CMonitor& v1, const CMonitor& v2);
    friend ostream& operator<<(ostream& os, const CMonitor& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMonitor& mo);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMonitor::CMonitor(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitor::CMonitor(const CMonitor& mo) {
    // EXISTING_CODE
    tx_cache = NULL;
    // EXISTING_CODE
    duplicate(mo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMonitor::~CMonitor(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::clear(void) {
    // EXISTING_CODE
    if (tx_cache) {
        tx_cache->Release();
        delete tx_cache;
    }
    tx_cache = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::initialize(void) {
    CAccountName::initialize();

    nAppearances = 0;
    lastExport = 0;
    firstAppearance = 0;
    latestAppearance = UINT_MAX;
    lastVisitedBlock = 0;
    sizeInBytes = 0;

    // EXISTING_CODE
    isStaging = false;
    bloom = bloom_t();
    tx_cache = NULL;
    apps.clear();
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::duplicate(const CMonitor& mo) {
    clear();
    CAccountName::duplicate(mo);

    nAppearances = mo.nAppearances;
    lastExport = mo.lastExport;
    firstAppearance = mo.firstAppearance;
    latestAppearance = mo.latestAppearance;
    lastVisitedBlock = mo.lastVisitedBlock;
    sizeInBytes = mo.sizeInBytes;

    // EXISTING_CODE
    isStaging = mo.isStaging;
    bloom = mo.bloom;
    tx_cache = NULL;  // we do not copy the tx_cache
    apps = mo.apps;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMonitor& CMonitor::operator=(const CMonitor& mo) {
    duplicate(mo);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMonitor::operator==(const CMonitor& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMonitor& v1, const CMonitor& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMonitor> CMonitorArray;
extern CArchive& operator>>(CArchive& archive, CMonitorArray& array);
extern CArchive& operator<<(CArchive& archive, const CMonitorArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MONITOR;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<address_t, CMonitor> CMonitorMap;  // NOLINT
extern string_q getTokenBalanceOf(const CMonitor& token, const address_t& holder, blknum_t blockNum);
extern string_q getTokenSymbol(const CMonitor& token, blknum_t blockNum);
extern string_q getTokenState(const CMonitor& token, const string_q& whichState, const CAbi& abi_spec,
                              blknum_t blockNum);
extern void establishMonitorFolders(void);
extern void cleanMonitorStage(void);
// EXISTING_CODE
}  // namespace qblocks
