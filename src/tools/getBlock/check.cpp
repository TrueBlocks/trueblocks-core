/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//------------------------------------------------------------
SFString diffStr(const SFString& str1, const SFString& str2) {
    SFString diff = str1;
    SFString junk = str2;
    while (!junk.empty()) {
        SFString line = nextTokenClear(junk,'\n');
        diff.Replace(line,"\n");
    }
    SFString ret = Strip(diff.Substitute("\n\n","\n"), '\n');
    if (!ret.empty())
        ret += "\n";
    return ret;
}

//------------------------------------------------------------
SFString removeField(const SFString& strIn, const SFString& field) {

    SFString search = "\"" + field + "\":";
    if (!strIn.Contains(search))
        return strIn;

    SFString ret;
    SFString str = strIn;
    //while (str.Contains(search))
    {
        size_t start = str.find(search);
        SFString before = str.substr(0,start);
        SFString rest   = str.substr(start);

        size_t end = rest.find(",")+1; // first comma
        if (rest.startsWith(search+"[")) // unless it's an array, then end of array
            end = rest.find("]")+1;
        SFString during = rest.substr(0,end);
        SFString after  = rest.substr(end);

        before = Strip(before, '\n');
        during = Strip(during, '\n');
        after  = Strip(after,  '\n');
        ret += (before + after);
        str = after;
    }

    return ret;
}

//------------------------------------------------------------
SFString cleanAll(const SFString& str, bool remove, bool isByzan) {

    static const SFString removes[] = {
        // fields in RPC but not in QuickBlocks
        "author", "condition", "creates", "difficulty", "extraData", "miner", "mixHash", "networkId", "chainId",
        "nonce", "nonce", // NOT A DUP--IT NEEDS TO BE HERE BECAUSE THERE ARE TWO DIFFERENT NONCES
        "publicKey", "r", "raw", "receiptsRoot", "s", "sealFields", "sha3Uncles", "size", "standardV", "stateRoot",
        "totalDifficulty", "transactionsRoot", "uncles", "v", "logsBloom",
        // fields in QuickBlocks but not in RPC
        "contractAddress", "cumulativeGasUsed", "receipt", "address", "data", "logIndex", "topics", "status", // status must be last because we only use it after byzan
    };
    uint32_t nRemoved = sizeof(removes) / sizeof(SFString);
    if (!remove)
        nRemoved = 0;
    else if (isByzan)
        nRemoved--; // don't remove status if were post-byzantium

    SFString orig = str;
    orig.ReplaceAny("\t\r {}","");
    orig.ReplaceAll(",",",\n"); // put everything on its own line
    for (uint32_t i = 0 ; i < nRemoved ; i++) {
        SFString search = "\"" + removes[i] + "\":";
        while (orig.Contains(search)) {
            orig = removeField(orig, removes[i]);
        }
    }
    orig = orig.Substitute("}]","");
    orig.ReplaceAll(",", "");
    orig.ReplaceAll("[", "");
    orig.ReplaceAll("]", "");
    orig.ReplaceAll("\"to\":null","\"to\":\"0x0\"");
    orig = StripAny(orig, "\t\n ");
    orig = orig.Substitute("\"result\":","").Substitute("\"transactions\":","").Substitute("\"logs\":","");
    orig = orig.Substitute("\"jsonrpc\":","");
    orig = orig.Substitute("0x" + SFString('0',512), "0x0"); // minimize bloom filters
    orig.ReplaceAll("\n\n","\n");
    // get rid of id
    SFString ret;
    while (!orig.empty()) {
        SFString line = nextTokenClear(orig, '\n');
        if (!line.startsWith("\"id\":"))
            ret += (line + "\n");
    }
    return Strip(ret.Substitute("\"\"","\"\n\"").Substitute("\n\n","\n"),'\n');
}

//------------------------------------------------------------
SFString sorted(const SFString& inIn) {
    SFStringArray inList, outList;
    SFString in = inIn;
    while (!in.empty()) {
        inList[inList.getCount()] = nextTokenClear(in,'\n');
    }
    inList.Sort(sortByStringValue);
    for (uint32_t i = 0 ; i < inList.getCount()-1 ; i++) {
        if (inList[i] != inList[i+1] && inList[i].Contains(":"))
            outList[outList.getCount()] = inList[i];
    }
    // add the last one of it's not already there
    if (inList[inList.getCount()-1] != outList[outList.getCount()-1])
        outList[outList.getCount()] = inList[inList.getCount()-1];

    SFString ret;
    for (uint32_t i = 0 ; i < outList.getCount() ; i++)
        ret += (outList[i] + "\n");
    return ret;
}

//------------------------------------------------------------
SFString normalizeBlock(const SFString& inIn, bool remove, bool isByzan) {
    return sorted(cleanAll(inIn, remove, isByzan));
}
