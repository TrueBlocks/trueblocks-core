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
#include "utillib.h"

namespace qblocks {

// known timestamps
#define blockZeroTs ((timestamp_t)1438269960)
#define istanbulTs ((timestamp_t)1575764709)

// known blocks
#define firstTransactionBlock 46147
#define firstTraceBlock 50871
#define byzantiumBlock 4370000
#define constantinopleBlock 7280000
#define instanbulBlock 9069000
#define londonBlock 12965000

//-----------------------------------------------------------------------
inline bool ddosRange(blknum_t bn) {
    return (bn >= 2283440 && bn <= 2718436);
}

}  // namespace qblocks
