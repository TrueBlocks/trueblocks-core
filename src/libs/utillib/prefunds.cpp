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

#include "exportcontext.h"
#include "prefunds.h"
#include "logging.h"

namespace qblocks {

extern string_q getConfigPath(const string_q& part);

//---------------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CAddressWeiMap prefundBalMap;

//-----------------------------------------------------------------------
void clearPrefundBals(void) {
    prefundBalMap.clear();
}

//-----------------------------------------------------------------------
static void addToMapPrefunds(CAccountName& account, uint64_t cnt) {
    // If it's already there, don't alter it or add it to the map
    if (expContext().namesMap[account.address].address == account.address)
        return;

    address_t addr = account.address;
    account = expContext().namesMap[addr];
    account.address = addr;
    account.tags = account.tags.empty() ? "80-Prefund" : account.tags;
    account.source = account.source.empty() ? "Genesis" : account.source;
    account.isPrefund = account.name.empty();

    string_q prefundName = "Prefund_" + padNum4(cnt);
    if (account.name.empty()) {
        account.name = prefundName;
    } else if (!contains(account.name, "Prefund_")) {
        account.name += " (" + prefundName + ")";
    } else {
        // do nothing
    }

    expContext().namesMap[account.address] = account;
}

//-----------------------------------------------------------------------
bool loadNamesPrefunds(void) {
    LOG4("Loading prefunds accounts");
    if (prefundBalMap.size() > 0) {
        LOG4("Already loaded");
        return true;
    }

    if (!loadNames())
        return false;

    CAccountNameArray prefunds;

    string_q prefundBin = getCachePath("names/names_prefunds.bin");
    if (fileExists(prefundBin)) {
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(prefundBin, modeReadOnly, LOCK_NOWAIT)) {
            nameCache >> prefunds;
            nameCache.Release();
        }
    } else {
        string_q prefundFile = getConfigPath("names/names_prefunds.tab");

        CStringArray lines;
        asciiFileToLines(prefundFile, lines);

        CStringArray fields;
        for (auto line : lines) {
            if (fields.empty()) {
                explode(fields, line, '\t');
            } else {
                if (!startsWith(line, '#') && contains(line, "0x")) {
                    CAccountName account;
                    account.parseText(fields, line);
                    prefunds.push_back(account);
                }
            }
        }

        CArchive nameCache(WRITING_ARCHIVE);
        if (nameCache.Lock(prefundBin, modeWriteCreate, LOCK_CREATE)) {
            nameCache << prefunds;
            nameCache.Release();
        }
    }

    if (!loadPrefundBals())
        return false;

    uint64_t cnt = 0;
    for (auto prefund : prefunds)
        addToMapPrefunds(prefund, cnt++);

    return true;
}

//-----------------------------------------------------------------------
bool loadPrefundBals(void) {
    LOG4("Loading prefund balances");
    if (prefundBalMap.size() > 0) {
        LOG4("Already loaded");
        return true;
    }

    string_q balanceBin = getCachePath("names/names_prefunds_bals.bin");
    if (fileExists(balanceBin)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(balanceBin, modeReadOnly, LOCK_NOWAIT)) {
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

    string_q prefundFile = getConfigPath("names/names_prefunds.tab");

    CStringArray lines;
    asciiFileToLines(prefundFile, lines);
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
    if (archive.Lock(balanceBin, modeWriteCreate, LOCK_NOWAIT)) {
        archive << uint64_t(prefundBalMap.size());
        for (const auto& item : prefundBalMap)
            archive << item.first << item.second;
        archive.Release();
        return true;
    }

    LOG_WARN("Could not lock prefund cache at: ", balanceBin);
    return false;
}

//-----------------------------------------------------------------------
bool forEveryPrefund(PREFUNDFUNC func, void* data) {
    if (!func)
        return false;

    for (auto prefund : prefundBalMap)
        if (!(*func)(prefund, data))
            return false;
    return true;
}

//-----------------------------------------------------------------------
wei_t prefundAt(const address_t& addr) {
    return prefundBalMap[addr];
}

}  // namespace qblocks
