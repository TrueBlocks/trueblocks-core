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
extern string_q getConfigPath(const string_q& part);

//-----------------------------------------------------------------------
struct NameOnDiscHeader {
  public:
    uint64_t magic;
    uint64_t version;
    uint64_t count;
};

//-----------------------------------------------------------------------
// TODO: These singletons are used throughout - as long as this code
// TODO: is not inside a server, it doesn't appear to have any downsides.
// TODO: Assuming this is true, we can remove this comment.
static CAddressNameMap namesMap;
static map<address_t, NameOnDisc*> namePtrMap;
static NameOnDisc* namesAllocated = nullptr;
static uint64_t nNameRecords = 0;
static uint64_t allocSize = 0;

//-----------------------------------------------------------------------
const char* STR_BIN_LOC = "names/names.bin";
const char* STR_LOG_LOC = "names/edit_log.txt";

//-----------------------------------------------------------------------
static bool readNamesFromBinary(void) {
    string_q binFile = getCachePath(STR_BIN_LOC);
    nNameRecords = (fileSize(binFile) / sizeof(NameOnDisc));  // This number may be too large, but we adjust it below
    namesAllocated = new NameOnDisc[nNameRecords + 100];
    if (!namesAllocated) {
        LOG_ERR("Could not allocation memory for names");
        return false;
    }
    memset(namesAllocated, 0, sizeof(NameOnDisc) * nNameRecords);
    allocSize = nNameRecords + 100;

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
    nNameRecords = header->count;
    archive.Read(namesAllocated, sizeof(NameOnDisc), nNameRecords);
    archive.Release();

    // We want to read the entire file into memory with one read, thus
    // the above code, but we need to access the records by address
    // which means we need a map, however, we don't want to copy the
    // memory, so we make a map that points from address to the memory
    // location of the record in the array. Note, this also sorts the
    // records without sorting anything.
    for (size_t i = 0; i < nNameRecords; i++)
        namePtrMap[namesAllocated[i].address] = &namesAllocated[i];

    return true;
}

//-----------------------------------------------------------------------
static bool readNamesFromAscii(void) {
    string_q txtFile = getConfigPath("names/names.tab");
    string_q customFile = getConfigPath("names/names_custom.tab");

    CStringArray lines;
    asciiFileToLines(txtFile, lines);
    asciiFileToLines(customFile, lines);

    clearNames();
    namesAllocated = new NameOnDisc[lines.size() + 100];
    if (!namesAllocated) {
        LOG_ERR("Could not allocation memory for names");
        return false;
    }
    memset(namesAllocated, 0, sizeof(NameOnDisc) * lines.size());
    allocSize = lines.size() + 100;
    nNameRecords = 0;

    CStringArray fields;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, '\t');
        } else {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account;
                account.parseText(fields, line);
                if (!hasName(account.address))
                    namesAllocated[nNameRecords++].name_2_Disc(account);
            }
        }
    }

    // See the above comment about the map
    for (size_t i = 0; i < nNameRecords; i++)
        namePtrMap[namesAllocated[i].address] = &namesAllocated[i];

    return true;
}

//-----------------------------------------------------------------------
static bool writeNamesToBinary(void) {
    string_q binFile = getCachePath(STR_BIN_LOC);
    CArchive out(WRITING_ARCHIVE);
    if (out.Lock(binFile, modeWriteCreate, LOCK_WAIT)) {
        // We treat one whole record (the first) as the header. Yes,
        // there's a bit of wasted space, but it's easier to figure
        // out the number of records in the file without opening it
        // nNameRecords = (sizeOfFileInBytes / sizeOfRecord) - 1
        NameOnDisc fake;
        memset(&fake, 0, sizeof(NameOnDisc) * 1);
        NameOnDiscHeader* header = (NameOnDiscHeader*)&fake;
        header->magic = 0xdeadbeef;
        header->version = getVersionNum();
        header->count = nNameRecords;
        out.Write(&fake, sizeof(NameOnDisc), 1);

        out.Write(namesAllocated, sizeof(NameOnDisc), nNameRecords);

        out.Release();
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------
bool loadNames(void) {
    LOG_TEST_STR("Loading names");
    if (nNames()) {
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

        if (!writeNamesToBinary())
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------
bool clearNames(void) {
    namesMap.clear();
    namePtrMap.clear();
    if (namesAllocated) {
        delete[] namesAllocated;
        namesAllocated = nullptr;
    }
    return true;
}

//-----------------------------------------------------------------------
bool findName(const address_t& addr, CAccountName& acct) {
    // When the caller ask for an account name, we conver the in-memory record
    // into that data structure. This takes time, so we cache it in the namesMap
    // for speed. If it's there, we use it.
    if (namesMap[addr].address == addr) {
        acct = namesMap[addr];
        return true;
    }

    // If this is the first time the caller has asked for this address, convert
    // the in-memory record and cache it.
    if (hasName(addr)) {
        namePtrMap[addr]->disc_2_Name(acct);
        namesMap[addr] = acct;
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
void addPrefundToNamesMap(CAccountName& account, uint64_t cnt) {
    if (namesMap[account.address].address == account.address)
        return;
    if (hasName(account.address))
        return;

    address_t addr = account.address;
    account = namesMap[addr];
    account.address = addr;
    account.tags = "80-Prefund";
    account.source = "Genesis";
    account.isPrefund = true;
    account.name = "Prefund_" + padNum4(cnt);

    // TODO: This does not add the name to the pointer map, therefore prefunds
    // TODO: are not processed by a call to forEveryName
    namesMap[account.address] = account;
}

//-----------------------------------------------------------------------
bool hasName(const address_t& addr) {
    return namePtrMap[addr] != nullptr;
}

//-----------------------------------------------------------------------
size_t nNames(void) {
    return namePtrMap.size();
}

//-----------------------------------------------------------------------
bool forEveryNameOld(NAMEFUNC func, void* data) {
    if (!func)
        return false;

    for (auto name : namePtrMap) {
        if (!name.second)
            continue;
        CAccountName acct;
        name.second->disc_2_Name(acct);
        if (!(*func)(acct, data))
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------
bool forEveryNameNew(NAMEODFUNC func, void* data) {
    if (!func)
        return false;

    for (auto name : namePtrMap) {
        if (!(*func)(name.second, data))
            return false;
    }

    return true;
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
bool updateName(const CAccountName& target, const string_q& crud) {
    if (!hasName(target.address)) {
        if (nNameRecords == allocSize) {
            LOG4("Growing names array");
            // TODO: This has to grow the array or we will crash. It's a simple
            // TODO: realloc, but then a clearing of the array and reloading
            // growNames(100);
        }
        namesAllocated[nNameRecords].name_2_Disc(target);
        namePtrMap[namesAllocated[nNameRecords].address] = &namesAllocated[nNameRecords];
        nNameRecords++;
    }
    NameOnDisc* nod = namePtrMap[target.address];
    if (crud == "create" || crud == "update") {
        LOG4((crud == "create" ? "Adding " : "Editing "), nod->address);
        // TODO: we could resort here, but it doesn't appear to matter. When we write
        // TODO: the file out, we should sort it
        nod->name_2_Disc(target);

    } else if (crud == "delete") {
        LOG4("Deleting ", nod->address);
        nod->flags |= IS_DELETED;

    } else if (crud == "undelete") {
        LOG4("Undeleting ", nod->address);
        nod->flags &= ~(IS_DELETED);

    } else if (crud == "remove") {
        LOG4("Removing ", nod->address);
        // We can copy the memory from the position one past this record to the
        // end of the array on top of this record to remove it. If this is the
        // last record, just decrement the number of records
        size_t sz = sizeof(NameOnDisc);
        NameOnDisc* start = namesAllocated;
        size_t nodRecord = size_t(nod - start) / sz;
        if (nodRecord < (nNameRecords - 1)) {
            memcpy(nod, nod + sizeof(NameOnDisc), (nNameRecords - nodRecord - 1) * sz);
        }
        nNameRecords--;
        writeNamesToBinary();

        // Reset the pointers
        clearNames();
        readNamesFromBinary();

    } else {
        // Don't know what this is, quit out
        return false;
    }

    writeNamesToBinary();

    if (!isTestMode()) {
        ostringstream editRecord;
        editRecord << Now().Format(FMT_JSON) << crud << "\t" << target.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
        stringToAsciiFile(getCachePath(STR_LOG_LOC), editRecord.str());
    }

    return true;
}

// TODO: When writing out the data to the .csv file
// TODO:    sort it
// TODO:    do not export zero addresses or prefunds or customs
// TODO:    turn off TEST_MODE while writing otherwise we lose the names
// TODO:    for autoname, do not write records that assign address to name
// TODO:    for autoname, do not write records that assign address to name
// TODO:    string_q copyBack = getGlobalConfig("ethNames")->getConfigStr("settings", "source", "<not_set>");

}  // namespace qblocks
