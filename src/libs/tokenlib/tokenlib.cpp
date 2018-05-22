/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "tokenlib.h"
#include "walletlib.h"

//-----------------------------------------------------------------------
void tokenlib_init(void)
{
    
    QApprovalEvent::registerClass();
    QTransferEvent::registerClass();
    QApprove::registerClass();
    QApproveAndCall::registerClass();
    QTransfer::registerClass();
    QTransferFrom::registerClass();
    walletlib_init();
}

//-----------------------------------------------------------------------
const SFString func_allowance_qb = "0xdd62ed3e";
const SFString func_approve_qb = "0x095ea7b3";
const SFString func_approveAndCall_qb = "0xcae9ca51";
const SFString func_balanceOf_qb = "0x70a08231";
const SFString func_decimals_qb = "0x313ce567";
const SFString func_name_qb = "0x06fdde03";
const SFString func_symbol_qb = "0x95d89b41";
const SFString func_totalSupply_qb = "0x18160ddd";
const SFString func_transfer_qb = "0xa9059cbb";
const SFString func_transferFrom_qb = "0x23b872dd";
const SFString func_version_qb = "0x54fd4d50";

//-----------------------------------------------------------------------
const CTransaction *promoteToToken(const CTransaction *p)
{
    if (p && (p->input.length()>=10 || p->input == "0x"))
    {
        SFString items[256];
        uint32_t nItems=0;

        SFString encoding = p->input.substr(0,10);
        SFString params   = p->input.substr(10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_approve_qb)
        {
            // function approve(address _spender, uint256 _value)
            // 0x095ea7b3
            QApprove *a = new QApprove;
            *(CTransaction*)a = *p; // copy in
            a->_spender = toAddress(params.substr(0*64,64));
            a->_value = toWei("0x"+params.substr(1*64,64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->function = toFunction("approve", params, nItems, items);
            return a;

        } else if (encoding == func_approveAndCall_qb)
        {
            // function approveAndCall(address _spender, uint256 _value, bytes _extraData)
            // 0xcae9ca51
            QApproveAndCall *a = new QApproveAndCall;
            *(CTransaction*)a = *p; // copy in
            a->_spender = toAddress(params.substr(0*64,64));
            a->_value = toWei("0x"+params.substr(1*64,64));
            a->_extraData = params.substr(2*64);
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            a->function = toFunction("approveAndCall", params, nItems, items);
            return a;

        } else if (encoding == func_transfer_qb)
        {
            // function transfer(address _to, uint256 _value)
            // 0xa9059cbb
            QTransfer *a = new QTransfer;
            *(CTransaction*)a = *p; // copy in
            a->_to = toAddress(params.substr(0*64,64));
            a->_value = toWei("0x"+params.substr(1*64,64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->function = toFunction("transfer", params, nItems, items);
            return a;

        } else if (encoding == func_transferFrom_qb)
        {
            // function transferFrom(address _from, address _to, uint256 _value)
            // 0x23b872dd
            QTransferFrom *a = new QTransferFrom;
            *(CTransaction*)a = *p; // copy in
            a->_from = toAddress(params.substr(0*64,64));
            a->_to = toAddress(params.substr(1*64,64));
            a->_value = toWei("0x"+params.substr(2*64,64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->function = toFunction("transferFrom", params, nItems, items);
            return a;

        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // returns NULL if not promoted
    return promoteToWallet(p);
}

//-----------------------------------------------------------------------
const SFString evt_Approval_qb = "0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925";
const SFString evt_Transfer_qb = "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef";

//-----------------------------------------------------------------------
const CLogEntry *promoteToTokenEvent(const CLogEntry *p)
{
    if (!p)
        return NULL;

    uint32_t nTops = p->topics.getCount();
    if (nTops>0) // the '0'th topic is the event signature
    {
        SFString data = p->data.substr(2);
        // EXISTING_CODE
        // EXISTING_CODE

        if (fromTopic(p->topics[0]) % evt_Approval_qb)
        {
            // event Approval(address indexed _owner, address indexed _spender, uint256 _value)
            // 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925
            QApprovalEvent *a = new QApprovalEvent;
            *(CLogEntry*)a = *p; // copy in
            a->_owner = toAddress(nTops > 1 ? fromTopic(p->topics[1]) : "");
            a->_spender = toAddress(nTops > 2 ? fromTopic(p->topics[2]) : "");
            a->_value = toWei("0x"+data.substr(0*64,64));
            return a;

        } else if (fromTopic(p->topics[0]) % evt_Transfer_qb)
        {
            // event Transfer(address indexed _from, address indexed _to, uint256 _value)
            // 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef
            QTransferEvent *a = new QTransferEvent;
            *(CLogEntry*)a = *p; // copy in
            a->_from = toAddress(nTops > 1 ? fromTopic(p->topics[1]) : "");
            a->_to = toAddress(nTops > 2 ? fromTopic(p->topics[2]) : "");
            a->_value = toWei("0x"+data.substr(0*64,64));
            return a;

        }
        // fall through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // returns NULL if not promoted
    return promoteToWalletEvent(p);
}

/*
 ABI for addr : 0xTokenLib
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"_initialAmount","type":"uint256"},{"name":"_tokenName","type":"string"},{"name":"_decimalUnits","type":"uint8"},{"name":"_tokenSymbol","type":"string"}],"type":"constructor"},{"payable":false,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]

*/
