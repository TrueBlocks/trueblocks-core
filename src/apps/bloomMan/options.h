#pragma once
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

class COptions : public COptionsBase {
public:
    bool              isStats;
    bool              isReWrite;
    bool              isCheck;
    bool              isCummulative;
    bool              isRaw;
    uint64_t          bucketSize;
    COptionsBlockList blocks;
    string_q          address_list;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//-------------------------------------------------------------------------
#define sep '\t'

//-------------------------------------------------------------------------
class CVisitor {
public:
    COptions options;
    blknum_t lastMarker;
    uint64_t nBlocks, nWithBlooms, nWithoutBlooms;
    uint64_t nBlooms, nBits, nBytes;

    CVisitor(const COptions& opts) : options(opts) { Reset(); }

    void Reset(void) {
        lastMarker = 0;
        nBlocks = nWithBlooms = nWithoutBlooms = 0;
        nBlooms = nBits = nBytes = 0;
    }

    void report(blknum_t bn) const {
        cout << string_q(90, ' ') << "\r";
        static bool here = false;
        if (!here) {
            here = true;
            cout << "blockNum" << sep;
            cout << "nBlocks" << sep << "nBloomFiles" << sep << "noBlooms" << sep;
            cout << "nBlooms" << sep << "nBits" << sep << "nBytes" << "\n";
        }
        cout << bn << sep;
        cout << nBlocks << sep << nWithBlooms << sep << nWithoutBlooms << sep;
        cout << nBlooms << sep << nBits << sep << nBytes << "\n";
        cout.flush();
    }

    void interumReport(blknum_t bn) const {
        cerr << bn << sep;
        cerr << nBlocks << sep << nWithBlooms << sep << nWithoutBlooms << sep;
        cerr << nBlooms << sep << nBits << sep << nBytes << "\r";
        cerr.flush();
    }
};
