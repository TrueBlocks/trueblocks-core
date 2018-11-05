/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

//----------------------------------------------------------------
class ByteMap {
public:
    char byte;
    char nibble[2];
};

//----------------------------------------------------------------
inline string_q addr_2_Bytes(const string_q& addr) {

    string_q ret = addr;
    return ret;
}

//----------------------------------------------------------------
int main(int argc, char *argv[]) {
    CAddressArray addrs = {
        "0xc001237c925f515ca309ebd19476af364dfbbea7",
        "0xb16738cd0bb79f9c725e9e9bdeae677419003c69",
        "0xc4aad17558fa95c8937d0856b2dad74c1a7a095f",
        "0x8eb24319393716668d768dcec29356ae9cffe285",
        "0xc001237c925f515ca309ebd19476af364dfbbea7",
        "0x479f1d7ef1ab69acc785bffa32601ae20cd10f63",
        "0x2b8331edaa445e07b17b9ebf45f460c40ccafe52",
        "0x97ee7b5740ba5831f76af9efa576bbd0309b07c0",
        "0x034f854b44d28e26386c1bc37ff9b20c6380b00d",
        "0xfd54078badd5653571726c3370afb127351a6f26",
        "0x97aad47ea665fa50002f732c22dbc3d243a08c2d",
        "0xcb5af8f7e1c95aeb566cf675456f7be8965bd292",
        "0x0abdace70d3790235af448c88547603b945604ea",
        "0x9394a399baf330c1901788c00e9bdb70bc651d8d",
        "0xeec606a66edb6f497662ea31b5eb1610da87ab5f",
        "0x8a28fb6c72c73e84d347aa673870aa8a3ce64db4",
        "0x3644b4c910019e0ec1807641da7f966974c3b895",
        "0xbc7bb93d1962bfe70cedeff0581d7fe010624078",
        "0x1fefceec92f11c6a6c3c3e50493448628869e32a",
    };

#if 1
    CAddressArray bytes;
    for (auto addr : addrs)
        bytes.push_back(addr_2_Bytes(addr));
    for (uint32_t n = 0 ; n < 2500; n++)
        for (auto item : bytes)
            cout << n << "\t" << item << "\n";
#else
#if 1
    for (uint32_t n = 0 ; n < 2500; n++)
        for (auto item : addrs)
            cout << n << "\t" << item << "\n";
#else
    CBigUintArray bytes;
    for (auto addr : addrs)
        bytes.push_back(str_2_Wei(addr));
    for (uint32_t n = 0 ; n < 2500; n++)
        for (auto item : bytes)
            cout << n << "\t" << item << "\n";
#endif
#endif

    return 1;
}

//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);
    } else {
        cout << path << "\n";
    }
    return true;
}

//----------------------------------------------------------------
bool visitBlock(CBlock& block, void *data) {

    cout << block << "\n";
    for (uint32_t tr = 0 ; tr < block.transactions.size() ; tr++) {
        cout << block.transactions[tr] << "\n";
    }
    return true;
}
