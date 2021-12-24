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
#include "names.h"
#include "logging.h"

namespace qblocks {

extern string_q getConfigPath(const string_q& part);

//---------------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CAccountNameMap namesMap;

//-----------------------------------------------------------------------
bool forEveryName(NAMEFUNC func, void* data) {
    if (!func)
        return false;

    for (auto name : namesMap)
        if (!(*func)(name.second, data))
            return false;
    return true;
}

//-----------------------------------------------------------------------
void addPrefundToNamesMap(CAccountName& account, uint64_t cnt) {
    // If it's already there, don't alter it or add it to the map
    if (namesMap[account.address].address == account.address)
        return;

    address_t addr = account.address;
    account = namesMap[addr];
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

    namesMap[account.address] = account;
}

//-----------------------------------------------------------------------
bool findName(const address_t& addr, CAccountName& acct) {
    if (namesMap[addr].address == addr) {
        acct = namesMap[addr];
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
bool findToken(const address_t& addr, CAccountName& acct) {
    const CAccountName item = namesMap[addr];
    if (item.address == addr) {
        bool t1 = contains(item.tags, "Tokens");
        bool t2 = contains(item.tags, "Contracts") && contains(item.name, "Airdrop");
        if (t1 || t2) {
            acct = item;
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------
uint64_t nNames(void) {
    return namesMap.size();
}

//-----------------------------------------------------------------------
void clearNames(void) {
    namesMap.clear();
}

//-----------------------------------------------------------------------
static void importTabFile(CStringArray& lines) {
    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, '\t');
        } else {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account;
                account.parseText(fields, line);
                if (namesMap[account.address].address != account.address)
                    namesMap[account.address] = account;
            }
        }
    }
    return;
}

//-----------------------------------------------------------------------
bool loadNames(void) {
    LOG_TEST_STR("Loading names");
    if (namesMap.size() > 0) {
        LOG_TEST_STR("Already loaded");
        return true;
    }

    time_q binDate = fileLastModifyDate(getCachePath("names/names.bin"));
    time_q txtDate = laterOf(fileLastModifyDate(getConfigPath("names/names.tab")),
                             fileLastModifyDate(getConfigPath("names/names_custom.tab")));

    if (txtDate < binDate) {
        string_q binFile = getCachePath("names/names.bin");
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            nameCache >> namesMap;
            nameCache.Release();
        }

    } else {
        CStringArray lines;

        string_q txtFile = getConfigPath("names/names.tab");
        string_q customFile = getConfigPath("names/names_custom.tab");
        asciiFileToLines(txtFile, lines);
        asciiFileToLines(customFile, lines);
        importTabFile(lines);

        establishFolder(getCachePath("names/"));
        string_q binFile = getCachePath("names/names.bin");
        CArchive nameCache(WRITING_ARCHIVE);
        if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
            nameCache << namesMap;
            nameCache.Release();
        }
    }

    return true;
}

// typedef enum {
//     IS_CUSTOM = (1 << 0),
//     IS_PREFUND = (1 << 1),
//     IS_CONTRACT = (1 << 2),
//     IS_ERC20 = (1 << 3),
//     IS_ERC721 = (1 << 4),
//     IS_DELETED = (1 << 5),
// } name_vars_t;

// struct NameOnDisc {
//   public:
//     char tags[30];
//     char address[42];
//     char name[120];
//     char symbol[30];
//     char source[180];
//     char description[255];
//     uint16_t decimals;
//     uint16_t nameVars;
// };

}  // namespace qblocks
