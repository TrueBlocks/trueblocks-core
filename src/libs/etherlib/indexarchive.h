#pragma once
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
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "appearance.h"

namespace qblocks {
//----------------------------------------------------------------
struct CHeaderRecord_base {
    uint32_t magic;
    uint8_t hash[32];
    uint32_t nAddrs;
    uint32_t nRows;
};

//---------------------------------------------------------------------------
class CIndexArchive : public CArchive {
  public:
    CHeaderRecord_base* header;
    uint64_t nAddrs;
    CAddressRecord_base* addresses;
    uint64_t nApps;
    CAppearance_base* appearances;

    explicit CIndexArchive(bool mode);
    ~CIndexArchive(void);
    bool ReadIndexFromBinary(const string_q& fn);

  private:
    char* rawData;
    CIndexArchive(void) : CArchive(READING_ARCHIVE) {
    }
};
}  // namespace qblocks
