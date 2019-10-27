#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "block.h"
#include "transaction.h"
#include "node.h"

namespace qblocks {

    //----------------------------------------------------------------
    struct CAddressRecord_base {
        uint8_t bytes[20];
        uint32_t offset;
        uint32_t cnt;
    };

    //----------------------------------------------------------------
    struct CAppearance_base {
        uint32_t blk;
        uint32_t txid;
        CAppearance_base(void) { blk = txid = 0; }
        CAppearance_base(uint32_t b, uint32_t t) : blk(b), txid(t) {}
        CAppearance_base(const string_q& b, const string_q& t) : blk((uint32_t)str_2_Uint(b)), txid((uint32_t)str_2_Uint(t)) {}
        CAppearance_base(string_q& line) {  // NOLINT
            replaceAll(line, ".", "\t");
            if (!contains(line, "\t"))
                return;
            blk = (uint32_t)str_2_Uint(nextTokenClear(line, '\t'));
            txid = (uint32_t)str_2_Uint(nextTokenClear(line, '\t'));
        }
    };
    typedef vector<CAppearance_base> CAppearanceArray_base;
    inline bool operator<(const CAppearance_base& v1, const CAppearance_base& v2) {
        return ((v1.blk != v2.blk) ? v1.blk < v2.blk : v1.txid < v2.txid);
    }

    //----------------------------------------------------------------
    struct CHeaderRecord_base {
        uint32_t magic;
        uint8_t hash[32];
        uint32_t nAddrs;
        uint32_t nRows;
    };
    //---------------------------------------------------------------------------
    class CAppearance {
    public:
        blknum_t bn;
        blknum_t tx;
        blknum_t tc;
        address_t addr;
        string_q reason;
        CAppearance(void) : bn(0), tx(0), tc(0), addr(""), reason("") { }
        CAppearance(const CAppearance& item) : bn(item.bn), tx(item.tx), tc(item.tc), addr(item.addr), reason(item.reason) { }
        CAppearance& operator=(const CAppearance& item) {
            bn = item.bn;
            tx = item.tx;
            tc = item.tc;
            addr = item.addr;
            reason = item.reason;
            return *this;
        }
        CAppearance(blknum_t b, blknum_t x, blknum_t c, const address_t& a, const string_q r)
            : bn(b), tx(x), tc(c), addr(a), reason(r) { }
        string_q Format(const string_q& fmt) const;
        friend ostream& operator<<(ostream& os, const CAppearance& item);
    };
    typedef vector<CAppearance> CAppearanceArray;
    typedef bool (*ADDRESSFUNC)(const CAppearance& item, void *data);
    extern bool isPotentialAddr(biguint_t test, address_t& addrOut);
    extern bool potentialAddr(ADDRESSFUNC func, void *data, const CAppearance& item, const string_q& potList);

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
    typedef map<CAppearance,bool,addrOnlyComparator> CAddressOnlyAppearanceMap;  // NOLINT
    typedef map<CAppearance,bool,addrTxComparator> CAddressTxAppearanceMap;  // NOLINT
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
        bool insertUnique(const CAppearance& _value);
    };

    extern int findAppearance(const void* v1, const void* v2);

    typedef enum { FM_PRODUCTION, FM_STAGING } freshen_e;
    extern string_q getMonitorPath(const string_q& addr, freshen_e mode = FM_PRODUCTION);
    extern string_q getMonitorLast(const string_q& addr, freshen_e mode = FM_PRODUCTION);
    extern string_q getMonitorExpt(const string_q& addr, freshen_e mode = FM_PRODUCTION);
    extern string_q getMonitorBals(const string_q& addr, freshen_e mode = FM_PRODUCTION);

}  // namespace qblocks
