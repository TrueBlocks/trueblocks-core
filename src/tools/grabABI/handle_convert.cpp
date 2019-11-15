/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
extern void removeComments(string_q& code);
//-----------------------------------------------------------------------
bool sol_2_Abi(CAbi& abi, const string_q& addr) {
    string_q solFile = addr + ".sol";
    string_q contents = asciiFileToString(solFile);
    removeComments(contents);
    replaceAll(contents, "\r", "\n");
    replaceAll(contents, "interface ", "contract ");
    replaceAll(contents, "function ", "~function ");
    replaceAll(contents, "event ", "~event ");
    // remove new solidity keyword in function declarations
    replaceAll(contents, " memory ", " ");
    replaceAll(contents, " storage ", " ");
    replaceAll(contents, " calldata ", " ");
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

//-----------------------------------------------------------------------
void removeComments(string_q& code) {
    replaceAll(code, "`", ""); // easier
    replaceAll(code, "*/", "`"); // easier
    string_q ret;
    char endChar = '\0';
    typedef enum { OUT, START, STOP, IN } State;
    State state = OUT;
    for (auto ch : code) {
        switch (state) {
            case OUT:
                switch (ch) {
                    case '/':
                        state = START;
                        break;
                    default:
                        ret += ch;
                        break;
                }
                break;
            case START:
                switch (ch) {
                    case '*':
                        state = IN;
                        endChar = '`'; // easier
                        break;
                    case '/':
                        state = IN;
                        endChar = '\n';
                        break;
                    default:
                        ret += '/';
                        state = OUT;
                        break;
                }
                break;
            case IN:
                if (ch == endChar) {
                    state = OUT;
                }
                break;
            default:
                ASSERT(0);
                return;
        }
    }
    code = substitute(substitute(ret, "\t", " "), "  ", " ");
}

