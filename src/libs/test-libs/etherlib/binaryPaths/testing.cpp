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

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    cout << getBinaryCacheFilename(CT_BLOCKS, 1001001) << endl;
    cout << getBinaryCacheFilename(CT_BLOOMS, 1001001) << endl;
    cout << getBinaryCacheFilename(CT_TXS,    1001001, 20) << endl;
    cout << getBinaryCacheFilename(CT_TRACES, 1001001) << endl;
    cout << getBinaryCacheFilename(CT_ACCTS,  1001001) << endl;

    cout << getBinaryCachePath(CT_BLOCKS, 1001001) << endl;
    cout << getBinaryCachePath(CT_BLOOMS, 1001001) << endl;
    cout << getBinaryCachePath(CT_TXS,    1001001, 20) << endl;
    cout << getBinaryCachePath(CT_TRACES, 1001001) << endl;
    cout << getBinaryCachePath(CT_ACCTS,  1001001) << endl;

    return 0;
}
