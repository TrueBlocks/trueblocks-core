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
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto addr : options.addrs) {
            if (once)
                cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CEthState));
            options.current = addr;
            options.blocks.forEveryBlockNumber(visitBlock, &options);
            once = false;
        }
        cout << exportPostamble(options.exportFmt, options.errors, expContext().fmtMap["meta"]);
    }

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void *data) {

    COptions *opt = reinterpret_cast<COptions *>(data);
    bool isText = opt->exportFmt & (TXT1|CSV1);

    if (!isTestMode()) {
        cerr << blockNum << "\r";
        cerr.flush();
    }

    if (blockNum < opt->oldestBlock)
        opt->oldestBlock = blockNum;

    if (opt->requestsHistory() && !nodeHasBalances(true))
        opt->errors.push_back("Request for historical state at block " + uint_2_Str(blockNum) + " not available.");

    CEthState state;
    state.address = opt->current;
    wei_t balance = getBalanceAt(state.address, blockNum);
    if (opt->changes) {
        if (balance == opt->prevBal)
            return !shouldQuit();
        opt->prevBal = balance;
    }

    if (opt->exclude_zero && balance <= opt->deminimus)
        return !shouldQuit();

    state.blockNumber = blockNum;
    if (opt->mode & ST_BALANCE)
        state.balance = balance;
    if (opt->mode & ST_NONCE)
        state.nonce = getNonceAt(state.address, blockNum);
    if (opt->mode & ST_CODE) {
        string_q code = getCodeAt(state.address);
        state.code = code;
        if (code.length() > 250 && !verbose)
            state.code = code.substr(0,20) + "..." + code.substr(code.length()-20, 100);
    }
    if (opt->mode & ST_STORAGE)
        state.storage = getStorageAt(state.address, 0);
    if (opt->mode & ST_DEPLOYED)
        state.deployed = getDeployBlock(state.address);
    if (opt->mode & ST_ACCTTYPE)
        state.accttype = (isContractAt(state.address) ? "Contract" : "EOA");

    if (true) {
        if (isText) {
            cout << state.Format(expContext().fmtMap["format"]) << endl;

        } else {
            if (!opt->first)
                cout << "," << endl;
            cout << "  ";
            incIndent();
            state.doExport(cout);
            decIndent();
            opt->first = false;
        }
    }

    return !shouldQuit();
}
