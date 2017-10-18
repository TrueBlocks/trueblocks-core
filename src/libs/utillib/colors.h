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
    inline SFUint32 barLen(SFUint32 newLen = 0) {
        static SFUint32 _barLen = 100;
        if (newLen)
            _barLen = newLen;
        return _barLen;
    }

    //-----------------------------------------------------------------------
    inline void progressBar(SFUint32 _part, SFUint32 _whole, const SFString& _tim = "") {

        double percent = 1.0;
        if (_whole > 0)
            percent = (_part / static_cast<double>(_whole));
        SFUint32 len = (SFUint32)(barLen() * percent);

        cout << cYellow << _part << cOff << " of " << cYellow << _whole << cOff;
        cout << " (" << cBlue << padLeft(fmtFloatp(100.*percent, 1), 5, ' ') << cOff << "%)";
        cout << " [" << SFString('x', len).Substitute("x", "░");
        cout << SFString(' ', max((SFUint32)0, barLen() - len));
        cout << "]";
        if (!_tim.empty())
            cout << " " << _tim;
        cout << "\r";
        cout.flush();
    }

    //-----------------------------------------------------------------------
    inline void progressBar(SFUint32 _part, SFUint32 _whole, double _tim) {
        static double val = -1.0;
        if (_part == 0 && _whole == 0) {
            // reset
            val = 0.;
            return;
        }

        CStringExportContext ctx;
        if (_tim > 0.) {
            val = max(val, _tim);  // keep it monotonic
            ctx << " in " << cGreen << val << cOff << " seconds.";
        }
        progressBar(_part, _whole, ctx.str);
    }
}  // namespace qblocks
