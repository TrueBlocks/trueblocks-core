#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <cstdint>
#include "basetypes.h"

namespace qblocks {

    double qbNow(void);

    typedef std::chrono::steady_clock::time_point timepoint_t;
    typedef std::chrono::duration<double> duration_t;
#define qbNow2() std::chrono::steady_clock::now()
    extern void resetClock(void);
    extern string_q TIC(double div = 1.);

}  // namespace qblocks
