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

//---------------------------------------------------------------------------------------------------
bool COptions::handle_call(void) {
    CStringArray callVariables;
    replaceAll(call, "|", "!");
    replaceAll(call, " !", "!");
    replaceAll(call, "! ", "!");
    explode(callVariables, call, '!');

    string_q contract = callVariables.size() > 0 ? callVariables[0] : "";
    contract = proxy_for.empty() ? contract : proxy_for;
    if (contract.empty() || !isAddress(contract))
        return usage("The first argument for the --call option must be a smart contract.");

    if (callVariables.size() < 2) {
        // a convienience to the user shows the ABI if running in terminal
        if (!isTestMode() && !isApiMode()) {
            ostringstream cmd;
            cmd << "chifra abis " << contract;
            if (system(cmd.str().c_str())) {
            }  // Don't remove cruft. Silences compiler warnings
            return false;
        }
        return usage("You must provide either a four-byte code or a function signature for the smart contract.");
    }

    theCall.address = contract;
    theCall.abi_spec.loadAbisFromKnown();
    theCall.abi_spec.loadAbiFromEtherscan(theCall.address);

    string_q fourByte = callVariables[1];
    if (!isHexStr(fourByte)) {
        // This is not a hex string...it better be a signature
        if (!contains(fourByte, "("))
            return usage("You must provide either a four-byte code or a function signature for the smart contract.");
        // It is a signature
        fourByte = startsWith(fourByte, "function ") ? fourByte : "function " + fourByte;
        replaceReverse(fourByte, ")(", ") returns (");
        if (!contains(fourByte, "returns"))
            return usage(
                "Please provide a return value with your signature. For example, \"getBalance() returns (uint)\"");
        CFunction func;
        fromDefinition(func, fourByte);
        theCall.abi_spec.loadAbiAddInterface(func);
        fourByte = func.encoding;
        theCall.encoding = fourByte;
        theCall.bytes += callVariables.size() > 2 ? callVariables[2] : "";
    } else {
        size_t len = fourByte.length();
        if (len < 10) {
            ostringstream os;
            os << "The four byte code you presented (" << fourByte << ") is only " << len << " bytes long.";
            return usage(os.str());
        } else if (len == 10) {
            // A regular four byte bytes follow in the next part if present...
            theCall.encoding = fourByte;
            theCall.bytes += callVariables.size() > 2 ? callVariables[2] : "";
        } else {
            // the entire call data may be in the second part
            theCall.encoding = fourByte.substr(0, 10);
            theCall.bytes = fourByte.substr(10);
        }
    }
    if (theCall.bytes.length() % 64) {
        ostringstream os;
        os << "The length (" << theCall.bytes.length() << ") of the bytes you presented (" << theCall.bytes
           << ") is not a multiple of 32 bytes.";
        return usage(os.str());
    }

    string_q fmt = STR_DISPLAY_ETHCALL;
    replace(fmt, "[{ENCODING}}", "[{SIGNATURE}]\t[{ENCODING}]");
    configureDisplay("getState", "CEthState", fmt);

    manageFields("CParameter:all", FLD_HIDE);
    manageFields("CParameter:name,signature,encoding,outputs", FLD_SHOW);

    manageFields("CFunction:stateMutability,type,constant", FLD_HIDE);

    manageFields("CEthCall:abi_spec,deployed", FLD_HIDE);
    manageFields("CEthCall:blockNumber,address,signature,compressedResult", FLD_SHOW);
    if (expContext().exportFmt == JSON1 || expContext().exportFmt == API1) {
        manageFields("CEthCall:signature", FLD_HIDE);
        manageFields("CEthCall:callResult", FLD_SHOW);
    }

    return true;
}

//--------------------------------------------------------------------------------
string_q cleanInput(const string_q& cmd) {
    if (!contains(cmd, "--call"))
        return cmd;

    ostringstream out;

    // The --call option allows for spaces in the function signature, so we need to remove them here
    bool inside = false;
    string_q str = cmd;
    replaceAll(str, "  ", " ");
    replaceAll(str, "( ", "(");
    replaceAll(str, " )", ")");
    char* s = (char*)str.c_str();
    while (*s) {
        if (*s == '(') {
            inside = true;
            out << *s;
        } else if (*s == ')') {
            inside = false;
            out << *s;
        } else if (*s == ' ') {
            out << (inside ? "," : " ");
        } else {
            out << *s;
        }
        s++;
    }
    return out.str();
}
