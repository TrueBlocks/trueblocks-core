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
#include "database.h"
#include "exportcontext.h"
#include "options_base.h"
#include "colors.h"
#include "filenames.h"
#include "accountname.h"
#include "rpcresult.h"

namespace qblocks {

//-----------------------------------------------------------------------
static void addToMap(CAddressNameMap& theMap, CAccountName& account, const string_q& tabFilename, uint64_t cnt) {
    if (theMap[account.address].address == account.address) {
        // first in wins;
        return;
    }

    if (contains(tabFilename, "_custom")) {
        // From the custom file - store the values found in the file
        account.is_custom = true;
        theMap[account.address] = account;

    } else if (contains(tabFilename, "_prefunds")) {
        // From the prefund file - force prefund marker, apply default values only if existing fields are empty
        address_t addr = account.address;
        account = theMap[addr];  // may be empty, but if not, let's pick up the existing values
        account.address = addr;
        account.tags = account.tags.empty() ? "80-Prefund" : account.tags;
        string_q prefundName = "Prefund_" + padNum4(cnt);
        account.is_prefund = account.name.empty();  // only mark as pre-fund if it didn't exist before
                                                    // clang-format off
        account.name = account.name.empty()                  ? prefundName
                       : contains(account.name, "(Prefund_") ? account.name
                                                             : account.name + " (" + prefundName + ")";
        // clang-format on
        account.source = account.source.empty() ? "Genesis" : account.source;
        theMap[account.address] = account;

    } else {
        // From the regular file - store the values found in the file
        theMap[account.address] = account;
    }
}

//-----------------------------------------------------------------------
bool loadPrefunds(const string_q& prefundFile) {
    // start with a clean slate
    expContext().prefundMap.clear();

    // Note: we don't need to check the dates to see if the prefunds.txt file has been updated
    // since it will never change. In that sense, the binary file is always right once it's created.
    string_q binFile = getCachePath("names/names_prefunds_bals.bin");
    if (!fileExists(binFile)) {
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

//-----------------------------------------------------------------------
bool importTabFile(CAddressNameMap& theMap, const string_q& tabFilename) {
    string_q prefundBin = getCachePath("names/names_prefunds.bin");

    uint64_t cnt = 0;
    if (contains(tabFilename, "prefunds")) {
        // This never changes so we should be able to only read this once in forever. If the binary file exists, use it
        if (fileExists(prefundBin)) {
            LOG4("Reading prefund names from binary cache");
            CArchive nameCache(READING_ARCHIVE);
            if (nameCache.Lock(prefundBin, modeReadOnly, LOCK_NOWAIT)) {
                CAccountNameArray prefunds;
                nameCache >> prefunds;
                nameCache.Release();
                for (auto prefund : prefunds)
                    addToMap(theMap, prefund, tabFilename, cnt++);
                return true;
            }
        }
    }

    CStringArray lines;
    asciiFileToLines(tabFilename, lines);
    if (lines.size() == 0)
        return false;

    string_q header = lines[0];
    if (!contains(header, "address\t"))
        return false;
    CStringArray fields;
    explode(fields, lines[0], '\t');
    if (fields.size() == 0)
        return false;

    const CAccountName emptyName;
    CAccountNameArray prefundArrayOut;
    prefundArrayOut.reserve(10000);
    for (auto it = next(lines.begin()); it != lines.end(); ++it) {
        string_q line = *it;
        if (!startsWith(line, '#') && contains(line, "0x")) {
            CAccountName account;
            account.parseText(fields, line);
            addToMap(theMap, account, tabFilename, cnt++);
            prefundArrayOut.push_back(account);
        }
    }

    if (contains(tabFilename, "prefund")) {
        // We can cache the prefunds binary since it will never change
        CArchive nameCache(WRITING_ARCHIVE);
        if (nameCache.Lock(prefundBin, modeWriteCreate, LOCK_CREATE)) {
            nameCache << prefundArrayOut;
            nameCache.Release();
        }
    }

    return true;
}

//-----------------------------------------------------------------------
bool COptionsBase::buildOtherMaps(void) {
    for (const auto& item : namesMap) {
        if (contains(item.second.tags, "Airdrop"))
            airdropMap[item.second.address] = true;
        bool isToken = !item.second.symbol.empty() || contains(item.second.tags, "Tokens") ||
                       contains(item.second.tags, "Airdrop");
        if (isToken)
            tokenMap[item.second.address] = item.second;
    }
    return true;
}

//-----------------------------------------------------------------------
bool COptionsBase::loadNames(void) {
    establishFolder(getCachePath("names/"));
    if (namesMap.size() > 0)  // already loaded
        return true;

    LOG8("Entering loadNames...");
    string_q prefundFile = getConfigPath("names/names_prefunds.tab");
    if (isEnabled(OPT_PREFUND)) {
        if (!loadPrefunds(prefundFile)) {
            return usage("Could not open prefunds data.");
        }
    }

    bool type1 = false;
    if (type1) {
        loadNamesDatabaseFromSQL();
        if (!importTabFile(namesMap, prefundFile))
            return usage("Could not open prefunds database...");
        // LOG8("Finished adding names from prefunds database...");
        buildOtherMaps();

    } else {
        string_q txtFile = getConfigPath("names/names.tab");
        string_q customFile = getConfigPath("names/names_custom.tab");
        time_q txtDate = laterOf(laterOf(fileLastModifyDate(txtFile), fileLastModifyDate(customFile)),
                                 fileLastModifyDate(prefundFile));

        string_q binFile = getCachePath("names/names.bin");
        time_q binDate = fileLastModifyDate(binFile);

        if (binDate > txtDate) {
            LOG8("Reading names from binary cache");
            CArchive nameCache(READING_ARCHIVE);
            if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
                nameCache >> namesMap;
                nameCache.Release();
                buildOtherMaps();
                return true;
            }
        }

        if (!importTabFile(namesMap, txtFile))
            return usage("Could not open names database...");

        if (!importTabFile(namesMap, customFile))
            return usage("Could not open custom names database...");

        if (!importTabFile(namesMap, prefundFile))
            return usage("Could not open prefunds database...");

        buildOtherMaps();

        CArchive nameCache(WRITING_ARCHIVE);
        if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
            nameCache << namesMap;
            nameCache.Release();
        }
    }

    return true;
}

//-----------------------------------------------------------------------
bool COptionsBase::findName(const string_q& addr, CAccountName& acct) {
    if (!loadNames())
        return false;

    if (namesMap[addr].address == addr) {
        acct = namesMap[addr];
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------
bool COptionsBase::findToken(CAccountName& acct, const address_t& addr) {
    if (tokenMap[addr].address == addr) {
        acct = tokenMap[addr];
        return true;
    }

    if (airdropMap[addr])
        return findName(addr, acct);

    return false;
}

// tags(30)    address (42)    name (80)    symbol (10)    source (80)    decimals (4)    description (300)

}  // namespace qblocks
