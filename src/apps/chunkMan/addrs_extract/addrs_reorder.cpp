#if 0
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

//-----------------------------------------------------------------------
#define indexFolder_sorted (indexFolder + "zips/")

//-----------------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        if (endsWith(path, ".txt.sorted")) {
            blknum_t end;
            blknum_t start = path_2_Bn(path, end);
            if (end >= 1000000 && start < 6500000) {
                cerr << path << "\tskip\r";
                cerr.flush();
                return true;
            }

            CStringArray lines;
            asciiFileToLines(path, lines);

            blknum_t prev = path_2_Bn(path) - 1;
            for (auto line : lines) {
                address_t addr = nextTokenClear(line, '\t');
                blknum_t cur = str_2_Uint(line);
                if (cur != prev && cur != (prev + 1))
                    cout << path << "\t" << padNum9(prev) << "\t" << padNum9(cur) << endl;
                else {
                    static int i = 0;
                    if (!(++i % 23)) {
                        cerr << addr << "\t" << line << "\r";
                        cerr.flush();
                    }
                }
                prev = cur;
            }
        }
    }
    return true;
}

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    forEveryFileInFolder(indexFolder_sorted, visitFile, NULL);
}

#if 0
//-----------------------------------------------------------------------
#define indexFolder_sorted (indexFolder + "sorted/")

//-----------------------------------------------------------------------
bool sortByBn(const string_q &v1, const string_q &v2) {
    return v1.substr(43, 9) < v2.substr(43, 9);
}

//-----------------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {

        if (endsWith(path, ".txt")) {

            string_q sPath = substitute(path, "/sorted/", "/zips/") + ".sorted";
            if (fileExists(sPath)) {
                cerr << "Skipping " << path << endl;
                return true;
            }

            CStringArray lines;
            asciiFileToLines(path, lines);
            sort(lines.begin(), lines.end(), sortByBn);
            linesToAsciiFile(sPath, lines);

            string_q start = lines[0];
            nextTokenClear(start, '\t');
            start = nextTokenClear(start, '\t');
            string_q end = lines[lines.size()-1];
            nextTokenClear(end, '\t');
            end = nextTokenClear(end, '\t');
            cout << sPath << "\t" << start << "\t" << end << endl;
        }
    }
    return true;
}

//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    forEveryFileInFolder(indexFolder_sorted, visitFile, NULL);
}
#endif

#if 0
#define indexFolder_fromwild substitute(indexFolder_sorted, "/sorted/", "/validated/")
#define indexFolder_fixed substitute(indexFolder_sorted, "/sorted/", "/u-validated/")

void exportLines2(CStringArray& lines);
void exportLines(CStringArray& lines, const string_q& filename);
bool visitLine(const char *line, void *data);
int main(int argc, const char *argv[]) {
    extern string_q fileStr;

    CStringArray lines;
    asciiFileToLines(indexFolder_fromwild + "000000001-000286900.txt", lines);
    cout << "importing 000000001-000286900.txt" << endl;
    asciiFileToLines(indexFolder_fromwild + "000286901-000492100.txt", lines);
    cout << "importing 000286901-000492100.txt" << endl;

    CStringArray files;
    explode(files, fileStr, '|');
    for (auto file : files) {
        string_q filename = indexFolder_fromwild + file;
        if (!fileExists(filename)) {
            cout << "No file " << filename << endl;
            return 0;
        }
        cout << "importing " << filename << endl;
        asciiFileToLines(filename, lines);
        cout << "\tsorting " << lines.size() << " records...";
        sort(lines.begin(), lines.end(), sortByBn);
        exportLines2(lines);
    }
    return 0;
}

bool isMarkerBlock(size_t size, blknum_t bn) {
    if (size < 750000)
        return false;
    static blknum_t prev = NOPOS;
    bool was50  = !(prev % 50);
    bool isDiff = (prev != bn);
    prev = bn;
    if (!was50)
        return false;
    cout << "b: " << bn << " w: " << was50 << " i: " << isDiff << endl;
    return was50 && isDiff;
}

void exportLines2(CStringArray& lines) {
    blknum_t first = NOPOS, last = 0;bool full=false;
    CStringArray output, remains;
    cout << " scanning...";
    for (auto line : lines) {
        CStringArray parts;
        explode(parts, line, '\t');
        blknum_t bn = str_2_Uint(parts[1]);
        if (full || isMarkerBlock(output.size(), bn)) {
            remains.push_back(line);
            if (!full) {
                cout << "done...accummulating";
                cout.flush();
                full = true;
            }
        } else {
            output.push_back(line);
            first = min(first, bn);
            last = max(last, bn);
        }
        if (!((output.size()+remains.size())%50000)) { cout << "."; cout.flush(); }
    }

    string_q outFile = indexFolder_fixed + padNum9(first) + "-" + padNum9(last) + ".txt";
    cout << "\n\texporting " << outFile << endl;
    sort(output.begin(), output.end());
    exportLines(output, outFile);

    lines = CStringArray();
    for (auto remain : remains)
        lines.push_back(remain);
}

bool visitLine(const char *line, void *data) {
    cout << line;
    return true;
}

string_q fileStr =
"000492101-000666200.txt|"
"000666201-000756800.txt|"
"000756801-000840700.txt|"
"000840701-000929200.txt|"
"000929201-001006650.txt|"
"001007101-001085700.txt|";

void exportLines(CStringArray& lines, const string_q& filename) {
    if (lines.size() == 0)
        return;

    sort(lines.begin(), lines.end());

    ostringstream os;
    for (auto line : lines)
        os << line << endl;
    stringToAsciiFile(filename, os.str());
}

#if 0
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    string_q changeStr =
    "000840701-000929200|000840701-000929200|000931551-001009200+";

    size_t x = 0;
    CStringArray changes;
    explode(changes, changeStr, '+');
    for (auto change : changes) {
        CStringArray files;
        explode(files, change, '|');
        string_q nFn  = indexFolder_fromwild + files[0] + ".txt";
        string_q eFn  = indexFolder_fromwild + files[1] + ".txt";
        string_q next = indexFolder_fromwild + files[2] + ".txt";

        if (!fileExists(eFn)) { cerr << "bad"; return 1; }
        //if (!fileExists(nFn)) { cerr << "bad"; return 1; }
        //if (!fileExists(next)) { cerr << "bad"; return 1; }

        moveFile(eFn, nFn);

        blknum_t last;
        blknum_t first = path_2_Bn(nFn, last);

        cout << "Reading records from " << nFn << endl;
        CStringArray lines, output, before, after;
        asciiFileToLines(nFn, lines);

        cout << "Searching for records between " << first << " and " << last << endl;
        for (auto line : lines) {
            CStringArray parts;
            explode(parts, line, '\t');
            blknum_t bn = str_2_Uint(parts[1]);
            if (bn < first)
                before.push_back(line);
            else if (bn > last)
                after.push_back(line);
            else
                output.push_back(line);
        }

        cout << "Exporting..." << endl;
        exportLines(output, nFn);
        exportLines(before, substitute(nFn, ".txt", "-before.txt"));
        exportLines(after, substitute(nFn, ".txt", "-after.txt"));

        cout << "Cleaning up..." << endl;
        if (after.size() > 0) {
            asciiFileToLines(next, after);
            exportLines(after, next);
            ::remove(substitute(nFn, ".txt", "-after.txt").c_str());
        }
    }

    etherlib_cleanup();
}

#if 0
static CStringArray flip, flop;

string_q files =
"002705110-002706116.txt|"
"002706117-002707262.txt|"
"002707263-002708240.txt|"
"002708241-002709259.txt|"
"002709260-002710280.txt|"
"002710281-002711245.txt|"
"002711246-002712189.txt|"
"002712190-002713190.txt|"
"002713191-002714110.txt|"
"002714111-002715037.txt|"
"002715038-002716870.txt|"
"002716871-002719208.txt|"
"002719209-002757590.txt|"
"002757591-002801860.txt|"
"002801861-002843868.txt|"
"002843869-002884930.txt|"
"002884931-002927950.txt|"
"002927951-002967010.txt|"
"002967011-003006120.txt|"
"003006121-003045650.txt|"
"003045651-003083500.txt|"
"003083501-003124170.txt|"
"003124171-003160730.txt|"
"003160731-003195930.txt|"
"003195931-003231350.txt|"
"003231351-003265990.txt|"
"junk|";

void loadAFile(CStringArray& lines, const string_q& fn) {
    string_q file = i ndexFolder + "sorted_by_block/" + fn;
    string_q backup = substitute(file, "/sorted_by_block/", "/backups/");
//    cout << fileExists(file) << " " << backup << endl;
    asciiFileToLines(file, lines);
    cout << lines.size() << endl;
    moveFile(file, backup);
}
#include <array>
//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    CStringArray linesIn;
    loadAFile(linesIn, nextTokenClear(files, '|'));
    loadAFile(linesIn, nextTokenClear(files, '|'));
    loadAFile(linesIn, nextTokenClear(files, '|'));
    loadAFile(linesIn, nextTokenClear(files, '|'));

    while (!files.empty()) {

        CStringArray linesOut;
        for (auto line : linesIn) {
            CStringArray parts;
            explode(parts, line, '\t');
            blknum_t bn = str_2_Uint(parts[1]);
            parts.clear();
            if (linesOut.size() >= 750000 && ((bn % 50) == 1)) {
                // we're done
                string_q f = linesOut[0];
                explode(parts, f, '\t');
                blknum_t first = str_2_Uint(parts[1]);
                parts.clear();
                string_q l = linesOut[linesOut.size()-1];
                explode(parts, l, '\t');
                blknum_t last = str_2_Uint(parts[1]);
                parts.clear();

                string_q path = i ndexFolder + "fixed/" + padNum9(first) + "-" + padNum9(last) + ".txt";
                ostringstream os;
                for (auto out : linesOut)
                    os << out << endl;
                stringToAsciiFile(path, os.str());

                CStringArray tmp;
                for (size_t i = linesOut.size() ; i < linesIn.size() ; i++)
                    tmp.push_back(linesIn[i]);
                linesIn.clear();
                for (auto ln : tmp)
                    linesIn.push_back(ln);
                loadAFile(linesIn, nextTokenClear(files, '|'));
                linesOut.clear();
                goto jim;

            } else {
                // we're not done
                linesOut.push_back(line);
            }
        }
    }

    etherlib_cleanup();
}
#endif
#endif
#endif
#endif