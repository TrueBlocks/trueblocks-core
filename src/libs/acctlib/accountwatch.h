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
#include "transaction.h"
#include "incomestatement.h"
#include "ethstate.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountWatch : public CAccountName {
public:
    CAbi abi_spec;
    CIncomeStatement statement;
    CEthStateArray stateHistory;
    wei_t curBalance;
    bool enabled;
    freshen_e fm_mode;

public:
    CAccountWatch(void);
    CAccountWatch(const CAccountWatch& ac);
    virtual ~CAccountWatch(void);
    CAccountWatch& operator=(const CAccountWatch& ac);

    DECLARE_NODE(CAccountWatch);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    CAccountWatch(const string_q& _addr, const string_q& _name, blknum_t fB, blknum_t lB, const string_q& _color);
    CAccountWatch(const address_t& _addr, const string_q& _name);
    bloom_t bloom;
    bool inBlock;
    CArchive *tx_cache;
    string_q extra_data;
    void writeLastBlock(blknum_t bn);
    void writeLastExport(blknum_t bn);
    void writeAnArray(const CAppearanceArray_base& array);
    void writeARecord(blknum_t bn, blknum_t tx_id);
    bool openCacheFile1(void);
    // EXISTING_CODE
    bool operator==(const CAccountWatch& item) const;
    bool operator!=(const CAccountWatch& item) const { return !operator==(item); }
    friend bool operator<(const CAccountWatch& v1, const CAccountWatch& v2);
    friend ostream& operator<<(ostream& os, const CAccountWatch& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAccountWatch& ac);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const CAccountWatch& ac) {
    // EXISTING_CODE
    tx_cache = NULL;
    // EXISTING_CODE
    duplicate(ac);
}

// EXISTING_CODE
//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const string_q& _addr, const string_q& _name, blknum_t fB, blknum_t lB, const string_q& _color) {
    initialize();
    address = toLower(_addr);
    name = _name;
    color = _color;
    firstAppearance = fB;
    latestAppearance = lB;
}

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const address_t& _addr, const string_q& _name) {
    initialize();
    address = toLower(_addr);
    name = _name;
    color = cBlue;
    firstAppearance = 0;
    latestAppearance = 0;
}
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountWatch::~CAccountWatch(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::clear(void) {
    // EXISTING_CODE
    if (tx_cache) {
        tx_cache->Release();
        delete tx_cache;
    }
    tx_cache = NULL;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::initialize(void) {
    CAccountName::initialize();

    abi_spec = CAbi();
    statement = CIncomeStatement();
    stateHistory.clear();
    curBalance = 0;
    enabled = true;
    fm_mode = FM_PRODUCTION;

    // EXISTING_CODE
    latestAppearance = UINT_MAX;
    bloom = 0;
    inBlock = false;
    tx_cache = NULL;
    extra_data = "";
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::duplicate(const CAccountWatch& ac) {
    clear();
    CAccountName::duplicate(ac);

    abi_spec = ac.abi_spec;
    statement = ac.statement;
    stateHistory = ac.stateHistory;
    curBalance = ac.curBalance;
    enabled = ac.enabled;
    fm_mode = ac.fm_mode;

    // EXISTING_CODE
    latestAppearance = ac.latestAppearance;
    bloom = ac.bloom;
    inBlock = ac.inBlock;
    tx_cache = NULL; // we do not copy the tx_cache
    extra_data = ac.extra_data;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountWatch& CAccountWatch::operator=(const CAccountWatch& ac) {
    duplicate(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAccountWatch::operator==(const CAccountWatch& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAccountWatch& v1, const CAccountWatch& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAccountWatch> CAccountWatchArray;
extern CArchive& operator>>(CArchive& archive, CAccountWatchArray& array);
extern CArchive& operator<<(CArchive& archive, const CAccountWatchArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_ACCOUNTWATCH;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef map<address_t,CAccountWatch> CAccountWatchMap;
extern void loadWatchList(const CToml& toml, CAccountWatchArray& monitors, const string_q& key);
// EXISTING_CODE
}  // namespace qblocks

