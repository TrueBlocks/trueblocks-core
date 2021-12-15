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

extern bool loadPrefunds(const string_q& prefundFile);
extern bool importTabFilePrefund(CAddressNameMap& theMap, const string_q& tabFilename);

//-----------------------------------------------------------------------
static bool importTabFile(CStringArray& lines, CAddressNameMap& theMap) {
    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, '\t');
        } else {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account;
                account.parseText(fields, line);
                if (theMap[account.address].address != account.address)
                    theMap[account.address] = account;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------
bool COptionsBase::loadNames(void) {
    establishFolder(getCachePath("names/"));
    if (namesMap.size() > 0)  // already loaded
        return true;

    LOG8("Entering loadNames...");

    string_q txtFile = getConfigPath("names/names.tab");
    string_q customFile = getConfigPath("names/names_custom.tab");
    string_q prefundFile = getConfigPath("names/names_prefunds.tab");

    time_q txtFileDate = fileLastModifyDate(txtFile);
    time_q customFileDate = fileLastModifyDate(customFile);
    time_q prefundFileDate = fileLastModifyDate(prefundFile);

    string_q binFile = getCachePath("names/names.bin");

    time_q binDate = fileLastModifyDate(binFile);
    time_q txtDate = laterOf(laterOf(txtFileDate, customFileDate), prefundFileDate);

    if (isEnabled(OPT_PREFUND)) {
        if (!loadPrefunds(prefundFile)) {
            return usage("Could not open prefunds data.");
        }
    }

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

    CStringArray lines;
    asciiFileToLines(txtFile, lines);
    asciiFileToLines(customFile, lines);
    if (!importTabFile(lines, namesMap))
        return usage("Could not import names database...");

    if (!importTabFilePrefund(namesMap, prefundFile))
        return usage("Could not open prefunds database...");

    buildOtherMaps();

    CArchive nameCache(WRITING_ARCHIVE);
    if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
        nameCache << namesMap;
        nameCache.Release();
    }

    return true;
}

//-----------------------------------------------------------------------
bool COptionsBase::buildOtherMaps(void) {
    for (const auto& item : namesMap) {
        if (item.second.symbol.empty())
            continue;

        bool t1 = contains(item.second.tags, "Tokens");
        bool t2 = contains(item.second.tags, "Contracts") && contains(item.second.name, "Airdrop");
        if (t1 || t2)
            tokenMap[item.second.address] = item.second;
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
    return false;
}

}  // namespace qblocks

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
