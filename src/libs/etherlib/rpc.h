#pragma once
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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include "etherlib.h"

namespace qblocks {

//-----------------------------------------------------
class IPCSocket {
  public:
    explicit IPCSocket(const string_q& _path);
    ~IPCSocket(void);
    string_q sendRequest(const string_q& _req);
    const string_q& path(void) const;

  private:
    FILE* m_fp;
    string_q m_path;
    int m_socket;
};

//-----------------------------------------------------
IPCSocket::~IPCSocket(void) {
    close(m_socket);
    fclose(m_fp);
}

//-----------------------------------------------------
const string_q& IPCSocket::path(void) const {
    return m_path;
}

//-----------------------------------------------------
class RPCSession {
  public:
    // CReceipt e th_getTransactionReceipt(const string_q& _transactionHash);
    //        string_q e th_get Code(const string_q& _address, const string_q& _blockNumber);
    //        string_q e th_get Balance(const string_q& _address, const string_q& _blockNumber);
    //        string_q e th_get StorageRoot(const string_q& _address, const string_q& _blockNumber);
    string_q rpcCall(const string_q& _methodName, const string_q& _args);

  private:
    explicit RPCSession(const string_q& _path);
    IPCSocket m_ipcSocket;
    size_t m_rpcSequence = 1;
};
}  // namespace qblocks
