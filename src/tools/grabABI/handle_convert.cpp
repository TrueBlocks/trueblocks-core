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
#include "options.h"

//TODO(tjayrush): This is terrible code
extern void snagSignatures(string_q& str);
//-----------------------------------------------------------------------
bool convertSolToABI(CAbi& abi, const string_q& addr) {
    string_q solFile = addr + ".sol";
    if (!fileExists(solFile))
        return false;
    string_q contents;
    asciiFileToString(solFile, contents);
    replaceAll(contents, "\r", "\n");
    replaceAll(contents, "function ", "~function ");
    replaceAll(contents, "event ", "~event ");
    replaceAll(contents, "{", ";");
    cleanString(contents, true);
    snagSignatures(contents);
    CStringArray lines;
    explode(lines, contents, '\n');
    for (auto line : lines) {
        replaceAll(line, "  ", " ");

        CFunction func;
        replaceAll(line, "(", "|");
        replaceAll(line, ")", "|");

        func.constant = (contains(line, "constant") || contains(line, "view"));
        func.type = trim(nextTokenClear(line, ' '));
        func.name = trim(nextTokenClear(line, '|'));

        string_q inputs = trim(substitute(substitute(nextTokenClear(line, '|'), ", ", ","), "\t", ""));
        CStringArray ins;
        explode(ins, inputs, ',');
        uint64_t cnt = 0;
        for (auto input : ins) {
            CParameter param;
            param.indexed = contains(input, "indexed"); replace(input, "indexed ", "");
            param.type = trim(nextTokenClear(input, ' '));
            if (param.type == "uint")
                param.type = "uint256";
            if (param.type == "uint[]")
                param.type = "uint256[]";
            param.name = trim(nextTokenClear(input, ' '));
            if (param.name.empty())
                param.name = "val_" + uint_2_Str(cnt++);
            func.inputs.push_back(param);
        }
        CStringArray parts;
        explode(parts, line, '|');
        cnt = 0;
        if (parts.size()>1 && contains(parts[0], "returns")) {
            string_q outputs = trim(substitute(substitute(nextTokenClear(parts[1], '|'), ", ", ","), "\t", ""));
            CStringArray rets;
            explode(rets, outputs, ',');
            for (auto ret : rets) {
                CParameter param;
                param.type = trim(nextTokenClear(ret, ' '));
                if (param.type == "uint")
                    param.type = "uint256";
                if (param.type == "uint[]")
                    param.type = "uint256[]";
                param.name = trim(nextTokenClear(ret, ' '));
                if (param.name.empty())
                    param.name = "ret_" + uint_2_Str(cnt++);
                func.outputs.push_back(param);
            }
        }
        abi.interfaces.push_back(func);
    }
    return true;
}

//-----------------------------------------------------------------------
void snagSignatures(string_q& str) {
    size_t pos = 0;
    typedef enum { OUT, IN } StateThing;
    StateThing state = OUT;
    for (auto ch : str) {
        switch (state) {
            case OUT:
                if (ch == '~')
                    state = IN;
                break;
            case IN:
                str[pos++] = ch;
                if (ch == ';') {
                    str[pos++] = '\n';
                    state = OUT;
                }
                break;
        }
    }
    str[pos] = '\0';
    str.resize(pos);
}
