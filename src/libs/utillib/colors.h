#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
