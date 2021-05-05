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

//----------------------------------------------------------------
static const CStringArray removes = {"internal", "virtual", "memory", "private", "external", "pure", "calldata"};

//----------------------------------------------------------------
static bool ofInterest(const string_q& line) {
    return (contains(line, "contract") || contains(line, "interface") || contains(line, "library") ||
            contains(line, "struct") || contains(line, "function") || contains(line, "event"));
}

//----------------------------------------------------------------
static string_q removeComments(const string_q& contents) {
    ostringstream os;
    ParseState state = OUT;
    char lastChar = 0;
    for (auto ch : contents) {
        switch (state) {
            case IN_COMMENT1:
                if (ch == COMMENT_END1) {
                    if (!isspace(lastChar))
                        os << ch;
                    state = OUT;
                } else {
                    // do nothing
                }
                break;
            case IN_COMMENT2:
                if (ch == COMMENT_END2) {
                    state = OUT;
                } else {
                    // do nothing
                }
                break;
            case OUT:
                if (ch == COMMENT1) {
                    state = IN_COMMENT1;
                } else if (ch == COMMENT2) {
                    state = IN_COMMENT2;
                } else {
                    if (!isspace(ch) || !isspace(lastChar))
                        os << ch;
                }
                break;
            default:
                break;
        }
        lastChar = ch;
    }

    return os.str();
}

//----------------------------------------------------------------
bool CAbi::loadAbiFromSolidity(const string_q& addr) {
    string_q solFile = addr + ".sol";
    string_q contents = asciiFileToString(solFile);

    replaceAll(contents, "/*", string_q(1, COMMENT2));
    replaceAll(contents, "//", string_q(1, COMMENT1));
    replaceAll(contents, "*/", string_q(1, COMMENT_END2));
    replaceAll(contents, "pragma", string_q(1, COMMENT1));  // simply remove pragmas
    replaceAll(contents, "constructor", "modifier");        // treat constructors like modifiers
    replaceAll(contents, "function", string_q(1, FUNCTION_START));
    replaceAll(contents, "struct", string_q(1, STRUCT_START));
    replaceAll(contents, "event", string_q(1, EVENT_START));
    replaceAll(contents, "modifier", string_q(1, MODIFIER_START));
    for (auto rem : removes)
        replaceAll(contents, rem, "");
    contents = removeComments(contents);

    CStringArray psStrs = {"OUT",         "IN",       "IN_COMMENT1", "IN_COMMENT2",
                           "IN_FUNCTION", "IN_EVENT", "IN_STRUCT",   "IN_MODIFIER"};
    ostringstream os;
    ParseState state = OUT;
    // ParseState pre_state = state;
    char lastChar = 0;
    size_t scopeCount = 0;
    for (auto ch : contents) {
        // pre_state = state;
        switch (state) {
            case IN_MODIFIER:
                if (ch == '{') {
                    scopeCount++;
                } else if (ch == '}') {
                    scopeCount--;
                    if (scopeCount == 0) {
                        state = OUT;
                    }
                }
                break;
            case IN_EVENT:
                if (ch == '\n') {
                    // do nothing
                } else if (ch == ';') {
                    if (!isspace(ch) || !isspace(lastChar))
                        os << ch;
                    state = OUT;
                } else {
                    if (!isspace(ch) || !isspace(lastChar))
                        os << ch;
                }
                break;
            case IN_STRUCT:
                if (ch == '\n') {
                    // do nothing
                } else if (ch == '{') {
                    os << ch;
                    scopeCount++;
                } else if (ch == '}') {
                    os << ch;
                    scopeCount--;
                    if (scopeCount == 0) {
                        state = OUT;
                    }
                } else {
                    if (!isspace(ch) || !isspace(lastChar))
                        os << ch;
                }
                break;
            case IN_FUNCTION:
                if (ch == '\n') {
                    // do nothing
                } else if (ch == ';') {
                    if (scopeCount == 0) {
                        os << ch;
                        state = OUT;
                    }
                } else if (ch == '{') {
                    if (scopeCount == 0)
                        os << ch;
                    scopeCount++;
                } else if (ch == '}') {
                    scopeCount--;
                    if (scopeCount == 0) {
                        os << ch;
                        state = OUT;
                    }
                } else {
                    if (scopeCount == 0 && (!isspace(ch) || !isspace(lastChar)))
                        os << ch;
                }
                break;
            case OUT:
                if (ch == EVENT_START) {
                    os << "event";
                    state = IN_EVENT;
                } else if (ch == STRUCT_START) {
                    os << "struct";
                    scopeCount = 0;
                    state = IN_STRUCT;
                } else if (ch == FUNCTION_START) {
                    os << "function";
                    scopeCount = 0;
                    state = IN_FUNCTION;
                } else if (ch == MODIFIER_START) {
                    scopeCount = 0;
                    state = IN_MODIFIER;
                } else {
                    if (!isspace(ch) || !isspace(lastChar))
                        os << ch;
                }
                break;
            default:
                break;
        }
        // if (state != pre_state)
        //     printf("");
        lastChar = ch;
    }

    contents = os.str();
    replaceAll(contents, "\n\n", "\n");
    replaceAll(contents, "\r", "");
    replaceAll(contents, " )", ")");

    CStringArray lines;
    explode(lines, contents, '\n');
    for (auto line : lines) {
        if (ofInterest(line)) {
            if (isTestMode())
                cerr << line << endl;
            if (contains(line, "function ") || contains(line, "event ")) {
                CFunction func;
                fromDefinition(func, line);
                loadAbiAddInterface(func);
            }
        }
    }
    sort(interfaces.begin(), interfaces.end(), sortByFuncName);
    return true;
}

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
