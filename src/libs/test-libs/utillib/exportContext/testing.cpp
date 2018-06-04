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
#include "utillib.h"
#include <iomanip>

#define sizeme(a) ((uint64_t)sizeof(a))
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    string c_str = "regular 'c' string";
    SFString myStr = "my string";
    bool b = true;
    char c = 'c';
    unsigned int ui = (unsigned int)-1;
    int i = -1;
    int64_t il = -1;
    uint64_t uil = (uint64_t)-1;
    float f = (float)10.333;
    double d = 1001001010.01121112;
    SFTime t = SFTime(2017,10,12,1,14,12);

    CStringExportContext ctx;
    ctx << "my string: " << myStr << " : " << sizeme(myStr) << "\n";
    ctx << "bool: " << b << " : " << sizeme(b) << "\n";
    ctx << "char: " << c << " : " << sizeme(c) << "\n";
    ctx << "unsigned int (this is a bug, should be 4294967295): " << ui << " : " << sizeme(ui) << "\n";
    ctx << "int: " << i << " : " << sizeme(i) << "\n";
    ctx << "float: " << f << " : " << sizeme(f) << "\n";
    ctx << "double: " << d << " : " << sizeme(d) << "\n";
    ctx << "time: " << t << " : " << sizeme(t) << "\n";
    ctx << "int64_t: " << il << " : " << sizeme(il) << "\n";
    ctx << "uint64_t: " << uil << " : " << sizeme(uil) << "\n";
    cout << ctx.str << "\n";

    ostringstream os;
    os.setf(ios::fixed);
    os << setprecision(10);
    os << "my string: " << myStr << " : " << sizeme(myStr) << "\n";
    os << "bool: " << b << " : " << sizeme(b) << "\n";
    os << "char: " << c << " : " << sizeme(c) << "\n";
    os << "unsigned int: " << ui << " : " << sizeme(ui) << "\n";
    os << "int: " << i << " : " << sizeme(i) << "\n";
    os << "float: " << f << " : " << sizeme(f) << "\n";
    os << "double: " << d << " : " << sizeme(d) << "\n";
    os << "time: " << t << " : " << sizeme(t) << "\n";
    os << "int64_t: " << il << " : " << sizeme(il) << "\n";
    os << "uint64_t: " << uil << " : " << sizeme(uil) << "\n";
    cout << os.str();

    return 0;
}
