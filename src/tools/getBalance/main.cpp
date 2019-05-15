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
                options.prevWei = 0;
                options.item = &item.second;
                options.blocks.forEveryBlockNumber(visitBlock, &options);
                first = false;
            }
        }
        cout << exportPostamble(options.exportFmt, expContext().fmtMap["meta"]);

#if 0
        if (options.total) {
            string_q dispBal = options.getDispBal(options.newestBlock, options.totalBal);
            cout << "        Total for " << cGreen << options.items.size() << cOff;
            cout << " accounts at " << cTeal << "latest" << cOff << " block";
            cout << " is " << cYellow << substitute(dispBal, "  ", " ") << cOff << "\n";
        }
#endif
    }

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void *data) {

    COptions *options = reinterpret_cast<COptions *>(data);
    wei_t balance = getBalanceAt(options->item->address, options->item->blockNumber);

    bool show = true;
    if (options->changes) {
        if (balance == options->prevWei)
            show = false;
        options->prevWei = balance;
    }
    if (show)
        show = (!options->noZero || balance > 0);
    if (!show)
        return !shouldQuit();

    options->item->blockNumber = blockNum;
    options->item->wei = balance;
    bool isText = (options->exportFmt & (TXT1|CSV1));
    if (isText) {
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
