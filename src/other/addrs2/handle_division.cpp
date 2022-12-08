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

// `handle_division` separates addresses in the given file into `nOutputs`
// files based on how many appearances the address has. It is fine-grained
// up until 20 appearances, putting each number of appearances below 20 in
// a separate file, then switches and addresses with between 21-100 (inclusive)
// in a separate file followed by 101-200, etc. up to 1000. It then puts 1001-10000,
// 10001-20000, etc. in separate files up to 100,000 then every past that in a single
// file.

bool visitLine(const char* line, void* data);
class FileDescriptor {
  public:
    string_q fileName;
    ofstream* out;
    FileDescriptor(void) {
        out = nullptr;
    };
    FileDescriptor(const FileDescriptor& fd) {
        fileName = fd.fileName;
        out = fd.out;  // they share a file pointer, be careful
    }
    ~FileDescriptor(void) {
        // Clean();
    }
    bool Clean(void) {
        if (!out)
            return true;
        if (!out->is_open()) {
            delete out;
            out = nullptr;
            return true;
        }
        LOG_INFO("Closing file: ", fileName, " at ", out);
        out->flush();
        out->close();
        delete out;
        out = nullptr;
        return true;
    }
    bool Open(void) {
        if (out) {
            return true;
        }
        out = new ofstream;
        if (!out) {
            LOG_ERR("Could not open file: ", fileName);
            Clean();
            return false;
        }

        out->open(fileName, ios::out | ios::trunc);
        if (!out->is_open()) {
            LOG_ERR("Could not open file: ", fileName);
            Clean();
            return false;
        }

        LOG_INFO("Opened file: ", fileName, " at ", out);
        return true;
    }
};
map<uint32_t, FileDescriptor> files;

//----------------------------------------------------------------
string_q path = "/Volumes/Tokenomics Data/addresses/combined/0000-to-0010.txt";
bool handle_division(void) {
    for (uint32_t i = 0; i <= 20; i++) {
        FileDescriptor fd = FileDescriptor();
        fd.fileName = "store/results/" + padNum9(i) + ".txt";
        fd.Open();
        files[i] = fd;
    }

    cout << path << ": " << fileExists(path) << endl;
    forEveryLineInAsciiFile(path, visitLine, nullptr);

    for (auto& file : files) {
        file.second.Clean();
    }

    return true;
}

CStringArray colors = {bYellow, bGreen, bTeal, bWhite, bBlue, bRed};
uint32_t lineNum = 0;
//----------------------------------------------------------------
bool visitLine(const char* line, void* data) {
    const char* tab = strstr(line, "\t");
    if (!tab)
        return true;

    uint32_t cnt = str_2_Uint(tab);
    if (cnt > 20)
        cnt = 0;
    *files[cnt].out << line;
    if (!(lineNum % 23)) {
        cerr << colors[cnt % cnt] << padNum9(lineNum) << "\t" << padNum9(cnt) << "\t" << substitute(line, "\n", "")
             << cOff << "       \r";
        cerr.flush();
    }
    lineNum++;

    return true;
}
