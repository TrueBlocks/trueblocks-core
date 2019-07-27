/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include <vector>
#include "etherlib.h"
#include "options.h"

//------------------------------------------------------------
string_q diffStr(const string_q& str1, const string_q& str2) {
    string_q diff = str1;
    string_q junk = str2;
    while (!junk.empty()) {
        string_q line = nextTokenClear(junk, '\n');
        replace(diff, line, "\n");
    }
    string_q ret = trim(substitute(diff, "\n\n", "\n"), '\n');
    if (!ret.empty())
        ret += "\n";
    return ret;
}

//------------------------------------------------------------
string_q removeField(const string_q& strIn, const string_q& field) {

    string_q search = "\"" + field + "\":";
    if (!contains(strIn, search))
        return strIn;

    string_q ret;
    string_q str = strIn;
    // while (contains(str, search))
    {
        size_t start = str.find(search);
        string_q before = extract(str, 0, start);
        string_q rest   = extract(str, start);

        size_t end = rest.find(",") + 1;  // first comma
        if (startsWith(rest, search+"["))  // unless it's an array, then end of array
            end = rest.find("]") + 1;
        string_q during = extract(rest, 0, end);
        string_q after  = extract(rest, end);

        before = trim(before, '\n');
        during = trim(during, '\n');
        after  = trim(after, '\n');
        ret += (before + after);
        str = after;
    }

    return ret;
}

//------------------------------------------------------------
const string_q removes[] = {
    // fields in RPC but not in QBlocks
    "author", "condition", "creates", "extraData", "mixHash", "networkId", "chainId",
    "nonce", "nonce",  // NOT A DUP--IT NEEDS TO BE HERE BECAUSE THERE ARE TWO DIFFERENT NONCES
    "publicKey", "r", "raw", "receiptsRoot", "s", "sealFields", "sha3Uncles", "size", "standardV", "stateRoot",
    "totalDifficulty", "transactionsRoot", "uncles", "v", "logsBloom",
    // fields in QBlocks but not in RPC
    "contractAddress", "cumulativeGasUsed", "receipt", "address", "data", "logIndex", "topics",
    "price", "finalized", "isError", "age", "date",

    // WARNING: status must be last because we only use it after byzan
    "status",
    // WARNING: status must be last because we only use it after byzan
};
size_t nRemoved = sizeof(removes) / sizeof(string_q);

//------------------------------------------------------------
string_q hiddenFields(void) {
    string_q ret = "Hidden fields:\n";
    for (size_t i = 0 ; i < nRemoved ; i++) {
        ret += removes[i];
        if (i < nRemoved-1)
            ret += ", ";
    }
    return ret + "\n\n";
}

//------------------------------------------------------------
string_q cleanAll(const string_q& str, bool remove, bool isByzan) {

    if (!remove)
        nRemoved = 0;
    else if (isByzan)
        nRemoved--;  // don't remove status if were post-byzantium

    string_q orig = str;
    replaceAny(orig, "\t\r {}", "");
    replaceAll(orig, ",", ",\n");  // put everything on its own line
    for (size_t i = 0 ; i < nRemoved ; i++) {
        string_q search = "\"" + removes[i] + "\":";
        while (contains(orig, search)) {
            orig = removeField(orig, removes[i]);
        }
    }
    orig = substitute(orig, "}]", "");
    replaceAll(orig, ",", "");
    replaceAll(orig, "[", "");
    replaceAll(orig, "]", "");
    replaceAll(orig, "\"to\":null", "\"to\":\"0x0\"");
    orig = trimWhitespace(orig);
    orig = substitute(substitute(substitute(orig, "\"result\":", ""), "\"transactions\":", ""), "\"logs\":", "");
    orig = substitute(orig, "\"jsonrpc\":", "");
    orig = substitute(orig, "0x" + string_q(512, '0'), "0x0");  // minimize bloom filters
    replaceAll(orig, "\n\n", "\n");
    // get rid of id
    string_q ret;
    while (!orig.empty()) {
        string_q line = nextTokenClear(orig, '\n');
        if (!startsWith(line, "\"id\":"))
            ret += (line + "\n");
    }
    return trim(substitute(substitute(ret, "\"\"", "\"\n\""), "\n\n", "\n"), '\n');
}

//------------------------------------------------------------
string_q sorted(const string_q& inIn) {

    vector<string_q> inList, outList;

    string_q in = inIn;
    while (!in.empty()) {
        inList.push_back(nextTokenClear(in, '\n'));
    }

    sort(inList.begin(), inList.end());

    for (size_t i = 0 ; i < inList.size()-1 ; i++)
        if (inList[i] != inList[i+1] && contains(inList[i], ":"))
            outList.push_back(inList[i]);

    // add the last one of it's not already there
    if (inList[inList.size()-1] != outList[outList.size()-1])
        outList.push_back(inList[inList.size()-1]);

    string_q ret;
    for (size_t i = 0 ; i < outList.size() ; i++)
        ret += (outList[i] + "\n");
    return ret;
}

//------------------------------------------------------------
string_q normalizeBlock(const string_q& inIn, bool remove, bool isByzan) {
    return sorted(cleanAll(inIn, remove, isByzan));
}
