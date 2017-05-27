/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
 /*
  *
  * This code was generated automatically from grabABI and makeClass from the
  * 'TokenLib' ABI file. You may edit the file,
  * but keep your edits inside the 'EXISTING_CODE' tags.
  *
  */
#include "tokenlib.h"
#include "walletlib.h"

//-----------------------------------------------------------------------
void tokenlib_init(void)
{
    QApprovalEvent::registerClass();
    QTransferEvent::registerClass();
    QApprove::registerClass();
    QTransfer::registerClass();
    QTransferFrom::registerClass();
    walletlib_init();
}

//-----------------------------------------------------------------------
const SFString func_approve_qb = "0x095ea7b3";
const SFString func_transfer_qb = "0xa9059cbb";
const SFString func_transferFrom_qb = "0x23b872dd";

//-----------------------------------------------------------------------
const CTransaction *promoteToToken(const CTransaction *p)
{
    if (p && (p->input.length()>=10 || p->input == "0x"))
    {
        SFString items[256];
        int nItems=0;

        SFString encoding = p->input.Left(10);
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
            a->function = "approve" + parse(params,nItems,items);
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
            a->function = "transfer" + parse(params,nItems,items);
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
            a->function = "transferFrom" + parse(params,nItems,items);
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

    uint32_t nTopics = p->topics.getCount();
    if (nTopics>0) // the '0'th topic is the event signature
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
            a->_owner = toAddress(nTopics > 1 ? fromTopic(p->topics[1]) : "");
            a->_spender = toAddress(nTopics > 2 ? fromTopic(p->topics[2]) : "");
            a->_value = toWei("0x"+data.substr(0*64,64));
            return a;

        } else if (fromTopic(p->topics[0]) % evt_Transfer_qb)
        {
            // event Transfer(address indexed _from, address indexed _to, uint256 _value)
            // 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef
            QTransferEvent *a = new QTransferEvent;
            *(CLogEntry*)a = *p; // copy in
            a->_from = toAddress(nTopics > 1 ? fromTopic(p->topics[1]) : "");
            a->_to = toAddress(nTopics > 2 ? fromTopic(p->topics[2]) : "");
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
//ABI From 'TokenLib'
[{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"}]
*/
