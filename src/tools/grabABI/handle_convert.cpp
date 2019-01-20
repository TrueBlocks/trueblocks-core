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
bool sol_2_Abi(CAbi& abi, const string_q& addr) {
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
        func.fromDefinition(line);
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
