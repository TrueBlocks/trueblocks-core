#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);
};

//---------------------------------------------------------------------
class T {
  public:
    string_q desc;
    bool bVal;
    string_q input;
    string_q expected;
    bool enabled;
    T(const string_q& d, bool v, const string_q& in, const string_q& ex) : desc(d), bVal(v), input(in), expected(ex) {
        enabled = true;
    }
    T(const string_q& d, const string_q& in, const string_q& ex) : desc(d), input(in), expected(ex) {
        enabled = true;
        bVal = false;
    }
    T(void) : desc(""), bVal(false), input(""), expected("") {
        enabled = true;
    }
    string_q report(const string_q& res) {
        ostringstream os;
        os << "\tinput:    0x";
        string_q in = substitute(input, "0x", "");
        while (!in.empty()) {
            string_q p = in.substr(0, 64);
            replace(in, p, "");
            os << p << endl;
            if (!in.empty())
                os << "\t\t    ";
        }
        os << "\texpected: " << expected << endl;
        os << "\tresult:   " << res << endl;
        os << endl;
        return os.str();
    }

    string_q check(const string_q& res) {
        if (!enabled)
            return "";
        ostringstream os;
        os << "  " << ((res == expected) ? greenCheck : redX) << "\tdesc:     " << desc << endl;
        if (verbose || res != expected) {
            os << report(res);
            if (res != expected) {
                cerr << os.str() << "\t" << cRed << "Test failed. Quitting..." << cOff << endl;
                quickQuitHandler(0);
            }
        }
        return os.str();
    }
};

//---------------------------------------------------------------------
extern void padTest(void);
extern void hexUtilTest(void);
extern void speedTest(void);
