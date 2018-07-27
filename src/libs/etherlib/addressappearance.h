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
    blknum_t bn;
    blknum_t tx;
    blknum_t tc;

public:
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
    friend bool operator<(const CAddressAppearance& v1, const CAddressAppearance& v2) {
        return v1.addr < v2.addr;
    }
    blknum_t getBn(void) const;
    blknum_t getTx(void) const;
    blknum_t getTc(void) const;
    void setBlock(CBlock *pBlock);
    void setTrans(CTransaction *pTrans);
    void setTrace(CTrace *pTrace, blknum_t t);
    friend ostream& operator<<(ostream& os, const CAddressAppearance& item);
};
typedef map<CAddressAppearance, uint64_t> CAddressAppearanceMap;
typedef vector<CAddressAppearance> CAddressAppearanceArray;
typedef bool (*ADDRESSFUNC)(const CAddressAppearance& item, void *data);
typedef bool (*TRANSFUNC)(const CTransaction *trans, void *data);

extern bool isPotentialAddr(biguint_t test, address_t& addrOut);
extern void potentialAddr(ADDRESSFUNC func, void *data, const CAddressAppearance& item, const string_q& potList);
}  // namespace qblocks

