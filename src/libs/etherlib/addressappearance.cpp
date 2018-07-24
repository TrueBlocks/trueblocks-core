/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include "block.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAddressAppearance& item) {
    os << item.bn << "\t";
    os << (item.tx == NOPOS ? "" : uint_2_Str(item.tx)) << "\t";
    os << (item.tc < 10 ? "" : uint_2_Str(item.tc - 10)) << "\t";
    os << item.addr << "\t";
    os << item.reason;
    return os;
}

//---------------------------------------------------------------------------
uint64_t insertUnique(CAddressAppearanceMap *addrMap, const CAddressAppearance& _value) {
    CAddressAppearanceMap::iterator it = addrMap->find(_value);
    if (it == addrMap->end())  // not found
        it = addrMap->insert(make_pair(_value, true)).first;
    return it->second;
}

//---------------------------------------------------------------------------
bool accumulateAddresses(const CAddressAppearance& item, void *data) {
    if (isZeroAddr(item.addr))
        return true;
    CAddressAppearance search(item.getBn(), item.getTx(), item.getTc(), item.addr, item.reason);
    insertUnique((CAddressAppearanceMap*)data, search);  // NOLINT
    return true;
}

//---------------------------------------------------------------------------
bool isPotentialAddr(biguint_t test, address_t& addrOut) {

    addrOut = "";

    static const biguint_t small = str_2_Wei(  "0x00000000000000ffffffffffffffffffffffffff");  // smallest address we find
    static const biguint_t large = str_2_Wei("0x010000000000000000000000000000000000000000");  // largest address we find
    if (test <= small || test >= large)
        return false;

    addrOut = bnu_2_Hex(test).c_str();
    // Totally a heuristic that can't really be supported, but a good probability that this isn't an address
    if (endsWith(addrOut, "00000000"))
        return false;

    if (addrOut.length() < 40)
        addrOut = padLeft(addrOut, 40, '0');
    addrOut = extract(addrOut, addrOut.length() - 40, 40);
    addrOut = toLower("0x" + addrOut);

    return true;
}

//---------------------------------------------------------------------------
void potentialAddr(ADDRESSFUNC func, void *data, const CAddressAppearance& item, const string_q& potList) {

    if (!func)
        return;

    // Pull out 32-byte chunks and check to see if they are addresses
    address_t addr;
    for (size_t s = 0 ; s < potList.length() / 64 ; s++) {
        biguint_t test = str_2_Wei("0x" + extract(potList, s*64, 64));
        if (isPotentialAddr(test, addr)) {
            CAddressAppearance it(item);
            it.addr = addr;
            (*func)(it, data);
        }
    }
}

//---------------------------------------------------------------------------
void foundOne(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const address_t& addr, const string_q& reason) {  // NOLINT
    CAddressAppearance item(bn, tx, tc, addr, reason);
    (*func)(item, data);
}

//---------------------------------------------------------------------------
void foundPot(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList, const string_q& reason) {  // NOLINT
    CAddressAppearance item(bn, tx, tc, "", reason);
    potentialAddr(func, data, item, potList);
}

//---------------------------------------------------------------------------
blknum_t CAddressAppearance::getBn(void) const { return bn; }
//---------------------------------------------------------------------------
blknum_t CAddressAppearance::getTx(void) const { return tx; }
//---------------------------------------------------------------------------
blknum_t CAddressAppearance::getTc(void) const { return tc; }
//---------------------------------------------------------------------------
void CAddressAppearance::setBlock(CBlock *pBlock) { bn = pBlock->blockNumber; }
//---------------------------------------------------------------------------
void CAddressAppearance::setTrans(CTransaction *pTrans) { tx = pTrans->transactionIndex; }
//---------------------------------------------------------------------------
void CAddressAppearance::setTrace(CTrace *pTrace, blknum_t t) { tc = t; }

}  // namespace qblocks

