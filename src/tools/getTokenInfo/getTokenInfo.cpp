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
extern string_q getTokenInfo(const string_q& which, CTokenInfo& w, const address_t& h, blknum_t b);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init("binary", quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (!options.tokenInfo.empty()) {
            if (options.watches.size() > 1)
                cout << "[\n";
            address_t holder = (options.tokenInfo == "balanceOf" ? options.holders[0] : "");
            for (auto watch : options.watches) {
                string_q blocks = options.getBlockNumList();
                while (!blocks.empty()) {
                    blknum_t blockNum = str_2_Uint(nextTokenClear(blocks, '|'));
                    cout << getTokenInfo(options.tokenInfo, watch, holder, blockNum) << "\n";
                    //TODO(tjayrush): global data
                    static size_t cnt=0;
                    if (cnt++ < options.watches.size() - 1)
                        cout << ",";
                    cout << "\n";
                }
            }
            if (options.watches.size() > 1)
                cout << "]\n";

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
    for (auto token : options.watches) {
        if (!options.asData)
            cout << "\n  For token contract: " << bBlue << token.address << cOff << "\n";

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
                        cout << blockNum << "\t" << token.address << "\t" << holder << "\t" << dispBal << "\n";
                    } else {
                        cout << "    Balance for account " << cGreen << holder << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << dispBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else {
                    if (options.asData) {
                        cerr << blockNum << "\t" << token.address << "\t" << holder << "         \r";
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
        for (auto token : options.watches) {
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
                        cout << blockNum << "\t" << token.address << "\t" << holder << "\t" << dispBal << "\n";
                    } else {
                        cout << "    Balance of token contract " << cGreen << token.address << cOff;
                        cout << " at block " << cTeal << blockNum << cOff;
                        cout << " is " << cYellow << dispBal << cOff << "\n";
                    }
                    needsNewline = false;
                } else {
                    if (options.asData) {
                        cerr << blockNum << "\t" << token.address << "\t" << holder << "         \r";
                    } else {
                        cerr << "    Balance of token contract " << cGreen << token.address << cOff;
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
string_q getTokenInfo(const string_q& which, CTokenInfo& token, const address_t& holder, blknum_t blockNum) {

    if (!isAddress(token.address))
        return "";
    ASSERT(holder.empty() || isAddress(holder));

    // we need this, so let's get it
    token.abi_spec.loadAbiByAddress(token.address);

    string_q cmd;
    string_q encoding;
    ostringstream os;
    if (which % "all") {
        for (auto opt : infoOptions) {
            opt = nextTokenClear(opt,'|');
            if (opt != "balanceOf" && opt != "all" && isValidInfo(opt, encoding)) {
                cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"[CMD]\"}, \"[BLOCK]\"]";
                replace(cmd, "[TOKEN]",  token.address);
                replace(cmd, "[CMD]",    encoding);  // encoded data for the transaction
                replace(cmd, "[BLOCK]",  uint_2_Hex(blockNum));
                CFunction ret;
                token.abi_spec.articulateOutputs(encoding, callRPC("eth_call", cmd, false), ret);
                os << "\"" << opt << "\": \"" << (ret.outputs.size() ? ret.outputs[0].value : "") << "\",\n";
            }
        }
        return "{\n\t" + trim(substitute(trim(trim(os.str(),'\n'),',')+"\n","\n","\n\t"),'\t') + "}";

    } else {
        if (isValidInfo(which, encoding)) {
            cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"[CMD][HOLDER]\"}, \"[BLOCK]\"]";
            if (which != "balanceOf")
                cmd = substitute(cmd, "[HOLDER]", "");
            replace(cmd, "[TOKEN]",  token.address);
            replace(cmd, "[CMD]",    encoding);
            replace(cmd, "[HOLDER]", padLeft(extract(holder, 2), 64, '0'));  // encoded data (may be empty)
            replace(cmd, "[BLOCK]",  uint_2_Hex(blockNum));
        }
    }

    string_q result = callRPC("eth_call", cmd, false);
//    if (isTestMode())
//        cerr << "eth_call for " << which << endl << "\twith: " << cmd << endl << "\treturned: " << result << endl;
    CFunction ret;
    token.abi_spec.articulateOutputs(encoding, result, ret);
    os << "{\n    \"" << which << "\": \"" << (ret.outputs.size() ? ret.outputs[0].value : "") << "\"\n}";
    return (which == "balanceOf" ? (ret.outputs.size() ? ret.outputs[0].value : "") : os.str());
}

//-------------------------------------------------------------------------
bool isValidInfo(const string_q which, string_q& encoding) {
    if (which == "all")
        return true;
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
CStringArray infoOptions = {
    "name|0x06fdde03",
    "totalSupply|0x18160ddd",
    "balanceOf|0x70a08231",
    "decimals|0x313ce567",
    "version|0x54fd4d50",
    "symbol|0x95d89b41",
    "all"
};
