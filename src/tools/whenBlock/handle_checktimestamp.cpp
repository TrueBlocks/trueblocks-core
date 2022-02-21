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

//-----------------------------------------------------------------------
bool checkTimestamp(CBlock& block, void* data) {
    COptions* opt = (COptions*)data;
    CTimeStamper* c = &opt->checker;
    LOG_INFO("Checking block (", block.blockNumber, ", ", block.timestamp, ")", (c->verbose ? "\n" : "\r"));

    string_q reason;
    if (block.blockNumber != c->expected) {
        reason = "num != expected";
    }

    if (block.blockNumber != c->prevBn + 1) {
        reason = "num != prev+1";
    }

    if (block.timestamp <= c->prevTs) {
        CBlock fix;
        getBlockHeader(fix, c->prevBn);
        opt->corrections.push_back(fix);
        reason = "ts > c->prevTs";
    }

    if (!reason.empty()) {
        ostringstream os;
        CBlock blk;
        getBlockHeader(blk, c->expected);
        LOG_WARN(reason, " at block ", c->expected, " ==> ", " sb:", block.blockNumber, ".", block.timestamp,
                 " is: ", blk.blockNumber, ".", blk.timestamp, " c: ", c->expected, ".", c->prevBn, ".", c->prevTs);
        opt->corrections.push_back(blk);
    }

    c->expected++;
    c->prevBn = block.blockNumber;
    c->prevTs = block.timestamp;

    return true;
}

//-----------------------------------------------------------------------
bool COptions::applyCorrections(void) {
    for (auto correction : corrections) {
        LOG_INFO("Applying correction for block ", correction.blockNumber);
        if (!correctTimestamp(correction.blockNumber, correction.timestamp))
            return false;
    }

    checker.verbose = true;
    for (auto correction : corrections) {
        checker.expected = correction.blockNumber;
        checker.prevBn = correction.blockNumber == 0 ? 0 : correction.blockNumber - 1;
        checker.prevTs = correction.timestamp - 1;
        checkTimestamp(correction, this);
    }

    return true;
}
