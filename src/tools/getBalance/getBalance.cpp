#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "options.h"

bool visitBlock(uint64_t num, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (verbose)
            cout << "running command: " << command << "\n";

        if (!options.parseArguments(command))
            return 0;

        uint64_t nAccts = countOf('|', options.addrs) + 1;
        bool needsTotal = (nAccts > 1 && options.total);

        // For each address
        while (!options.addrs.empty()) {
            options.state.curAddr = nextTokenClear(options.addrs, '|');
            options.blocks.forEveryBlockNumber(visitBlock, &options);
        }

        if (needsTotal) {
            SFString sBal = to_string(options.state.totalVal).c_str();
            if (expContext().asEther) {
                sBal = wei2Ether(to_string(options.state.totalVal).c_str());
            } else if (expContext().asDollars) {
                CBlock blk;
                getBlock(blk, getLatestBlockFromClient());
                sBal = padLeft("$" + dispDollars(blk.timestamp, options.state.totalVal),14);
            }
            cout << "        Total for " << cGreen << nAccts << cOff;
            cout << " accounts at " << cTeal << "latest" << cOff << " block";
            cout << " is " << cYellow << sBal.Substitute("  "," ") << cOff << "\n";
        }
    }

    if (options.state.needsNewline)
        cerr << "                                                                                                                 \n";
    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void *data) {

    COptions *options = (COptions*)data;
    if (blockNum > options->state.latestBlock) {
        SFString late = (isTestMode() ? "--" : asStringU(options->state.latestBlock));
        return usage("Block " + asStringU(blockNum) + " is later than the last valid block " + late + ". Quitting...");
    }

    SFUintBN bal = getBalance(options->state.curAddr, blockNum, false);
    options->state.totalVal += bal;
    SFString sBal = to_string(bal).c_str();
    if (expContext().asEther) {
        sBal = wei2Ether(to_string(bal).c_str());
    } else if (expContext().asDollars) {
        CBlock blk;
        getBlock(blk, blockNum);
        sBal = padLeft("$" + dispDollars(blk.timestamp, bal), 14);
    }

    options->state.needsNewline = true;
    bool show = true;
    if (options->changes) {
        if (bal == options->state.lastBal)
            show = false;
        options->state.lastBal = bal;
    }

    if (show && (bal > 0 || !options->noZero)) {

        if (options->asData) {
            cout << blockNum << "\t" << options->state.curAddr << "\t" << sBal << "\n";
        } else {
            cout << "    Balance for account " << cGreen << options->state.curAddr << cOff;
            cout << " at block " << cTeal << blockNum << cOff;
            cout << " is " << cYellow << sBal << cOff << "\n";
        }

        options->state.needsNewline = false;

    } else if (!isTestMode()) {

        if (options->asData) {
            cerr << blockNum << "\t" << options->state.curAddr << "         \r";
        } else {
            cerr << "    Balance for account " << cGreen << options->state.curAddr << cOff;
            cerr << " at block " << cTeal << blockNum << cOff;
            cerr << " is " << cYellow << sBal << cOff << "           \r";
        }
    }
    cerr.flush();
    cout.flush();

    return !shouldQuit();
}
