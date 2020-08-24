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
            errorMessage("Invalid cachePath (" + folder.getFullPath() + ") in config file.");
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
bool loadPrefunds(const string_q& prefundFile, COptionsBase& options) {
    // Note: we don't need to check the dates to see if the prefunds.txt file has been updated
    // since it will never change. In that sense, the binary file is always right once it's created.
    string_q binFile = getCachePath("names/names_prefunds_bals.bin");
    if (!fileExists(binFile)) {
        if (!fileExists(prefundFile))
            return false;
        CStringArray lines;
        asciiFileToLines(prefundFile, lines);
        bool first = true;
        for (auto line : lines) {
            if (!first && !startsWith(line, '#')) {
                CStringArray parts;
                explode(parts, line, '\t');
                options.prefundWeiMap[toLower(parts[0])] = str_2_Wei(parts[1]);
            }
            first = false;
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

typedef map<address_t, CAccountName> name_map_t;
//-----------------------------------------------------------------------
void addToMap(name_map_t& theMap, CAccountName& account, const string_q& tabFilename, uint64_t cnt) {
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
        account.name =
            account.name.empty()
                ? prefundName
                : contains(account.name, "(Prefund_") ? account.name : account.name + " (" + prefundName + ")";
        account.source = account.source.empty() ? "Genesis" : account.source;
        theMap[account.address] = account;
        cnt++;

    } else {
        // From the regular file - store the values found in the file
        theMap[account.address] = account;
    }
}

//-----------------------------------------------------------------------
bool importTabFile(name_map_t& theMap, const string_q& tabFilename) {
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
bool COptionsBase::loadNames(void) {
    ENTER("loadNames");

    if (getEnvStr("NO_NAMES") == "true")
        EXIT_NOMSG(true);

    if (namedAccounts.size() > 0)
        EXIT_NOMSG(true);

    LOG4("Entering loadNames...");
    establishFolder(getCachePath("names/"));

    string_q txtFile = configPath("names/names.tab");
    string_q customFile = configPath("names/names_custom.tab");
    string_q prefundFile = configPath("names/names_prefunds.tab");

    // A final set of options that do not have command line options
    if (isEnabled(OPT_PREFUND)) {
        if (!loadPrefunds(prefundFile, *this)) {
            EXIT_USAGE("Could not open prefunds data. Quitting...");
        }
    }

    string_q binFile = getCachePath("names/names.bin");
    time_q binDate = fileLastModifyDate(binFile);

    time_q txtDate = fileLastModifyDate(txtFile);
    txtDate = laterOf(txtDate, fileLastModifyDate(customFile));
    txtDate = laterOf(txtDate, fileLastModifyDate(prefundFile));

    // If none of the source files is out of date, load the entire names database as quickly as possible
    if (binDate > txtDate) {
        LOG4("Reading names from binary cache");
        CArchive nameCache(READING_ARCHIVE);
        if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
            nameCache >> namedAccounts;
            nameCache.Release();
            EXIT_NOMSG(true);
        }
    }

    name_map_t theMap;
    if (!importTabFile(theMap, txtFile))
        EXIT_USAGE("Could not open names database...");
    LOG4("Finished adding names from regular database...");

    if (!importTabFile(theMap, customFile))
        EXIT_USAGE("Could not open custom names database...");
    LOG4("Finished adding names from custom database...");

    if (!importTabFile(theMap, prefundFile))
        EXIT_USAGE("Could not open prefunds database...");
    LOG4("Finished adding names from prefunds database...");

    // theMap is already sorted by address, so simply copy it into the array
    for (auto item : theMap)
        namedAccounts.push_back(item.second);

    LOG4("Writing binary cache");
    CArchive nameCache(WRITING_ARCHIVE);
    if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
        nameCache << namedAccounts;
        nameCache.Release();
    }
    LOG4("Finished writing binary cache...");

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
using ResultPair = std::pair<CAccountNameArray::iterator, CAccountNameArray::iterator>;

//-----------------------------------------------------------------------
bool COptionsBase::forEveryNamedAccount(NAMEFUNC func, void* data) {
    if (!func)
        return false;

    for (auto namedAccount : namedAccounts) {
        if (!(*func)(namedAccount, data))
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------
bool COptionsBase::getNamedAccount(CAccountName& acct, const string_q& addr) {
    if (!loadNames())
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
