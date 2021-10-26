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

        if (!options.call.empty()) {
            if (once)
                cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CEthCall));
            options.blocks.forEveryBlockNumber(visitBlock, &options);
            once = false;

        } else {
            for (auto addr : options.addrs) {
                if (once)
                    cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CEthState));
                options.current = addr;
                options.blocks.forEveryBlockNumber(visitBlock, &options);
                once = false;
            }
        }
        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    }

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = expContext().exportFmt & (TXT1 | CSV1);

    if (!isTestMode() && opt->blocks.nItems() > 10) {
        cerr << blockNum << "\r";
        cerr.flush();
    }

    if (blockNum < opt->oldestBlock)
        opt->oldestBlock = blockNum;

    if (opt->requestsHistory() && !isArchiveNode())
        opt->errors.push_back("Request for historical state at block " + uint_2_Str(blockNum) + " not available.");

    if (!opt->theCall.address.empty()) {
        opt->theCall.blockNumber = blockNum;
        if (doEthCall(opt->theCall)) {
            CTransaction art;
            art.input = opt->theCall.encoding + opt->theCall.bytes;
            opt->abi_spec.articulateTransaction(&art);
            opt->theCall.callResult.inputs = art.articulatedTx.inputs;
            //            opt->theCall.address = opt->opt->callVariables[0];
            if (isText) {
                cout << opt->theCall.Format(expContext().fmtMap["format"]) << endl;

            } else {
                if (!opt->firstOut)
                    cout << "," << endl;
                cout << "  ";
                indent();
                opt->theCall.toJson(cout);
                unindent();
                opt->firstOut = false;
            }

        } else {
            ostringstream os;
            os << "No result from call to " << opt->theCall.address << " with fourbyte " << opt->theCall.encoding
               << " at block " << blockNum;
            opt->errors.push_back(os.str());
        }

    } else {
        CEthState state;
        state.address = opt->current;
        wei_t balance = getBalanceAt(state.address, blockNum);
        if (opt->changes) {
            if (balance == opt->prevBal)
                return !shouldQuit();
            opt->prevBal = balance;
        }

        if (opt->no_zero && balance <= opt->deminimus)
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
        if (opt->modeBits & ST_STORAGE)
            state.storage = getStorageAt(state.address, 0, opt->latestBlock);
        if (opt->modeBits & ST_DEPLOYED) {
            blknum_t dep = getDeployBlock(state.address);
            state.deployed = dep == NOPOS ? 0 : dep;
        }
        if (opt->modeBits & ST_ACCTTYPE)
            state.accttype = (isContractAt(state.address, opt->latestBlock) ? "Contract" : "EOA");

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
    }
    return !shouldQuit();
}
