/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"

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
    SFString italic     = "\e[3m";
    SFString underline  = "\e[4m";
    SFString reversed   = "\e[7m";
    SFString strikethru = "\e[7m";
    SFString greenCheck = "\e[0;32m✓\e[0m";
    SFString redX       = "\e[0;31mX\e[0m";

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
}  // namespace qblocks
