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

// // extern string_q traceOneBlock(blknum_t num, COptions& opt);
// //---------------------------------------------------------------------------------------------------
// bool traceBlock(uint64_t num, void* data) {
//     COptions* opt = (COptions*)data;  // NOLINT
//     if (opt->isRaw) {
//         string_q results;
//         queryRawBlockTrace(results, num);
//         bool isText = (expContext().exportFmt & (TXT1 | CSV1));
//         if (!opt->firstOut) {
//             if (!isText)
//                 cout << ",";
//             cout << endl;
//         }
//         cout << results;
//         opt->firstOut = false;
//         return !shouldQuit();
//     } else {
//         CBlock block;
//         queryBlock(block, uint_2_Str(num), true);
//         block.forEveryTrace(visitTrace, data);
//         return !shouldQuit();
//     }
// }

// //------------------------------------------------------------
// bool visitTrace(CTrace& trace, void* data) {
//     COptions* opt = reinterpret_cast<COptions*>(data);
//     bool isText = (expContext().exportFmt & (TXT1 | CSV1));
//     if (!opt->firstOut) {
//         if (!isText)
//             cout << ",";
//         cout << endl;
//     }
//     cout << trace.Format(expContext().fmtMap["format"]);
//     opt->firstOut = false;
//     return true;
// }
