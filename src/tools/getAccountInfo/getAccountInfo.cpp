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

extern blknum_t whenDeployed(const address_t& addr);
bool visitBlock(uint64_t num, void *data);
//-------------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.blocks.forEveryBlockNumber(visitBlock, &options);
    }

    if (!options.hasHistory()) {
        cerr << cRed << "    Warning: " << cOff;
        cerr << "Your node does not have historical balances. Historical information is incorrect.\n";
    }

    return 0;
}

//-------------------------------------------------------------------------------------
bool visitBlock(uint64_t bn, void *data) {

    COptions *options = (COptions*)data;
    if (bn < options->oldestBlock)
        options->oldestBlock = bn;
    for (auto const& addr : options->addrs) {

        string_q code1 = getCodeAt(addr, bn);
        bool hasCode   = isContractAt(addr, bn);

        if (options->diff) {
            address_t addr2 = *(&addr+1);
            string_q code2 = getCodeAt(addr2, bn);
            cout << "Code at address '" << addr << "' and '" << addr2 + "' are "
            << (code1 == code2 ? "identical" : "different") << "\n";
            break;

        } else if (options->showBytes) {

            cout << "Code at address: " << addr << ":\n";
            cout << code1 << "\n";

        } else if (options->asData) {
            cout << addr << "\t" << (hasCode ? "true" : "false") << (options->when ? "\t" + uint_2_Str(whenDeployed(addr)) : "") << "\n";

        } else if (options->when) {
            ASSERT(hasCode);
            cout << "whenDeployed(" << addr << "): " << whenDeployed(addr) << "\n";

        } else {
            cout << "getAccountInfo(" << addr << "): " << (hasCode ? "true" : "false") << "\n";
            if (verbose)
                cout << code1 << "\n";
//#error
//                if (verbose)
//                    cout << getStorageAt(addr, 0) << " " << getNonceAt(addr) << "\n";
        }
    }
    return true;
}

//--------------------------------------------------------------
blknum_t findCodeAt_binarySearch(const address_t& addr, blknum_t first, blknum_t last) {
    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        bool atMid  = isContractAt(addr, mid);
        bool atMid1 = isContractAt(addr, mid + 1);
        if (!atMid && atMid1)
            return mid;  // found it
        if (atMid) {
            // we're too high, so search below
            return findCodeAt_binarySearch(addr, first, mid-1);
        }
        // we're too low, so search above
        return findCodeAt_binarySearch(addr, mid+1, last);
    }
    return first;
}

//-------------------------------------------------------------------------------------
blknum_t whenDeployed(const address_t& addr) {
    if (!isContractAt(addr))
        return NOPOS;
    blknum_t num = findCodeAt_binarySearch(addr, 0, getLastBlock_client() - (5 * (60/14)));  // five minutes ago
    return (num ? num+1 : NOPOS);
}
