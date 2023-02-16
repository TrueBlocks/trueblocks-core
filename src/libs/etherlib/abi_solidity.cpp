/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "abi.h"

namespace qblocks {

//--------------------------------------------------------------
static string_q findEndingParens(string_q& in) {
    string_q ret;
    int cnt = 0;
    for (auto ch : in) {
        switch (ch) {
            case ')':
                cnt--;
                if (cnt < 1) {
                    replace(in, "(" + ret + ")", "");
                    return trim(ret);
                }
                ret += ch;
                break;
            case '(':
                if (cnt)
                    ret += ch;
                cnt++;
                break;
            default:
                ret += ch;
                break;
        }
    }
    replace(in, "(" + ret + ")", "");
    return trim(ret);
}

//-----------------------------------------------------------------------
bool fromDefinition(CFunction& func, const string_q& lineIn) {
    string_q line = lineIn;
    line = substitute(line, ", ", ",");  // clean up
    line = substitute(line, "\t", "");   // clean up
    while (contains(line, " ["))
        replace(line, " [", "[");  // clean up
    while (contains(line, " ]"))
        replace(line, " ]", "]");  // clean up
    string_q outStr = substitute(substitute(substitute(nextTokenClear(line, '{'), "\n", ""), ";", ""), "returns", "|");
    replace(outStr, "function(", "function (");
    replace(outStr, "event(", "event (");

    func.type = trim(nextTokenClear(outStr, ' '));
    func.name = trim(nextTokenClear(outStr, '('));
    outStr = "(" + outStr;

    string_q inStr = trim(findEndingParens(outStr));
    string_q intern = trim(nextTokenClear(outStr, '|'));
    outStr = trim(outStr);

    func.anonymous = contains(intern, "anonymous");
    func.constant = contains(intern, "constant") || contains(intern, "view");
    // clang-format off
    func.stateMutability = (contains(substitute(intern, "nonpayable", ""), "payable") ? "payable"
                       : contains(intern, "view")                                ? "view"
                                                                                 : "");
    // clang-format on
    // internals:
    //  external
    //  internal
    //  override
    //  private
    //  public
    //  pure
    //  restricted
    //  virtual
    CStringArray ts = {"memory", "payable", "calldata"};
    for (auto t : ts) {
        inStr = substitute(inStr, t, "");
        outStr = substitute(outStr, t, "");
    }

    uint64_t cnt = 0;
    CStringArray inputArray;
    explode(inputArray, inStr, ',');
    for (auto input : inputArray) {
        CParameter param;
        fromDefinition(param, input);
        if (param.name.empty())
            param.name = "val_" + uint_2_Str(cnt++);
        func.inputs.push_back(param);
    }

    outStr = substitute(substitute(outStr, "(", ""), ")", "");
    cnt = 0;
    CStringArray outputArray;
    explode(outputArray, outStr, ',');
    for (auto output : outputArray) {
        CParameter param;
        fromDefinition(param, output);
        if (param.name.empty())
            param.name = "ret_" + uint_2_Str(cnt++);
        func.outputs.push_back(param);
    }

    func.finishParse();
    return true;
}

//-----------------------------------------------------------------------
static string_q elementaryName(const string_q& in) {
    // clang-format off
    if (startsWith(in, "int["))    return "int256" + in.substr(3);
    if (in == "int")               return "int256";
    if (startsWith(in, "uint["))   return "uint256" + in.substr(4);
    if (in == "uint")              return "uint256";
    if (startsWith(in, "fixed["))  return "fixed128x128" + in.substr(5);
    if (in == "fixed")             return "fixed128x128";
    if (startsWith(in, "ufixed[")) return "ufixed128x128" + in.substr(6);
    if (in == "ufixed")            return "ufixed128x128";
    // clang-format on
    return in;
}

//-----------------------------------------------------------------------
bool fromDefinition(CParameter& param, const string_q& strIn) {
    string_q str = strIn;
    param.indexed = contains(str, "indexed");
    str = trim(substitute(str, "indexed", ""));
    param.type = trim(elementaryName(nextTokenClear(str, ' ')));
    if (contains(param.type, '['))
        param.is_flags |= IS_ARRAY;
    param.name = trim(str);
    if (contains(param.type, "+")) {
        while (contains(param.type, "(")) {
            replaceAll(param.type, "(", "<tuple>");
            replaceAll(param.type, ")", "</tuple>");
            string_q tuple = snagFieldClear(param.type, "tuple");
            CStringArray parts;
            explode(parts, tuple, '+');
            size_t cnt = 1;
            for (auto part : parts) {
                CParameter p;
                p.type = nextTokenClear(part, ' ');
                p.name = (part.empty() ? ("val_" + uint_2_Str(cnt++)) : part);
                param.components.push_back(p);
            }
            param.type = "tuple" + param.type;
        }
    }
    return true;
}

}  // namespace qblocks
