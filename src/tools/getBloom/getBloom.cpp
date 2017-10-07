/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern SFString doOneBloom(SFUint32 num, const COptions& opt);
//------------------------------------------------------------
int main(int argc, const char * argv[]) {

    // Tell the system where the blocks are and which version to use
    etherlib_init("fastest");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
        if (!options.quiet)
            cout << (options.isMulti() ? "[" : "");
        if (options.blocks.isRange) {
            for (SFUint32 i = options.blocks.start ; i < options.blocks.stop ; i++) {
                cout << doOneBloom(i, options);
                if (!options.quiet) {
                    if (i < options.blocks.stop-1)
                        cout << ",";
                    cout << "\n";
                    if (options.isCheck) {
                        // TODO(any): read 'raw' and compare result with gold
                    }

                } else if (!(i%150)) {
                    cout << ".";
                    cout.flush();

                } else if (!(i%1000)) {
                    cout << "+";
                    cout.flush();
                }
            }
        } else {
            for (SFUint32 i = 0 ; i < options.blocks.nNums ; i++) {
                cout << doOneBloom(options.blocks.nums[i], options);
                if (!options.quiet) {
                    if (i < options.blocks.nNums - 1)
                        cout << ",";
                    cout << "\n";
                    if (options.isCheck) {
                        // TODO(any): read 'raw' and compare result with gold
                    }

                } else if (!(i%150)) {
                    cout << ".";
                    cout.flush();

                } else if (!(i%1000)) {
                    cout << "+";
                    cout.flush();
                }
            }
        }
        if (!options.quiet)
            cout << (options.isMulti() ? "]" : "");
    }

    return 0;
}

//------------------------------------------------------------
SFString doOneBloom(SFUint32 num, const COptions& opt) {

    CBlock gold;
    SFString result;
    SFString numStr = asStringU(num);
    if (opt.isRaw) {

        if (!queryRawBlock(result, numStr, true, opt.terse)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        }

    } else {
        // queryBlock returns false if there are no transactions, so ignore the return value
        queryBlock(gold, numStr, true);
        if (/* DISABLES CODE */ (false)) { // turn this on to force a write of the block to the disc
            SFString fileName = getBinaryFilename1(gold.blockNumber);
            writeToBinary(gold, fileName);
        }

        if (getSource().Contains("Only")) {
            // --source::cache mode doesn't include timestamp in transactions
            for (txnum_t t = 0 ; t < gold.transactions.getCount() ; t++) {
                gold.transactions[t].timestamp = (SFUint32)gold.timestamp;
            }

        }
        result = gold.Format();
    }

    return (opt.quiet ? "" : result);
}
