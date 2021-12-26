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
#include "prefunds.h"
#include "logging.h"

namespace qblocks {

//-----------------------------------------------------------------------
extern bool loadNames_int(void);
extern bool clearNames_int(void);
extern bool findName_int(const address_t& addr, CAccountName& acct);
extern bool findToken_int(const address_t& addr, CAccountName& acct);
extern bool hasName_int(const address_t& addr);
extern size_t nNames_int(void);
extern void addPrefundToNamesMap_int(CAccountName& account, uint64_t cnt);
extern bool loadNames2025(void);
extern bool clearNames2025(void);
extern bool findName2025(const address_t& addr, CAccountName& acct);
extern bool findToken2025(const address_t& addr, CAccountName& acct);
extern bool hasName2025(const address_t& addr);
extern size_t nNames2025(void);
extern void addPrefundToNamesMap2025(CAccountName& account, uint64_t cnt);

extern string_q getConfigPath(const string_q& part);

//-----------------------------------------------------------------------
// TODO: These singletons are used throughout - it doesn't appear to have any downsides.
// TODO: Assuming this is true, eventually we can remove this comment.
static CAccountNameMap namesMap;

const char* STR_BIN_LOC = "names/names.bin";
const char* STR_BIN_LOC2025 = "names/names2025.bin";

//-----------------------------------------------------------------------
bool oldNames = false;

//-----------------------------------------------------------------------
enum {
    IS_NONE = (0),
    IS_CUSTOM = (1 << 0),
    IS_PREFUND = (1 << 1),
    IS_CONTRACT = (1 << 2),
    IS_ERC20 = (1 << 3),
    IS_ERC721 = (1 << 4),
    IS_DELETED = (1 << 5),
};

NameOnDisc::NameOnDisc(void) : decimals(0), flags(0) {
}

//-----------------------------------------------------------------------
map<address_t, NameOnDisc*> namePtrMap;
CAddressNameMap names2025Map;
uint64_t nRecords = 0;
NameOnDisc* names = nullptr;

//-----------------------------------------------------------------------
struct NameOnDiscHeader {
  public:
    uint64_t magic;
    uint64_t version;
    uint64_t count;
};

bool loadNames(bool old) {
    if (old)
        return loadNames_int();
    return loadNames2025();
}
bool clearNames(bool old) {
    if (old)
        return clearNames_int();
    return clearNames2025();
}
bool findName(bool old, const address_t& addr, CAccountName& acct) {
    if (old)
        return findName_int(addr, acct);
    return findName2025(addr, acct);
}
bool findToken(bool old, const address_t& addr, CAccountName& acct) {
    if (old)
        return findToken_int(addr, acct);
    return findToken2025(addr, acct);
}
void addPrefundToNamesMap(bool old, CAccountName& account, uint64_t cnt) {
    if (old)
        return addPrefundToNamesMap_int(account, cnt);
    return addPrefundToNamesMap2025(account, cnt);
}
bool hasName(bool old, const address_t& addr) {
    if (old)
        return hasName_int(addr);
    return hasName2025(addr);
}
size_t nNames(bool old) {
    if (old)
        return nNames_int();
    return nNames2025();
}

//-----------------------------------------------------------------------
bool hasName_int(const address_t& addr) {
    return namesMap[addr].address == addr;
}

//-----------------------------------------------------------------------
bool hasName2025(const address_t& addr) {
    return namePtrMap[addr] != nullptr;
}

//-----------------------------------------------------------------------
static bool readNamesFromBinary(void) {
    string_q binFile = getCachePath(STR_BIN_LOC);
    CArchive nameCache(READING_ARCHIVE);
    if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
        nameCache >> namesMap;
        nameCache.Release();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
static bool readNamesFromBinary2025(void) {
    string_q binFile = getCachePath(STR_BIN_LOC2025);
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
bool clearNames_int(void) {
    // string_q binFile = getCachePath(STR_BIN_LOC);
    // ::remove(binFile.c_str());
    namesMap.clear();
    return true;
}

//-----------------------------------------------------------------------
bool clearNames2025(void) {
    // string_q binFile = getCachePath(STR_BIN_LOC2025);
    // ::remove(binFile.c_str());
    namePtrMap.clear();
    names2025Map.clear();
    if (names) {
        delete[] names;
        names = nullptr;
    }
    return true;
}

//-----------------------------------------------------------------------
#if 0
bool addPrefund_int(const address_t& addr, void* data) {
    uint32_t count = *(uint32_t*)data;
    *((uint32_t*)data) = count + 1;

    // order matters
    if (hasName_int(addr))
        return true;

    CAccountName account;
    account.address = addr;
    account.tags = "80-Prefund";
    account.source = "Genesis";
    account.isPrefund = true;
    account.name = "Prefund_" + padNum4(count);
    namesMap[account.address] = account;

    return true;
}
#endif

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
                if (!hasName_int(account.address))
                    namesMap[account.address] = account;
            }
        }
    }
    return true;
}

#if 0
//-----------------------------------------------------------------------
bool addPrefund2025(const address_t& addr, void* data) {
    uint32_t count = *(uint32_t*)data;
    *((uint32_t*)data) = count + 1;

    // order matters
    if (hasName2025(addr))
        return true;

    NameOnDisc* nod = &names[nRecords++];
    strcpy(nod->tags, "80-Prefund");
    strcpy(nod->address, addr.c_str());
    strcpy(nod->name, ("Prefund_" + padNum4(count)).c_str());
    // strcpy(nod->symbol, "");
    strcpy(nod->source, "Genesis");
    // strcpy(nod->description, "");
    nod->decimals = 0;
    nod->flags = IS_PREFUND;
    return true;
}
#endif

//-----------------------------------------------------------------------
static bool readNamesFromAscii2025(void) {
    string_q txtFile = getConfigPath("names/names.tab");
    string_q customFile = getConfigPath("names/names_custom.tab");

    CStringArray lines;
    asciiFileToLines(txtFile, lines);
    asciiFileToLines(customFile, lines);

    clearNames2025();
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
                if (!hasName2025(account.address))
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
    string_q binFile = getCachePath(STR_BIN_LOC);
    CArchive nameCache(WRITING_ARCHIVE);
    if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
        nameCache << namesMap;
        nameCache.Release();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
static bool writeNamesBinary2025(void) {
    string_q binFile = getCachePath(STR_BIN_LOC2025);
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
bool forEveryNameOld(NAMEFUNC func, void* data) {
    if (!func)
        return false;

    if (oldNames) {
        for (auto name : namesMap) {
            if (!(*func)(name.second, data))
                return false;
        }
    } else {
        for (auto name : namePtrMap) {
            CAccountName acct;
            name.second->disc_2_Name(acct);
            if (!(*func)(acct, data))
                return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------
bool forEveryNameNew(NAMEODFUNC func, void* data) {
    if (!func)
        return false;

    if (oldNames) {
        for (auto name : namesMap) {
            NameOnDisc disc;
            disc.name_2_Disc(name.second);
            if (!(*func)(&disc, data))
                return false;
        }
    } else {
        for (auto name : namePtrMap) {
            if (!(*func)(name.second, data))
                return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------
size_t nNames_int(void) {
    return namesMap.size();
}

//-----------------------------------------------------------------------
size_t nNames2025(void) {
    return namePtrMap.size();
}

//-----------------------------------------------------------------------
bool loadNames_int(void) {
    LOG_TEST_STR("Loading names");
    if (nNames_int()) {
        LOG_TEST_STR("Already loaded");
        return true;
    }

    time_q binDate = fileLastModifyDate(getCachePath(STR_BIN_LOC));
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
bool loadNames2025(void) {
    LOG_TEST_STR("Loading names");
    if (nNames2025()) {
        LOG_TEST_STR("Already loaded");
        return true;
    }

    time_q binDate = fileLastModifyDate(getCachePath(STR_BIN_LOC2025));
    time_q txtDate = laterOf(fileLastModifyDate(getConfigPath("names/names.tab")),
                             fileLastModifyDate(getConfigPath("names/names_custom.tab")));

    if (txtDate < binDate) {
        if (!readNamesFromBinary2025())
            return false;

    } else {
        if (!readNamesFromAscii2025())
            return false;

        if (!writeNamesBinary2025())
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------
bool findName_int(const address_t& addr, CAccountName& acct) {
    if (hasName_int(addr)) {
        acct = namesMap[addr];
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------
bool findName2025(const address_t& addr, CAccountName& acct) {
    if (names2025Map[addr].address == addr) {
        acct = names2025Map[addr];
        return true;
    }
    if (hasName2025(addr)) {
        namePtrMap[addr]->disc_2_Name(acct);
        names2025Map[addr] = acct;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
bool findToken_int(const address_t& addr, CAccountName& acct) {
    CAccountName item;
    if (findName_int(addr, item)) {
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
bool findToken2025(const address_t& addr, CAccountName& acct) {
    CAccountName item;
    if (findName2025(addr, item)) {
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
void addPrefundToNamesMap_int(CAccountName& account, uint64_t cnt) {
    if (hasName_int(account.address))
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
void addPrefundToNamesMap2025(CAccountName& account, uint64_t cnt) {
    if (names2025Map[account.address].address == account.address)
        return;
    if (hasName2025(account.address))
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

    // TODO: This does not add the name to the pointer map, therefore prefunds won't be processed by forEveryName
    names2025Map[account.address] = account;
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

}  // namespace qblocks
