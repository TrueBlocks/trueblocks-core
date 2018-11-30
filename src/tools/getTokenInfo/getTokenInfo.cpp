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
#include "options.h"

void reportByToken(COptions& options);
void reportByAccount(COptions& options);
extern string_q getTokenInfo(const string_q& which, const address_t& t, const address_t& h, blknum_t b);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    CStringArray commands;
    explode(commands, options.commandList, '\n');
    for (auto command : commands) {
        if (!options.parseArguments(command))
            return 0;

        if (!options.tokenInfo.empty()) {
            cout << getTokenInfo(options.tokenInfo, options.tokens[0], "", options.newestBlock) << "\n";

        } else {
            if (options.asData)
                cout << "block\ttoken\tholder\ttoken balance\n";
            if (options.byAccount)
                reportByAccount(options);
            else
                reportByToken(options);
        }
    }

    if (!options.hasHistory()) {
        cerr << cRed << "    Warning: " << cOff;
        cerr << "Your node does not have historical balances. Historical information is incorrect.\n";
    }

    return 0;
}

//--------------------------------------------------------------
void reportByToken(COptions& options) {

    biguint_t totalVal = 0;
    uint64_t cnt = 0;

    bool needsNewline = true;
    // For each token contract
    for (auto token : options.tokens) {
        if (!options.asData)
            cout << "\n  For token contract: " << bBlue << token << cOff << "\n";

        // For each holder
        for (auto holder : options.holders) {
            cnt++;

            // For each block
            string_q blocks = options.getBlockNumList();
            while (!blocks.empty()) {
                blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
                if (blockNum < options.oldestBlock)
                    options.oldestBlock = blockNum;
                biguint_t bal = str_2_Wei(getTokenInfo("balanceOf", token, holder, blockNum));
                totalVal += bal;
                string_q dispBal = options.getDispBal(options.newestBlock, bal, options.asData);
                needsNewline = true;
                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << blockNum << "\t" << token << "\t" << holder << "\t" << dispBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << holder << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << dispBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else if (!isTestMode()) {
                    if (options.asData) {
                        cerr << blockNum << "\t" << token << "\t" << holder << "         \r";
                    } else {
                        cerr << "    Balance for account " << cGreen << holder << cOff;
                        cerr << " at block " << cTeal << blockNum << cOff;
                        cerr << " is " << cYellow << dispBal << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }

    if (options.total && cnt > 1) {
        string_q dispBal = options.getDispBal(options.newestBlock, totalVal, options.asData);
        cout << "        Total for " << cGreen << cnt << cOff;
        cout << " accounts at " << cTeal << "latest" << cOff << " block";
        cout << " is " << cYellow << dispBal << cOff << "\n";
    }

    if (needsNewline)
        cerr << string_q(104, ' ') << "\n";
}

//--------------------------------------------------------------
void reportByAccount(COptions& options) {

    biguint_t totalVal = 0;
    uint64_t cnt = 0;

    bool needsNewline = true;
    // For each holder
    for (auto holder : options.holders) {
        if (!options.asData)
            cout << "\n  For account: " << bBlue << holder << cOff << "\n";

        // For each token contract
        for (auto token : options.holders) {
            cnt++;

            // For each block
            string_q blocks = options.getBlockNumList();
            while (!blocks.empty()) {
                blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
                if (blockNum < options.oldestBlock)
                    options.oldestBlock = blockNum;
                biguint_t bal = str_2_Wei(getTokenInfo("balanceOf", token, holder, blockNum));
                totalVal += bal;
                string_q dispBal = options.getDispBal(options.newestBlock, bal, options.asData);
                needsNewline = true;
                if (bal > 0 || !options.noZero) {
                    if (options.asData) {
                        cout << blockNum << "\t" << token << "\t" << holder << "\t" << dispBal << "\n";
                    } else {
                        cout << "    Balance of token contract " << cGreen << token << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << dispBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else if (!isTestMode()) {
                    if (options.asData) {
                        cerr << blockNum << "\t" << token << "\t" << holder << "         \r";
                    } else {
                        cerr << "    Balance of token contract " << cGreen << token << cOff;
                        cerr << " at block " << cTeal << blockNum << cOff;
                        cerr << " is " << cYellow << dispBal << cOff << "           \r";
                    }
                }
                cerr.flush();
                cout.flush();
            }
        }
    }

    if (options.total && cnt > 1) {
        string_q dispBal = options.getDispBal(options.newestBlock, totalVal, options.asData);
        cout << "        Total for " << cGreen << cnt << cOff;
        cout << " accounts at " << cTeal << "latest" << cOff << " block";
        cout << " is " << cYellow << dispBal << cOff << "\n";
    }

    if (needsNewline)
        cerr << "                                                                                              \n";
}

//-------------------------------------------------------------------------
string_q getTokenInfo(const string_q& which, const address_t& token, const address_t& holder, blknum_t blockNum) {

    if (!isAddress(token))
        return "";
    ASSERT(holder.empty() || isAddress(holder));

    string_q cmd;
    if (which % "balanceOf") {
        cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"0x70a08231[HOLDER]\"}, \"[BLOCK]\"]";
        replace(cmd, "[TOKEN]",  token);
        replace(cmd, "[HOLDER]", padLeft(extract(holder, 2), 64, '0'));  // encoded data for the transaction
        replace(cmd, "[BLOCK]",  uint_2_Hex(blockNum));
    } else {
        string_q encoding;
        if (isValidInfo(which, encoding)) {
            cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"[CMD]\"}, \"[BLOCK]\"]";
            replace(cmd, "[TOKEN]",  token);
            replace(cmd, "[CMD]",    encoding);  // encoded data for the transaction
            replace(cmd, "[BLOCK]",  uint_2_Hex(blockNum));
        }
    }

    return cmd.empty() ? "" : callRPC("eth_call", cmd, false);
}

//-------------------------------------------------------------------------
bool isValidInfo(const string_q which, string_q& encoding) {
    for (auto item : infoOptions) {
        if (contains(item, which+"|")) {
            encoding = item;
            nextTokenClear(encoding, '|');
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------------
CStringArray infoOptions = { "name|0x06fdde03", "totalSupply|0x18160ddd", "decimals|0x313ce567", "version|0x54fd4d50", "symbol|0x95d89b41", "all" };
