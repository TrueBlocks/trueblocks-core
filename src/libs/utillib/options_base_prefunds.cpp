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
#include "basetypes.h"
#include "options_base.h"
#include "logging.h"

namespace qblocks {

//-----------------------------------------------------------------------
static void addToMapPrefunds(CAddressNameMap& theMap, CAccountName& account, uint64_t cnt) {
    // If it's already there, don't alter it or add it to the map
    if (theMap[account.address].address == account.address)
        return;

    address_t addr = account.address;
    account = theMap[addr];
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

    theMap[account.address] = account;
}

//-----------------------------------------------------------------------
bool importTabFilePrefund(CAddressNameMap& theMap, const string_q& tabFilename) {
    string_q prefundBin = getCachePath("names/names_prefunds.bin");

    uint64_t cnt = 0;
    if (fileExists(prefundBin)) {
        LOG4("Reading prefund names from binary cache");
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(prefundBin, modeReadOnly, LOCK_NOWAIT)) {
            CAccountNameArray prefunds;
            nameCache >> prefunds;
            nameCache.Release();
            for (auto prefund : prefunds)
                addToMapPrefunds(theMap, prefund, cnt++);
            return true;
        }
    }

    CAccountNameArray prefundArrayOut;
    prefundArrayOut.reserve(10000);

    CStringArray lines;
    asciiFileToLines(tabFilename, lines);

    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, '\t');
        } else {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account;
                account.parseText(fields, line);
                addToMapPrefunds(theMap, account, cnt++);
                prefundArrayOut.push_back(account);
            }
        }
    }

    CArchive nameCache(WRITING_ARCHIVE);
    if (nameCache.Lock(prefundBin, modeWriteCreate, LOCK_CREATE)) {
        nameCache << prefundArrayOut;
        nameCache.Release();
    }

    return true;
}

//-----------------------------------------------------------------------
bool loadPrefunds(const string_q& prefundFile) {
    expContext().prefundMap.clear();

    string_q binFile = getCachePath("names/names_prefunds_bals.bin");
    if (fileExists(binFile)) {
        CArchive archive(READING_ARCHIVE);
        if (!archive.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            LOG_WARN("Could not unlock prefund cache at: ", binFile);
            return false;
        }
        uint64_t count;
        archive >> count;
        for (size_t i = 0; i < count; i++) {
            string_q key;
            wei_t wei;
            archive >> key >> wei;
            expContext().prefundMap[key] = wei;
        }
        archive.Release();
        return true;
    }

    if (!fileExists(prefundFile)) {
        LOG_WARN("Cannot find prefund file at: ", prefundFile);
        return false;
    }
    CStringArray lines;
    asciiFileToLines(prefundFile, lines);
    bool first = true;
    for (auto line : lines) {
        if (!first && !startsWith(line, '#')) {
            CStringArray parts;
            explode(parts, line, '\t');
            expContext().prefundMap[toLower(parts[0])] = str_2_Wei(parts[1]);
        }
        first = false;
    }

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(binFile, modeWriteCreate, LOCK_NOWAIT)) {
        LOG_WARN("Could not lock prefund cache at: ", binFile);
        return false;
    }

    CAddressWeiMap::iterator it = expContext().prefundMap.begin();
    archive << uint64_t(expContext().prefundMap.size());
    while (it != expContext().prefundMap.end()) {
        archive << it->first << it->second;
        it++;
    }
    archive.Release();
    return true;
}

}  // namespace qblocks
