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
#include "options.h"

//-----------------------------------------------------------------------------
hash_t namehash(const string_q& domainIn) {
    CStringArray parts;
    explode(parts, domainIn, '.');
    hash_t node = "0x" + string_q(64, '0');
    for (auto part : parts)
        node = keccak256(node + keccak256(part));
    return "Doesn't work: " + node;
}

//-----------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    cout << "namehash:  " << namehash("") << endl;
    cout << "should be: "
         << "0x0000000000000000000000000000000000000000000000000000000000000000" << endl;

    cout << "namehash:  " << namehash("eth") << endl;
    cout << "should be: "
         << "0x93cdeb708b7545dc668eb9280176169d1c33cfd8ed6f04690a0bcc88a93fc4ae" << endl;

    cout << "namehash:  " << namehash("foo.eth") << endl;
    cout << "should be: "
         << "0xde9b09fd7c5f901e23a3f19fecc54828e9c848539801e86591bd9801b019f84f" << endl;

    etherlib_cleanup();
    return 0;
}
