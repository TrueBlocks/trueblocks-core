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

bool visitFile(const string_q& path, void *data);
//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);
    string_q path = "/Volumes/Samsung_T7/scraper/monitors/exports/*";
    forEveryFileInFolder(path, visitFile, NULL);
    return 1;
}

bool visitFile(const string_q& path, void *data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        if (contains(path, "0x")) {
            cout << "processing path: " << path << "...";
            string_q contents;
            asciiFileToString(path, contents);
            CStringArray lines;
            lines.reserve(800000);
            explode(lines, contents, '\n');
            ostringstream os;
            for (auto line : lines) {
                string_q block = nextTokenClear(line, '\t');
                os << padLeft(block, 9, '0') << "\t" << padLeft(line, 5, '0') << endl;
            }
            stringToAsciiFile(path, os.str());
            cout << "done." << endl;
        }
    }
    return true;
}
