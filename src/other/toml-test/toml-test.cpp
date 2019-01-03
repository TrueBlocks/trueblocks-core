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
#include "toml.h"

bool old=true;
//----------------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc == 2)
        old = false;
    etherlib_init("binary", quickQuitHandler);
    forEveryFileInFolder("./tests/", visitFile, NULL);
    return 1;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {
        if (contains(path, "array-empty"))
            return true;
        if (endsWith(path, ".toml")) {
            cout << "path: " << path << endl;
            if (old) {
                CToml toml(path);
                cout << toml << endl;
            } else {
                try {
                    CNewToml g(path);
                    cout << g << endl;
                } catch (const parse_exception& e) {
                    cerr << "Failed to parse " << path << ": " << e.what() << endl;
                    return false;
                }
            }
        }
    }
    return true;
}
