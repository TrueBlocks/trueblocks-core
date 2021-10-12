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

//----------------------------------------------------------------
bool COptions::handle_extract() {
#if 0
        static size_t x = 0;
        x++;
        if ((x % 50))
            return true;
        CStringArray colors = {
            bGreen, bBlue, bTeal, bMagenta, bYellow, bWhite, cGreen, cBlue, cTeal, cMagenta, cYellow, cWhite,
        };
#define NN (2048)
        size_t cnt = 0;
        for (auto bloom : blooms) {
            // if (startBlock < 13000000)
            //     continue;
            for (size_t i = 0; i < getBloomWidthInBits(); i++) {
                if (bloom.isBitLit(i))
                    cout << colors[cnt % 12] << '1' << cOff;
                else
                    cout << '.';
                if (!(i % NN))
                    cout << " " << startBlock << endl;
                cout.flush();
                // usleep(10000);
            }
            cout << cRed << string_q(150, '=') << cOff << endl << endl;
            cnt++;
        }
        if (false) {
            for (auto bloom : blooms) {
                cout << delim << cnt++ << delim << bloom.nInserted;
                cout << delim;
                for (size_t i = 0; i < getBloomWidthInBytes(); i += NN) {
                    bloom.showBloom(cout, (i * NN), NN);
                    cout << endl << delim << delim << delim;
                }
                cout << cGreen << string_q(150, '=') << cOff << endl << endl;
            }
        }
#endif
    LOG_INFO("Not yet implemented");
    return true;
}
