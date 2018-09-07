/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

extern string_q reformat(const string_q& in, uint32_t len);
//-----------------------------------------------------------------------
void COptions::displayTransaction(ostream& os, const CTransaction *theTrans) const {

    string_q functionStr = theTrans->Format("[{FUNCTION}]");

    string_q format = substitute(substitute(substitute(fmtStr, "{", "{"), "}", "}"), "\n", "\n");
    format = substitute(substitute(format, "[{FUNCTION}]", functionStr), "[{FUNC}]", toProper(nextTokenClear(functionStr,'|')));

    string_q fmt = format;
    if (expContext().asDollars) {
        replaceAll(fmt, "VALUE}",    "VALUE}++USD_V++");
        replaceAll(fmt, "ETHER}",    "ETHER}++USD_V++");
        replaceAll(fmt, "GASCOST}",  "GASCOST}++USD_GC++");
    }

    string_q transStr = substitute(theTrans->Format(fmt), "...", "...");
    if (contains(transStr, "++PRICE++")) {
        timestamp_t ts = str_2_Ts(theTrans->Format("[{TIMESTAMP}]"));
        transStr = substitute(transStr, "++PRICE++", asDollars(ts, weiPerEther));
    }
    os << transStr;

    return;
}

//-----------------------------------------------------------------------
void COptions::displayTrace(ostream& os, const CTransaction *theTrans) const {

    const CTraceArray& traces = theTrans->traces;
//    bool err = theTrans->isError;

    string_q fmt = fmtStr;
    if (fmt.empty())
        fmt = fmtStr;
    fmt = substitute(substitute(fmt, "\n\t","\n\t\t\t"), "{ARTICULATEDTX}","{w:100:ARTICULATEDTX}...");
    for (uint32_t t = 0 ; t < traces.size() ; t++) {
        const CTrace *trace = &traces[t];
        CTransaction trans1;
        trans1.blockNumber             = theTrans->blockNumber;
        trans1.timestamp               = (theTrans->pBlock ? theTrans->pBlock->timestamp : theTrans->timestamp);
        trans1.transactionIndex        = theTrans->transactionIndex;
        trans1.gas                     = theTrans->gas;
        trans1.gasPrice                = theTrans->gasPrice;
        trans1.to                      = trace->action.to;
        trans1.from                    = trace->action.from;
        trans1.input                   = trace->action.input;
        trans1.value                   = trace->action.value;
        trans1.receipt.contractAddress = trace->action.address;
        trans1.receipt.gasUsed         = trace->result.gasUsed;
//        trans1.function                = articulateTransaction(&trans1)->function;
        os << "\n\t" << "trace " << t << ":\t" << substitute(trans1.Format(fmt), " UTC", "");
    }
    os << "\n";
    return;
}

//-----------------------------------------------------------------------
string_q reformat(const string_q& in, uint32_t len) {
    string_q ret = in;
    if (ret.length() > len+10) {
        string_q parts[1000];
        uint32_t nParts = 0;
        while (!ret.empty()) {
            parts[nParts++] = extract(ret, 0, len);
            replace(ret, parts[nParts-1], "");
            if (parts[nParts-1].length()==len) {
                parts[nParts-1] += "...";
                parts[nParts-1] += "\n\t\t\t    ";
            }
        }
        for (uint32_t xx = 0 ; xx < nParts ; xx++)
            ret += parts[xx];
    }
    return ret;
}

