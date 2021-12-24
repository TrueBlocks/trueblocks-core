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
    char tags[30+1];
    char address[42+1];
    char name[120+1];
    char symbol[30+1];
    char source[180+1];
    char description[255+1];
    uint16_t decimals;
    uint16_t flags;
    NameOnDisc(void) { decimals = flags = 0; }
    void fromOld(const CAccountName& nm) {
        strncpy(tags, nm.tags.c_str(), nm.tags.length());
        strncpy(address, nm.address.c_str(), nm.address.length());
        strncpy(name, nm.name.c_str(), nm.name.length());
        strncpy(symbol, nm.symbol.c_str(), nm.symbol.length());
        strncpy(description, nm.description.c_str(), nm.description.length());
        decimals = uint16_t(nm.decimals);
        flags |= (nm.isCustom ? IS_CUSTOM : IS_NONE);
        flags |= (nm.isPrefund ? IS_PREFUND : IS_NONE);
        flags |= (nm.isContract ? IS_CONTRACT : IS_NONE);
        flags |= (nm.isErc20 ? IS_ERC20 : IS_NONE);
        flags |= (nm.isErc721 ? IS_ERC721 : IS_NONE);
        flags |= (nm.isDeleted() ? IS_DELETED : IS_NONE);
    }
    string_q Format(void) const {
        ostringstream os;
        os << tags << "\t" << address << "\t" << name << "\t" << symbol << "\t" << description << "\t" << decimals << "\t" << flags;
        return os.str();
    }
};
struct NameOnDiscHeader {
public:
    uint64_t magic;
    uint64_t version;
    uint64_t count;
};

map<address_t, NameOnDisc *> namePtrMap;

//--------------------------------------------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    
#define NTESTS 30
    if (argc < 2) {
        for (size_t x = 0 ; x < NTESTS ; x++) {
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock(getCachePath("names/names.bin"), modeReadOnly, LOCK_NOWAIT)) {
                archive >> expContext().namesMap;
                archive.Release();
            }
            for (auto item : expContext().namesMap)
                cout << item.second.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
            cerr << "A-" << x << endl;
        }
    } else if (argc < 3) {
        uint64_t nRecords = (fileSize("./out.bin") / sizeof(NameOnDisc)); // may be one too large, but we'll adjust below
        NameOnDisc *names = new NameOnDisc[nRecords];
        memset(names, 0, sizeof(NameOnDisc) * nRecords);

        for (size_t x = 0 ; x < NTESTS ; x++) {
            NameOnDisc fake;
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock("./out.bin", modeReadOnly, LOCK_NOWAIT)) {
                NameOnDiscHeader *header = (NameOnDiscHeader*)&fake;
                archive.Read(&header->magic, sizeof(uint64_t), 1);
                archive.Seek(SEEK_SET, 0);
                if (header->magic == 0xdeadbeef) {
                    archive.Read(&fake, sizeof(NameOnDisc), 1);
                    ASSERT(header.version >= getVersionNum(0,18,0));
                    nRecords = header->count;
                    archive.Read(names, sizeof(NameOnDisc), nRecords);
                    archive.Release();
                    for (size_t i = 0 ; i < nRecords ; i++)
                        namePtrMap[names[i].address] = &names[i];
                    
                    for (auto item : namePtrMap)
                        cout << item.second->Format() << endl;
                    
                    cerr << "B-" << x << endl;
                } else {
                    cerr << "Got an invalid file header in names file: " << header->magic << "." << header->version << "." << header->count << endl;
                    archive.Release();
                }
            }
        }

    } else {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(getCachePath("names/names.bin"), modeReadOnly, LOCK_NOWAIT)) {
            archive >> expContext().namesMap;
            archive.Release();

            NameOnDisc *names = new NameOnDisc[expContext().namesMap.size()];
            memset(names, 0, sizeof(NameOnDisc) * expContext().namesMap.size());
            uint64_t nRecords = 0;
            
            for (auto item : expContext().namesMap)
                names[nRecords++].fromOld(item.second);
            
            for (size_t i = 0 ; i < nRecords ; i++)
                cout << names[i].Format() << endl;
            
            CArchive out(WRITING_ARCHIVE);
            if (out.Lock("./out.bin", modeWriteCreate, LOCK_WAIT)) {
                NameOnDisc fake;
                memset(&fake, 0, sizeof(NameOnDisc) * 1);
                NameOnDiscHeader *header = (NameOnDiscHeader*)&fake;
                header->magic = 0xdeadbeef;
                header->version = getVersionNum();
                header->count = nRecords;
                out.Write(&fake, sizeof(NameOnDisc), 1);
                out.Write(names, sizeof(NameOnDisc), nRecords);
                out.Release();
            }

            cerr << fileExists("./out.bin") << "\t" << fileSize("./out.bin") << "\t" << fileSize(getCachePath("names/names.bin")) << endl;

        } else {
            cerr << "Could not open names.bin" << endl;
        }
    }
    
    etherlib_cleanup();
    return 1;
}
