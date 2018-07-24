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

    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        cout << (options.isMulti() || !options.isRaw ? "[" : "");
        string_q list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = str_2_Uint(nextTokenClear(list, '|'));
            string_q result = doOneBloom(bn, options);
            cout << result;
            if (!options.asBars) {
                if (!list.empty())
                    cout << ",";
                cout << "\n";
            }
        }
        cout << (options.isMulti() || !options.isRaw ? "]" : "");
    }

    return 0;
}

string_q asBar(const string_q& blIn) {
    SFUintBN bloom = str_2_BigUint(blIn);
    return substitute(substitute(substitute(extract("0x" + bloom_2_Bits(bloom), 2), "0", ""), "1", "-"), "---", "🁡");
}

#include "bloom_blocks.h"
string_q doOneBloom(uint64_t num, const COptions& opt) {

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    string_q numStr = uint_2_Str(num);

    if (opt.isRaw) {

        if (!opt.receipt) {
            HIDE_FIELD(CBloomBlock, "transactions");
        } else {
            HIDE_FIELD(CBloomBlock, "number");
            HIDE_FIELD(CBloomBlock, "logsBloom");
        }

        string_q r = getRawBlock(num);
        CBloomBlock rawBlock;
        rawBlock.parseJson(cleanUpJson((char*)r.c_str()));  // NOLINT
        HIDE_FIELD(CBloomTrans, "hash");

        if (opt.asBits) {
            SFUintBN bloom = str_2_BigUint(rawBlock.logsBloom);
            rawBlock.logsBloom = bloom_2_Bits(bloom);
            for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                bloom = str_2_BigUint(rawBlock.transactions[i].receipt.logsBloom);  // .at cannot go past end of vector!
                rawBlock.transactions.at(i).receipt.logsBloom =
                    bloom_2_Bits(bloom);
            }
        }

        if (opt.asBars) {
            ostringstream os;
            if (opt.receipt)
                os << "\n" << string_q(90, '-') << " " << rawBlock.number << string_q(90, '-') << "\n";
            else
                os << num << ": ";
            os << asBar(rawBlock.logsBloom) << "\n";
            for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
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

        CBloomArray blooms;
        readBloomArray(blooms, substitute(getBinaryFilename(num), "/blocks/", "/blooms/"));
        ostringstream os;
        os << "\n\t\"blockNumber\": \"" << num << "\"\n";
        for (size_t i = 0 ; i < blooms.size(); i++) {
            //            os << asBar(bloom_2_Bits(blooms[i])) << "\n";
            os << "0x" << blooms[i] << "\n";
        }
        result = os.str().c_str();
    }
    return result;
}
