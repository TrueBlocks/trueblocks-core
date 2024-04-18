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
#include "basetypes.h"
#include "colors.h"

namespace qblocks {

string_q cBlack = "\e[0;30m";
string_q cRed = "\e[0;31m";
string_q cGreen = "\e[0;32m";
string_q cYellow = "\e[0;33m";
string_q cBlue = "\e[0;34m";
string_q cMagenta = "\e[0;35m";
string_q cTeal = "\e[0;36m";
string_q cWhite = "\e[0;37m";
string_q cOff = "\e[0m";

string_q bbold = "\e[1m";
string_q dim = "\e[2m";
string_q italic = "\e[3m";
string_q underline = "\e[4m";
string_q reversed = "\e[7m";
string_q strikethru = "\e[7m";
string_q greenCheck = "\e[0;32m✓\e[0m";
string_q yellowCaution = "\e[7m\e[0;33m!!\e[0m";
string_q redX = "\e[0;31mX\e[0m";
string_q whiteStar = "\e[0;37m✽\e[0m";

void colorsDim(void) {
    cWhite = (cWhite + dim + italic);
    cGreen = cYellow = cBlue = cMagenta = cTeal = cOff = cBlack = cWhite;
    bbold = italic = underline = reversed = strikethru = "";
    greenCheck = "✓";
    redX = "X";
    whiteStar = "*";
}

void colorsOff(void) {
    cBlack = cRed = cGreen = cYellow = cBlue = "";
    cMagenta = cTeal = cWhite = cOff = bbold = "";
    italic = underline = reversed = strikethru = "";
    greenCheck = "✓";
    redX = "X";
    whiteStar = "*";
}

void colorsOn(void) {
    cBlack = "\e[0;30m";
    cRed = "\e[0;31m";
    cGreen = "\e[0;32m";
    cYellow = "\e[0;33m";
    cBlue = "\e[0;34m";
    cMagenta = "\e[0;35m";
    cTeal = "\e[0;36m";
    cWhite = "\e[0;37m";
    cOff = "\e[0m";
    bbold = "\e[1m";
    italic = "\e[3m";
    underline = "\e[4m";
    reversed = "\e[7m";
    strikethru = "\e[7m";
    greenCheck = "\e[0;32m✓\e[0m";
    redX = "\e[0;31mX\e[0m";
    whiteStar = "\e[0;37m✽\e[0m";
}

bool colorsDisabled(void) {
    return cBlack == "";
}

}  // namespace qblocks
