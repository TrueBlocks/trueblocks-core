#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
    extern SFString cBlack;
    extern SFString cRed;
    extern SFString cGreen;
    extern SFString cYellow;
    extern SFString cBlue;
    extern SFString cMagenta;
    extern SFString cTeal;
    extern SFString cWhite;
    extern SFString cOff;

    extern SFString bbold;
    extern SFString italic;
    extern SFString underline;
    extern SFString reversed;
    extern SFString strikethru;
    extern SFString greenCheck;
    extern SFString redX;

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
    inline SFString convertColor(const SFString& colorIn) {
        bool bold = colorIn.Contains("_b");
        bool ital = colorIn.Contains("_i");
        SFString color = colorIn;
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
    extern void progressBar(uint64_t _part, uint64_t _whole, const SFString& msgs);
    extern void progressBar(uint64_t _part, uint64_t _whole, double _tim, const SFString& msgs);
    extern uint64_t barLen(uint64_t newLen = 0);
    extern double getProgBarVal(void);
}  // namespace qblocks
