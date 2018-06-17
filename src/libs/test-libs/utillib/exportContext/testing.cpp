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

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    string c_str = "regular 'c' string";
    string_q myStr = "my string";
    bool b = true;
    char c = 'c';
    unsigned int ui = (unsigned int)-1;
    int i = -1;
    int64_t il = -1;
    uint64_t uil = (uint64_t)-1;
    float f = (float)10.333;
    double d = 1001001010.01121112;

    CStringExportContext ctx;
    ctx << "bool: " << b << " : " << sizeof(b) << "\n";
    ctx << "char: " << c << " : " << sizeof(c) << "\n";
    ctx << "unsigned int: " << ui << " : " << sizeof(ui) << "\n";
    ctx << "int: " << i << " : " << sizeof(i) << "\n";
    ctx << "float: " << f << " : " << sizeof(f) << "\n";
    ctx << "double: " << d << " : " << sizeof(d) << "\n";
    ctx << "int64_t: " << il << " : " << sizeof(il) << "\n";
    ctx << "uint64_t: " << uil << " : " << sizeof(uil) << "\n";
    cout << ctx.str << "\n";

    ostringstream os;
    os.setf(ios::fixed);
    os << setprecision(10);
    os << "bool: " << b << " : " << sizeof(b) << "\n";
    os << "char: " << c << " : " << sizeof(c) << "\n";
    os << "unsigned int: " << ui << " : " << sizeof(ui) << "\n";
    os << "int: " << i << " : " << sizeof(i) << "\n";
    os << "float: " << f << " : " << sizeof(f) << "\n";
    os << "double: " << d << " : " << sizeof(d) << "\n";
    os << "int64_t: " << il << " : " << sizeof(il) << "\n";
    os << "uint64_t: " << uil << " : " << sizeof(uil) << "\n";
    cout << os.str();

    return 0;
}
