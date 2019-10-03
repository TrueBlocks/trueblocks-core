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
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        cout << (options.isMulti() ? "[\n" : "");
        string_q list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = str_2_Uint(nextTokenClear(list, '|'));
            cout << doOneBloom(bn, options);
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

//-------------------------------------------------------------------------------------
typedef string_q (*BLOOMTOSTRFUNC)(const bloom_t& bloom, uint64_t bitBnd, void *data);
//-------------------------------------------------------------------------------------
string_q pctBar(const bloom_t& bloom, uint64_t bitBnd, void *data) { uint64_t div = (uint64_t)data; return string_q(bitsTwiddled(bloom) * bitBnd / div, '-'); }
string_q bitBar(const bloom_t& bloom, uint64_t unused, void *data) { return string_q(bitsTwiddled(bloom), '-');              }
string_q bars  (const bloom_t& bloom, uint64_t unused, void *data) { return bloom_2_Bar(bloom);                              }

//-------------------------------------------------------------------------------------
string_q showBloom_oldblooms(blknum_t bn, uint64_t bitBnd, const CBloomArray& blooms, BLOOMTOSTRFUNC func, void *data=NULL) {
    if (!func)
        return "";
    ostringstream os;
    for (size_t bl = 0 ; bl < blooms.size() ; bl++) {
        string_q head = (bl == 0 ? uint_2_Str(bn) + ": " : "");
        string_q line = (bitsTwiddled(blooms[bl]) == 0 ? "" : (*func)(blooms[bl], bitBnd, data));
        if (bl == 0 || !line.empty()) {
            os << padLeft(head, 9);
            if (!line.empty())
                os << line;
            os  << "\n";
        }
    }
    return os.str();
}

#include "bloom_blocks.h"
//-------------------------------------------------------------------------------------
string_q doOneBloom(uint64_t num, const COptions& opt) {

    CBloomArray blooms;

    ostringstream os;
    if (opt.isRaw) {

        CBloomBlock rawBlock;
        string_q raw = getRawBlock(num);
        rawBlock.parseJson3(raw);
        if (opt.blockOnly)
            rawBlock.transactions.clear();

        blooms.push_back(str_2_BigUint(rawBlock.logsBloom));
        if (opt.receiptsOnly) {
            for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++)
                blooms.push_back(str_2_BigUint(rawBlock.transactions[i].receipt.logsBloom));
        }

             if (opt.asBars)    return showBloom_oldblooms(num, opt.bitBound, blooms, bars);
        else if (opt.asBitBars) return showBloom_oldblooms(num, opt.bitBound, blooms, bitBar);
        else if (opt.asPctBars) return showBloom_oldblooms(num, opt.bitBound, blooms, pctBar, (void*)1024);
        else {
            CBloomTransArray showing;
            if (opt.asBits)
                rawBlock.logsBloom = bloom_2_Bits(str_2_BigUint(rawBlock.logsBloom));
            for (size_t i = 0 ; i < rawBlock.transactions.size() ; i++) {
                bloom_t bloom = str_2_BigUint(rawBlock.transactions.at(i).receipt.logsBloom);  // .at cannot go past end of vector!
                if (verbose || bloom != 0) {
                    if (opt.asBits)
                        rawBlock.transactions.at(i).receipt.logsBloom = bloom_2_Bits(bloom);
                    CBloomTrans t = rawBlock.transactions.at(i);
                    showing.push_back(t);
                }
            }
            rawBlock.transactions = showing;
            SHOW_FIELD(CBloomBlock, "transactions");
            if (rawBlock.transactions.size() == 0)
                HIDE_FIELD(CBloomBlock, "transactions");
            rawBlock.doExport(os);
        }

    } else {

        string_q fileName = getBinaryCacheFilename(CT_BLOOMS, num);
        readBloomFromBinary(blooms, fileName);
             if (opt.asBars)    return showBloom_oldblooms(num, opt.bitBound, blooms, bars);
        else if (opt.asBitBars) return showBloom_oldblooms(num, opt.bitBound, blooms, bitBar);
        else if (opt.asPctBars) {
            bloom_t one_bloom = 0;
            uint64_t n = blooms.size();
            for (size_t i = 0 ; i < blooms.size() ; i++)
                one_bloom = joinBloom(one_bloom, blooms[i]);
            blooms.clear(); blooms.push_back(one_bloom);
            return showBloom_oldblooms(num, opt.bitBound, blooms, pctBar, (void*)(1024 * n));

        } else {

            os << "{\n";
            os << "\t\"blockNumber\": \"" << num << "\",\n";
            if (verbose || opt.bitCount) {
                os << "\t\"bitCount\": [";
                if (blooms.size()) os << "\n";
                for (size_t i = 0 ; i < blooms.size() ; i++) {
                    os << "\t\t" << uint_2_Str(bitsTwiddled(blooms[i]));
                    if (i < blooms.size() - 1)
                        os << ",";
                    os << "\n";
                }
                if (blooms.size()) os << "\t";
                os << "],\n";
                if (verbose)
                    os << "\t\"sizeInBytes\": " << fileSize(fileName) << ",\n";
            }
            os << "\t\"eab\": [";
            if (blooms.size()) os << "\n";
            for (size_t i = 0 ; i < blooms.size() ; i++) {
                bloom_t bloom = blooms[i];
                if (verbose || bloom != 0) {
                    if (opt.asBits) {
                        os << "\t\t\"0x" << bloom_2_Bits(bloom) << "\"";

                    } else {
                        os << "\t\t\"0x" << bloom_2_Bytes(bloom) << "\"";

                    }
                    if (i < blooms.size() - 1)
                        os << ",";
                    os << "\n";
                }
            }
            if (blooms.size()) os << "\t";
            os << "]\n}";

        }
    }
    return os.str();
}
