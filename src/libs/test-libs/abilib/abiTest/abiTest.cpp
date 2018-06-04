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

extern SFString getTestData(void);
extern bool test_encodings(void);
extern bool test_generation(void);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        while (!options.mode.empty()) {
            SFString mode = nextTokenClear(options.mode, '|');
            if (mode == "encoding") {
                cout << "Encodings test...\n";
                cout << (test_encodings() ? "...passed" : "...failed") << "\n";
                cout << "\n";

            } else if (mode == "generation") {
                cout << "Generation test...\n";
                cout << (test_generation() ? "...passed" : "...failed") << "\n";
                cout << "\n";
                
            }
        }
    }
    return 0;
}

//--------------------------------------------------------------
bool test_encodings(void) {

    bool ret = true;

    SFString contents = getTestData().Substitute("  ", " ");
    nextTokenClear(contents, '\n');  // skip header row
    while (!contents.empty()) {
        SFString expected = nextTokenClear(contents, '\n');
        if (!expected.startsWith(';')) {
            SFString type  = nextTokenClear(expected, ' ');
            SFString text  = nextTokenClear(expected, ' ');
            SFString myHex = "0x"+string2Hex(text);
            SFString mySha = getSha3(myHex);
            if (type == "function")
                mySha = mySha.substr(0,10);
            bool result = mySha == expected;
            cout << "\t" << type << ": " << text << " "
                    << "encoding: " << mySha << " "
                    << (result ? greenCheck : redX) << "\n";
            ret &= result;
        }
    }
    return ret;
}

//--------------------------------------------------------------
bool test_generation(void) {
    CFunction::registerClass();
    CParameter::registerClass();

    CAbi abi;
    cout << "ABI of test1.json\n";
    abi.loadABIFromFile("./test1.json");
    cout << abi << "\n";
    abi.clearABI();

    cout << "ABI of test2.json\n";
    abi.loadABIFromFile("./test2.json");
    cout << abi << "\n";
    return true;
}

//--------------------------------------------------------------
SFString getTestData(void) {

    return
      "type     signature                                  expected\n"
      "event    ContributionMade(address)                  0x97a3367c201ad38e0d37322fd0ffa1b93457541ae8baf20eb9aa50bb83fcabef\n"  // NOLINT
      "event    RefundPayoutClaimed(uint256)               0x5e5c64a3ed3fb7fd17b9ca165f3ebe344a48d7c845dc947fc27249da676e7fbd\n"  // NOLINT
      "event    BeneficiaryPayoutClaimed(address,uint256)  0x22c1e24047f1e0c1af6f78290547f44645cdd2ad4d06b09115a162e41460c4d5\n"  // NOLINT
      "function emergencyCall()                            0x299ed37a\n"
      "function setup(address,address)                     0x2d34ba79\n"
      "function withdrawFunding()                          0x30b9af98\n"
      "function changeBaseValue(uint256)                   0x92a781d8\n"
      "function fund()                                     0xb60d4288\n"
      "function updateStage()                              0xc062f578\n"
      "function withdrawForWorkshop()                      0xf3a44fe1\n"
      "function issueTokens(address,uint256)               0x475a9fa9\n"
      "function depositRevenue()                           0x44e43cb8\n"
      "function withdrawRevenue()                          0x4f573cb2\n"
      "function softWithdrawRevenueFor(address)            0x5d0be9de\n"
      "event    Transfer(address,address,uint256)          0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef\n"  // NOLINT
      "event    Approval(address,address,uint256)          0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925\n"  // NOLINT
      "function transferFrom(address,address,uint256)      0x23b872dd\n"
      "function approve(address,uint256)                   0x095ea7b3\n"
      "function transfer(address,uint256)                  0xa9059cbb\n";
}
