/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
bool COptionsBase::loadPrefunds(void) {
    // Note: we don't need to check the dates to see if the prefunds.txt file has been updated
    // since it will never change. In that sense, the binary file is always right once it's created.
    string_q binFile = configPath("names/names_prefunds.bin");
    string_q txtFile = configPath("names/names_prefunds.txt");
    if (!fileExists(binFile)) {
        if (!fileExists(txtFile))
            return false;
        CStringArray lines;
        asciiFileToLines(txtFile, lines);
        for (auto line : lines) {
            if (!startsWith(line, '#')) {
                CStringArray parts;
                explode(parts, line, '\t');
                prefundWeiMap[toLower(parts[0])] = str_2_Wei(parts[1]);
            }
        }
        CArchive archive(WRITING_ARCHIVE);
        if (!archive.Lock(binFile, modeWriteCreate, LOCK_NOWAIT))
            return false;
        addr_wei_mp::iterator it = prefundWeiMap.begin();
        archive << uint64_t(prefundWeiMap.size());
        while (it != prefundWeiMap.end()) {
            archive << it->first << it->second;
            it++;
        }
        archive.Release();
        return true;
    }
    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock(binFile, modeReadOnly, LOCK_NOWAIT))
        return false;
    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        string_q key;
        wei_t wei;
        archive >> key >> wei;
        prefundWeiMap[key] = wei;
    }
    archive.Release();
    return true;
}

//-----------------------------------------------------------------------
bool loadNames(COptionsBase& options) {
    if (options.namedAccounts.size() > 0)
        return true;

    string_q txtFile = configPath("names/names.txt");
    time_q txtDate = fileLastModifyDate(txtFile);

    string_q customFile = configPath("names/names_custom.txt");
    if (fileLastModifyDate(customFile) > txtDate)
        txtDate = fileLastModifyDate(customFile);

    string_q prefundFile = configPath("names/names_prefunds.txt");
    if (fileLastModifyDate(prefundFile) > txtDate)
        txtDate = fileLastModifyDate(prefundFile);

    string_q binFile = substitute(txtFile, ".txt", ".bin");
    time_q binDate = fileLastModifyDate(binFile);
    if (binDate > txtDate) {
        LOG4("Reading names from binary cache");
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            nameCache >> options.namedAccounts;
            nameCache.Release();
            return true;
        }
    }

    LOG4("Updating names database");
    string_q fields = "group|address|name|symbol|source|logo|description";
    CStringArray txtFields;
    explode(txtFields, fields, '|');
    CStringArray lines;
    asciiFileToLines(txtFile, lines);
    for (auto line : lines) {
        if (!startsWith(line, '#') && contains(line, "0x")) {
            CAccountName account;
            account.parseText(txtFields, line);
            options.namedAccounts.push_back(account);
        }
    }

    LOG4("Updating custom names database");
    lines.clear();
    asciiFileToLines(customFile, lines);
    for (auto line : lines) {
        if (!startsWith(line, '#') && contains(line, "0x")) {
            CAccountName account;
            account.parseText(txtFields, line);
            account.is_custom = true;
            options.namedAccounts.push_back(account);
        }
    }

    LOG4("Updating prefunds database");
    uint32_t cnt = 0;
    txtFields.clear();
    lines.clear();
    fields = "address|balance";
    explode(txtFields, fields, '|');
    asciiFileToLines(prefundFile, lines);
    for (auto line : lines) {
        if (!startsWith(line, '#')) {
            CAccountName account;
            account.parseText(txtFields, line);
            account.is_prefund = true;
            account.group = "80-Prefund";
            account.name = "Prefund_" + padNum4(cnt++);
            account.source = "Genesis";
            options.namedAccounts.push_back(account);
        }
    }

    sort(options.namedAccounts.begin(), options.namedAccounts.end());

    LOG4("Writing binary cache");
    CArchive nameCache(WRITING_ARCHIVE);
    if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
        nameCache << options.namedAccounts;
        nameCache.Release();
    }

    return true;
}

//-----------------------------------------------------------------------
using ResultPair = std::pair<CAccountNameArray::iterator, CAccountNameArray::iterator>;

//-----------------------------------------------------------------------
string_q COptionsBase::findNameByAddress(const string_q& addr) {
    CAccountName search;
    search.address = addr;
    return (getNamedAccount(search, addr) ? search.name : addr);
}

//-----------------------------------------------------------------------
bool COptionsBase::getNamedAccount(CAccountName& acct, const string_q& addr) {
    if (!loadNames(*this))
        return false;

    CAccountName search;
    search.address = addr;
    ResultPair range = equal_range(namedAccounts.begin(), namedAccounts.end(), search);
    if (range.first == namedAccounts.end() || range.first->address != addr)
        return false;
    acct = *range.first;
    return true;
}

// group(30)    address (42)    name (80)    symbol (10)    source (80)    logo (80)    description (300)

}  // namespace qblocks
