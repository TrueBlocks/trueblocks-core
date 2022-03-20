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
#include <thread>  // NOLINT
#include "node.h"
#include "node_curl.h"
#include "node_curl_debug.h"

namespace qblocks {

#ifdef DEBUG_RPC
//-------------------------------------------------------------------------
string_q showResult(const string_q& result, const string_q& msg) {
    cout.flush();
    cerr.flush();

    ostringstream os;
    if (!msg.empty()) {
        os << msg << endl;
    }

    if (!result.empty()) {
        os << result << endl;
        // string_q res = result;
        // cleanString(res, false);
        // replaceAny(res, "{}[]", "");
        // replaceAll(res, ",", "\n");
        // replaceAll(res, "\"result\":", "");
        // CStringArray lines;
        // explode(lines, res, '\n');
        // sort(lines.begin(), lines.end());
        // os << "result:\n";
        // for (auto line : lines) {
        //     replaceAny(line, "\n", "");
        //     if (!line.empty() && !contains(line, "\"id\":") && !contains(line, "\"chainId\":") &&
        //         !contains(line, "\"condition\":") && !contains(line, "\"creates\":") &&
        //         !contains(line, "\"publicKey\":") && !contains(line, "\"jsonrpc\":") && !contains(line, "\"raw\":")
        //         && !contains(line, "\"standardV\":") && !contains(line, "\"transactionLogIndex\":") &&
        //         !contains(line, "\"type\":") && !contains(line, "\"author\":") && !contains(line, "\"sealFields\":"))
        //         os << "--[" << line << "]--" << endl;
        // }
    }

    return os.str();
}

string_q debugCurlCommand(const string_q& id, const string_q& method, const string_q& params, const string_q& url) {
    ostringstream os;
    os << endl;
    os << string_q(120, '-') << endl;
    os << "curl ";
    os << "--data '{\"method\":\"" << method;
    os << "\",\"params\":" << params << ",\"id\":\"" << id << "\",\"jsonrpc\":\"2.0\"}' ";
    os << "-H \"Content-Type: application/json\" ";
    os << "-X POST " << url;
    return os.str();
}
#else
bool unusedToAvoidWarning = false;
#endif

}  // namespace qblocks