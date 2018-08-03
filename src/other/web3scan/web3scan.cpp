// #define SHOW_ERRORS
// #define SHOW_CREATES
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "dailysummary.h"

//-------------------------------------------------------------------------
const char* STR_FORMAT =
"[{date}]\t"
"[{nBlocks}]\t"
"[{startBlock}]\t"
"[{endBlock}]\t"
"[{nTraces}]\t"
"[{nTxErrors}]\t[{nTcErrors}]\t[{nErrors}]\t"
"[{nTxCreates}]\t[{nTcCreates}]\t[{nTcFails}]\t[{nCreates}]\t"
"[{nCalls}]\t"
"[{nSelfDestruct}]";

//-------------------------------------------------------------------------
class CAccumulator {
public:
    COptions options;
    blknum_t totBlocks;
    bool foundError;
    bool foundCreate;
    CDailySummaryArray summary;
    CAccumulator(void) : totBlocks(0)
        { summary.reserve(2000); CDailySummary sum; sum.timestamp = str_2_Ts(earliestDate); summary.push_back(sum); }
    void noticeBlock(const CBlock& block) {
        totBlocks++;
        summary.back().nBlocks++;
        timestamp_t ts = str_2_Ts(BOD(ts_2_Date(block.timestamp)));
        if (summary.back().timestamp != ts) {
            summary.back().endBlock = block.blockNumber - 1;
            if (summary.size() > 1) {
                cout << string_q(120, ' ') << "\r";
                stringstream os;
                os << substitute(summary.back().Format(STR_FORMAT), "\n", " ") << "\n";
                cout << os.str();
                appendToAsciiFile("theData/web3scan.txt", os.str());
            }

            CDailySummary newDay;
            newDay.startBlock = block.blockNumber;
            newDay.timestamp = ts;
            summary.push_back(newDay);
        }
    }
    blknum_t bn;
    blknum_t tx;
    void noticeTransaction(const CTransaction& trans) {
        bn = trans.blockNumber;
        tx = trans.transactionIndex;
        if (trans.isError) {
#ifdef SHOW_ERRORS
            cerr << trans.Format("[{DATESH}]\t[{BLOCKNUMBER}].[{TRANSACTIONINDEX}]\t[{HASH}]") << "\n";
#endif
            foundError = true;
        }
        bool hasCreate = !isZeroAddr(trans.receipt.contractAddress);
        if (hasCreate) {
            foundCreate = true;
#ifdef SHOW_CREATES
            cerr << summary.back().Format("[{nCreates}]") << ".\t"
                    << trans.Format("[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t[{HASH}]\t[{ISERROR}]\ttop-level\t")
                   << trans.receipt.contractAddress << "\n";
#endif
        }
    }
    void noticeTrace(const CTrace& trace) {
        summary.back().nTraces++;
        bool isFirstTrace = trace.traceAddress.empty();
        string_q tAddr =
            substitute(
            substitute(
            substitute(trim(trace.Format("[{traceAddress}]"), '\"'),
            "\n", ""),
            "\"", "-"),
            "-,-", "-");
        if (trace.isError()) {
            summary.back().nTxErrors +=  isFirstTrace;
            summary.back().nTcErrors += !isFirstTrace;
            if (tAddr.empty())
                tAddr = "-";
#ifdef SHOW_ERRORS
            cerr << trace.Format(padLeft(tAddr, 15) +
                "\t[{BLOCKNUMBER}].[{transactionPosition}]\t[{type}]\t[{error}]") << "\n";
#endif
            foundError = true;
        }
        bool hasCreate = (trace.type == "create" ||
                          trace.action.callType == "create") && !isZeroAddr(trace.result.address);
        if (hasCreate) {
            summary.back().nTxCreates +=  isFirstTrace;
            summary.back().nTcCreates += !isFirstTrace;  // (!isFirstTrace && !isZeroAddr(trace.result.address));
//            summary.back().nTcFails   += (!isFirstTrace && isZeroAddr(trace.result.address));
            foundCreate = true;
#ifdef SHOW_CREATES
            cerr << "\t" << summary.back().Format("[{nCreates}]") << ".\t"
                    << bn << "\t" << tx << "\t" << trace.transactionHash << "\t"
                    << trace.action.callType << "-" << trace.type << "\t" << trace.result.address << "\n";
#endif
        } else if (trace.type == "create" || trace.action.callType == "create") {
            summary.back().nTcFails++;
            foundCreate = true;
#ifdef SHOW_CREATES
            cerr << "\t" << summary.back().Format("[{nCreates}]") << ".\t"
                    << bn << "\t" << tx << "\t" << trace.transactionHash << "\t"
                    << trace.action.callType << "-" << trace.type << "\t" << trace.result.address << "\n";
#endif
        }
#if 0
cerr << bn << "\t" << tx << "\t" << trace.transactionHash << "\t"
     << trace.isError() << "\t" "\t" << trace.action.callType << "\t"
     << trace.type << "\t" << trace.result.address << "\t" << tAddr << "\n";
#endif
        summary.back().nSelfDestruct += (trace.type == "suicide");
        summary.back().nCalls += (trace.type == "call");
    }
};

void loadData(CAccumulator *acc);
void saveData(CAccumulator *acc);
//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);
    CDailySummary::registerClass();

    CAccumulator accum;
    if (!accum.options.prepareArguments(argc, argv))
        return 0;

    string_q command = nextTokenClear(accum.options.commandList, '\n');
    if (!accum.options.parseArguments(command))
        return 0;

    loadData(&accum);
    forEveryBlockOnDisc(visitBlock, &accum, accum.options.firstBlock, accum.options.nBlocks);
    saveData(&accum);

    return 1;
}

CDailySummaryArray existing;
//-------------------------------------------------------------------------
void loadData(CAccumulator *acc) {

    string_q contents = asciiFileToString("theData/web3scan.txt");
    nextTokenClear(contents, '\n');  // skip headers
    while (!contents.empty()) {
        string_q line = nextTokenClear(contents, '\n');
        CDailySummary day(line);
        existing.push_back(day);
    }

    ostringstream os;
    os << substitute(substitute(toUpper(STR_FORMAT), "[{", ""), "}]", "") << "\n";
    cout << os.str();
    stringToAsciiFile("theData/web3scan.txt", os.str());
    for (auto elem : existing) {
        if (elem.startBlock >= acc->options.firstBlock)
            continue;
        stringstream os2;
        os2 << substitute(elem.Format(STR_FORMAT), "\n", " ") << "\n";
        cout << os2.str();
        appendToAsciiFile("theData/web3scan.txt", os2.str());
    }
}

//-------------------------------------------------------------------------
void saveData(CAccumulator *acc) {
    for (auto elem : existing) {
        if (elem.startBlock >= acc->options.firstBlock + acc->options.nBlocks - 1) {
            stringstream os2;
            os2 << substitute(elem.Format(STR_FORMAT), "\n", " ") << "\n";
            cout << os2.str();
            appendToAsciiFile("theData/web3scan.txt", os2.str());
        }
    }
}

//-------------------------------------------------------------------------
bool visitBlock(CBlock& block, void *data) {
    CAccumulator *acc = reinterpret_cast<CAccumulator*>(data);
    acc->noticeBlock(block);
    cout << ts_2_Date(block.timestamp) << ". " << acc->totBlocks << " of " << acc->options.nBlocks << "\r";
    forEveryTransactionInBlock(visitTransaction, data, block);
    cout.flush();
    return true;
}

//-------------------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    CAccumulator *acc = reinterpret_cast<CAccumulator*>(data);
    acc->foundError = false;
    acc->foundCreate = false;
    acc->noticeTransaction(trans);
    forEveryTraceInTransaction(visitTrace, data, trans);
    cout << "\t\t\t\t\t\t" << trans.blockNumber << "\t" << trans.transactionIndex << "     \r";
    cout.flush();
#ifdef SHOW_ERRORS
    if (acc->foundError)
        cerr << string_q(120, '-') << "\n";
#endif
#ifdef SHOW_CREATES
    if (acc->foundCreate)
        cerr << string_q(120, '-') << "\n";
#endif
    return true;
}

//-------------------------------------------------------------------------
bool visitTrace(CTrace& trace, void *data) {
    CAccumulator *acc = reinterpret_cast<CAccumulator*>(data);
    acc->noticeTrace(trace);
    return true;
}
