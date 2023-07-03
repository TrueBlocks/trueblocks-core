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

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    cerr << "TB_CONFIG_ENV: " << substitute(relativize(getEnvStr("TB_CONFIG_ENV")), getRpcProvider(), "--rpc--")
         << endl;

    ostringstream os;
    os << getBinaryCacheFilename(CT_BLOCKS, 1001001) << endl;
    os << getBinaryCacheFilename(CT_TXS, 1001001, 20) << endl;
    os << getBinaryCacheFilename(CT_TRACES, 1001001, 20, "10") << endl;

    os << getPathToBinaryCache(CT_BLOCKS, 1001001) << endl;
    os << getPathToBinaryCache(CT_TXS, 1001001, 20) << endl;
    os << getPathToBinaryCache(CT_TRACES, 1001001, 20, "1,1") << endl;

    cout << substitute(os.str(), cacheFolder, "./") << endl;

    return 0;
}
