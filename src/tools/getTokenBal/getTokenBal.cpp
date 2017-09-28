/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#define NOWALLETLIB
#include "tokenlib.h"
#include "options.h"

void reportByToken(const COptions& options);
void reportByAccount(const COptions& options);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    etherlib_init("parity");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.asData)
            cout << "block\ttoken\tholder\ttoken balance\n";
        if (options.byAccount)
            reportByAccount(options);
        else
            reportByToken(options);

    }
    return 0;
}

//--------------------------------------------------------------
void reportByToken(const COptions& options) {

    // For each token contract
    SFString tokens = options.tokens;
    while (!tokens.empty()) {
        SFAddress token = nextTokenClear(tokens, '|');
        if (!options.asData)
            cout << "\n  For token contract: " << bBlue << token << cOff << "\n";

        // For each holder
        SFString holders = options.holders;
        while (!holders.empty()) {
            SFAddress holder = nextTokenClear(holders, '|');

            // For each block
            SFString blocks = options.blocks;
            while (!blocks.empty()) {

                blknum_t block = toLongU(nextTokenClear(blocks, '|'));

                SFUintBN bal = getTokenBalance(token, holder, block);
                SFString sBal = to_string(bal).c_str();
                if (expContext().asEther)
                    sBal = wei2Ether(to_string(bal).c_str());

                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << block << "\t" << token << "\t" << holder << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << holder << cOff;
                        cout << " at block " << cTeal << block << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                } else {
                    if (options.asData) {
                        cerr << block << "\t" << token << "\t" << holder << "         \r";
                    } else {
                        cerr << "    Balance for account " << cGreen << holder << cOff;
                        cerr << " at block " << cTeal << block << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }
}

//--------------------------------------------------------------
void reportByAccount(const COptions& options) {
    // For each holder
    SFString holders = options.holders;
    while (!holders.empty()) {
        SFAddress holder = nextTokenClear(holders, '|');
        if (!options.asData)
            cout << "\n  For account: " << bBlue << holder << cOff << "\n";

        // For each token contract
        SFString tokens = options.tokens;
        while (!tokens.empty()) {
            SFAddress token = nextTokenClear(tokens, '|');

            // For each block
            SFString blocks = options.blocks;
            while (!blocks.empty()) {

                blknum_t block = toLongU(nextTokenClear(blocks, '|'));

                SFUintBN bal = getTokenBalance(token, holder, block);
                SFString sBal = to_string(bal).c_str();
                if (expContext().asEther)
                    sBal = wei2Ether(to_string(bal).c_str());

                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << block << "\t" << token << "\t" << holder << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance of token contract " << cGreen << token << cOff;
                        cout << " at block " << cTeal << block << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                } else {
                    if (options.asData) {
                        cout << block << "\t" << token << "\t" << holder << "\n";
                    } else {
                        cout << "    Balance of token contract " << cGreen << token << cOff;
                        cout << " at block " << cTeal << block << cOff << "\n";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }
}
