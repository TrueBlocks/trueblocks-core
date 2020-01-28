/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "rpc.h"

namespace qblocks {

void unused2(void) {
}

}  // namespace qblocks

#if 0
//-----------------------------------------------------
IPCSocket::IPCSocket(const string_q& _path): m_path(_path) {
    if (_path.length() >= sizeof(sockaddr_un::sun_path)) {
        cerr << "Error opening IPC: socket path is too long!" << "\n";
        return;
    }

    struct sockaddr_un saun;
    memset(&saun, 0, sizeof(sockaddr_un));
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, _path.c_str());  // NOLINT
    saun.sun_len = sizeof(struct sockaddr_un);

    if ((m_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        cerr << "Error creating IPC socket object" << "\n";
        return;
    }

    if (connect(m_socket, reinterpret_cast<struct sockaddr const*>(&saun), sizeof(struct sockaddr_un)) < 0) {
        cerr << "Error connecting to IPC socket: " << "\n";
        return;
    }

    m_fp = fdopen(m_socket, "r");
}

//-----------------------------------------------------
string_q IPCSocket::sendRequest(const string_q& _req) {
    send(m_socket, (const char*)_req, _req.length(), 0);

    char c;
    string_q response;
    while ((c = fgetc(m_fp)) != EOF) {
        if (c != '\n')
            response += c;
        else
            break;
    }
    return response;
}

//-----------------------------------------------------
CReceipt RPCSession::eth_get TransactionReceipt(const string_q& _transactionHash) {
    CReceipt receipt;

    string_q const result = rpcCall("eth_getTransactionReceipt", { _transactionHash });
    if (result.empty()) {
        cerr << "Result from eth_getTransactionReceipt call is empty. Quitting...\n";
        return;
    }
    receipt.gasUsed = result["gasUsed"];
    receipt.contractAddress = result["contractAddress"];
    for (auto const& log : result["logs"]) {
        LogEntry entry;
        entry.address = log["address"];
        entry.data = log["data"];
        for (auto const& topic : log["topics"])
            entry.topics.push_back(topic);
        receipt.logEntries.push_back(entry);
    }
    return receipt;
}

//-----------------------------------------------------
string_q RPCSession::rpcCall(const string_q& _methodName, const string_q& _args) {
    string_q request = "{\"jsonrpc\":\"2.0\",\"method\":\"" + _methodName + "\",\"params\":[";
    string_q args = _args;
    while (!args.empty()) {
        request += "\"" + nextTokenClear(args, '|') + "\"";
        if (!args.empty())
            request += ",";
    }
    request += "],\"id\":" + int_2_Str(m_rpcSequence) + "}";
    ++m_rpcSequence;

    string_q result = m_ipcSocket.sendRequest(request);
    if (contains(result, "error")) {
        cerr << "Error on JSON-RPC call: " << result << "\n";
        return;
    }
    return result;
}

//-----------------------------------------------------
string_q RPCSession::eth_get Code(const string_q& _address, const string_q& _blockNumber) {
    return rpcCall("eth_get Code", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
string_q RPCSession::eth_get Balance(const string_q& _address, const string_q& _blockNumber) {
    return rpcCall("eth_getBalanc e", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
string_q RPCSession::eth_getSt orageRoot(const string_q& _address, const string_q& _blockNumber) {
    return rpcCall("eth_getSt orageRoot", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
RPCSession::RPCSession(const string_q& _path) : m_ipcSocket(_path) {
}

#endif
