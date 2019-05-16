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

    bool first = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        bool isText = (options.exportFmt & (TXT1|CSV1));
        if (isText && options.items.size() == 0) {
            LOG_INFO("No results");

        } else {
            for (auto item : options.items) {
                if (first)
                    cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], item.second.getRuntimeClass());
                options.prevBal = 0;
                options.item = (CEthState*)&item.second;
                options.blocks.forEveryBlockNumber(visitBlock, &options);
                first = false;
            }
        }
        if (!options.hasHistory())
            LOG_WARN("This node does not have historical state. The results presented above are incorrect.");
        cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);
    }

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void *data) {

    if (!isTestMode()) { cerr << blockNum << "\r"; cerr.flush(); }
    COptions *options = reinterpret_cast<COptions *>(data);
    if (blockNum < options->oldestBlock)
        options->oldestBlock = blockNum;

    wei_t balance = getBalanceAt(options->item->address, blockNum);
    if (options->changes) {
        if (balance == options->prevBal)
            return !shouldQuit();
        options->prevBal = balance;
    }

    if (options->exclude_zero && balance <= options->deminimus)
        return !shouldQuit();

    options->item->blockNumber = blockNum;
    if (options->mode & ST_BALANCE)
        options->item->balance = balance;
    if (options->mode & ST_NONCE)
        options->item->nonce = getNonceAt(options->item->address, blockNum);
    if (options->mode & ST_CODE) {
        string_q code = getCodeAt(options->item->address);
        options->item->code = code;
        if (code.length() > 250)
            options->item->code = code.substr(0,20) + "..." + code.substr(code.length()-20, 100);
    }
    if (options->mode & ST_STORAGE)
        options->item->storage = getStorageAt(options->item->address, 0);
    if (options->mode & ST_DEPLOYED)
        options->item->deployed = getDeployBlock(options->item->address);
    if (options->mode & ST_ACCTTYPE)
        options->item->accttype = (isContractAt(options->item->address) ? "Contract" : "EOA");

    if ((options->exportFmt & (TXT1|CSV1))) {
        cout << options->item->Format(expContext().fmtMap["format"]) << endl;

    } else {
        static bool first = true;
        if (!first)
            cout << "," << endl;
        cout << "  ";
        incIndent();
        options->item->doExport(cout);
        decIndent();
        first = false;
    }

    return !shouldQuit();
}
