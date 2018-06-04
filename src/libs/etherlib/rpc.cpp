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
#if 0
#include <stdio.h>
#include "rpc.h"

namespace qblocks {

//-----------------------------------------------------
IPCSocket::IPCSocket(SFString const& _path): m_path(_path)
{
    if (_path.length() >= sizeof(sockaddr_un::sun_path))
    { cerr << "Error opening IPC: socket path is too long!" << "\n"; exit(0); }

    struct sockaddr_un saun;
    memset(&saun, 0, sizeof(sockaddr_un));
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, (const char*)_path);
    saun.sun_len = sizeof(struct sockaddr_un);

    if ((m_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    { cerr << "Error creating IPC socket object" << "\n"; exit(0); }

    if (connect(m_socket, reinterpret_cast<struct sockaddr const*>(&saun), sizeof(struct sockaddr_un)) < 0)
    { cerr << "Error connecting to IPC socket: " << "\n"; exit(0); }

    m_fp = fdopen(m_socket, "r");
}

//-----------------------------------------------------
SFString IPCSocket::sendRequest(SFString const& _req)
{
    send(m_socket, (const char*)_req, _req.length(), 0);

    char c;
    SFString response;
    while ((c = fgetc(m_fp)) != EOF)
    {
        if (c != '\n')
            response += c;
        else
            break;
    }
    return response;
}

//-----------------------------------------------------
CReceipt RPCSession::eth_getTransactionReceipt(SFString const& _transactionHash)
{
    CReceipt receipt;

    SFString const result = rpcCall("eth_getTransactionReceipt", { _transactionHash });
    if (result.empty())
    {
        cerr << "Result from eth_getTransactionReceipt call is empty. Quitting...\n";
        exit(0);
    }
    receipt.gasUsed = result["gasUsed"];
    receipt.contractAddress = result["contractAddress"];
    for (auto const& log: result["logs"])
    {
        LogEntry entry;
        entry.address = log["address"];
        entry.data = log["data"];
        for (auto const& topic: log["topics"])
            entry.topics.push_back(topic);
        receipt.logEntries.push_back(entry);
    }
    return receipt;
}

//-----------------------------------------------------
SFString RPCSession::rpcCall(SFString const& _methodName, SFString const& _args)
{
    SFString request = "{\"jsonrpc\":\"2.0\",\"method\":\"" + _methodName + "\",\"params\":[";
    SFString args = _args;
    while (!args.empty())
    {
        request += "\"" + nextTokenClear(args, '|') + "\"";
        if (!args.empty())
            request += ",";
    }
    request += "],\"id\":" + asString(m_rpcSequence) + "}";
    ++m_rpcSequence;

    SFString result = m_ipcSocket.sendRequest(request);
    if (result.Contains("error"))
    {
        cerr << "Error on JSON-RPC call: " << result << "\n";
        exit(0);
    }
    return result;
}

//-----------------------------------------------------
SFString RPCSession::eth_getCode(SFString const& _address, SFString const& _blockNumber)
{
    return rpcCall("eth_getCode", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
SFString RPCSession::eth_getBalanc e(SFString const& _address, SFString const& _blockNumber)
{
    return rpcCall("eth_getBalanc e", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
SFString RPCSession::eth_getSt orageRoot(SFString const& _address, SFString const& _blockNumber)
{
    return rpcCall("eth_getSt orageRoot", _address + "|" + _blockNumber);
}

//-----------------------------------------------------
RPCSession::RPCSession(const SFString& _path) : m_ipcSocket(_path)
{
}

}  // namespace qblocks

#endif

void unused2(void) {}
