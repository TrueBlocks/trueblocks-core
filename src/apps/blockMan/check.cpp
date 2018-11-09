/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool verifyBlock(const CBlock& block,   string_q& result);
//----------------------------------------------------------------------------------
// Called for each indexed non-empty block (i.e. blocks with transactions). If the
// block has zero transactions, you need to delete that file, and regenerate the index.
//----------------------------------------------------------------------------------
bool visitNonEmptyBlock(CBlock& block, void *data) {
    COptions *options = reinterpret_cast<COptions*>(data);

    string_q blkNum = padLeft("#" + uint_2_Str(block.blockNumber), 9);
    int64_t nTrans = (int64_t)block.transactions.size();
    if (!nTrans) {
        cerr << "Block " << asYellow(blkNum)
        << " has zero transactions, but is in the non-empty index."
        << " Delete the index and re-generate it with"
        << "--refreshen.\n";

    } else {
        string_q fileName = getBinaryFilename(block.blockNumber);
        if (!fileExists(fileName)) {
            cerr << "Non-empty binary file for block " << asYellow(blkNum) << " does not exist in the cache.\n";
        }
        static uint64_t fullHits = 0;
        cerr << "Full block  "
        << asYellow(blkNum) << " has "
        << asYellow(padNum5T(nTrans)) << " transactions "
        << greenCheck
        << " (nBlocks: " << asYellow(++fullHits) << ")"
        << "         \r";
    }
    if (block.gasUsed == 0)
        cerr << "Block " << asYellow(blkNum) << " which has transactions, uses zero gas: " << block.gasUsed << ". Probably wrong.\n";
    cerr.flush();

    if (options->deepCheck) {
        string_q result;
        if (!verifyBlock(block, result)) {
            cout << result << "\n";
            cout.flush();
        }
        cout.flush();
    }

    return true;
}

//----------------------------------------------------------------------------------
// Called for each supposedly empty block (i.e. blocks with zero transactions). If the
// block has one or more transactions, you need to regenerate the index. (The file will
// have been written to the disc during the queryBlock call).
//----------------------------------------------------------------------------------
bool visitEmptyBlock(CBlock& block, void *data) {
    COptions *options = reinterpret_cast<COptions*>(data);

    string_q blkNum = padLeft("#" + uint_2_Str(block.blockNumber), 9);
    int64_t nTrans = (int64_t)block.transactions.size();
    if (nTrans) {
        cerr << "Block " << asYellow(blkNum) << " has "
        << asYellow(padNum5T(nTrans)) << " transactions, but"
        << " is not in the non-empty block index. The binary file is on the hard drive, but you"
        << " need to re-generate the index with --refreshen.\n";

    } else {
        string_q fileName = getBinaryFilename(block.blockNumber);
        if (fileExists(fileName)) {
            cerr << "The binary file for block " << asYellow(blkNum) << " exists, but is is not in index.\n";
        }
        static uint64_t emptyHits = 0;
        cerr << "Empty block "
        << asYellow(blkNum) << " has "
        << asYellow(padNum5T(nTrans)) << " transactions "
        << greenCheck
        << " (nBlocks: " << asYellow(++emptyHits) << ")"
        << "         \r";
    }
    if (block.gasUsed > 0)
        cerr << "Block " << asYellow(blkNum) << " which is marked as empty, uses more than zero gas: " << block.gasUsed << "\n";
    cerr.flush();

    if (options->deepCheck) {
        string_q result;
        if (!verifyBlock(block, result)) {
            cout << "Difference:\n" << result << "\n";
            cout.flush();
        }
    }

    return true;
}

//-----------------------------------------------------------------------
bool verifyBlock(const CBlock& qBlock, string_q& result)
{
    string_q nStr; // the node's block
    queryRawBlock(nStr, uint_2_Str(qBlock.blockNumber), true, expContext().hashesOnly);
    nStr = cleanUpJson((char*)nStr.c_str());

    string_q qStr = substitute(qBlock.Format(), "blockNumber", "number"); // our block
    qStr = cleanUpJson((char*)qStr.c_str());
//#define DEBUG_VERIFY
#if DEBUG_VERIFY
    cout << string_q(80, '#') << "\nqb: ";
    cout << qStr << "\n";
    cout << string_q(80, '-') << "\nnd: ";
    cout << nStr << "\n\n";
#endif

    // We extract the field values from the node for those fields that we store in our cache. We then
    // use the string generated from our data to substitute in the node generated string with empty.
    // If the strings are identical, the data is identical. We return the differences if the check fails
    string_q nnStr = "{";
    string_q fields = "gasLimit|gasUsed|hash|logsBloom|number|parentHash|timestamp|transactions|"; // we only store these data fields
    while (!fields.empty())
    {
        string_q field = nextTokenClear(fields,'|');
        size_t f1 = nStr.find(field);
        string_q tail = extract(nStr, f1);
        size_t f2 = tail.find(",");
        if (field=="transactions")
            f2 = tail.find("],") + 1;
        string_q tField = substitute(extract(tail, 0, ((f2 == NOPOS) ? tail.length() : f2)), "0x" + string_q(512, '0'), "0x0");
        nnStr += (tField + ",");
#if DEBUG_VERIFY
        cout << field << " = " << tField << "\n";
#endif
    }
    nnStr += "}";
    replace(nnStr, ",}", "}");
#if DEBUG_VERIFY
    cout << nnStr << "\n\n";
#endif
    replace(nnStr, qStr, "");
    if (!nnStr.empty())
    {
        result = ("  qb: " + qStr + "\n  nd: " + nnStr + "\n");
        return false;
    }
    return true;
}



