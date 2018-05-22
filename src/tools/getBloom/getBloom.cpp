/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern const char *STR_FMT_BLOOMS_OUT;
extern SFString doOneBloom(uint64_t num, const COptions& opt);
//------------------------------------------------------------
int main(int argc, const char * argv[]) {

    etherlib_init(quickQuitHander);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        cout << (options.isMulti() ? "[" : "");
        SFString list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = toLongU(nextTokenClear(list, '|'));
            SFString result = doOneBloom(bn, options);
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

SFString toBits(const SFString& blIn) {
    SFString ret = toLower(blIn).substr(2);
    ret.ReplaceAll("0","0000");
    ret.ReplaceAll("1","0001");
    ret.ReplaceAll("2","0010");
    ret.ReplaceAll("3","0011");
    ret.ReplaceAll("4","0100");
    ret.ReplaceAll("5","0101");
    ret.ReplaceAll("6","0110");
    ret.ReplaceAll("7","0111");
    ret.ReplaceAll("8","1000");
    ret.ReplaceAll("9","1001");
    ret.ReplaceAll("a","1010");
    ret.ReplaceAll("b","1011");
    ret.ReplaceAll("c","1100");
    ret.ReplaceAll("d","1101");
    ret.ReplaceAll("e","1110");
    ret.ReplaceAll("f","1111");
    return "0x"+ret;
}
SFString asBar(const SFString& blIn) {
    return toBits(blIn).substr(2).Substitute("0", "").Substitute("1", "-").Substitute("---", "🁡");
}

#include "bloom_blocks.h"
SFString doOneBloom(uint64_t num, const COptions& opt) {

    CBlock gold;
    gold.blockNumber = num;
    SFString result;
    SFString numStr = asStringU(num);

    if (opt.isRaw) {

        if (!opt.receipt) {
            HIDE_FIELD(CBloomBlock, "transactions");
        } else {
            HIDE_FIELD(CBloomBlock, "number");
            HIDE_FIELD(CBloomBlock, "logsBloom");
        }

        SFString r = getRawBlock(num);
        CBloomBlock rawBlock;
        rawBlock.parseJson(cleanUpJson((char*)r.c_str()));
        HIDE_FIELD(CBloomTrans,"hash");

        if (opt.asBits) {
            rawBlock.logsBloom = toBits(rawBlock.logsBloom);
            for (uint32_t i = 0 ; i < rawBlock.transactions.getCount() ; i++)
                rawBlock.transactions[i].receipt.logsBloom = toBits(rawBlock.transactions[i].receipt.logsBloom);
        }
        if (opt.asBars) {
            ostringstream os;
            if (opt.receipt)
                os << "\n" << SFString('-',90) << " " << rawBlock.number << SFString('-',90) << "\n";
            else
                os << num << ": ";
            os << asBar(rawBlock.logsBloom) << "\n";
            for (uint32_t i = 0 ; i < rawBlock.transactions.getCount() ; i++) {
                if (opt.receipt) {
                    SFString x = asBar(rawBlock.transactions[i].receipt.logsBloom);
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
        readBloomArray(blooms, getBinaryFilename(num).Substitute("/blocks/", "/blooms/"));
        ostringstream os;
        os << "\n" << SFString('-',90) << " " << num << SFString('-',90) << "\n";
        for (uint32_t i = 0 ; i < blooms.getCount(); i++) {
            os << asBar(bloom2Bits(blooms[i])) << "\n";
        }
        result = os.str().c_str();
    }
    return result;
}
