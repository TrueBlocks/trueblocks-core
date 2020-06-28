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
    if (addr == accountedFor || isZeroAddr(addr))
        return;
    map[addr]++;
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

//-----------------------------------------------------------------------
bool COptions::reportNeighbors(void) {
    bool testMode = isTestMode() || getEnvStr("HIDE_NAMES") == "true";

    CNameStatsArray fromUnnamed;
    CNameStatsArray fromNamed;
    for (auto addr : fromAddrMap) {
        CAccountName acct;
        acct.address = addr.first;
        getNamedAccount(acct, addr.first);
        if (acct.name.empty()) {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            fromUnnamed.push_back(stats);
        } else {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            fromNamed.push_back(stats);
        }
    }

    {
        sort(fromNamed.begin(), fromNamed.end());
        ostringstream os;
        bool frst = true;
        os << ", \"namedFrom\": {";
        for (auto stats : fromNamed) {
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
        sort(fromUnnamed.begin(), fromUnnamed.end());
        ostringstream os;
        os << ", \"unNamedFrom\": {";
        bool frst = true;
        for (auto stats : fromUnnamed) {
            if (!frst)
                os << ",";
            os << "\"" << stats.address << "\": " << stats.count;
            frst = false;
        }
        os << "}";
        expContext().fmtMap["meta"] += os.str();
    }

    CNameStatsArray toUnnamed;
    CNameStatsArray toNamed;
    for (auto addr : toAddrMap) {
        CAccountName acct;
        acct.address = addr.first;
        getNamedAccount(acct, addr.first);
        if (isZeroAddr(acct.address)) {
            acct.tags = "Contract Creation";
            acct.name = "Contract Creation";
        }
        if (acct.name.empty()) {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            toUnnamed.push_back(stats);
        } else {
            CNameStats stats(acct.address, acct.tags, acct.name, addr.second);
            toNamed.push_back(stats);
        }
    }

    {
        sort(toNamed.begin(), toNamed.end());
        ostringstream os;
        bool frst = true;
        os << ", \"namedTo\": {";
        for (auto stats : toNamed) {
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
        sort(toUnnamed.begin(), toUnnamed.end());
        ostringstream os;
        os << ", \"unNamedTo\": {";
        bool frst = true;
        for (auto stats : toUnnamed) {
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
