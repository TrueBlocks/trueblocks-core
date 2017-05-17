#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
    SFString           eth_getBalance            ( SFString const& _address, SFString const& _blockNumber );
    SFString           eth_getStorageRoot        ( SFString const& _address, SFString const& _blockNumber );
    SFString           rpcCall                   (SFString const& _methodName, SFString const& _args);

private:
    RPCSession               (SFString const& _path);

    IPCSocket m_ipcSocket;
    size_t m_rpcSequence = 1;
};
}
