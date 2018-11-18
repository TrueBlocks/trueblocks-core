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

extern const char *STR_FMT_BLOOMS_OUT;
extern string_q doOneBloom(uint64_t num, const COptions& opt);
//-------------------------------------------------------------------------------------
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

        cout << (options.isMulti() ? "[\n" : "");
        string_q list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = str_2_Uint(nextTokenClear(list, '|'));
            string_q result = doOneBloom(bn, options);
            cout << result;
            if (!options.asBars && !options.asBitBars && !options.asPctBars) {
                if (!list.empty())
                    cout << ",";
                cout << "\n";
            }
        }
        cout << (options.isMulti() ? "]" : "");
    }

    return 0;
}

#include "bloom_blocks.h"
//-------------------------------------------------------------------------------------
string_q doOneBloom(uint64_t num, const COptions& opt) {

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    string_q numStr = uint_2_Str(num);

    if (opt.isRaw) {

        string_q r = getRawBlock(num);
        CBloomBlock rawBlock;
        rawBlock.parseJson3(r);
        HIDE_FIELD(CBloomTrans, "hash");

        if (opt.blockOnly) {
            rawBlock.transactions.clear();
            HIDE_FIELD(CBloomBlock, "transactions");
        }

        if (opt.receiptsOnly) {
            //HIDE_FIELD(CBloomBlock, "number");
            HIDE_FIELD(CBloomBlock, "logsBloom");
        }

        if (opt.bitCount) {
            UNHIDE_FIELD(CBloomReceipt, "bitCount");
            UNHIDE_FIELD(CBloomBlock, "bitCount");
        }

        if (opt.asBits) {
            bloom_t bloom = str_2_BigUint(rawBlock.logsBloom);
            rawBlock.logsBloom = bloom_2_Bits(bloom);
            for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                bloom = str_2_BigUint(rawBlock.transactions[i].receipt.logsBloom);  // .at cannot go past end of vector!
                rawBlock.transactions.at(i).receipt.logsBloom = bloom_2_Bits(bloom);
            }
        }

        if (opt.asBars) {
            ostringstream os;
            os << num << ": ";
            bloom_t bloom = str_2_BigUint(rawBlock.logsBloom);
            os << bloom_2_Bar(bloom) << "\n";
            if (opt.receiptsOnly) {
                for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                    bloom = str_2_BigUint(rawBlock.transactions[i].receipt.logsBloom);
                    string_q x = bloom_2_Bar(bloom);
                    if (!x.empty())
                        os << padLeft("", 9) << x << "\n";
                }
            }
            return os.str().c_str();
        }

        if (opt.asBitBars) {
            ostringstream os;
            os << num << ": ";
            bloom_t bloom = str_2_BigUint(rawBlock.logsBloom);
            os << string_q(bitsTwiddled(bloom), '-') << "\n";
            if (opt.receiptsOnly) {
                for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                    bloom = str_2_BigUint(rawBlock.transactions[i].receipt.logsBloom);
                    string_q x = string_q(bitsTwiddled(bloom), '-') ;
                    if (!x.empty())
                        os << padLeft("", 9) << x << "\n";
                }
            }
            return os.str().c_str();
        }

        if (opt.asPctBars) {
            ostringstream os;
            os << num << ": ";
            bloom_t bloom = str_2_BigUint(rawBlock.logsBloom);
            os << string_q(bitsTwiddled(bloom) * 200 / 1024, '-') << "\n";
            if (opt.receiptsOnly) {
                for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                    bloom = str_2_BigUint(rawBlock.transactions[i].receipt.logsBloom);
                    string_q x = string_q(bitsTwiddled(bloom) * 200 / 1024, '-') ;
                    if (!x.empty())
                        os << padLeft("", 9) << x << "\n";
                }
            }
            return os.str().c_str();
        }

        ostringstream os;
        if (opt.blockOnly)
            os << rawBlock.Format() << "\n";
        else
            rawBlock.doExport(os);
        return os.str().c_str();

    } else {

        CBloomArray blooms;
        readBloomArray(blooms, substitute(getBinaryFilename(num), "/blocks/", "/blooms/"));

        if (opt.asBars) {
            ostringstream os;
            for (size_t i = 0 ; i < blooms.size() ; i++) {
                string_q head = (i == 0 ? uint_2_Str(num) + ": " : "");
                os << padLeft(head, 9) << bloom_2_Bar(blooms[i]) << "\n";
            }
            return os.str().c_str();
        }

        if (opt.asBitBars) {
            ostringstream os;
            for (size_t i = 0 ; i < blooms.size() ; i++) {
                string_q head = (i == 0 ? uint_2_Str(num) + ": " : "");
                os << padLeft(head, 9) << string_q(bitsTwiddled(blooms[i]), '-') << "\n";
            }
            return os.str().c_str();
        }

        if (opt.asPctBars) {
            ostringstream os;
            bloom_t all = 0;
            for (size_t i = 0 ; i < blooms.size() ; i++)
                all = joinBloom(all, blooms[i]);
            os << padLeft(uint_2_Str(num), 9) << ": ";
            if (blooms.size())
                os << string_q(bitsTwiddled(all) * 200 / (1024 * blooms.size()), '-');
            os << "\n";
            return os.str().c_str();
        }

        ostringstream os;
        os << "{\n";
        os << "\t\"blockNumber\": \"" << num << "\",\n";
        if (opt.bitCount) {
            os << "\t\"bitCounts\": [\n";
            for (size_t i = 0 ; i < blooms.size() ; i++) {
                os << "\t\t" << uint_2_Str(bitsTwiddled(blooms[i]));
                if (i < blooms.size() - 1)
                    os << ",";
                os << "\n";
            }
            os << "\t],\n";
        }
        os << "\t\"eab\": [\n";
        for (size_t i = 0 ; i < blooms.size() ; i++) {
            bloom_t bloom = blooms[i];
            if (opt.asBits) {
                os << "\t\t\"0x" << bloom_2_Bits(bloom) << "\"";

            } else {
                os << "\t\t\"0x" << bloom_2_Bytes(bloom) << "\"";

            }
            if (i < blooms.size() - 1)
                os << ",";
            os << "\n";
        }
        os << "\t]\n}";

        result = os.str().c_str();
    }
    return result;
}
