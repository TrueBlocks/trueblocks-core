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
#include <vector>
#include <map>
#include "etherlib.h"
#include "block.h"
#include "transaction.h"

namespace qblocks {

//---------------------------------------------------------------------------
class CAddressAppearance {
public:
    blknum_t bn;
    blknum_t tx;
    blknum_t tc;
    address_t addr;
    string_q reason;
    CAddressAppearance(void) : bn(0), tx(0), tc(0), addr(""), reason("") { }
    CAddressAppearance(const CAddressAppearance& item)
        : bn(item.bn), tx(item.tx), tc(item.tc), addr(item.addr), reason(item.reason) { }
    CAddressAppearance& operator=(const CAddressAppearance& item) {
        bn = item.bn;
        tx = item.tx;
        tc = item.tc;
        addr = item.addr;
        reason = item.reason;
        return *this;
    }
    CAddressAppearance(blknum_t b, blknum_t x, blknum_t c, const address_t& a, const string_q r)
        : bn(b), tx(x), tc(c), addr(a), reason(r) { }
    string_q Format(const string_q& fmt) const;
    friend ostream& operator<<(ostream& os, const CAddressAppearance& item);
};
typedef vector<CAddressAppearance> CAddressAppearanceArray;
typedef bool (*ADDRESSFUNC)(const CAddressAppearance& item, void *data);
extern bool isPotentialAddr(biguint_t test, address_t& addrOut);
extern bool potentialAddr(ADDRESSFUNC func, void *data, const CAddressAppearance& item, const string_q& potList);

//---------------------------------------------------------------------------
struct addrOnlyComparator {
    bool operator()(const CAddressAppearance& v1, const CAddressAppearance& v2) const {
        return v1.addr < v2.addr;
    }
};

//---------------------------------------------------------------------------
struct addrTxComparator {
    bool operator()(const CAddressAppearance& v1, const CAddressAppearance& v2) const {
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
typedef map<CAddressAppearance,bool,addrOnlyComparator> CAddressOnlyAppearanceMap;
typedef map<CAddressAppearance,bool,addrTxComparator> CAddressTxAppearanceMap;
//---------------------------------------------------------------------------
class CUniqueState {
public:
    ADDRESSFUNC func;
    void *data;
    CAddressOnlyAppearanceMap *addrOnlyMap;
    CAddressTxAppearanceMap *addrTxMap;
public:
    CUniqueState(ADDRESSFUNC f, void *d, bool perTx) {
        func = f;
        data = d;
        addrOnlyMap = (perTx ? NULL : new CAddressOnlyAppearanceMap);
        addrTxMap   = (perTx ? new CAddressTxAppearanceMap : NULL);
    }
    bool insertUnique(const CAddressAppearance& _value);
 };

}  // namespace qblocks
