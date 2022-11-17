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
class CMonitorHeader {
  public:
    uint16_t magic;
    uint8_t unused;
    uint8_t deleted;
    uint32_t lastScanned;
};
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
    CAppearanceArray_mon apps;

  public:
    string_q getPathToMonitor(const address_t& addr, bool staging) const;
    size_t getRecordCnt(const string_q& path) const;

    bool readMonitorHeader(CMonitorHeader& header) const;
    bool readAppearances(MONAPPFUNC func, void* data);

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
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMonitor::initialize(void) {
    CAccountName::initialize();

    nAppearances = 0;
    lastExport = 0;
    firstAppearance = 0;
    latestAppearance = UINT_MAX;
    lastVisitedBlock = NOPOS;
    sizeInBytes = 0;

    // EXISTING_CODE
    isStaging = false;
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
extern CArchive& operator<<(CArchive& archive, const CMonitor& mon);
extern CArchive& operator>>(CArchive& archive, CMonitor& mon);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MONITOR;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool isMonitorFilePath(const string_q& path);
// EXISTING_CODE
}  // namespace qblocks
