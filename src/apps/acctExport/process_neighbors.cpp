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
#include "options.h"

//-----------------------------------------------------------------------
void COptions::addNeighbor(CAddressUintMap& map, const address_t& addr) {
    if ((addr == accountedFor.address || isZeroAddr(addr)))
        return;
    map[addr]++;
}

//-----------------------------------------------------------------------
void COptions::markNeighbors(const CTransaction& trans) {
    addNeighbor(fromAddrMap, trans.from);
    addNeighbor(toAddrMap, trans.to);
    for (auto log : trans.receipt.logs) {
        if (logFilter.emitters.size())
            emitterAddrMap[log.address]++;
    }
    for (auto trace : trans.traces) {
        fromTraceAddrMap[trace.action.from]++;
        toTraceAddrMap[trace.action.to]++;
        if (factory)
            creationMap[trace.result.newContract]++;
    }
}

//-----------------------------------------------------------------------
class CNameStats {
  public:
    address_t address;
    string_q tags;
    string_q name;
    uint64_t count;
    bool is_custom;
    bool is_contract;
    bool is_erc20;
    bool is_erc721;
    explicit CNameStats(const CAccountName& acct, uint64_t c = 0) {
        address = acct.address;
        tags = acct.tags;
        name = acct.name;
        is_custom = acct.is_custom;
        is_contract = acct.is_contract;
        is_erc20 = acct.is_erc20;
        is_erc721 = acct.is_erc721;
        count = c;
    }

  private:
    CNameStats() {
    }
};
typedef vector<CNameStats> CNameStatsArray;

//-------------------------------------------------------------------------
inline bool operator<(const CNameStats& v1, const CNameStats& v2) {
    if (v1.count != v2.count)
        return v1.count > v2.count;  // We want to sort reverse by count
    return v1.address < v2.address;
}

//-------------------------------------------------------------------------
bool doOne(COptions* options, const CAddressUintMap& theMap, const string_q& type) {
    if (theMap.size() == 0)
        return false;

    bool testMode = isTestMode() || getEnvStr("HIDE_NAMES") == "true";

    CNameStatsArray unnamed;
    CNameStatsArray named;
    for (auto addr : theMap) {
        CAccountName acct;
        acct.address = addr.first;
        options->findName(addr.first, acct);
        if (acct.name.empty()) {
            CNameStats stats(acct, addr.second);
            unnamed.push_back(stats);
        } else {
            CNameStats stats(acct, addr.second);
            named.push_back(stats);
        }
    }

    {
        sort(named.begin(), named.end());
        ostringstream os;
        bool frst = true;
        os << ", \"named" << type << "\": {";
        for (auto stats : named) {
            if (testMode && (stats.is_custom || contains(stats.tags, "Individuals")))
                stats.name = "Name " + stats.address.substr(0, 10);
            if (!frst)
                os << ",";
            os << "\"" << stats.address << "\": { ";
            os << "\"tags\": \"" << stats.tags << "\", ";
            os << "\"name\": \"" << stats.name << "\", ";
            if (stats.is_contract)
                os << "\"is_contract\": true, ";
            if (stats.is_erc20)
                os << "\"is_erc20\": true, ";
            if (stats.is_erc721)
                os << "\"is_erc721\": true, ";
            os << "\"count\": " << stats.count << " }";
            frst = false;
        }
        os << "}\n";
        expContext().fmtMap["meta"] += os.str();
    }

    {
        sort(unnamed.begin(), unnamed.end());
        ostringstream os;
        os << ", \"unNamed" << type << "\": {";
        bool frst = true;
        for (auto stats : unnamed) {
            if (!frst)
                os << ",";
            os << "\"" << stats.address << "\": " << stats.count;
            frst = false;
        }
        os << "}";
        expContext().fmtMap["meta"] += os.str();
    }
    return true;
}

//-----------------------------------------------------------------------
bool COptions::reportNeighbors(void) {
    doOne(this, fromAddrMap, "From");
    doOne(this, toAddrMap, "To");
    if (logFilter.emitters.size() > 0)
        doOne(this, emitterAddrMap, "Emitters");
    if (factory)
        doOne(this, creationMap, "Creations");
    if (traces) {
        doOne(this, fromTraceAddrMap, "TraceFrom");
        doOne(this, toTraceAddrMap, "TraceTo");
    }
    return true;
}
