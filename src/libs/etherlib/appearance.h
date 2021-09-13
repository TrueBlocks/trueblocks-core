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
#include "indexarchive.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAppearance : public CBaseNode {
  public:
    blknum_t bn;
    blknum_t tx;
    blknum_t tc;
    address_t addr;
    string_q reason;

  public:
    CAppearance(void);
    CAppearance(const CAppearance& ap);
    virtual ~CAppearance(void);
    CAppearance& operator=(const CAppearance& ap);

    DECLARE_NODE(CAppearance);

    // EXISTING_CODE
    CAppearance(blknum_t b, blknum_t x, blknum_t c, const address_t& a, const string_q r)
        : bn(b), tx(x), tc(c), addr(a), reason(r) {
    }
    // string_q Format(const string_q& fmt) const;
    // EXISTING_CODE
    bool operator==(const CAppearance& it) const;
    bool operator!=(const CAppearance& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CAppearance& v1, const CAppearance& v2);
    friend ostream& operator<<(ostream& os, const CAppearance& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAppearance& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAppearance::CAppearance(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearance::CAppearance(const CAppearance& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAppearance::~CAppearance(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearance::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearance::initialize(void) {
    CBaseNode::initialize();

    bn = 0;
    tx = 0;
    tc = 0;
    addr = "";
    reason = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAppearance::duplicate(const CAppearance& ap) {
    clear();
    CBaseNode::duplicate(ap);

    bn = ap.bn;
    tx = ap.tx;
    tc = ap.tc;
    addr = ap.addr;
    reason = ap.reason;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAppearance& CAppearance::operator=(const CAppearance& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAppearance::operator==(const CAppearance& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAppearance& v1, const CAppearance& v2) {
    // EXISTING_CODE
    return ((v1.bn != v2.bn) ? v1.bn < v2.bn : v1.tx < v2.tx);
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAppearance> CAppearanceArray;
extern CArchive& operator>>(CArchive& archive, CAppearanceArray& array);
extern CArchive& operator<<(CArchive& archive, const CAppearanceArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAppearance& app);
extern CArchive& operator>>(CArchive& archive, CAppearance& app);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_APPEARANCE;

//---------------------------------------------------------------------------
// EXISTING_CODE
typedef bool (*ADDRESSFUNC)(const address_t& addr, void* data);
typedef bool (*APPEARANCEFUNC)(const CAppearance& item, void* data);
extern bool isPotentialAddr(biguint_t test, address_t& addrOut);
extern bool potentialAddr(APPEARANCEFUNC func, void* data, const CAppearance& item, const string_q& potList);

//---------------------------------------------------------------------------
struct addrOnlyComparator {
    bool operator()(const CAppearance& v1, const CAppearance& v2) const {
        return v1.addr < v2.addr;
    }
};

//---------------------------------------------------------------------------
struct addrTxComparator {
    bool operator()(const CAppearance& v1, const CAppearance& v2) const {
        if (v1.addr < v2.addr)
            return true;
        if (v1.addr > v2.addr)
            return false;
        if (v2.tx == NOPOS)
            return false;
        return v1.tx < v2.tx;
    }
};

//---------------------------------------------------------------------------
typedef map<CAppearance, bool, addrOnlyComparator> CAddressOnlyAppearanceMap;  // NOLINT
typedef map<CAppearance, bool, addrTxComparator> CAddressTxAppearanceMap;      // NOLINT
//---------------------------------------------------------------------------
class CUniqueState {
  public:
    APPEARANCEFUNC func;
    void* data;
    CAddressOnlyAppearanceMap* addrOnlyMap;
    CAddressTxAppearanceMap* addrTxMap;

  public:
    CUniqueState(APPEARANCEFUNC f, void* d, bool perTx) {
        func = f;
        data = d;
        addrOnlyMap = (perTx ? NULL : new CAddressOnlyAppearanceMap);
        addrTxMap = (perTx ? new CAddressTxAppearanceMap : NULL);
    }
    bool insertUnique(const CAppearance& _value);
};

extern int findAddresses(const void* v1, const void* v2);
// EXISTING_CODE
}  // namespace qblocks
