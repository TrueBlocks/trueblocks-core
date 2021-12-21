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

//---------------------------------------------------------------------------
// TODO: This is a singleton used throughout - it doesn't appear to have any downsides. If not, remove this comment.
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
    tokenMap.clear();
    prefundMap.clear();
}

//---------------------------------------------------------------------------
bool findToken(const address_t& addr, CAccountName& acct) {
    if (expC.tokenMap.size() == 0) {
        for (const auto& item : expC.namesMap) {
            if (item.second.symbol.empty())
                continue;
            bool t1 = contains(item.second.tags, "Tokens");
            bool t2 = contains(item.second.tags, "Contracts") && contains(item.second.name, "Airdrop");
            if (t1 || t2)
                expC.tokenMap[item.second.address] = item.second;
        }
    }

    if (expC.tokenMap[addr].address == addr) {
        acct = expC.tokenMap[addr];
        return true;
    }

    return false;
}

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

extern string_q getConfigPath(const string_q& part);
//-----------------------------------------------------------------------
bool loadPrefunds(void) {
    string_q prefundFile = getConfigPath("names/names_prefunds.tab");
    expC.prefundMap.clear();

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
            expC.prefundMap[key] = wei;
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
            expC.prefundMap[toLower(parts[0])] = str_2_Wei(parts[1]);
        }
        first = false;
    }

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(binFile, modeWriteCreate, LOCK_NOWAIT)) {
        LOG_WARN("Could not lock prefund cache at: ", binFile);
        return false;
    }

    CAddressWeiMap::iterator it = expC.prefundMap.begin();
    archive << uint64_t(expC.prefundMap.size());
    while (it != expC.prefundMap.end()) {
        archive << it->first << it->second;
        it++;
    }
    archive.Release();
    return true;
}

//-----------------------------------------------------------------------
wei_t prefundAt(const address_t& addr) {
    return expC.prefundMap[addr];
}

}  // namespace qblocks
