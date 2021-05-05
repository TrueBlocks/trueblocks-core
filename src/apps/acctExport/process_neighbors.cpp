/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
void COptions::addNeighbor(CAddressUintMap& map, const address_t& addr) {
    if ((addr == accountedFor || isZeroAddr(addr)))
        return;
    map[addr]++;
}

//-----------------------------------------------------------------------
void COptions::markNeighbors(const CTransaction& trans) {
    addNeighbor(fromAddrMap, trans.from);
    addNeighbor(toAddrMap, trans.to);
    for (auto log : trans.receipt.logs) {
        if (emitter)
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
        options->getNamedAccount(acct, addr.first);
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
            if (testMode && (stats.is_custom || contains(stats.tags, "Friends")))
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
    if (emitter)
        doOne(this, emitterAddrMap, "Emitters");
    if (factory)
        doOne(this, creationMap, "Creations");
    if (traces) {
        doOne(this, fromTraceAddrMap, "TraceFrom");
        doOne(this, toTraceAddrMap, "TraceTo");
    }
    return true;
}
