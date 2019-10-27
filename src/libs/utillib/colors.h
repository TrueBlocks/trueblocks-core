#pragma once
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

namespace qblocks {

    // regular colors
    extern string_q cBlack;
    extern string_q cRed;
    extern string_q cGreen;
    extern string_q cYellow;
    extern string_q cBlue;
    extern string_q cMagenta;
    extern string_q cTeal;
    extern string_q cWhite;
    extern string_q cOff;

    extern string_q bbold;
    extern string_q italic;
    extern string_q underline;
    extern string_q reversed;
    extern string_q strikethru;
    extern string_q greenCheck;
    extern string_q yellowCaution;
    extern string_q redX;
    extern string_q whiteStar;

    // bold italic colors
    #define biBlack  (cBlack+bbold+italic)

    // bold colors
    #define bBlack   (cBlack+bbold)
    #define bRed     (cRed+bbold)
    #define bGreen   (cGreen+bbold)
    #define bYellow  (cYellow+bbold)
    #define bBlue    (cBlue+bbold)
    #define bMagenta (cMagenta+bbold)
    #define bTeal    (cTeal+bbold)
    #define bWhite   (cWhite+bbold)

    // italic colors
    #define iBlack   (cBlack+italic)
    #define iRed     (cRed+italic)
    #define iGreen   (cGreen+italic)
    #define iYellow  (cYellow+italic)
    #define iBlue    (cBlue+italic)
    #define iMagenta (cMagenta+italic)
    #define iTeal    (cTeal+italic)
    #define iWhite   (cWhite+italic)

    #define asYellow(a) cYellow << a << cOff

    extern void colorsOff(void);
    extern void colorsOn(void);
    extern void colorsDim(void);
    extern bool colorsDisabled(void);

    //-----------------------------------------------------------------------
    inline string_q convertColor(const string_q& colorIn) {
        bool bold = contains(colorIn, "_b");
        bool ital = contains(colorIn, "_i");
        string_q color = colorIn;
        color = nextTokenClear(color, '_');
        if (color == "black"  ) return (bold ? bBlack   : ital ? iBlack   : cBlack   );
        else if (color == "blue"   ) return (bold ? bBlue    : ital ? iBlue    : cBlue    );
        else if (color == "red"    ) return (bold ? bRed     : ital ? iRed     : cRed     );
        else if (color == "green"  ) return (bold ? bGreen   : ital ? iGreen   : cGreen   );
        else if (color == "yellow" ) return (bold ? bYellow  : ital ? iYellow  : cYellow  );
        else if (color == "magenta") return (bold ? bMagenta : ital ? iMagenta : cMagenta );
        else if (color == "teal"   ) return (bold ? bTeal    : ital ? iTeal    : cTeal    );
        else if (color == "white"  ) return (bold ? bWhite   : ital ? iWhite   : cWhite   );
        return "";
    }

    //-----------------------------------------------------------------------
    extern void progressBar(uint64_t _part, uint64_t _whole, const string_q& msgs);
    extern void progressBar(uint64_t _part, uint64_t _whole, double _tim, const string_q& msgs);
    extern uint64_t barLen(uint64_t newLen = 0);
    extern double getProgBarVal(void);

#define BG (colorsDisabled() ? "" : cBlack + italic + "\e[48;5;245m")

}  // namespace qblocks
