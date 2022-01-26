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
#include "chain.h"

extern const char* STR_THESTRING;
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    CChain::registerClass();
    loadEnvironmentPaths();
    etherlib_init(quickQuitHandler);
    CStringArray lines, header;
    asciiFileToLines("./chains.csv", lines);
    for (auto line : lines) {
        if (startsWith(line, '#'))
            continue;
        if (header.empty()) {
            explode(header, line, ',');
        } else {
            CChain chain;
            chain.parseCSV(header, line);
            establishFolder("config/" + chain.chain_name + "/trueBlocks.toml");
            string_q fmt = substitute(STR_THESTRING, "[{VERSION}]", getVersionStr(false, false));
            string_q res = chain.Format(fmt);
            while (contains(res, "++")) {
                replace(res, "++", "{");
                replace(res, "++", "}");
            }
            while (contains(res, "**")) {
                replace(res, "**", "[");
                replace(res, "**", "]");
            }
            replace(res, "ipfsGateway = \"\"", "ipfsGateway = \"<not_set>\"");
            stringToAsciiFile("config/" + chain.chain_name + "/trueBlocks.toml", res);
            cout << "Wrote " << chain.chain_name << endl;
        }
    }
    etherlib_cleanup();
    return 1;
}

const char* STR_THESTRING =
    "**version**\n"
    "# Do not edit this value\n"
    "current = \"[{VERSION}]\"\n"
    "\n"
    "# only the following values are customizable per chain"
    "**settings**\n"
    "rpcProvider = \"[{RPC_PROVIDER}]\"\n"
    "chainId = \"[{CHAIN_ID}]\"\n"
    "symbol = \"[{SYMBOL}]\"\n"
    "ipfsGateway = \"[{IPFS_GATEWAY}]\"\n"
    "remoteExplorer = \"[{REMOTE_EXPLORER}]\"\n";
