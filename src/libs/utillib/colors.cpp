/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "basetypes.h"
#include "conversions.h"
#include "exportcontext.h"
#include "colors.h"

namespace qblocks {

    SFString cBlack     = "\e[0;30m";
    SFString cRed       = "\e[0;31m";
    SFString cGreen     = "\e[0;32m";
    SFString cYellow    = "\e[0;33m";
    SFString cBlue      = "\e[0;34m";
    SFString cMagenta   = "\e[0;35m";
    SFString cTeal      = "\e[0;36m";
    SFString cWhite     = "\e[0;37m";
    SFString cOff       = "\e[0m";

    SFString bbold      = "\e[1m";
    SFString dim        = "\e[2m";
    SFString italic     = "\e[3m";
    SFString underline  = "\e[4m";
    SFString reversed   = "\e[7m";
    SFString strikethru = "\e[7m";
    SFString greenCheck = "\e[0;32m✓\e[0m";
    SFString redX       = "\e[0;31mX\e[0m";

    void colorsDim(void) {
        cWhite = (cWhite+dim+italic);
        cGreen = cYellow = cBlue = cMagenta = cTeal = cOff = cBlack = cWhite;
        bbold = italic = underline = reversed = strikethru = "";
        greenCheck = "✓"; redX = "X";
    }

    void colorsOff(void) {
        cBlack = cRed = cGreen = cYellow = cBlue = "";
        cMagenta = cTeal = cWhite = cOff = bbold = "";
        italic = underline = reversed = strikethru = "";
        greenCheck = "✓"; redX = "X";
    }

    void colorsOn(void) {
        cBlack     = "\e[0;30m";
        cRed       = "\e[0;31m";
        cGreen     = "\e[0;32m";
        cYellow    = "\e[0;33m";
        cBlue      = "\e[0;34m";
        cMagenta   = "\e[0;35m";
        cTeal      = "\e[0;36m";
        cWhite     = "\e[0;37m";
        cOff       = "\e[0m";
        bbold      = "\e[1m";
        italic     = "\e[3m";
        underline  = "\e[4m";
        reversed   = "\e[7m";
        strikethru = "\e[7m";
        greenCheck = "\e[0;32m✓\e[0m";
        redX       = "\e[0;31mX\e[0m";
    }

    bool colorsDisabled(void) { return cBlack == ""; }

    //-----------------------------------------------------------------------
    uint64_t barLen(uint64_t newLen) {
        static uint64_t _barLen = 100;
        if (newLen)
            _barLen = newLen;
        return _barLen;
    }

    //-----------------------------------------------------------------------
    void progressBar(uint64_t _part, uint64_t _whole, const SFString& msgs) {
        SFString endMsg = msgs;
        SFString begMsg = nextTokenClear(endMsg, '|');
        double percent = 1.0;
        if (_whole > 0)
            percent = (_part / static_cast<double>(_whole));
        uint64_t len = (uint64_t)(barLen() * percent);

        cout << begMsg << (begMsg.empty() ? " " : "");
        cout << " [" << SFString('x', len).Substitute("x", "░");
        cout << SFString(' ', max((uint64_t)0, barLen() - len));
        cout << "] ";
        cout << cYellow << _part << cOff << " of " << cYellow << _whole << cOff;
        cout << " (" << bBlue << fmtFloatp(100.*percent, 1) << cOff << "%)";
        cout << (endMsg.empty() ? " " : "") << endMsg;
        cout << "\r";
        cout.flush();
    }

    static double pb_Value = -1.0;
    double getProgBarVal(void) { return pb_Value; }

    //-----------------------------------------------------------------------
    void progressBar(uint64_t _part, uint64_t _whole, double _tim, const SFString& begMsg) {
        if (_part == 0 && _whole == 0) {
            // reset
            pb_Value = 0.;
            return;
        }

        CStringExportContext ctx;
        if (_tim > 0.) {
            pb_Value = max(pb_Value, _tim);  // keep it monotonic
            ctx << " in " << cYellow << pb_Value << cOff << " seconds.";
        }
        progressBar(_part, _whole, begMsg+"|"+ctx.str);
    }
}  // namespace qblocks
