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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include "etherlib.h"

namespace qblocks {

//-----------------------------------------------------
class IPCSocket
{
public:
    IPCSocket   ( SFString const& _path );
    ~IPCSocket   ( );
    SFString        sendRequest ( SFString const& _req);

    SFString const& path        ( void ) const;

private:
    FILE *m_fp;
    SFString m_path;
    int m_socket;
};

//-----------------------------------------------------
IPCSocket::~IPCSocket(void)
{
    close(m_socket);
    fclose(m_fp);
}

//-----------------------------------------------------
SFString const& IPCSocket::path( void ) const
{
    return m_path;
}

//-----------------------------------------------------
class RPCSession
{
public:
    CReceipt           eth_getTransactionReceipt ( SFString const& _transactionHash );
    SFString           eth_getCode               ( SFString const& _address, SFString const& _blockNumber );
    SFString           eth_getBalanc e            ( SFString const& _address, SFString const& _blockNumber );
    SFString           eth_getSt orageRoot        ( SFString const& _address, SFString const& _blockNumber );
    SFString           rpcCall                   (SFString const& _methodName, SFString const& _args);

private:
    RPCSession               (SFString const& _path);

    IPCSocket m_ipcSocket;
    size_t m_rpcSequence = 1;
};
}
