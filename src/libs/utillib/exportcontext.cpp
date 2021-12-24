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
#include "logging.h"

namespace qblocks {

extern string_q getConfigPath(const string_q& part);

//---------------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CExportContext expC;

//---------------------------------------------------------------------------
CExportContext& expContext(void) {
    return expC;
}

//---------------------------------------------------------------------------
CExportContext::CExportContext(void) {
    lev = 0;
    spcs = 2;
    tab = ' ';
    nl = '\n';
    quoteNums = false;
    quoteKeys = true;
    endingCommas = false;
    hexNums = false;
    hashesOnly = false;
    asEther = false;
    asDollars = false;
    asWei = true;
    tsMemMap = nullptr;
    tsCnt = 0;
    exportFmt = (isApiMode() ? API1 : TXT1);
    namesMap.clear();
}

//-----------------------------------------------------------------------
bool findName(const address_t& addr, CAccountName& acct) {
    if (expC.namesMap[addr].address == addr) {
        acct = expC.namesMap[addr];
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
bool findToken(const address_t& addr, CAccountName& acct) {
    const CAccountName item = expC.namesMap[addr];
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
    return expC.namesMap.size();
}

//-----------------------------------------------------------------------
void clearNames(void) {
    expC.namesMap.clear();
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
                if (expC.namesMap[account.address].address != account.address)
                    expC.namesMap[account.address] = account;
            }
        }
    }
    return;
}

//-----------------------------------------------------------------------
bool loadNames(void) {
    LOG4("Loading names");
    if (expC.namesMap.size() > 0) {
        LOG4("Already loaded");
        return true;
    }

    time_q binDate = fileLastModifyDate(getCachePath("names/names.bin"));
    time_q txtDate = laterOf(fileLastModifyDate(getConfigPath("names/names.tab")),
                             fileLastModifyDate(getConfigPath("names/names_custom.tab")));

    if (txtDate < binDate) {
        string_q binFile = getCachePath("names/names.bin");
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            nameCache >> expC.namesMap;
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
            nameCache << expC.namesMap;
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

//---------------------------------------------------------------------------
void indent(void) {
    expC.lev++;
}

//---------------------------------------------------------------------------
void unindent(void) {
    expC.lev--;
}

//---------------------------------------------------------------------------
string_q indentStr(void) {
    return string_q(expC.spcs * expC.lev, expC.tab);
}

//---------------------------------------------------------------------------
bool isJson(void) {
    return (expC.exportFmt == JSON1 || expC.exportFmt == API1 || expC.exportFmt == NONE1);
}

}  // namespace qblocks
