/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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

extern const char *STR_FMT_BLOOMS_OUT;
extern string_q doOneBloom(uint64_t num, const COptions& opt);
//------------------------------------------------------------
int main(int argc, const char * argv[]) {

    etherlib_init(quickQuitHander);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        cout << (options.isMulti() ? "[" : "");
        string_q list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = toLongU(nextTokenClear(list, '|'));
            string_q result = doOneBloom(bn, options);
            cout << result;
            if (!options.asBars) {
                if (!list.empty())
                    cout << ",";
                cout << "\n";
            }
        }
        cout << (options.isMulti() ? "]" : "");
    }

    return 0;
}

string_q toBits(const string_q& blIn) {
    string_q ret = toLower(blIn).substr(2);
    replaceAll(ret, "0", "0000");
    replaceAll(ret, "1", "0001");
    replaceAll(ret, "2", "0010");
    replaceAll(ret, "3", "0011");
    replaceAll(ret, "4", "0100");
    replaceAll(ret, "5", "0101");
    replaceAll(ret, "6", "0110");
    replaceAll(ret, "7", "0111");
    replaceAll(ret, "8", "1000");
    replaceAll(ret, "9", "1001");
    replaceAll(ret, "a", "1010");
    replaceAll(ret, "b", "1011");
    replaceAll(ret, "c", "1100");
    replaceAll(ret, "d", "1101");
    replaceAll(ret, "e", "1110");
    replaceAll(ret, "f", "1111");
    return "0x"+ret;
}
string_q asBar(const string_q& blIn) {
    return substitute(substitute(substitute(toBits(blIn).substr(2), "0", ""), "1", "-"), "---", "🁡");
}

#include "bloom_blocks.h"
string_q doOneBloom(uint64_t num, const COptions& opt) {

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    string_q numStr = asStringU(num);

    if (opt.isRaw) {

        if (!opt.receipt) {
            HIDE_FIELD(CBloomBlock, "transactions");
        } else {
            HIDE_FIELD(CBloomBlock, "number");
            HIDE_FIELD(CBloomBlock, "logsBloom");
        }

        string_q r = getRawBlock(num);
        CBloomBlock rawBlock;
        rawBlock.parseJson(cleanUpJson((char*)r.c_str()));
        HIDE_FIELD(CBloomTrans,"hash");

        if (opt.asBits) {
            rawBlock.logsBloom = toBits(rawBlock.logsBloom);
            for (uint32_t i = 0 ; i < rawBlock.transactions.size() ; i++)
                rawBlock.transactions[i].receipt.logsBloom = toBits(rawBlock.transactions[i].receipt.logsBloom);
        }
        if (opt.asBars) {
            ostringstream os;
            if (opt.receipt)
                os << "\n" << string_q(90, '-') << " " << rawBlock.number << string_q(90, '-') << "\n";
            else
                os << num << ": ";
            os << asBar(rawBlock.logsBloom) << "\n";
            for (uint32_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                if (opt.receipt) {
                    string_q x = asBar(rawBlock.transactions[i].receipt.logsBloom);
                    if (!x.empty())
                        os << x << "\n";
                }
            }
            return os.str().c_str();
        }

        ostringstream os;
        rawBlock.doExport(os);
        return os.str().c_str();

    } else {

        SFBloomArray blooms;
        readBloomArray(blooms, substitute(getBinaryFilename(num), "/blocks/", "/blooms/"));
        ostringstream os;
        os << "\n" << string_q(90, '-') << " " << num << string_q(90, '-') << "\n";
        for (uint32_t i = 0 ; i < blooms.size(); i++) {
            os << asBar(bloom2Bits(blooms[i])) << "\n";
        }
        result = os.str().c_str();
    }
    return result;
}
