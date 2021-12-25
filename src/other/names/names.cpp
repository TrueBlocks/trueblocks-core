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

#define NTESTS 20
#define NRUNS 500

//-----------------------------------------------------------------------
bool showName(CAccountName& name, void* data) {
    if (name.address == "0xf503017d7baf7fbc0fff7492b751025c6a78179b") return true;
    cout << name.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
    return true;
}

//-----------------------------------------------------------------------
bool showName2(NameOnDisc* name, void* data) {
    if (name && string_q(name->address) == "0xf503017d7baf7fbc0fff7492b751025c6a78179b") return true;
    cout << (name ? name->Format() : "") << endl;
    return true;
}

//-----------------------------------------------------------------------
CStringArray tests = {
    "0x43c65d1234edde9c9bc638f1fb284e1eb0c7ca1d",
    "0xa6a840e50bcaa50da017b91a0d86b8b2d41156ee",
    "0x29e240cfd7946ba20895a7a02edb25c210f9f324",
};

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    for (size_t x = 0; x < NTESTS; x++) {
        if (argc < 2) {
            loadNames();
            forEveryName(showName, nullptr);
            for (size_t i = 0; i < NRUNS; i++) {
                for (auto test : tests) {
                    CAccountName name;
                    findName(test, name);
                    cout << name << endl;
                    findToken(test, name);
                    cout << name << endl;
                }
            }
            cerr << "A-" << x << endl;
            clearNames();

        } else if (argc < 3) {
            loadNames2();
            forEveryName2(showName2, nullptr);
            for (size_t i = 0; i < NRUNS; i++) {
                for (auto test : tests) {
                    CAccountName name;
                    findName2(test, name);
                    cout << name << endl;
                    findToken2(test, name);
                    cout << name << endl;
                }
            }
            cerr << "B-" << x << endl;
            clearNames2();
        }
    }

    etherlib_cleanup();
    return 1;
}
