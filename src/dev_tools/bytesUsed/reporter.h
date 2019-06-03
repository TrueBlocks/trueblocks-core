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

//-----------------------------------------------------------------------
class CFileCounter {
public:
    biguint_t cur_nFiles;
    biguint_t cur_nBytes;
    biguint_t tot_nFiles;
    biguint_t tot_Bytes;
    string_q largestFile;
    string_q smallestFile;
    CFileCounter(void) : cur_nFiles(0), cur_nBytes(0), tot_nFiles(0), tot_Bytes(0) {
        // largestFile = "";
        // smallestFile = "";
    }
    void accumulate(void) {
        tot_nFiles += cur_nFiles;
        tot_Bytes += cur_nBytes;
    }
    void reset(void) {
        cur_nFiles = 0;
        cur_nBytes = 0;
    }
    string_q stats(void) const {
        ostringstream os;
        os << tot_Bytes << "\t"
            << tot_nFiles << "\t"
            << (tot_nFiles!=0 ? tot_Bytes / tot_nFiles : 0);
        return os.str().c_str();
    }
};

//-----------------------------------------------------------------------
class CReporter {
public:
    CFileCounter blocks;
//    CFileCounter blooms;
    CReporter(void) { }
};

//-----------------------------------------------------------------------
extern bool countBlockBytes(const string_q& path, void *data);
extern bool countBloomBytes(const string_q& path, void *data);

