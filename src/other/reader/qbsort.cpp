/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "options.h"

//--------------------------------------------------------------
class CItem {
    CItem(void) { }
public:
    blknum_t bn;
    blknum_t tx;
    hash_t   hash;
    bool     isnull;
    CItem(const string_q& line) {
        isnull = contains(line, "null");
        hash = line;
        bn = str_2_Uint(nextTokenClear(hash, '\t'));
        tx = str_2_Uint(nextTokenClear(hash, '\t'));
    }
    friend bool operator<(const CItem& v1, const CItem& v2) {
        if (v1.bn < v2.bn)
            return true;
        if (v1.tx < v2.tx)
            return true;
        return v1.hash < v2.hash;
    }
    bool operator==(const CItem& i) const {
        if (i.bn != bn) return false;
        if (i.tx != tx) return false;
        if (i.hash != hash) return false;
        return true;
    }
    bool operator!=(const CItem& i) const { return !operator==(i); }
};
typedef vector<CItem> CItemArray;

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CItem& item) {
    os << item.bn << "\t" << (item.isnull ? "null" : uint_2_Str(item.tx)) << "\t" << item.hash;
    return os;
}

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CItemArray items;
        string_q contents;
        asciiFileToString(options.fileName, contents);
        if (startsWith(contents, "blockNumber"))
            nextTokenClear(contents, '\n'); // skip header line
        CStringArray lines;
        explode(lines, contents, '\n');
        for (auto line : lines) {
            if (!countOf(line, '\t'))
                return usage("Invalid line: " + line + "\n");
            items.push_back(CItem(line));
        }

        if (options.sort) {
            std::sort(items.begin(), items.end(), [](const CItem& lhs, const CItem& rhs) {
                uint64_t n1 = lhs.bn * 10000 + lhs.tx;
                uint64_t n2 = rhs.bn * 10000 + rhs.tx;
                return n1 < n2;
            });
        }

        CItem lastItem("0\t0\t0");
        cout << "blocknumber\ttransactionindex\thash\n";
        for (auto item : items) {
            if (item != lastItem) {
                cout << item << "\n";
                lastItem = item;
            }
        }
    }
    return 0;
}
