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
#include "basetypes.h"
#include "version.h"

namespace qblocks {

#define MAJOR 0
// ALSO CHANGE IN ROOT.GO
#define MINOR 16
// ALSO CHANGE IN ROOT.GO
#define BUILD 0
#define SUBVERS "alpha"
#define PRODUCT_NAME "GHC-TrueBlocks//"
//--------------------------------------------------------------------------------
uint32_t getVersionNum(void) {
    return getVersionNum(MAJOR, MINOR, BUILD);
}

//--------------------------------------------------------------------------------
uint32_t getVersionNum(uint16_t maj, uint16_t min, uint16_t build) {
    return ((maj * 1000000) + (min * 1000) + (build));
}

//--------------------------------------------------------------------------------
string_q getVersionStr(bool incProg, bool incGit) {
    ostringstream os;
    if (incProg)
        os << PRODUCT_NAME;

    if (isTestMode())
        os << "vers"
           << "-" << SUBVERS;
    else
        os << MAJOR << "." << MINOR << "." << BUILD << "-" << SUBVERS;

    if (incGit) {
        if (isTestMode())
            os << "-"
               << "-git-hash-"
               << "-"
               << "-git-ts-";
        else
            os << "-" << GIT_COMMIT_HASH << "-" << ts_2_Date(GIT_COMMIT_TS).Format(FMT_SHORT);
    }
    return os.str();
}

}  // namespace qblocks
