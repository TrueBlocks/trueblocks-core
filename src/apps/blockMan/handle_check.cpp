/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------------
extern bool checkDups(blknum_t bn, void *data);
extern bool checkEmptyBlock(blknum_t bn, void *data);
extern bool checkEmptyBlock2(CBlock& block, void *data);
extern bool checkNonEmptyBlock(CBlock& node, void *data);
extern void report(ostream& os, const string_q& str, blknum_t bn, uint64_t nTx);
//----------------------------------------------------------------------------------
void handle_check(COptions& options) {

    if (options.incEmpty) {
        forEveryEmptyBlockByNumber(checkEmptyBlock, &options, options.startBlock, options.nBlocks, options.skip);
        cout << endl;
    }

    if (options.incFull) {
        forEveryNonEmptyBlockOnDisc(checkNonEmptyBlock, &options, options.startBlock, options.nBlocks, options.skip);
        cout << endl;
    }

    blknum_t prev = NOPOS;
    forEveryNonEmptyBlockByNumber(checkDups, &prev, options.startBlock, options.nBlocks, options.skip);
}

//----------------------------------------------------------------------------------
bool checkDups(blknum_t bn, void *data) {
    blknum_t *prev = (blknum_t*)data;
    if (*prev == bn)
        cout << "Duplicate record at " << bn << " " << redX << "\n";
    *prev = bn;
    return true;
}

//----------------------------------------------------------------------------------
bool checkEmptyBlock(blknum_t bn, void *data) {

    ostringstream os;
    if (!fileExists(getBinaryFilename(bn))) {
        // block is not in index and its file does not exist (ok)
        os << "Non-indexed block [{BN}] has [{NTX}] transactions [{GCK}]          \r";
        report(cerr, os.str(), bn, 0);
        return true;
    }

    CBlock block;
    getBlock(block, bn);
    return checkEmptyBlock2(block, data);
}

//----------------------------------------------------------------------------------
bool checkEmptyBlock2(CBlock& block, void *data) {

    ostringstream os;
    string_q fileName = getBinaryFilename(block.blockNumber);
    if (block.transactions.size()) {
        // block is not in index but it has transactions (not ok)
        os << "Non-indexed block [{BN}] has [{NTX}] transactions but is not in the index [{RCK}]\n";
        report(cout, os.str(), block.blockNumber, block.transactions.size());

    } else if (fileExists(fileName)) {
        // block is not in index but its file exists (not ok)
        os << "Non-indexed block [{BN}] has zero transactions but its file exists on disc [{YCK}]\n";
        report(cout, os.str(), block.blockNumber, block.transactions.size());

    } else if (block.gasUsed > 0) {
        // block is not in index but it uses gas (not ok)
        os << "Non-indexed block [{BN}] has zero transactions but uses more than zero gas [{RCK}]\n";
        report(cout, os.str(), block.blockNumber, block.transactions.size());

    } else {
        // block is okay
        os << "Non-indexed block [{BN}] has [{NTX}] transactions [{GCK}]          \r";
        report(cerr, os.str(), block.blockNumber, block.transactions.size());
    }
    return true;
}

string_q yellowCaution = "\e[7m\e[0;33m!!\e[0m";
//----------------------------------------------------------------------------------
bool checkNonEmptyBlock(CBlock& block, void *data) {

    ostringstream os;
    string_q fileName = getBinaryFilename(block.blockNumber);
    if (!block.transactions.size()) {
        // block is in index, but it has no transactions (not ok)
        os << "Indexed block [{BN}] has zero transactions but is in the index [{RCK}]\n";
        report(cout, os.str(), block.blockNumber, block.transactions.size());

    } else if (!fileExists(fileName)) {
        // block is in index, but its file does not exist (not ok)
        os << "Indexed block [{BN}] has [{NTX}] transactions but its file does not exist [{YCK}]\n";
//        os << "getBlock --force " << block.blockNumber << "\n";
        report(cout, os.str(), block.blockNumber, block.transactions.size());

    } else if (block.gasUsed == 0) {
        // block is in index, but it doesn't use any gas (not ok)
        os << "Indexed block [{BN}] has [{NTX}] transactions but uses no gas [{RCK}]\n";
        report(cout, os.str(), block.blockNumber, block.transactions.size());

    } else {
        // block is okay
        os << "Indexed block [{BN}] has [{NTX}] transactions [{GCK}]          \r";
        report(cerr, os.str(), block.blockNumber, block.transactions.size());
    }
    return true;
}

//----------------------------------------------------------------------------------
void report(ostream& os, const string_q& str, blknum_t bn, uint64_t nTx) {
    string_q result = str;
    result = substitute(result, "[{",  cYellow);
    result = substitute(result, "}]",  cOff);
    result = substitute(result, "BN",  padLeft(uint_2_Str(bn),9));
    result = substitute(result, "NTX", padLeft(uint_2_Str(nTx),5));
    result = substitute(result, "GCK", greenCheck);
    result = substitute(result, "RCK", redX);
    result = substitute(result, "YCK", yellowCaution);
    os << result;
    os.flush();
}
