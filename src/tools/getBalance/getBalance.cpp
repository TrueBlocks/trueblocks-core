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
#include "options.h"

bool visitBlock(uint64_t num, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    CStringArray commands;
    explode(commands, options.commandList, '\n');
    for (auto command : commands) {
        if (verbose)
            cout << "running command: " << command << "\n";

        if (!options.parseArguments(command))
            return 0;

        // For each address
        for (auto addr : options.addrs) {
            options.state.curAddr = addr;
            options.blocks.forEveryBlockNumber(visitBlock, &options);
        }

        if (options.total && options.addrs.size() > 1) {
            string_q dispBal = options.getDispBal(options.newestBlock, options.state.totalVal, options.asData);
            cout << "        Total for " << cGreen << options.addrs.size() << cOff;
            cout << " accounts at " << cTeal << "latest" << cOff << " block";
            cout << " is " << cYellow << substitute(dispBal, "  ", " ") << cOff << "\n";
        }
    }

    if (options.state.totalVal == 0 && options.noZero) {
        cerr << "    No values were reported\n";
        options.state.needsNewline = false;
    }

    if (options.state.needsNewline)
        cerr << string_q(103, ' ') << "\n";

    if (!options.hasHistory()) {
        cerr << cRed << "    Warning: " << cOff;
        cerr << "Your node does not have historical balances. Historical information is incorrect.\n";
    }

    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void *data) {

    COptions *options = reinterpret_cast<COptions *>(data);
    if (blockNum < options->oldestBlock)
        options->oldestBlock = blockNum;

    if (blockNum > options->newestBlock) {
        string_q late = (isTestMode() ? "--" : uint_2_Str(options->newestBlock));
        return usage("Block " + uint_2_Str(blockNum) + " is later than the last valid block " + late + ". Quitting...");
    }

    biguint_t bal = getBalance(options->state.curAddr, blockNum, false);
    options->state.totalVal += bal;

    string_q dispBal = options->getDispBal(blockNum, bal, options->asData);
    options->state.needsNewline = true;
    bool show = true;
    if (options->changes) {
        if (bal == options->state.lastBal)
            show = false;
        options->state.lastBal = bal;
    }

    if (show && (bal > 0 || !options->noZero)) {

        if (options->asData) {
            cout << blockNum << "\t" << options->state.curAddr << "\t" << dispBal << "\n";
        } else {
            cout << "    Balance for account " << cGreen << options->state.curAddr << cOff;
            cout << " at block " << cTeal << blockNum << cOff;
            cout << " is " << cYellow << dispBal << cOff << "\n";
        }

        options->state.needsNewline = false;

    } else if (!isTestMode()) {

        if (options->asData) {
            cerr << blockNum << "\t" << options->state.curAddr << "         \r";
        } else {
            cerr << "    Balance for account " << cGreen << options->state.curAddr << cOff;
            cerr << " at block " << cTeal << blockNum << cOff;
            cerr << " is " << cYellow << dispBal << cOff << "           \r";
        }
    }
    cerr.flush();
    cout.flush();

    return !shouldQuit();
}
