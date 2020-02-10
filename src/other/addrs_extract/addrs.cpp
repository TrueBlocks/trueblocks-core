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

//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);
    forEveryFileInFolder(getCachePath("addr_output/sorted/"), visitFile, NULL);
    etherlib_cleanup();
    return 1;
}

typedef struct {
public:
    char addr[42];
    char t1;
    char bn[9];
    char t2;
    char tx[5];
    char nl;
} Thing;

//----------------------------------------------------------------
int sortByBlock(const void *v1, const void *v2) {
    Thing *t1 = (Thing*)v1;
    Thing *t2 = (Thing*)v2;

    int ret = strncmp(t1->bn, t2->bn, 9);
    if (ret)
        return ret;

    ret = strncmp(t1->addr, t2->addr, 42);
    if (ret)
        return ret;

    ret = strncmp(t1->tx, t2->tx, 5);
    return ret;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {

        blknum_t end;
        blknum_t start =
            bnFromPath(path, end);
        if (start < 6000000 || start > 7000000) {
            cerr << "skipping " << start << " " << end << "\n";
            return true;
        }

        CArchive file(READING_ARCHIVE);
        if (file.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            uint64_t size = fileSize(path);
            uint64_t nRecords = size / sizeof(Thing);
            Thing *t = new Thing[nRecords + 1];
            bzero(t, sizeof(Thing) * (nRecords+1));
            if (t) {
                file.Read(t, sizeof(Thing), nRecords);
                for (size_t i = 0 ; i < nRecords ; i++) {
                    t[i].t1 = t[i].t2 = t[i].nl = '\0';
                }
                qsort(t, nRecords, sizeof(Thing), sortByBlock);
                blknum_t last = str_2_Uint(t->bn);
                ostringstream os;
                for (size_t i = 0 ; i < nRecords ; i++) {
                    blknum_t bn = str_2_Uint(t[i].bn);
                    if (bn != last) {
                        string_q f = padNum9(last);
                        f = "./" + f.substr(0,3) + "/" + f.substr(3,3) + "/" + f + ".txt";
                        establishFolder(f);
                        cerr << f << endl;
                        stringToAsciiFile(f, os.str());
                        os.str("");
                        os.clear();
                        last = bn;
                    }
                    os << t[i].addr << "\t" << t[i].bn << "\t" << t[i].tx << endl;
                }
                string_q f = t[nRecords-1].bn;
                f = "./" + f.substr(0,3) + "/" + f.substr(3,3) + "/" + f + ".txt";
                establishFolder(f);
                stringToAsciiFile(f, os.str());
                delete [] t;
            }
        }
    }
    return true;
}
