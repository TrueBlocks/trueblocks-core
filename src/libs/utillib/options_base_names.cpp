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

//-------------------------------------------------------------------------
string_q getCachePath(const string_q& _part) {
    // TODO(tjayrush): global data
    static string_q g_cachePath;
    if (!g_cachePath.empty())  // leave early if we can
        return substitute((g_cachePath + _part), "//", "/");

    {  // give ourselves a frame - always enters - forces creation in the frame
       // Wait until any other thread is finished filling the value.
        mutex aMutex;
        lock_guard<mutex> lock(aMutex);

        // Another thread may have filled the data while we were waiting
        if (!g_cachePath.empty())
            return substitute((g_cachePath + _part), "//", "/");

        // Otherwise, fill the value
        CToml toml(configPath("quickBlocks.toml"));
        string_q path = toml.getConfigStr("settings", "cachePath", "<NOT_SET>");
        if (path == "<NOT_SET>") {
            // May have been an old installation, so try to upgrade
            path = toml.getConfigStr("settings", "blockCachePath", "<NOT_SET>");
            if (path == "<NOT_SET>")
                path = configPath("cache/");
            toml.setConfigStr("settings", "cachePath", path);
            toml.deleteKey("settings", "blockCachePath");
            toml.writeFile();
        }

        CFilename folder(path);
        if (!folderExists(folder.getFullPath()))
            establishFolder(folder.getFullPath());

        g_cachePath = folder.getFullPath();
        if (!folder.isValid()) {
            cerr << "{ \"errors\": [\"Invalid cachePath (" << folder.getFullPath()
                 << ") in config file. Quitting.\"] }\n";
            path = configPath("cache/");
            CFilename fallback(path);
            g_cachePath = fallback.getFullPath();
        }
        if (!endsWith(g_cachePath, "/"))
            g_cachePath += "/";
    }

    return substitute((g_cachePath + _part), "//", "/");
}

//-----------------------------------------------------------------------
bool loadPrefunds(COptionsBase& options) {
    // Note: we don't need to check the dates to see if the prefunds.txt file has been updated
    // since it will never change. In that sense, the binary file is always right once it's created.
    string_q binFile = getCachePath("names/names_prefunds.bin");
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
                options.prefundWeiMap[toLower(parts[0])] = str_2_Wei(parts[1]);
            }
        }
        CArchive archive(WRITING_ARCHIVE);
        if (!archive.Lock(binFile, modeWriteCreate, LOCK_NOWAIT))
            return false;
        addr_wei_mp::iterator it = options.prefundWeiMap.begin();
        archive << uint64_t(options.prefundWeiMap.size());
        while (it != options.prefundWeiMap.end()) {
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
        options.prefundWeiMap[key] = wei;
    }
    archive.Release();
    return true;
}

//-----------------------------------------------------------------------
bool loadNames(COptionsBase& options) {
    if (options.namedAccounts.size() > 0)
        return true;

    establishFolder(getCachePath("names/"));

    // A final set of options that do not have command line options
    if (options.isEnabled(OPT_PREFUND))
        if (!loadPrefunds(options))
            return options.usage("Could not open prefunds data. Quitting...");

    string_q binFile = getCachePath("names/names.bin");
    time_q binDate = fileLastModifyDate(binFile);

    string_q txtFile = configPath("names/names.txt");
    string_q customFile = configPath("names/names_custom.txt");
    string_q prefundFile = configPath("names/names_prefunds.txt");

    time_q txtDate = fileLastModifyDate(txtFile);
    txtDate = laterOf(txtDate, fileLastModifyDate(customFile));
    txtDate = laterOf(txtDate, fileLastModifyDate(prefundFile));

    if (binDate > txtDate) {
        LOG4("Reading names from binary cache");
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            nameCache >> options.namedAccounts;
            nameCache.Release();
            return true;
        }
    }

    CStringArray txtFields;
    string_q fields;
    CStringArray lines;

    LOG4("Updating names database");
    txtFields.clear();
    lines.clear();
    fields = "tags|address|name|symbol|source|decimals|description";
    explode(txtFields, fields, '|');
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
            account.tags = "80-Prefund";
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

//-----------------------------------------------------------------------
string_q COptionsBase::findNameByAddress(const string_q& addr) {
    CAccountName search;
    search.address = addr;
    return (getNamedAccount(search, addr) ? search.name : addr);
}

// tags(30)    address (42)    name (80)    symbol (10)    source (80)    decimals (4)    description (300)

}  // namespace qblocks
