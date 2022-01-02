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
// NOTE: This file has a lot of NOLINT's in it. Because it's someone else's code, I wanted
// to be conservitive in changing it. It's easier to hide the lint than modify the code

#define LOGGING_LEVEL_TEST
#include "exportcontext.h"
#include "prefunds.h"
#include "names.h"
#include "logging.h"
#include "options_base.h"

namespace qblocks {

//---------------------------------------------------------------------------
string_q getChain(void) {
    string_q ret = getGlobalConfig()->getConfigStr("settings", "chain", "mainnet");
    if (!endsWith(ret, "/"))
        ret += "/";
    return ret;
}

//---------------------------------------------------------------------------
string_q getChainConfigPath(const string_q& part) {
    // cerr << "getChainConfigPath: " << bGreen << (getConfigPath(getChain() + "config/") + part) << cOff << endl;
    return getConfigPath(part);
}

//---------------------------------------------------------------------------
string_q getChainCachePath(const string_q& part) {
    // cerr << "getChainCachePath: " << bGreen << substitute(getCachePath(part), "cache/", getChain() + "cache/") <<
    // cOff
    //      << endl;
    return getCachePath(part);
}

//---------------------------------------------------------------------------
// We define these so they don't run until they are called...
#define STR_PREFUND_BALANCES_TAB1 getChainConfigPath("names/names_prefunds.tab")
#define STR_PREFUND_BALANCES_BIN1 getChainCachePath("names/names_prefunds_bals.bin")

//---------------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CAddressWeiMap prefundBalMap;

//-----------------------------------------------------------------------
void clearPrefundBals(void) {
    prefundBalMap.clear();
}

//-----------------------------------------------------------------------
bool loadPrefundBalances(void) {
    LOG_TEST_STR("Loading prefund balances");
    if (prefundBalMap.size() > 0) {
        LOG_TEST_STR("Already loaded");
        return true;
    }

    if (fileExists(STR_PREFUND_BALANCES_BIN1)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(STR_PREFUND_BALANCES_BIN1, modeReadOnly, LOCK_NOWAIT)) {
            uint64_t count;
            archive >> count;
            for (size_t i = 0; i < count; i++) {
                wei_t amount;
                string_q address;
                archive >> address >> amount;
                prefundBalMap[address] = amount;
            }
            archive.Release();
            return true;
        }
    }

    CStringArray lines;
    asciiFileToLines(STR_PREFUND_BALANCES_TAB1, lines);
    for (auto line : lines) {
        if (startsWith(line, "0x")) {
            CStringArray parts;
            explode(parts, line, '\t');
            string_q address = toLower(parts[0]);
            wei_t amount = str_2_Wei(parts[1]);
            prefundBalMap[address] = amount;
        }
    }

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(STR_PREFUND_BALANCES_BIN1, modeWriteCreate, LOCK_NOWAIT)) {
        archive << uint64_t(prefundBalMap.size());
        for (const auto& item : prefundBalMap)
            archive << item.first << item.second;
        archive.Release();
        return true;
    }

    LOG_WARN("Could not lock prefund cache at: ", STR_PREFUND_BALANCES_BIN1);
    return false;
}

//-----------------------------------------------------------------------
bool forEveryPrefund(ALLOCFUNC func, void* data) {
    if (!func)
        return false;

    for (auto prefund : prefundBalMap) {
        Allocation alloc;
        alloc.address = prefund.first;
        alloc.amount = prefund.second;
        if (!(*func)(alloc, data))
            return false;
    }
    return true;
}

//-----------------------------------------------------------------------
wei_t prefundAt(const address_t& addr) {
    return prefundBalMap[addr];
}

//-----------------------------------------------------------------------
bool findLargest(const Allocation& prefund, void* data) {
    Allocation* largest = (Allocation*)data;
    if (prefund.amount > largest->amount) {
        largest->address = prefund.address;
        largest->amount = prefund.amount;
    }
    return true;
}

//-----------------------------------------------------------------------
Allocation largestPrefund(void) {
    if (prefundBalMap.size() == 0)
        loadPrefundBalances();
    Allocation largest;
    forEveryPrefund(findLargest, &largest);
    return largest;
}

}  // namespace qblocks

#if 0
//-----------------------------------------------------------------------
bool readPrefundBals(void) {
    if (!fileExists(STR_PREFUND_BALANCES_BIN1))
        return false;

    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock(STR_PREFUND_BALANCES_BIN1, modeReadOnly, LOCK_NOWAIT))
        return false;

    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        wei_t amount;
        string_q address;
        archive >> address >> amount;
        prefundBalMap[address] = amount;
    }
    archive.Release();

    return true;
}

//-----------------------------------------------------------------------
bool readPrefundAscii(void) {
    if (!fileExists(STR_PREFUND_BALANCES_TAB1))
        return false;

    CStringArray lines;
    asciiFileToLines(STR_PREFUND_BALANCES_TAB1, lines);
    for (auto line : lines) {
        if (startsWith(line, "0x")) {
            CStringArray parts;
            explode(parts, line, '\t');
            string_q address = toLower(parts[0]);
            wei_t amount = str_2_Wei(parts[1]);
            prefundBalMap[address] = amount;
        }
    }
    return true;
}

//-----------------------------------------------------------------------
bool writePrefundBin(void) {
    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(STR_PREFUND_BALANCES_BIN1, modeWriteCreate, LOCK_NOWAIT)) {
        LOG_WARN("Could not lock prefund cache at: ", STR_PREFUND_BALANCES_BIN1);
        return false;
    }

    archive << uint64_t(prefundBalMap.size());
    for (const auto& item : prefundBalMap)
        archive << item.first << item.second;
    archive.Release();

    return true;
}

//-----------------------------------------------------------------------
bool loadPrefundBalances(void) {
    LOG_TEST_STR("Loading prefund balances");
    if (prefundBalMap.size() > 0) {
        LOG_TEST_STR("Already loaded");
        return true;
    }

    if (readPrefundBals())
        return true;

    if (!readPrefundAscii())
        return false;

    return writePrefundBin();
}

#endif
