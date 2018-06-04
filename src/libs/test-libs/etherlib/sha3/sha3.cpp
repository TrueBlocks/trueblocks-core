#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "etherlib.h"
#include "options.h"

extern void doTests(void);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (isTestMode()) {
        doTests();
    } else {
        while (!options.commandList.empty()) {
            SFString command = nextTokenClear(options.commandList, '\n');
            if (!options.parseArguments(command))
                return 0;
            SFString in = argv[1];
            cout << "in: " << in << "\n";
            SFString hex = in.startsWith("0x") ? in : "0x" + string2Hex(in);
            cout << "hex: " << hex << "\n";
            SFString out = getSha3(hex);
            cout << "out: " << out << "\n";
        }
    }
    return 0;
}

//--------------------------------------------------------------
extern const char* STR_TEST_DATA;
//--------------------------------------------------------------
void doTests(void) {
    SFString in;
    bool hasIn = false;
    SFString contents = STR_TEST_DATA;
    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!line.startsWith('#')) {
            if (!hasIn) {
                in = line;
                hasIn = true;
            } else {
                SFString hex = in.startsWith("0x") ? in : "0x" + string2Hex(in);
                SFString out = getSha3(hex);
                cout << "in:\t\t" << in << "\n"
                    << "hex:\t\t" << hex << "\n"
                    << "expected:\t" << line << "\n"
                    << "out:\t\t" << out << " " << (out == line ? greenCheck : redX) << "\n\n";
                hasIn = false;
            }
        }
    }
}

//--------------------------------------------------------------
const char* STR_TEST_DATA =
"#\n"
"# From http://www.di-mgt.com.au/sha_testvectors.html\n"
"# pairs of input / expected output on subsequent lines\n"
"#\n"
"#\n"
"# from RPC web page (already hex)\n"
"0x68656c6c6f20776f726c64\n"
"0x47173285a8d7341e5e972fc677286384f802f8ef42a5ec5f03bbfa254cb01fad\n"
"#\n"
"# empty string \n"
"\n"
"0xc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470\n"
"#\n"
"# longer string\n"
"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu \n"
"0x4fdc431ca3869f7a79a3a43acbb0019c769c667ad29fb36896209969b8550af7\n"
"#\n"
"The quick brown fox jumps over the lazy dog\n"
"0x4d741b6f1eb29cb2a9b9911c82f56fa8d73b04959d3d9d222895df6c0b28aa15\n"
"#\n"
"The quick brown fox jumps over the lazy dog.\n"
"0x578951e24efd62a3d63a86f7cd19aaa53c898fe287d2552133220370240b572d\n";
