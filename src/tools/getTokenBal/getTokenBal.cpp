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
#define NOWALLETLIB
#include "etherlib.h"
#include "options.h"

void reportByToken(COptions& options);
void reportByAccount(COptions& options);
extern biguint_t getTokenInfo(const string_q& value, const address_t& token, const address_t& holder, blknum_t blockNum);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.asData)
            cout << "block\ttoken\tholder\ttoken balance\n";
        if (options.byAccount)
            reportByAccount(options);
        else
            reportByToken(options);

    }

    if ((options.latestBlock - options.earliestBlock) > 250 && !nodeHasBalances() && !isTestMode())
        cerr << cRed << "    Warning: " << cOff << "The node you're using does not have historical balances. Reported "
                                                    "values may be wrong.\n";

    return 0;
}

//--------------------------------------------------------------
void reportByToken(COptions& options) {

    biguint_t totalVal = 0;
    uint64_t nAccts = countOf(options.holders, '|') + 1;
    bool needsTotal = (nAccts > 1 && options.total);

    bool needsNewline = true;
    // For each token contract
    string_q tokens = options.tokens;
    while (!tokens.empty()) {
        address_t token = nextTokenClear(tokens, '|');
        if (!options.asData)
            cout << "\n  For token contract: " << bBlue << token << cOff << "\n";

        // For each holder
        string_q holders = options.holders;
        while (!holders.empty()) {
            address_t holder = nextTokenClear(holders, '|');

            // For each block
            string_q blocks = options.getBlockNumList();
            while (!blocks.empty()) {
                blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
                if (blockNum < options.earliestBlock)
                    options.earliestBlock = blockNum;
                biguint_t bal = getTokenInfo("balance", token, holder, blockNum);
                totalVal += bal;
                string_q sBal = bnu_2_Str(bal);
                if (expContext().asEther) {
                    sBal = wei_2_Ether(bnu_2_Str(bal));
                } else if (expContext().asDollars) {
                    CBlock blk;
                    getBlock(blk, blockNum);
                    sBal = padLeft("$" + dispDollars(blk.timestamp, bal), 14);
                }

                needsNewline = true;
                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << blockNum << "\t" << token << "\t" << holder << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << holder << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else if (!isTestMode()) {
                    if (options.asData) {
                        cerr << blockNum << "\t" << token << "\t" << holder << "         \r";
                    } else {
                        cerr << "    Balance for account " << cGreen << holder << cOff;
                        cerr << " at block " << cTeal << blockNum << cOff;
                        cerr << " is " << cYellow << sBal << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }

    if (needsTotal) {
        string_q sBal = bnu_2_Str(totalVal);
        if (expContext().asEther) {
            sBal = wei_2_Ether(bnu_2_Str(totalVal));
        } else if (expContext().asDollars) {
            CBlock blk;
            getBlock(blk, getLatestBlockFromClient());
            sBal = padLeft("$" + dispDollars(blk.timestamp, totalVal), 14);
        }
        cout << "        Total for " << cGreen << nAccts << cOff;
        cout << " accounts at " << cTeal << "latest" << cOff << " block";
        cout << " is " << cYellow << substitute(sBal, "  ", " ") << cOff << "\n";
    }

    if (needsNewline)
        cerr << string_q(104, ' ') << "\n";
}

//--------------------------------------------------------------
void reportByAccount(COptions& options) {

    biguint_t totalVal = 0;
    uint64_t nAccts = countOf(options.holders, '|') + 1;
    bool needsTotal = (nAccts > 1 && options.total);

    bool needsNewline = true;
    // For each holder
    string_q holders = options.holders;
    while (!holders.empty()) {
        address_t holder = nextTokenClear(holders, '|');
        if (!options.asData)
            cout << "\n  For account: " << bBlue << holder << cOff << "\n";

        // For each token contract
        string_q tokens = options.tokens;
        while (!tokens.empty()) {
            address_t token = nextTokenClear(tokens, '|');

            // For each block
            string_q blocks = options.getBlockNumList();
            while (!blocks.empty()) {
                blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
                if (blockNum < options.earliestBlock)
                    options.earliestBlock = blockNum;
                biguint_t bal = getTokenInfo("balance", token, holder, blockNum);
                totalVal += bal;
                string_q sBal = bnu_2_Str(bal);
                if (expContext().asEther) {
                    sBal = wei_2_Ether(bnu_2_Str(bal));
                } else if (expContext().asDollars) {
                    CBlock blk;
                    getBlock(blk, blockNum);
                    sBal = padLeft("$" + dispDollars(blk.timestamp, bal), 14);
                }

                needsNewline = true;
                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << blockNum << "\t" << token << "\t" << holder << "\t" << sBal << "\n";
                    } else {
                        cout << "    Balance of token contract " << cGreen << token << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << sBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else if (!isTestMode()) {
                    if (options.asData) {
                        cerr << blockNum << "\t" << token << "\t" << holder << "         \r";
                    } else {
                        cerr << "    Balance of token contract " << cGreen << token << cOff;
                        cerr << " at block " << cTeal << blockNum << cOff;
                        cerr << " is " << cYellow << sBal << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }

    if (needsTotal) {
        string_q sBal = bnu_2_Str(totalVal);
        if (expContext().asEther) {
            sBal = wei_2_Ether(bnu_2_Str(totalVal));
        } else if (expContext().asDollars) {
            CBlock blk;
            getBlock(blk, getLatestBlockFromClient());
            sBal = padLeft("$" + dispDollars(blk.timestamp, totalVal), 14);
        }
        cout << "        Total for " << cGreen << nAccts << cOff;
        cout << " accounts at " << cTeal << "latest" << cOff << " block";
        cout << " is " << cYellow << substitute(sBal, "  ", " ") << cOff << "\n";
    }

    if (needsNewline)
        cerr << "                                                                                              \n";
}

//-------------------------------------------------------------------------
biguint_t getTokenInfo(const string_q& value, const address_t& token, const address_t& holder, blknum_t blockNum) {

    ASSERT(isAddress(token));
    ASSERT(isAddress(holder));

    string_q t = "0x" + padLeft(extract(token, 2), 40, '0');  // address to send the command to
    string_q h =        padLeft(extract(holder, 2), 64, '0');  // encoded data for the transaction

    string_q cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"0x70a08231[HOLDER]\"}, \"[BLOCK]\"]";
    //        string_q cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"0x18160ddd\"}, \"[BLOCK]\"]";
    replace(cmd, "[TOKEN]",  t);
    replace(cmd, "[HOLDER]", h);
    replace(cmd, "[BLOCK]",  uint_2_Hex(blockNum));

    return str_2_Wei(callRPC("eth_call", cmd, false));
}

