/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool checkEmptyBlock(blknum_t bn, void *data);
extern void report(const string_q& str, bool ok, blknum_t bn, uint64_t nTx);
//----------------------------------------------------------------------------------
void handle_check(COptions& options) {
    blknum_t latest = getLatestBlockFromClient();

    if (options.empty) {
        forEveryEmptyBlockByNumber(checkEmptyBlock, &options, options.startBlock, latest);
        cout << endl;
    }

    if (options.full) {
        forEveryNonEmptyBlockOnDisc(visitNonEmptyBlock, &options, options.startBlock, latest);
        cout << endl;
    }
}

//----------------------------------------------------------------------------------
bool visitEmptyBlock(CBlock& block, void *data) {

    bool ok = true;
    ostringstream os;
    if (block.transactions.size()) {
        os << "Block [{BN}] has [{NTX}] transactions but is not in the index [{CK}]\n";
        ok = false;
    } else {
        string_q fileName = getBinaryFilename(block.blockNumber);
        if (fileExists(fileName)) {
            os << "Block [{BN}] has zero transactions but its file exists in the cache [{CK}]\n";
            ok = false;
        }
        if (block.gasUsed > 0) {
            os << "Block [{BN}] has zero transactions but uses more than zero gas [{CK}]\n";
            ok = false;
        }
    }
    if (ok)
        os << "Block [{BN}] has [{NTX}] transactions [{CK}] ([{CNT}])          \r";

    report(os.str(), ok, block.blockNumber, block.transactions.size());

    return true;
}

//----------------------------------------------------------------------------------
bool visitNonEmptyBlock(CBlock& block, void *data) {

    static uint64_t hitCount = 0;
    ++hitCount;

    bool ok = true;
    ostringstream os;
    if (block.transactions.size()) {
        string_q fileName = getBinaryFilename(block.blockNumber);
        if (!fileExists(fileName)) {
            os << "Block [{BN}] has [{NTX}] transactions but is not in the cache [{CK}]\n";
            ok = false;
        }
        if (block.gasUsed == 0) {
            os << "Block [{BN}] has [{NTX}] transactions but uses no gas [{CK}]\n";
            ok = false;
        }
    } else {
        os << "Block [{BN}] has zero transactions but is in the index [{CK}]\n";
        ok = false;
    }
    if (ok)
        os << "Block [{BN}] has [{NTX}] transactions [{CK}] ([{CNT}]-[{SZ}] b)          \r";

    report(os.str(), ok, block.blockNumber, block.transactions.size());

    return true;
}

//----------------------------------------------------------------------------------
bool checkEmptyBlock(blknum_t bn, void *data) {
    if (!fileExists(getBinaryFilename(bn))) {
        ostringstream os;
        os << "Block [{BN}] has [{NTX}] transactions [{CK}] ([{CNT}])          \r";
        report(os.str(), true, bn, 0);
        return true;
    }
    CBlock block;
    getBlock(block, bn);
    return visitEmptyBlock(block, data);
}

//----------------------------------------------------------------------------------
void report(const string_q& str, bool ok, blknum_t bn, uint64_t nTx) {
    static uint64_t hitCount = 0;
    string_q result = str;
    result = substitute(result, "[{",  cYellow);
    result = substitute(result, "}]",  cOff);
    result = substitute(result, "BN",  padLeft(uint_2_Str(bn),9));
    result = substitute(result, "SZ",  uint_2_Str(fileSize(getBinaryFilename(bn))));
    result = substitute(result, "NTX", padLeft(uint_2_Str(nTx),5));
    result = substitute(result, "CK",  (ok ? greenCheck : redX));
    result = substitute(result, "CNT", uint_2_Str(++hitCount));
    if (!ok) { cout << "\t" << result; cout.flush(); }
    else     { cerr << "\t" << result; cerr.flush(); }
}
