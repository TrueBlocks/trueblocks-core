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
bool CVisitor::displayTransaction(uint32_t which, const CTransaction *theTrans, void *data) {

    ASSERT(!watches[which].disabled);
    nDisplayed++;
    lastWhich = which;
    lastTrans = theTrans;

    const CTransaction *promoted = promoteToFunc(theTrans);
    if (!promoted)
        promoted = theTrans;  // revert to the original

    SFString contractName = watches[which].name;
    SFString transType = (SFString(promoted->getRuntimeClass()->m_ClassName).substr(1));

    //----------------------------------
    bool wantsEvents = screenFmt.Contains("[{EVENTS}]");
    bool wantsParse = screenFmt.Contains("[{PARSE}]");
    SFString format  = screenFmt.Substitute("[{EVENTS}]","").Substitute("[{PARSE}]","");
    if (true) { //!format.empty()) {

        setColors(theTrans, watches[which].color);

        SFString func = promoted->function;
        SFString fmt = format.Substitute("[{FUNCTION}]", func).Substitute("[{FUNC}]", toProper(nextTokenClear(func,'|')));
        fmt.ReplaceAll("[{CONTRACT}]", contractName);
        fmt.ReplaceAll("[{CONTRACT3}]", contractName.Left(3));
        fmt.ReplaceAll("[{CONTRACT10}]", padRight(contractName.Left(10),10));
        fmt.ReplaceAll("[{TYPE}]", transType);
        fmt.ReplaceAll("[{TYPE20}]", padRight(transType.Left(15),15) + (theTrans->isInternalTx ? "" : "`"));
        fmt.Replace("[{P}]", (promoted == theTrans?"":"\t\tparsed: %[{w:130:PARSED}]#\r\n"));

        SFString head;
        head += ((promoted->isInternalTx ? promoted->to : contractName) + "::" +
                 transType + (promoted->isInternalTx ? " (logs from " + contractName + ")" : ""));
        fmt.Replace("[{HEAD}]", head);
        fmt.Replace("[{SEP}\r\n]", SFString('-', 180) + "\r\n");

        SFString transStr = promoted->Format(fmt).Substitute("\t"," ");

        SFString c1 = color, c2 = hiColor, c3 = hiColor2, c4 = cOff;
        if (theTrans->isError) {
            c1 = c2 = c3 = c4 = biBlack;
        } else if (theTrans->isInternalTx) {
            c1 = c2 = c3 = c4 = cRed;
        }
        cout << c1 << transStr.Substitute("#", c1).Substitute("@", c2).Substitute("%", c3).Substitute("`", c4) << cOff;
    }

    //----------------------------------
    if (parse_on || wantsParse) {
        SFString parsed = promoted->Format("\r\n[{PARSED}]\r\n")
        .Substitute(" ", "").Substitute(",", ", ").Substitute("{", "{ ").Substitute("}", " }");
        cout << iTeal << Strip(parsed, ',');
    }

    //----------------------------------
    SFString evtList;
    if (logs_on || wantsEvents) {
        if (logs_on)
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
            if (logs_on) {
                cout << iYellow << "  " << padLeft(asString(i),2) << ". " << padRight(eventType.Left(15),15) << " " << evtStr << cOff << "\r\n";
            }
            evtList += eventType + ",";

            // If it was promoted, delete it
            if (promotedLog != l)
                delete promotedLog;
        }
    }
    if (wantsEvents)
        cout << iYellow << "[" << Strip(evtList, ',') << "]";

    //----------------------------------
    if (notify) {

        // Display a Mac notification if we're on Mac
        SFString from = promoted->Format("[{FROM}]").Left(5)+"...";
        SFString to = promoted->Format("[{TO}]").Left(5)+"...";
        SFString cmd = "osascript -e 'display notification \"❤️" + transType + ": " +
                        from + "==>" + to + "\" with title \"" + contractName + "\"'";
        doCommand(cmd);
    }

    // TODO(tjrayrush): when should we show the accounting traces?
    if (trace_on || autoTrace) {
        showColoredTrace(promoted->hash, theTrans->isError);
        if (bloom_on && promoted->receipt.logsBloom != 0) {
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
SFString nameAccount(const SFString& address) {
    SFString name = address;
         if (address % "0xbb9bc244d798123fde783fcc1c72d3bb8c189413") name = "The DAO";
    else if (address % "0x807640a13483f8ac783c557fcdf27be11ea4ac7a") name = "Extra Balance";
    if (name.length() >= 42)
        return name;
    size_t need = 42 - name.length() - 6;  // '6' for ' (' and '...)'
    name = name + " (" + address.substr(0,need) + "...)";
    return name;
}

//-----------------------------------------------------------------------
void CVisitor::showColoredBloom(const SFBloom& bloom, const SFString& msg, const SFString& res) {
    SFString bl = fromBloom(bloom).substr(2);
    for (int i = 0 ; i < bl.length() ; i = i + 128) {
        SFString m = padLeft(" ",16);
        if (i == 0)
            m = "    " + msg + bBlack + " 0x";
        cout << m << bl.substr(i,128).Substitute("0",bBlack+"."+cOff) << (i == 0 ? " "+res : "") << "\r\n";
    }
}

//-----------------------------------------------------------------------
void CVisitor::showColoredTrace(const SFHash& hash, bool err) {

    CTraceArray traces;
    getTraces(traces, hash);

    for (int t = 0 ; t < traces.getCount() ; t++) {
        const CTrace *tt = &traces[t];
        if (t > 0) {
            CBlock unused;
            if (!err)
                accountForIntTransaction(unused, tt);
        }

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
            from.Replace(watches[i].address, c1 + nameAccount(watches[i].address) + c2);
              to.Replace(watches[i].address, c1 + nameAccount(watches[i].address) + c2);
        }

        {
            SFString exBal = "0x807640a13483f8ac783c557fcdf27be11ea4ac7a";
            SFString c1 = bBlue, c2 = cOff;
            if (err) { c1 = c2 = biBlack; }
            from.Replace(exBal, c1 + nameAccount(exBal) + c2);
            to.Replace(exBal, c1 + nameAccount(exBal) + c2);
        }

        SFString c1 = biBlack, c2 = cOff, c3 = bRed+italic;
        if (err) { c1 = c2 = c3 = biBlack; }
        if (from.length()) {
            cout << c1 <<  "\r\n    " << padNum4(t) << ":" << c2;
            cout << c1 <<  " \"from\": "         << c2 << from;
            cout << c1 << ", \"to\": "           << c2 << to;
            cout << c1 << ", \"subtraces\": "    << c2 << subtraces;
            cout << c1 << ", \"traceAddress\": " << c2 << traceAddress;
            cout << c1 << ", \"input\": "        << c2 << input;

            SFUintBN wei = canonicalWei(value);
            cout << c1 << ", \"value\": ";
            cout << (wei == 0 ? "" : c3);
            cout << wei2Ether(asStringBN(wei)) << c2;
            cout << c1 << " }" << cOff;
        }
    }
    cout << "\r\n";
}
