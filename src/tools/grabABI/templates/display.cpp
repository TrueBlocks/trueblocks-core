/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "parselib.h"
#include "debug.h"

extern string_q reformat(const string_q& in, uint32_t len);
//-----------------------------------------------------------------------
void COptions::displayTrans(ostream& os, const CTransaction* theTrans) const {
    CTransaction* promoted = reinterpret_cast<CTransaction*>(promoteToFunc(theTrans));
    ASSERT(promoted);

    //----------------------------------
    timestamp_t ts = str_2_Ts(promoted->Format("[{TIMESTAMP}]"));
    string_q functionStr = promoted->Format("[{FUNCTION}]");

    string_q format =
        substitute(substitute(substitute(transFmt, "{", cWhite + "{"), "}", "}" + cYellow), "\r\n", "\r\n" + cYellow);
    bool wantsParse = contains(format, "{PARSE}");
    format = substitute(substitute(substitute(format, "{PARSE}", ""), "[{FUNCTION}]", functionStr), "[{FUNC}]",
                        toProper(nextTokenClear(functionStr, '|')));

    string_q fmt = format;
    if (expContext().asDollars) {
        replaceAll(fmt, "VALUE}", "VALUE}++USD_V++");
        replaceAll(fmt, "ETHER}", "ETHER}++USD_V++");
        replaceAll(fmt, "GASCOST}", "GASCOST}++USD_GC++");
    }

    if (json_on) {
        for (auto log : theTrans->receipt.logs) {
            CLogEntry* promotedLog = reinterpret_cast<CLogEntry*>(promoteToEvent(&log));
            promoted->receipt.events.push_back(promotedLog);
        }
    }

    string_q transStr = substitute(promoted->Format(fmt), "...", cYellow + "..." + cOff);

    if (contains(transStr, "++PRICE++"))
        transStr = substitute(transStr, "++PRICE++", displayDollars(ts, weiPerEther()));

    // promoted->receipt.clear();

    bool isJson = transFmt.empty();

    //----------------------------------
    string_q evtList;
    if (!logs_on && (contains(transStr, "++EVENT_LIST++") || isJson)) {
        for (uint32_t i = 0; i < theTrans->receipt.logs.size(); i++) {
            const CLogEntry* l = &theTrans->receipt.logs[i];
            const CLogEntry* promotedLog = promoteToEvent(l);
            string_q eventType = extract(string_q(promotedLog->getRuntimeClass()->m_ClassName), 1);
            evtList += "\"" + eventType + " (" + extract(promotedLog->address, 0, 5) + "...)\"";
            if (i < theTrans->receipt.logs.size() - 1)
                evtList += ", ";
            delete promotedLog;
        }
    }

    if (!evtList.empty()) {
        replaceAll(transStr, "++EVENT_LIST++", (cTeal + "[ " + evtList + " ]" + cOff));
    } else {
        replaceAll(transStr, "++EVENT_LIST++", "");
    }

    os << transStr;

    if (trace_on)
        displayTrace(os, theTrans);
    else if (logs_on)
        os << "\n";

    //----------------------------------
    if (logs_on) {
        for (uint32_t i = 0; i < theTrans->receipt.logs.size(); i++) {
            if (!isJson) {
                HIDE_FIELD(CLogEntry, "data");
                HIDE_FIELD(CLogEntry, "logIndex");
                HIDE_FIELD(CLogEntry, "topics");
                HIDE_FIELD(CLogEntry, "address");
            } else {
                UNHIDE_FIELD(CTransaction, "receipt");
            }

            //----------------------------------
            // Try to promote it. If we can't promote it, revert to the original.
            const CLogEntry* l = &theTrans->receipt.logs[i];
            const CLogEntry* promotedLog = promoteToEvent(l);
            // Display it.
            string_q eventType = extract(string_q(promotedLog->getRuntimeClass()->m_ClassName), 1);
            string_q evtStr =
                substitute(substitute(promotedLog->t oJson1(), "{", "{ \"type\": \"" + eventType + "\" "), "}", " }");
            if (!isJson) {
                evtStr = substitute(reformat(evtStr, 140), "...", cYellow + "..." + cOff);
                os << "\t" << cYellow << "log " << i << cOff << ":\t\t" << annotate(evtStr) << "\r\n";
            }

            delete promotedLog;
        }
    }

    //----------------------------------
    if (parse_on || wantsParse) {
        string_q parsed = substitute(
            substitute(substitute(substitute(promoted->Format("\r\n[{PARSED}]\r\n"), " ", ""), ",", ", "), "{", "{ "),
            "}", " }");
        os << iTeal << trim(parsed, ',');
    }

    // If the transaction was promoted, clear that up
    delete promoted;

    return;
}

//-----------------------------------------------------------------------
void COptions::displayTrace(ostream& os, const CTransaction* theTrans) const {
    const CTraceArray& traces = theTrans->traces;

    string_q fmt = traceFmt;
    if (fmt.empty())
        fmt = transFmt;
    fmt = substitute(substitute(fmt, "\r\n\t", "\r\n\t\t\t"), "{ARTICULATED}", "{w:100:ARTICULATED}...");
    for (uint32_t t = 0; t < traces.size(); t++) {
        const CTrace* trace = &traces[t];
        CTransaction trans1;
        trans1.blockNumber = theTrans->blockNumber;
        trans1.timestamp = (theTrans->pBlock ? theTrans->pBlock->timestamp : theTrans->timestamp);
        trans1.transactionIndex = theTrans->transactionIndex;
        trans1.gas = theTrans->gas;
        trans1.gasPrice = theTrans->gasPrice;
        trans1.to = trace->action.to;
        trans1.from = trace->action.from;
        trans1.input = trace->action.input;
        trans1.value = trace->action.value;
        trans1.receipt.contractAddress = trace->action.address;
        trans1.receipt.gasUsed = trace->result.gasUsed;
        //        trans1.function                = promoteToFunc(&trans1)->function;
        os << "\r\n\t" << cYellow << "trace " << t << ":\t" << cWhite << substitute(trans1.Format(fmt), " UTC", "");
    }
    os << "\r\n";
    return;
}

//-----------------------------------------------------------------------
string_q reformat(const string_q& in, uint32_t len) {
    string_q ret = in;
    if (ret.length() > len + 10) {
        string_q parts[1000];
        uint32_t nParts = 0;
        while (!ret.empty()) {
            parts[nParts++] = extract(ret, 0, len);
            replace(ret, parts[nParts - 1], "");
            if (parts[nParts - 1].length() == len) {
                parts[nParts - 1] += "...";
                parts[nParts - 1] += "\r\n\t\t\t    ";
            }
        }
        for (uint32_t xx = 0; xx < nParts; xx++)
            ret += parts[xx];
    }
    return ret;
}
