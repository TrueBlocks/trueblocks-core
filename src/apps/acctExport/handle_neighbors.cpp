/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//-----------------------------------------------------------------------
void COptions::addNeighbor(addr_count_map_t& map, const address_t& addr) {
    if ((addr == expContext().accountedFor || isZeroAddr(addr)))
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
    CNameStats(const address_t& a, const string_q& t, const string_q& n, uint64_t c = 0) {
        address = a;
        tags = t;
        name = n;
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
bool doOne(COptions* options, const addr_count_map_t& theMap, const string_q& type) {
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
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            unnamed.push_back(stats);
        } else {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            named.push_back(stats);
        }
    }

    {
        sort(named.begin(), named.end());
        ostringstream os;
        bool frst = true;
        os << ", \"named" << type << "\": {";
        for (auto stats : named) {
            if (testMode && contains(stats.tags, "Friends"))
                stats.name = "Name " + stats.address.substr(0, 10);
            if (!frst)
                os << ",";
            os << "\"" << stats.address << "\": { \"tags\": \"" << stats.tags << "\", \"name\": \"" << stats.name
               << "\", \"count\": " << stats.count << " }";
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
