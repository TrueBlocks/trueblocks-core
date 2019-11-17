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
#include "utillib.h"

#define ONE(a) cout << "[" << a << " " << #a << " " << cOff << "] ";
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    //    etherlib_init(quickQuitHandler);

    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (isTestMode())
        colorsOn();

    ONE(cBlack);
    ONE(cRed);
    ONE(cGreen);
    ONE(cYellow);
    ONE(cBlue);
    ONE(cMagenta);
    ONE(cTeal);
    ONE(cWhite);
    ONE(bYellow + strikethru);
    cout << "\n";  // NOLINT
    ONE(bBlack);
    ONE(bRed);
    ONE(bGreen);
    ONE(bYellow);
    ONE(bBlue);
    ONE(bMagenta);
    ONE(bTeal);
    ONE(bWhite);
    ONE(cGreen + strikethru);
    cout << "\n";  // NOLINT
    ONE(iBlack);
    ONE(iRed);
    ONE(iGreen);
    ONE(iYellow);
    ONE(iBlue);
    ONE(iMagenta);
    ONE(iTeal);
    ONE(iWhite);
    ONE(cBlue + strikethru + bbold);
    cout << "\n";  // NOLINT
    ONE(bBlack + italic);
    ONE(biBlack);
    ONE(cBlack + bbold);
    ONE(cBlack + italic + bbold);
    ONE(cBlack + bbold);
    ONE(cMagenta + strikethru + italic);
    cout << "\n";  // NOLINT
    ONE(cRed + underline);
    ONE(cRed + reversed);
    ONE(cRed + bbold + italic + underline);
    ONE(cRed + bbold + underline + reversed + bbold);
    cout << "\n";  // NOLINT

    // turn off colors
    cout << "\n\n";

    colorsOff();
    ONE(cBlack);
    ONE(cRed);
    ONE(cGreen);
    ONE(cYellow);
    ONE(cBlue);
    ONE(cMagenta);
    ONE(cTeal);
    ONE(cWhite);
    ONE(bYellow + strikethru);
    cout << "\n";  // NOLINT
    ONE(bBlack);
    ONE(bRed);
    ONE(bGreen);
    ONE(bYellow);
    ONE(bBlue);
    ONE(bMagenta);
    ONE(bTeal);
    ONE(bWhite);
    ONE(cGreen + strikethru);
    cout << "\n";  // NOLINT
    ONE(iBlack);
    ONE(iRed);
    ONE(iGreen);
    ONE(iYellow);
    ONE(iBlue);
    ONE(iMagenta);
    ONE(iTeal);
    ONE(iWhite);
    ONE(cBlue + strikethru + bbold);
    cout << "\n";  // NOLINT
    ONE(bBlack + italic);
    ONE(biBlack);
    ONE(cBlack + bbold);
    ONE(cBlack + italic + bbold);
    ONE(cBlack + bbold);
    ONE(cMagenta + strikethru + italic);
    cout << "\n";  // NOLINT
    ONE(cRed + underline);
    ONE(cRed + reversed);
    ONE(cRed + bbold + italic + underline);
    ONE(cRed + bbold + underline + reversed + bbold);
    cout << "\n";  // NOLINT

    return 0;
}
