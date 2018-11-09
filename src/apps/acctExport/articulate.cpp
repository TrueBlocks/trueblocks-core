/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "tokenlib.h"
#include "walletlib.h"
#include "options.h"

//----------------------------------------------------------------------------
inline unsigned char hex2Ascii(char *str) {
    unsigned char c;
    c =  (unsigned char)((str[0] >= 'A' ? ((str[0]&0xDF)-'A')+10 : (str[0]-'0')));
    c *= 16;
    c = (unsigned char)(c + (str[1] >= 'A' ? ((str[1]&0xDF)-'A')+10 : (str[1]-'0')));
    return c;
}

//----------------------------------------------------------------------------
inline string_q hex2String(const string_q& inHex) {
    string_q ret, in = startsWith(inHex, "0x") ? extract(inHex, 2) : inHex;
    while (!in.empty()) {
        string_q nibble = extract(in, 0, 2);
        in = extract(in, 2);
        ret += (char)hex2Ascii((char*)nibble.c_str());  // NOLINT
    }
    return ret;
}

//------------------------------------------------------------------------------
#define old_grabPart(a, b)       trimLeading(extract((a), 64*(b), 64), '0')
#define old_toBigNum2(a, b)      string_q(bnu_2_Str(str_2_Wei("0x" + old_grabPart(a, b))).c_str())
#define old_grabBigNum(a, b)     strtoull(old_grabPart(a, b).c_str(), NULL, 16)
#define old_toAddr(a, b)         "0x" + padLeft(old_grabPart(a, b), 40, '0')
#define old_toBigNum3(a, b)      padNum3(old_grabBigNum(a, b))
#define old_toBoolean(a, b)      (old_grabBigNum(a, b) ? "true" : "false")
#define old_toBytes(a, b)        extract((a), 64*(b), 64)
string_q parseTheInput2(const string_q& params, size_t nItems, string_q *types) {

    string_q ret;
    for (size_t item = 0 ; item < nItems ; item++) {
        string_q t = types[item];
        bool isDynamic = (t == "string" || t == "bytes" || contains(t, "[]"));

        string_q val;
             if ( t == "address"                    )   val =          old_toAddr     (params, item);  // NOLINT
        else if ( t == "bool"                       )   val =          old_toBoolean  (params, item);
        else if ( t == "uint3"                      )   val =          old_toBigNum3  (params, item);
        else if ( contains(t, "int") &&   !isDynamic)   val =          old_toBigNum2  (params, item);
        else if ( contains(t, "bytes") && !isDynamic)   val =          old_toBytes    (params, item);
        else if (!isDynamic                         )   val = "unknown type: " + t;
        else if ( isDynamic ) {
            size_t start = str_2_Uint(old_toBigNum2(params, item)) / (size_t)32;
            size_t len = old_grabBigNum(params, start);
            if (len == NOPOS)
                len = params.length()-start;
            if (t == "string") {
                string_q ss1 = extract(params, (start+1) * 64, len * 2);
                string_q ss2 = hex2String(ss1);
                ss2 = substitute(ss2, "\n", "\\n");
                ss2 = substitute(ss2, "\r", "");
                ss2 = substitute(ss2, "\"", "\\\"");
                val += ss2;
            } else {
                val = "0x" + extract(params, (start+1) * 64, len * 2);
            }
        }
        ret += ("|" + val);
    }

    return "\"" + trim(ret, '|') + "\"";
}

//---------------------------------------------------------------------------
string_q decodeRLP3(const string_q& name, const string_q& input, size_t nItems, string_q *items) {
    string_q quote = "\"";
    string_q params = input;
    string_q result = parseTheInput2(params, nItems, items);
    result = substitute(result, "|", "\", \"");
    return quote + name + quote + ", " + result;
}

//---------------------------------------------------------------------------
bool decodeRLP2(CFunction *func, const string_q& input) {
    string_q items[256];
    size_t nItems = 0;
    for (auto param : func->inputs)
        items[nItems++] = param.type;
    string_q decoded = substitute(decodeRLP3(func->name, input, nItems, items), "\"", "");
    nextTokenClear(decoded, ',');
    for (size_t i = 0 ; i < func->inputs.size(); i++)
        func->inputs[i].value = trim(nextTokenClear(decoded, ','));
    return true;
}

//-----------------------------------------------------------------------
bool articulateTransaction(const CAbi& abi, CTransaction *p) {

    if (p->func)
        return false;

    // articulate the events, so we can return with a fully articulated object
    for (size_t i = 0 ; i < p->receipt.logs.size() ; i++)
        articulateEvent(abi, &p->receipt.logs[i]);

    // articulate the traces, so we can return with a fully articulated object
    for (size_t i = 0 ; i < p->traces.size() ; i++)
        articulateTrace(abi, &p->traces[i]);

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);

        for (size_t i = 0 ; i < abi.abiByEncoding.size(); i ++) {
            CFunction *ff = (CFunction *)&abi.abiByEncoding[i];
            if (encoding % ff->encoding) {
                p->func = new CFunction(abi.abiByEncoding[i]);
                return decodeRLP2(p->func, params);
            }
        }
    }
    return articulateToken(p);
}

//-----------------------------------------------------------------------
bool articulateEvent(const CAbi& abi, CLogEntry *p) {

    if (p->func)
        return false;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);
        string_q params;
        bool first = true;
        for (auto t : p->topics) {
            if (!first)
                params += extract(topic_2_Str(t),2);
            first = false;
        }
        params += data;

        for (size_t i = 0 ; i < abi.abiByEncoding.size(); i ++) {
            CFunction *ff = (CFunction *)&abi.abiByEncoding[i];
            if (ff->encoding % topic_2_Str(p->topics[0])) {
                p->func = new CFunction(abi.abiByEncoding[i]);
                return decodeRLP2(p->func, params);
            }
        }
    }
    return articulateTokenEvent(p);
}

//-----------------------------------------------------------------------
bool articulateTrace(const CAbi& abi, CTrace *p) {

    if (p->func)
        return false;

    if (p && (p->action.input.length() >= 10 || p->action.input == "0x")) {
        string_q encoding = extract(p->action.input, 0, 10);
        string_q params   = extract(p->action.input, 10);

        for (size_t i = 0 ; i < abi.abiByEncoding.size(); i ++) {
            CFunction *ff = (CFunction *)&abi.abiByEncoding[i];
            if (encoding % ff->encoding) {
                p->func = new CFunction(abi.abiByEncoding[i]);
                return decodeRLP2(p->func, params);
            }
        }
    }

#if 1
    // Not really hacky, but sort of hacky way to articulate token and wallet traces. We pretend the trace is
    // a transaction and take advantage of existing code. All we need is the function pointer anyway.
    CTransaction dummy;
    dummy.to = p->action.to; dummy.from = p->action.to; dummy.input = p->action.input;
    dummy.value = p->action.value; dummy.gas = p->action.gas;
    bool ret = articulateToken(&dummy);
    p->func = dummy.func;
    dummy.func = NULL;
    return ret;
#else
    return false;
#endif
}
