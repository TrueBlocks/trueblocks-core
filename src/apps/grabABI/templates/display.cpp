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
#include "parselib.h"
#include "debug.h"

//-----------------------------------------------------------------------
bool CVisitor::displayTransaction(const CTransaction *theTrans) const {

    const CTransaction *promoted = promoteToFunc(theTrans);
    if (!promoted)
        promoted = theTrans;  // revert to the original

    //----------------------------------
    SFString theFmt = screenFmt; //(verbose ? verboseFmt : screenFmt);
    bool wantsEvents = theFmt.Contains("[{EVENTS}]");
    bool wantsParse  = theFmt.Contains("[{PARSE}]");
    SFString format  = theFmt.Substitute("[{EVENTS}]","").Substitute("[{PARSE}]","");
    if (true) { //!format.empty()) {

        SFString func = promoted->function;
        SFString fmt  = format.Substitute("[{FUNCTION}]", func).Substitute("[{FUNC}]", toProper(nextTokenClear(func,'|')));
        if (expContext().asDollars) {
            fmt.ReplaceAll("VALUE}",    "VALUE}++USDV++");
            fmt.ReplaceAll("ETHER}",    "ETHER}++USDV++");
            fmt.ReplaceAll("GASPRICE}", "GASPRICE}++USDGP++");
            fmt.ReplaceAll("GASCOST}",  "GASCOST}++USDGC++");
        }
        fmt.Replace("[{P}]", (promoted == theTrans?"":"\t\tparsed: %[{w:130:PARSED}]#\r\n"));

        SFString head;
        fmt.Replace("[{HEAD}]", head);
        fmt.Replace("[{SEP}\r\n]", SFString('-', 180) + "\r\n");

        SFString transStr = promoted->Format(fmt);

        if (expContext().asDollars) {
            timestamp_t ts = toUnsigned(promoted->Format("[{TIMESTAMP}]"));
            transStr.ReplaceAll("++USDV++",  asDollars(ts, toWei(promoted->Format("[{VALUE}]"))));
            transStr.ReplaceAll("++USDGP++", asDollars(ts, toWei(promoted->Format("[{GASPRICE}]"))));
            transStr.ReplaceAll("++USDGC++", asDollars(ts, toWei(promoted->Format("[{GASCOST}]"))));
        }
        transStr = annotate(transStr);
        cout << cOff << transStr;
    }

    //----------------------------------
    if (opts.parse_on || wantsParse) {
        SFString parsed = promoted->Format("\r\n[{PARSED}]\r\n")
                .Substitute(" ", "").Substitute(",", ", ").Substitute("{", "{ ").Substitute("}", " }");
        cout << iTeal << Strip(parsed, ',');
    }

    //----------------------------------
    SFString evtList;
    if (opts.logs_on || wantsEvents) {
        if (opts.logs_on)
            cout << "\r\n";
        for (int i = 0 ; i < theTrans->receipt.logs.getCount() ; i++) {

            // Try to promote it. If we can't promote it, revert to the original.
            const CLogEntry *l = &theTrans->receipt.logs[i];
            const CLogEntry *promotedLog = promoteToEvent(l);
            if (!promotedLog)
                promotedLog = l;

            // Display it.
            SFString eventType = (SFString(promotedLog->getRuntimeClass()->m_ClassName).substr(1));
            SFString evtStr = promotedLog->toJson1();
            if (opts.logs_on) {
                cout << iYellow << "  "
                        << padLeft(asString(i),2) << ". "
                        << padRight(eventType.Left(15),15) << " "
                        << evtStr << cOff << "\r\n";
            }
            evtList += eventType + ",";

            // If it was promoted, delete it
            if (promotedLog != l)
                delete promotedLog;
        }
    }
    if (wantsEvents)
        cout << iYellow << "[" << Strip(evtList, ',') << "]";

    if (opts.trace_on) {
        timestamp_t ts = toUnsigned(theTrans->Format("[{TIMESTAMP}]"));
        showColoredTrace(ts, theTrans->traces, theTrans->isError);
        if (opts.bloom_on && promoted->receipt.logsBloom != 0) {
            showColoredBloom(promoted->receipt.logsBloom, "Tx bloom:", "");
            cout << "\r\n";
            for (int t=0;t<watches.getCount()-1;t++) {
                SFBloom b = makeBloom(watches[t].address);
                showColoredBloom(b,watches[t].color + padRight(watches[t].name.Left(9),9) + cOff, (isBloomHit(b, promoted->receipt.logsBloom) ? greenCheck : redX));
                cout << "\r\n";
            }
        }
    }

    // If the transaction was promoted, clear that up
    if (theTrans != promoted)
        delete promoted;

    cout << cOff;
    cout << "\r\n";
    cout.flush();
    return true;
}

//-----------------------------------------------------------------------
void CVisitor::showColoredBloom(const SFBloom& bloom, const SFString& msg, const SFString& res) const {
    SFString bl = fromBloom(bloom).substr(2);
    for (int i = 0 ; i < bl.length() ; i = i + 128) {
        SFString m = padLeft(" ",16);
        if (i == 0)
            m = "    " + msg + bBlack + " 0x";
        cout << m << bl.substr(i,128).Substitute("0",bBlack+"."+cOff) << (i == 0 ? " "+res : "") << "\r\n";
    }
}

//-----------------------------------------------------------------------
void CVisitor::showColoredTrace(timestamp_t ts, const CTraceArray& traces, bool err) const {

    for (int t = 0 ; t < traces.getCount() ; t++) {
        const CTrace *tt = &traces[t];

        SFString type = tt->Format("[{ACTION::CALLTYPE}]");
        SFString from = tt->Format("[{ACTION::FROM}]");
        SFString to = tt->Format("[{ACTION::TO}]");
        SFString value = tt->Format("[{ACTION::VALUE}]");
        SFString subtraces = tt->Format("[{SUBTRACES}]");
        SFString traceAddress = tt->Format("[{TRACEADDRESS}]");
        SFString input = tt->Format("[{ACTION::INPUT}]");
        if (traceAddress.empty())
            traceAddress = "[ ]";

        for (int i = 0 ; i < watches.getCount()-1 ; i++) {
            SFString c1 = watches[i].color, c2 = cOff;
            if (err) { c1 = c2 = biBlack; }
            from.Replace(watches[i].address, c1 + watches[i].address + c2);
              to.Replace(watches[i].address, c1 + watches[i].address + c2);
        }

        SFString c1 = biBlack, c2 = cOff, c3 = bRed;
        if (err) { c1 = c2 = c3 = biBlack; }
        if (from.length()) {
            cout << c1 <<  "\r\n    " << padNum4(t) << ":" << c2;
            // TODO(tjayrush) use formatting string here
            cout << c1 << " { \"type\": "           << c2 << type;
            cout << c1 <<  " \"from\": "         << c2 << annotate(from);
            cout << c1 << ", \"to\": "           << c2 << annotate(to);
//            cout << c1 << ", \"subtraces\": "    << c2 << subtraces;
//            cout << c1 << ", \"traceAddress\": " << c2 << traceAddress;
//            cout << c1 << ", \"input\": "        << c2 << input;

            SFUintBN wei = canonicalWei(value);
            cout << c1 << ", \"value\": ";
            cout << (wei == 0 ? cOff : c3);
            cout << wei2Ether(asStringBN(wei)) << (expContext().asDollars ? asDollars(ts, wei) : "") << c2;
            cout << c1 << " }" << cOff;
        }
    }
    cout << "\r\n";
}

//-----------------------------------------------------------------------
SFString CVisitor::annotate(const SFString& strIn) const {
    SFString ret = strIn;
    for (int i=0;i<watches.getCount();i++) {
        ret = ret.Substitute(watches[i].address, watches[i].displayName(true,8));
    }
    for (int i=0;i<named.getCount();i++) {
        ret = ret.Substitute(named[i].address, named[i].displayName(true,8));
    }
    return ret;
}
