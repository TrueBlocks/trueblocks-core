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
#include "etherlib.h"

namespace qblocks {
//--------------------------------------------------------------------------------------------------------------------
enum {
    IS_NONE = (0),
    IS_CUSTOM = (1 << 0),
    IS_PREFUND = (1 << 1),
    IS_CONTRACT = (1 << 2),
    IS_ERC20 = (1 << 3),
    IS_ERC721 = (1 << 4),
    IS_DELETED = (1 << 5),
};

//--------------------------------------------------------------------------------------------------------------------
class NameOnDisc {
  public:
    char tags[30 + 1];
    char address[42 + 1];
    char name[120 + 1];
    char symbol[30 + 1];
    char source[180 + 1];
    char description[255 + 1];
    uint16_t decimals;
    uint16_t flags;
    NameOnDisc(void) : decimals(0), flags(0) {
    }
    void fromOld(const CAccountName& nm);
    void toOld(CAccountName& nm) const;
    string_q Format(void) const;
};

//--------------------------------------------------------------------------------------------------------------------
struct NameOnDiscHeader {
  public:
    uint64_t magic;
    uint64_t version;
    uint64_t count;
};

extern bool writeNames2(void);

}  // namespace qblocks

extern bool showName(CAccountName& name, void* data);
extern bool showName2(NameOnDisc* name, void* data);

uint64_t nRecords = 0;
NameOnDisc* names = nullptr;

CStringArray tests = {
    "0x43c65d1234edde9c9bc638f1fb284e1eb0c7ca1d",
    "0xa6a840e50bcaa50da017b91a0d86b8b2d41156ee",
    "0x29e240cfd7946ba20895a7a02edb25c210f9f324",
};

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

#define NTESTS 20
#define NRUNS 500
    if (argc < 2) {
        for (size_t x = 0; x < NTESTS; x++) {
            loadNames();
            forEveryName(showName, nullptr);
            for (size_t i = 0; i < NRUNS; i++) {
                for (auto test : tests) {
                    CAccountName name;
                    findName(test, name);
                    cout << name << endl;
                }
            }
            cerr << "A-" << x << endl;
            clearNames();
        }

    } else if (argc < 3) {
        for (size_t x = 0; x < NTESTS; x++) {
            loadNames2();
            forEveryName2(showName2, nullptr);
            for (size_t i = 0; i < NRUNS; i++) {
                for (auto test : tests) {
                    CAccountName name;
                    findName2(test, name);
                    cout << name << endl;
                }
            }
            cerr << "B-" << x << endl;
            clearNames2();
        }

    } else {
        CAddressNameMap names2Map2;
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(getCachePath("names/names.bin"), modeReadOnly, LOCK_NOWAIT)) {
            archive >> names2Map2;
            archive.Release();

            names = new NameOnDisc[names2Map2.size()];
            memset(names, 0, sizeof(NameOnDisc) * names2Map2.size());
            nRecords = 0;

            for (auto item : names2Map2)
                names[nRecords++].fromOld(item.second);

            // for (size_t i = 0; i < nRecords; i++) {
            //     CAccountName nm;
            //     names[i].toOld(nm);
            //     cout << nm.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
            // }

            if (writeNames2())
                return 1;

            cerr << fileExists("./out.bin") << "\t" << fileSize("./out.bin") << "\t"
                 << fileSize(getCachePath("names/names.bin")) << endl;

            clearNames2();

        } else {
            cerr << "Could not open names.bin" << endl;
        }
    }

    etherlib_cleanup();
    return 1;
}

namespace qblocks {
//--------------------------------------------------------------------------------------------------------------------
void NameOnDisc::fromOld(const CAccountName& nm) {
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
}

//--------------------------------------------------------------------------------------------------------------------
void NameOnDisc::toOld(CAccountName& nm) const {
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
}

//--------------------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------------
map<address_t, NameOnDisc*> namePtrMap;
CAddressNameMap names2Map;

//-----------------------------------------------------------------------
bool showName(CAccountName& name, void* data) {
    cout << name.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
    return true;
}

typedef bool (*NAMEODFUNC)(NameOnDisc* name, void* data);
//-----------------------------------------------------------------------
bool showName2(NameOnDisc* name, void* data) {
    cout << name->Format() << endl;
    return true;
}

namespace qblocks {
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
bool findName2(const address_t& addr, CAccountName& acct) {
    if (names2Map[addr].address == addr) {
        acct = names2Map[addr];
        return true;
    }
    if (namePtrMap[addr]->address == addr) {
        namePtrMap[addr]->toOld(acct);
        names2Map[addr] = acct;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------
bool loadNames2(void) {
    nRecords = (fileSize("./out.bin") / sizeof(NameOnDisc));  // may be one too large, but we'll adjust below
    names = new NameOnDisc[nRecords];
    memset(names, 0, sizeof(NameOnDisc) * nRecords);

    NameOnDisc fake;
    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock("./out.bin", modeReadOnly, LOCK_NOWAIT)) {
        cerr << "Could not lock out.bin" << endl;
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
bool clearNames2(void) {
    if (names) {
        delete[] names;
        names = nullptr;
    }
    return true;
}

//-----------------------------------------------------------------------
bool writeNames2(void) {
    CArchive out(WRITING_ARCHIVE);
    if (out.Lock("./out.bin", modeWriteCreate, LOCK_WAIT)) {
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
}  // namespace qblocks