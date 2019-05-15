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

//-------------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.blocks.forEveryBlockNumber(visitBlock, &options);
    }

        if (!options.hasHistory())
            LOG_WARN("This node does not have historical state. The results presented above are incorrect.");

    acctlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitBlock(uint64_t blockNum, void *data) {

    COptions *options = reinterpret_cast<COptions *>(data);
    if (blockNum < options->oldestBlock)
        options->oldestBlock = blockNum;
    for (auto const& addr : options->items) {

        string_q code1 = getCodeAt(addr, blockNum);
        bool hasCode   = isContractAt(addr, blockNum);

        if (options->showBytes) {
            cout << "Code at address: " << addr << ":\n";
            cout << code1 << "\n";

        } else if (options->when) {
            ASSERT(hasCode);
            cout << "whenDeployed(" << addr << "): " << whenDeployed(addr) << "\n";

        } else {
            cout << "getAccountInfo(" << addr << "): " << (hasCode ? "true" : "false") << "\n";
            if (verbose)
                cout << code1 << "\n";
        }
    }
    return !shouldQuit();
}
