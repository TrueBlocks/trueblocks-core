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

        bool isText = (options.exportFmt & (TXT1|CSV1));
        if (isText && options.items.size() == 0) {
            LOG_INFO("No results");

        } else {
            for (auto item : options.items) {
                if (once)
                    cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], item.second.getRuntimeClass());
                options.prevBal = 0;
                options.item = (CEthState*)&item.second;
                options.blocks.forEveryBlockNumber(visitBlock, &options);
                once = false;
            }
        }
        if (options.hasHistory() && !nodeHasBalances()) {
            if (isText) {
                cerr << "Your node does not report historical state. Results are incorrect." << endl;
            } else {
                expContext().fmtMap["meta"] =
                ", \"warning\": \"Your node does not report historical state. Results are incorrect.\"";
            }
        }
        cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);
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

    wei_t balance = getBalanceAt(opt->item->address, blockNum);
    if (opt->changes) {
        if (balance == opt->prevBal)
            return !shouldQuit();
        opt->prevBal = balance;
    }

    if (opt->exclude_zero && balance <= opt->deminimus)
        return !shouldQuit();

    opt->item->blockNumber = blockNum;
    if (opt->mode & ST_BALANCE)
        opt->item->balance = balance;
    if (opt->mode & ST_NONCE)
        opt->item->nonce = getNonceAt(opt->item->address, blockNum);
    if (opt->mode & ST_CODE) {
        string_q code = getCodeAt(opt->item->address);
        opt->item->code = code;
        if (code.length() > 250 && !verbose)
            opt->item->code = code.substr(0,20) + "..." + code.substr(code.length()-20, 100);
    }
    if (opt->mode & ST_STORAGE)
        opt->item->storage = getStorageAt(opt->item->address, 0);
    if (opt->mode & ST_DEPLOYED)
        opt->item->deployed = getDeployBlock(opt->item->address);
    if (opt->mode & ST_ACCTTYPE)
        opt->item->accttype = (isContractAt(opt->item->address) ? "Contract" : "EOA");

    if (true) {
        if (isText) {
            cout << opt->item->Format(expContext().fmtMap["format"]) << endl;

        } else {
            if (!opt->first)
                cout << "," << endl;
            cout << "  ";
            incIndent();
            opt->item->doExport(cout);
            decIndent();
            opt->first = false;
        }
    }

    return !shouldQuit();
}
