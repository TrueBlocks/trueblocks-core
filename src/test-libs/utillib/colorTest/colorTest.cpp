/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"

#define ONE(a) cout << "[" << a << " " << #a << " " << cOff << "] ";
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    ONE(cBlack); ONE(cRed); ONE(cGreen); ONE(cYellow); ONE(cBlue); ONE(cMagenta); ONE(cTeal); ONE(cWhite); ONE(bYellow + strikethru); cout << "\n";
    ONE(bBlack); ONE(bRed); ONE(bGreen); ONE(bYellow); ONE(bBlue); ONE(bMagenta); ONE(bTeal); ONE(bWhite); ONE(cGreen + strikethru ); cout << "\n";
    ONE(iBlack); ONE(iRed); ONE(iGreen); ONE(iYellow); ONE(iBlue); ONE(iMagenta); ONE(iTeal); ONE(iWhite); ONE(cBlue + strikethru + bbold); cout << "\n";
    ONE(bBlack + italic); ONE(biBlack); ONE(cBlack + bbold); ONE(cBlack + italic + bbold); ONE(cBlack + bbold); ONE(cMagenta + strikethru + italic); cout << "\n";
    ONE(cRed + underline); ONE(cRed + reversed); ONE(cRed + bbold + italic + underline); ONE(cRed + bbold + underline + reversed + bbold); cout << "\n";

    // turn off colors
    cout << "\n\n";

    colorsOff();
    ONE(cBlack); ONE(cRed); ONE(cGreen); ONE(cYellow); ONE(cBlue); ONE(cMagenta); ONE(cTeal); ONE(cWhite); ONE(bYellow + strikethru); cout << "\n";
    ONE(bBlack); ONE(bRed); ONE(bGreen); ONE(bYellow); ONE(bBlue); ONE(bMagenta); ONE(bTeal); ONE(bWhite); ONE(cGreen + strikethru ); cout << "\n";
    ONE(iBlack); ONE(iRed); ONE(iGreen); ONE(iYellow); ONE(iBlue); ONE(iMagenta); ONE(iTeal); ONE(iWhite); ONE(cBlue + strikethru + bbold); cout << "\n";
    ONE(bBlack + italic); ONE(biBlack); ONE(cBlack + bbold); ONE(cBlack + italic + bbold); ONE(cBlack + bbold); ONE(cMagenta + strikethru + italic); cout << "\n";
    ONE(cRed + underline); ONE(cRed + reversed); ONE(cRed + bbold + italic + underline); ONE(cRed + bbold + underline + reversed + bbold); cout << "\n";

    return 0;
}
