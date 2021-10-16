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
#include "options.h"

// static char delim = ',';
//----------------------------------------------------------------
static bool bloomVisitFunc(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", bloomVisitFunc, data);

    } else {
        blknum_t endBlock = NOPOS;
        blknum_t startBlock = bnFromPath(path, endBlock);
        blknum_t last = *(blknum_t*)data;
        if (last >= startBlock)
            return true;

        CBloomArray blooms;
        readBloomFromBinary(path, blooms);

        const CStringArray colors = {
            bGreen, bBlue, bTeal, bMagenta, bYellow, bWhite, cGreen, cBlue, cTeal, cMagenta, cYellow, cWhite,
        };
        const size_t nColors = 12;  // sizeof(colors) / sizeof(string_q);

#define NN (2048)
        size_t cnt = 0;
        for (auto bloom : blooms) {
            for (size_t i = 0; i < getBloomWidthInBits(); i++) {
                if (bloom.isBitLit(i))
                    cout << colors[i % nColors] << '1' << cOff;
                else
                    cout << '.';
                if (!(i % NN))
                    cout << " " << startBlock << endl;
                cout.flush();
            }
            cout << cRed << string_q(150, '=') << cOff << endl << endl;
            cnt++;
        }
    }
    return true;
}

//----------------------------------------------------------------
bool COptions::handle_extract_blooms(void) {
    blknum_t last = 0;
    return forEveryFileInFolder(indexFolder_blooms, bloomVisitFunc, &last);
}
