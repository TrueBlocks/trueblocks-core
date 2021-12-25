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

//-----------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CAccountNameMap namesMap;

//-----------------------------------------------------------------------
static bool readNamesFromBinary(void) {
    string_q binFile = getCachePath("names/names.bin");
    CArchive nameCache(READING_ARCHIVE);
    if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
        nameCache >> namesMap;
        nameCache.Release();
        return true;
    }
    return false;
}

typedef bool (*NAMEODFUNC)(NameOnDisc* name, void* data);
//-----------------------------------------------------------------------
map<address_t, NameOnDisc*> namePtrMap;
CAddressNameMap names2Map;
uint64_t nRecords = 0;
NameOnDisc* names = nullptr;

//-----------------------------------------------------------------------
struct NameOnDiscHeader {
  public:
    uint64_t magic;
    uint64_t version;
    uint64_t count;
};

//-----------------------------------------------------------------------
bool hasName(const address_t& addr) {
    return namesMap[addr].address == addr;
}

//-----------------------------------------------------------------------
bool hasName2(const address_t& addr) {
    return namePtrMap[addr] != nullptr;
}

//-----------------------------------------------------------------------
static bool readNamesFromBinary2(void) {
    string_q binFile = getCachePath("names/names2.bin");
    nRecords = (fileSize(binFile) / sizeof(NameOnDisc));  // may be one too large, but we'll adjust below
    names = new NameOnDisc[nRecords];
    memset(names, 0, sizeof(NameOnDisc) * nRecords);

    NameOnDisc fake;
    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
        cerr << "Could not lock " << binFile << endl;
        return false;
    }

    NameOnDiscHeader* header = (NameOnDiscHeader*)&fake;
    archive.Read(&header->magic, sizeof(uint64_t), 1);
    archive.Seek(SEEK_SET, 0);
    if (header->magic != 0xdeadbeef) {
        cerr << "Got an invalid file header in names file: ";
        cerr << header->magic << "." << header->version << "." << header->count;
        cerr << endl;
        archive.Release();
        return false;
    }

    archive.Read(&fake, sizeof(NameOnDisc), 1);
    ASSERT(header.version >= getVersionNum(0, 18, 0));
    nRecords = header->count;
    archive.Read(names, sizeof(NameOnDisc), nRecords);
    archive.Release();

    for (size_t i = 0; i < nRecords; i++)
        namePtrMap[names[i].address] = &names[i];

    return true;
}

//-----------------------------------------------------------------------
static bool readNamesFromAscii(void) {
    string_q txtFile = getConfigPath("names/names.tab");
    string_q customFile = getConfigPath("names/names_custom.tab");

    CStringArray lines;
    asciiFileToLines(txtFile, lines);
    asciiFileToLines(customFile, lines);

    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, '\t');
        } else {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account;
                account.parseText(fields, line);
                if (!hasName(account.address))
                    namesMap[account.address] = account;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------
static bool readNamesFromAscii2(void) {
    string_q txtFile = getConfigPath("names/names.tab");
    string_q customFile = getConfigPath("names/names_custom.tab");

    CStringArray lines;
    asciiFileToLines(txtFile, lines);
    asciiFileToLines(customFile, lines);

    clearNames2();
    names = new NameOnDisc[lines.size()];
    memset(names, 0, sizeof(NameOnDisc) * lines.size());
    nRecords = 0;

    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, '\t');
        } else {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account;
                account.parseText(fields, line);
                names[nRecords++].name_2_Disc(account);
            }
        }
    }

    for (size_t i = 0; i < nRecords; i++)
        namePtrMap[names[i].address] = &names[i];

    return true;
}

//-----------------------------------------------------------------------
static bool writeNamesBinary(void) {
    establishFolder(getCachePath("names/"));
    string_q binFile = getCachePath("names/names.bin");
    CArchive nameCache(WRITING_ARCHIVE);
    if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
        nameCache << namesMap;
        nameCache.Release();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
static bool writeNamesBinary2(void) {
    string_q binFile = getCachePath("names/names2.bin");
    CArchive out(WRITING_ARCHIVE);
    if (out.Lock(binFile, modeWriteCreate, LOCK_WAIT)) {
        NameOnDisc fake;
        memset(&fake, 0, sizeof(NameOnDisc) * 1);
        NameOnDiscHeader* header = (NameOnDiscHeader*)&fake;
        header->magic = 0xdeadbeef;
        header->version = getVersionNum();
        header->count = nRecords;
        out.Write(&fake, sizeof(NameOnDisc), 1);
        out.Write(names, sizeof(NameOnDisc), nRecords);
        out.Release();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
bool NameOnDisc::name_2_Disc(const CAccountName& nm) {
    strncpy(tags, nm.tags.c_str(), nm.tags.length());
    strncpy(address, nm.address.c_str(), nm.address.length());
    strncpy(name, nm.name.c_str(), nm.name.length());
    strncpy(symbol, nm.symbol.c_str(), nm.symbol.length());
    strncpy(source, nm.source.c_str(), nm.source.length());
    strncpy(description, nm.description.c_str(), nm.description.length());
    decimals = uint16_t(nm.decimals);
    flags |= (nm.isCustom ? IS_CUSTOM : IS_NONE);
    flags |= (nm.isPrefund ? IS_PREFUND : IS_NONE);
    flags |= (nm.isContract ? IS_CONTRACT : IS_NONE);
    flags |= (nm.isErc20 ? IS_ERC20 : IS_NONE);
    flags |= (nm.isErc721 ? IS_ERC721 : IS_NONE);
    flags |= (nm.isDeleted() ? IS_DELETED : IS_NONE);
    return true;
}

//-----------------------------------------------------------------------
bool NameOnDisc::disc_2_Name(CAccountName& nm) const {
    nm.tags = tags;
    nm.address = address;
    nm.name = name;
    nm.symbol = symbol;
    nm.source = source;
    nm.description = description;
    nm.decimals = decimals;
    nm.isCustom = (flags & IS_CUSTOM);
    nm.isPrefund = (flags & IS_PREFUND);
    nm.isContract = (flags & IS_CONTRACT);
    nm.isErc20 = (flags & IS_ERC20);
    nm.isErc721 = (flags & IS_ERC721);
    nm.m_deleted = (flags & IS_DELETED);
    return true;
}

//-----------------------------------------------------------------------
string_q NameOnDisc::Format(void) const {
    ostringstream os;
    os << tags << "\t" << address << "\t" << name << "\t" << symbol << "\t" << source << "\t";
    os << (decimals == 0 ? "" : uint_2_Str(decimals)) << "\t" << description << "\t";
    os << (flags & IS_PREFUND ? "true" : "false") << "\t";
    os << (flags & IS_CUSTOM ? "true" : "false") << "\t";
    os << (flags & IS_DELETED ? "true" : "false") << "\t";
    os << (flags & IS_CONTRACT ? "true" : "false") << "\t";
    os << (flags & IS_ERC20 ? "true" : "false") << "\t";
    os << (flags & IS_ERC721 ? "true" : "false");
    return os.str();
}

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
bool forEveryName2(NAMEODFUNC func, void* data) {
    if (!func)
        return false;
    for (auto name : namePtrMap) {
        if (!(*func)(name.second, data))
            return false;
    }
    return true;
}

//-----------------------------------------------------------------------
bool loadNames(void) {
    LOG_TEST_STR("Loading names");
    if (nNames()) {
        LOG_TEST_STR("Already loaded");
        return true;
    }

    time_q binDate = fileLastModifyDate(getCachePath("names/names.bin"));
    time_q txtDate = laterOf(fileLastModifyDate(getConfigPath("names/names.tab")),
                             fileLastModifyDate(getConfigPath("names/names_custom.tab")));

    if (txtDate < binDate) {
        if (!readNamesFromBinary())
            return false;

    } else {
        if (!readNamesFromAscii())
            return false;

        if (!writeNamesBinary())
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------
bool loadNames2(void) {
    LOG_TEST_STR("Loading names 2");
    if (nNames2()) {
        LOG_TEST_STR("Already loaded 2");
        return true;
    }

    time_q binDate = fileLastModifyDate(getCachePath("names/names2.bin"));
    time_q txtDate = laterOf(fileLastModifyDate(getConfigPath("names/names.tab")),
                             fileLastModifyDate(getConfigPath("names/names_custom.tab")));

    if (txtDate < binDate) {
        if (!readNamesFromBinary2())
            return false;

    } else {
        if (!readNamesFromAscii2())
            return false;

        if (!writeNamesBinary2())
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------
void clearNames(void) {
    namesMap.clear();
}

//-----------------------------------------------------------------------
bool clearNames2(void) {
    if (names) {
        delete[] names;
        names = nullptr;
    }
    return true;
}

//-----------------------------------------------------------------------
bool findName(const address_t& addr, CAccountName& acct) {
    if (hasName(addr)) {
        acct = namesMap[addr];
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------
bool findName2(const address_t& addr, CAccountName& acct) {
    if (names2Map[addr].address == addr) {
        acct = names2Map[addr];
        return true;
    }
    if (hasName2(addr)) {
        namePtrMap[addr]->disc_2_Name(acct);
        names2Map[addr] = acct;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
bool findToken(const address_t& addr, CAccountName& acct) {
    CAccountName item;
    if (findName(addr, item)) {
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
bool findToken2(const address_t& addr, CAccountName& acct) {
    CAccountName item;
    if (findName2(addr, item)) {
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
size_t nNames(void) {
    return namesMap.size();
}

//-----------------------------------------------------------------------
size_t nNames2(void) {
    return namePtrMap.size();
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

}  // namespace qblocks
