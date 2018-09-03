/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "processing.h"

extern string_q reformat(const string_q& in, uint32_t len);
//-----------------------------------------------------------------------
void COptions::displayTransaction(ostream& os, const CTransaction *theTrans) const {

    string_q functionStr = theTrans->Format("[{FUNCTION}]");

    string_q format = substitute(substitute(substitute(transFmt, "{", cWhite + "{"), "}", "}" + cYellow), "\r\n", "\r\n" + cYellow);
    format = substitute(substitute(format, "[{FUNCTION}]", functionStr), "[{FUNC}]", toProper(nextTokenClear(functionStr,'|')));

    string_q fmt = format;
    if (expContext().asDollars) {
        replaceAll(fmt, "VALUE}",    "VALUE}++USD_V++");
        replaceAll(fmt, "ETHER}",    "ETHER}++USD_V++");
        replaceAll(fmt, "GASCOST}",  "GASCOST}++USD_GC++");
    }

    string_q transStr = substitute(theTrans->Format(fmt), "...", cYellow + "..." + cOff);
    if (contains(transStr, "++PRICE++")) {
        timestamp_t ts = str_2_Ts(theTrans->Format("[{TIMESTAMP}]"));
        transStr = substitute(transStr, "++PRICE++", asDollars(ts, weiPerEther));
    }
    os << transStr;

    if (trace_on)
        displayTrace(os, theTrans);
    else if (logs_on)
        os << "\n";

    bool isJson = transFmt.empty();

    //----------------------------------
    if (logs_on) {
        for (uint32_t i = 0 ; i < theTrans->receipt.logs.size() ; i++) {

            if (!isJson) {
                HIDE_FIELD(CLogEntry,"data");
                HIDE_FIELD(CLogEntry,"logIndex");
                HIDE_FIELD(CLogEntry,"topics");
                HIDE_FIELD(CLogEntry,"address");
            } else {
                UNHIDE_FIELD(CTransaction,"receipt");
            }

            //----------------------------------
            const CLogEntry *l = &theTrans->receipt.logs[i];
            string_q eventType = extract(string_q(l->getRuntimeClass()->m_ClassName), 1);
            string_q evtStr = substitute(substitute(l->toJson1(), "{","{ \"type\": \"" + eventType + "\" "), "}", " }");
            if (!isJson) {
                evtStr = substitute(reformat(evtStr, 140), "...", cYellow + "..." + cOff);
                os << "\t" << cYellow << "log " << i << cOff << ":\t\t" << annotate(evtStr) << "\r\n";;
            }
        }
    }

    return;
}

//-----------------------------------------------------------------------
void COptions::displayTrace(ostream& os, const CTransaction *theTrans) const {

    const CTraceArray& traces = theTrans->traces;
//    bool err = theTrans->isError;

    string_q fmt = traceFmt;
    if (fmt.empty())
        fmt = transFmt;
    fmt = substitute(substitute(fmt, "\r\n\t","\r\n\t\t\t"), "{ARTICULATEDTX}","{w:100:ARTICULATEDTX}...");
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
        os << "\r\n\t" << cYellow << "trace " << t << ":\t" << cWhite << substitute(trans1.Format(fmt), " UTC", "");
    }
    os << "\r\n";
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
                parts[nParts-1] += "\r\n\t\t\t    ";
            }
        }
        for (uint32_t xx = 0 ; xx < nParts ; xx++)
            ret += parts[xx];
    }
    return ret;
}

