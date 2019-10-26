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
#include <algorithm>
#include "block.h"
#include "appearance.h"

namespace qblocks {

    //---------------------------------------------------------------------------
    ostream& operator<<(ostream& os, const CAppearance& item) {
        os << item.bn << "\t";
        os << (item.tx == NOPOS ? "" : uint_2_Str(item.tx)) << "\t";
        os << (item.tc < 10 ? "" : uint_2_Str(item.tc - 10)) << "\t";
        os << item.addr << "\t";
        os << item.reason;
        return os;
    }

    //---------------------------------------------------------------------------
    string_q CAppearance::Format(const string_q& fmt) const {
        if (fmt.empty()) {
            ostringstream os;
            os << *this;
            return os.str();
        }
        string_q ret = fmt;
        ret = substitute(ret, "{BN}", uint_2_Str(bn));
        ret = substitute(ret, "{TX}", tx == NOPOS ? "" : uint_2_Str(tx));
        ret = substitute(ret, "{TC}", tc < 10 ? "" : uint_2_Str(tc-10));
        ret = substitute(ret, "{ADDR}", addr);
        ret = substitute(ret, "{REASON}", substitute(substitute(reason, "[", "`"), "]", "+"));
        ret = substitute(ret, "[", "");
        ret = substitute(ret, "]", "");
        ret = substitute(substitute(ret, "`", "["), "+", "]");
        return ret;
    }

    //---------------------------------------------------------------------------
    bool CUniqueState::insertUnique(const CAppearance& _value) {
        if (addrOnlyMap) {
            CAddressOnlyAppearanceMap::iterator it = addrOnlyMap->find(_value);
            if (it == addrOnlyMap->end()) {  // not found
                it = addrOnlyMap->insert(make_pair(_value, true)).first;
                if (func)
                    if (!(*func)(it->first, data))
                        return false;
            }
            return it->second;
        }

        CAddressTxAppearanceMap::iterator it = addrTxMap->find(_value);
        if (it == addrTxMap->end()) {  // not found
            it = addrTxMap->insert(make_pair(_value, true)).first;
            if (func)
                if (!(*func)(it->first, data))
                    return false;
        }
        return it->second;
    }

    //---------------------------------------------------------------------------
    bool accumulateAddresses(const CAppearance& item, void *data) {
        if (isZeroAddr(item.addr))
            return true;
        CUniqueState * state = (CUniqueState*)data;
        CAppearance search(item.bn, item.tx, item.tc, item.addr, item.reason);
        return state->insertUnique(search);  // NOLINT
    }

    //---------------------------------------------------------------------------
    bool CBlock::forEveryUniqueAddress(ADDRESSFUNC func, TRANSFUNC traceFilter, void *data) {
        if (!func)
            return false;
        CUniqueState state(func, data, false);
        return forEveryAddress(accumulateAddresses, traceFilter, &state);
    }

    //---------------------------------------------------------------------------
    bool CBlock::forEveryUniqueAddressPerTx(ADDRESSFUNC func, TRANSFUNC traceFilter, void *data) {
        if (!func)
            return false;
        CUniqueState state(func, data, true);
        return forEveryAddress(accumulateAddresses, traceFilter, &state);
    }

    //---------------------------------------------------------------------------
    bool isPotentialAddr(biguint_t test, address_t& addrOut) {

        addrOut = "";

        // smallest address we find
        static const biguint_t small = str_2_Wei(  "0x00000000000000ffffffffffffffffffffffffff");
        // largest address we find
        static const biguint_t large = str_2_Wei("0x010000000000000000000000000000000000000000");
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
    bool potentialAddr(ADDRESSFUNC func, void *data, const CAppearance& item, const string_q& potList) {

        if (!func)
            return false;

        // Pull out 32-byte chunks and check to see if they are addresses
        address_t addr;
        for (size_t s = 0 ; s < potList.length() / 64 ; s++) {
            biguint_t test = str_2_Wei("0x" + extract(potList, s*64, 64));
            if (isPotentialAddr(test, addr)) {
                CAppearance it(item);
                it.addr = addr;
                if (!(*func)(it, data))
                    return false;
            }
        }
        return true;
    }

    //---------------------------------------------------------------------------
    bool foundOne(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const address_t& addr, const string_q& reason) {  // NOLINT
        CAppearance item(bn, tx, tc, addr, reason);
        return (*func)(item, data);
    }

    //---------------------------------------------------------------------------
    bool foundPot(ADDRESSFUNC func, void *data, blknum_t bn, blknum_t tx, blknum_t tc, const string_q& potList, const string_q& reason) {  // NOLINT
        CAppearance item(bn, tx, tc, "", reason);
        return potentialAddr(func, data, item, potList);
    }

    //---------------------------------------------------------------------------
    string_q getMonitorLast(const string_q& addr, freshen_e mode) {
        string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
        if (!isTestMode() && !isAddress(addr)) {
            cerr << "Not an address: " << addr << endl;
            quickQuitHandler(0);
        }
        return getCachePath(base + addr + ".last.txt");
    }

    //---------------------------------------------------------------------------
    string_q getMonitorExpt(const string_q& addr, freshen_e mode) {
        string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
        if (!isTestMode() && !isAddress(addr)) {
            cerr << "Not an address: " << addr << endl;
            quickQuitHandler(0);
        }
        return getCachePath(base + addr + ".expt.txt");
    }

    //---------------------------------------------------------------------------
    string_q getMonitorBals(const string_q& addr, freshen_e mode) {
        string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
        if (!isTestMode() && !isAddress(addr)) {
            cerr << "Not an address: " << addr << endl;
            quickQuitHandler(0);
        }
        return getCachePath(base + addr + ".bals.bin");
    }

    //---------------------------------------------------------------------------
    string_q getMonitorPath(const string_q& addr, freshen_e mode) {
        string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
        if (!isAddress(addr)) // empty for example
            return getCachePath(base + addr);
        return getCachePath(base + addr + ".acct.bin");
    }

    //----------------------------------------------------------------
    int findAppearance(const void* v1, const void* v2) {
        const CAddressRecord_base *at1 = (CAddressRecord_base*)v1;
        const CAddressRecord_base *at2 = (CAddressRecord_base*)v2;
        for (size_t i = 0 ; i < 20 ; i++) {
            int ret = at1->bytes[i] - at2->bytes[i];
            if (ret)
                return ret;
        }
        return 0;
    }
}  // namespace qblocks

