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
void tokenlib_init(void) {
    QApprovalEvent::registerClass();
    QApprovalForAllEvent::registerClass();
    QTransferEvent::registerClass();
    QApprove::registerClass();
    QApproveAndCall::registerClass();
    QOwnerOf::registerClass();
    QSafeTransferFrom::registerClass();
    QSafeTransferFromToke::registerClass();
    QSetApprovalForAll::registerClass();
    QTransfer::registerClass();
    QTransferFrom::registerClass();
    walletlib_init();
}

//-----------------------------------------------------------------------
const string_q func_allowance_qb = "0xdd62ed3e";
const string_q func_approve_qb = "0x095ea7b3";
const string_q func_approveAndCall_qb = "0xcae9ca51";
const string_q func_balanceOf_qb = "0x70a08231";
const string_q func_decimals_qb = "0x313ce567";
const string_q func_exists_qb = "0x4f558e79";
const string_q func_getApproved_qb = "0x081812fc";
const string_q func_isApprovedForAll_qb = "0xe985e9c5";
const string_q func_name_qb = "0x06fdde03";
const string_q func_ownerOf_qb = "0x6352211e";
const string_q func_safeTransferFrom_qb = "0x42842e0e";
const string_q func_safeTransferFromToke_qb = "0xb88d4fde";
const string_q func_setApprovalForAll_qb = "0xa22cb465";
const string_q func_symbol_qb = "0x95d89b41";
const string_q func_totalSupply_qb = "0x18160ddd";
const string_q func_transfer_qb = "0xa9059cbb";
const string_q func_transferFrom_qb = "0x23b872dd";
const string_q func_version_qb = "0x54fd4d50";

//-----------------------------------------------------------------------
const CTransaction *promoteToToken(const CTransaction *p) {
    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_approve_qb) {
            // function approve(address _spender, uint256 _value)
            // 0x095ea7b3
            QApprove *a = new QApprove;
            *(CTransaction*)a = *p;  // NOLINT
            a->_spender = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->function = toFunction("approve", params, nItems, items);
            return a;

        } else if (encoding == func_approveAndCall_qb) {
            // function approveAndCall(address _spender, uint256 _value, bytes _extraData)
            // 0xcae9ca51
            QApproveAndCall *a = new QApproveAndCall;
            *(CTransaction*)a = *p;  // NOLINT
            a->_spender = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_extraData = extract(params, 2*64);
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            a->function = toFunction("approveAndCall", params, nItems, items);
            return a;

        } else if (encoding == func_ownerOf_qb) {
            // function ownerOf(uint256 _tokenId)
            // 0x6352211e
            QOwnerOf *a = new QOwnerOf;
            *(CTransaction*)a = *p;  // NOLINT
            a->_tokenId = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->function = toFunction("ownerOf", params, nItems, items);
            return a;

        } else if (encoding == func_safeTransferFrom_qb) {
            // function safeTransferFrom(address _from, address _to, uint256 _tokenId)
            // 0x42842e0e
            QSafeTransferFrom *a = new QSafeTransferFrom;
            *(CTransaction*)a = *p;  // NOLINT
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_tokenId = str_2_Wei("0x" + extract(params, 2*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->function = toFunction("safeTransferFrom", params, nItems, items);
            return a;

        } else if (encoding == func_safeTransferFromToke_qb) {
            // function safeTransferFrom(address _from, address _to, uint256 _tokenId, bytes _data)
            // 0xb88d4fde
            QSafeTransferFromToke *a = new QSafeTransferFromToke;
            *(CTransaction*)a = *p;  // NOLINT
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_tokenId = str_2_Wei("0x" + extract(params, 2*64, 64));
            a->_data = extract(params, 3*64);
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            a->function = toFunction("safeTransferFromToke", params, nItems, items);
            return a;

        } else if (encoding == func_setApprovalForAll_qb) {
            // function setApprovalForAll(address _operator, bool _approved)
            // 0xa22cb465
            QSetApprovalForAll *a = new QSetApprovalForAll;
            *(CTransaction*)a = *p;  // NOLINT
            a->_operator = str_2_Addr(extract(params, 0*64, 64));
            a->_approved = str_2_Int(extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "bool";
            a->function = toFunction("setApprovalForAll", params, nItems, items);
            return a;

        } else if (encoding == func_transfer_qb) {
            // function transfer(address _to, uint256 _value)
            // 0xa9059cbb
            QTransfer *a = new QTransfer;
            *(CTransaction*)a = *p;  // NOLINT
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->function = toFunction("transfer", params, nItems, items);
            return a;

        } else if (encoding == func_transferFrom_qb) {
            // function transferFrom(address _from, address _to, uint256 _value)
            // 0x23b872dd
            QTransferFrom *a = new QTransferFrom;
            *(CTransaction*)a = *p;  // NOLINT
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 2*64, 64));
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
const string_q evt_Approval_qb = "0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925";
const string_q evt_ApprovalForAll_qb = "0x17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c31";
const string_q evt_Transfer_qb = "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef";

//-----------------------------------------------------------------------
const CLogEntry *promoteToTokenEvent(const CLogEntry *p) {
    if (!p)
        return NULL;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);
        // EXISTING_CODE
        // EXISTING_CODE

        if (topic_2_Str(p->topics[0]) % evt_Approval_qb) {
            // event Approval(address indexed _owner, address indexed _spender, uint256 _value)
            // 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925
            QApprovalEvent *a = new QApprovalEvent;
            *(CLogEntry*)a = *p;  // NOLINT
            a->_owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_spender = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_value = str_2_Wei("0x" + extract(data, 0*64, 64));
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ApprovalForAll_qb) {
            // event ApprovalForAll(address indexed _owner, address indexed _operator, bool _approved)
            // 0x17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c31
            QApprovalForAllEvent *a = new QApprovalForAllEvent;
            *(CLogEntry*)a = *p;  // NOLINT
            a->_owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_operator = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_approved = str_2_Int("0x" + extract(data, 0*64, 64));
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Transfer_qb) {
            // event Transfer(address indexed _from, address indexed _to, uint256 _value)
            // 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef
            QTransferEvent *a = new QTransferEvent;
            *(CLogEntry*)a = *p;  // NOLINT
            a->_from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_value = str_2_Wei("0x" + extract(data, 0*64, 64));
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
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"version","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"},{"name":"_extraData","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_tokenId","type":"uint256"}],"name":"ownerOf","outputs":[{"name":"_owner","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_tokenId","type":"uint256"}],"name":"exists","outputs":[{"name":"_exists","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_tokenId","type":"uint256"}],"name":"getApproved","outputs":[{"name":"_operator","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_operator","type":"address"},{"name":"_approved","type":"bool"}],"name":"setApprovalForAll","outputs":[{"name":"x","type":"y"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_operator","type":"address"}],"name":"isApprovedForAll","outputs":[{"name":"_ret","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_tokenId","type":"uint256"}],"name":"safeTransferFrom","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_tokenId","type":"uint256"},{"name":"_data","type":"bytes"}],"name":"safeTransferFrom","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"_initialAmount","type":"uint256"},{"name":"_tokenName","type":"string"},{"name":"_decimalUnits","type":"uint8"},{"name":"_tokenSymbol","type":"string"}],"type":"constructor"},{"payable":false,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_spender","type":"address"},{"indexed":false,"name":"_value","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_owner","type":"address"},{"indexed":true,"name":"_operator","type":"address"},{"indexed":false,"name":"_approved","type":"bool"}],"name":"ApprovalForAll","type":"event"}]

*/
