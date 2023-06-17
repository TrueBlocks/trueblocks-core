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
int main(int argc, const char* argv[]) {
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
                cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CEthState));
            options.current = addr;
            options.blocks.forEveryBlockNumber(visitForState, &options);
            once = false;
        }

        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    }

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitForState(uint64_t blockNum, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = expContext().exportFmt & (TXT1 | CSV1);

    if (!isTestMode() && opt->blocks.nItems() > 10) {
        cerr << blockNum << "\r";
        cerr.flush();
    }

    if (blockNum < opt->oldestBlock)
        opt->oldestBlock = blockNum;

    CEthState state;
    state.address = opt->current;
    wei_t balance = getBalanceAt(state.address, blockNum);
    if (opt->changes) {
        if (balance == opt->prevBal)
            return !shouldQuit();
        opt->prevBal = balance;
    }

    if (opt->no_zero && balance == 0)
        return !shouldQuit();

    state.blockNumber = blockNum;
    if (opt->modeBits & ST_BALANCE)
        state.balance = balance;
    if (opt->modeBits & ST_NONCE)
        state.nonce = getNonceAt(state.address, blockNum);
    if (opt->modeBits & ST_CODE) {
        string_q code = getCodeAt(state.address, opt->latestBlock);
        state.code = code;
        if (code.length() > 250 && !verbose)
            state.code = code.substr(0, 20) + "..." + code.substr(code.length() - 20, 100);
    }

    if (opt->modeBits & ST_DEPLOYED || opt->modeBits & ST_PROXY) {
        blknum_t dep = getDeployBlock(state.address);
        state.deployed = dep == NOPOS ? 0 : dep;
    }

    if (opt->modeBits & ST_PROXY) {
        getProxyContract(state.address, blockNum, state.proxy);
    }

    if (opt->modeBits & ST_ACCTTYPE) {
        if (!state.proxy.empty()) {
            state.accttype = "Proxy";
        } else {
            state.accttype = (isContractAt(state.address, opt->latestBlock) ? "Contract" : "EOA");
        }
    }

    if (true) {
        if (isText) {
            cout << state.Format(expContext().fmtMap["format"]) << endl;

        } else {
            if (!opt->firstOut)
                cout << "," << endl;
            cout << "  ";
            indent();
            state.toJson(cout);
            unindent();
            opt->firstOut = false;
        }
    }

    return !shouldQuit();
}
