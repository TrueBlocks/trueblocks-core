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
#include "parselib.h"

//-----------------------------------------------------------------------
void parselib_init(QUITHANDLER qh) {
    etherlib_init(qh);
    CBaseTransferedEvent::registerClass();
    CNewBaseEvent::registerClass();
    CNewHandleEvent::registerClass();
    COwnershipTransferredEvent::registerClass();
    CChangePrice::registerClass();
    CGetBaseQuick::registerClass();
    CRegisterBase::registerClass();
    CRegisterHandle::registerClass();
    CTransferBase::registerClass();
    CTransferOwnership::registerClass();
    CWithdraw::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_baseRegistred = "0xac08e444";
const string_q func_changePrice = "0xa2b40d19";
const string_q func_doesOwnBase = "0x3acbd9eb";
const string_q func_findAddress = "0x378bc89d";
const string_q func_getBaseQuick = "0x81252680";
const string_q func_getPrice = "0x98d5fdca";
const string_q func_handleIndex = "0x48986e4c";
const string_q func_isRegistered = "0x27258b22";
const string_q func_owner = "0x8da5cb5b";
const string_q func_ownsBase = "0x7792e7f1";
const string_q func_price = "0xa035b1fe";
const string_q func_registerBase = "0x265a3afd";
const string_q func_registerHandle = "0x441230e4";
const string_q func_transferBase = "0xe22325ad";
const string_q func_transferOwnership = "0xf2fde38b";
const string_q func_withdraw = "0x3ccfd60b";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_changePrice) {
            // function changePrice(uint256 _price)
            // 0xa2b40d19
            CChangePrice *a = new CChangePrice;
            a->CTransaction::operator=(*p);
            a->_price = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("changePrice", params, nItems, items);
            return a;

        } else if (encoding == func_getBaseQuick) {
            // function getBaseQuick(bytes32 _base)
            // 0x81252680
            CGetBaseQuick *a = new CGetBaseQuick;
            a->CTransaction::operator=(*p);
            a->_base = extract(params, 0*64, 64);
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("getBaseQuick", params, nItems, items);
            return a;

        } else if (encoding == func_registerBase) {
            // function registerBase(bytes32 _base)
            // 0x265a3afd
            CRegisterBase *a = new CRegisterBase;
            a->CTransaction::operator=(*p);
            a->_base = extract(params, 0*64, 64);
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("registerBase", params, nItems, items);
            return a;

        } else if (encoding == func_registerHandle) {
            // function registerHandle(bytes32 _base, bytes32 _handle, address _addr)
            // 0x441230e4
            CRegisterHandle *a = new CRegisterHandle;
            a->CTransaction::operator=(*p);
            a->_base = extract(params, 0*64, 64);
            a->_handle = extract(params, 1*64, 64);
            a->_addr = str_2_Addr(extract(params, 2*64, 64));
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("registerHandle", params, nItems, items);
            return a;

        } else if (encoding == func_transferBase) {
            // function transferBase(bytes32 _base, address _newAddress)
            // 0xe22325ad
            CTransferBase *a = new CTransferBase;
            a->CTransaction::operator=(*p);
            a->_base = extract(params, 0*64, 64);
            a->_newAddress = str_2_Addr(extract(params, 1*64, 64));
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("transferBase", params, nItems, items);
            return a;

        } else if (encoding == func_transferOwnership) {
            // function transferOwnership(address newOwner)
            // 0xf2fde38b
            CTransferOwnership *a = new CTransferOwnership;
            a->CTransaction::operator=(*p);
            a->newOwner = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("transferOwnership", params, nItems, items);
            return a;

        } else if (encoding == func_withdraw) {
            // function withdraw()
            // 0x3ccfd60b
            CWithdraw *a = new CWithdraw;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("withdraw", params, nItems, items);
            return a;

        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return promoteToToken(p);
}

//-----------------------------------------------------------------------
const string_q evt_BaseTransfered = "0x8d2701dfa5297b68d0b7531cf9f8c4ac81517a6270519c81c6b16082797adc77";
const string_q evt_NewBase = "0x514f075421f451de6bb4f10263e707d283f255473a3682a6427c0356727d528d";
const string_q evt_NewHandle = "0xa5a4e093e832dd981aa7a9351bad0fdf82966fb32d32220c734c6e62536916b7";
const string_q evt_OwnershipTransferred = "0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0";

//-----------------------------------------------------------------------
const CLogEntry *promoteToEvent(const CLogEntry *p) {

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q items[256];
        size_t nItems = 0;
        string_q data = extract(p->data, 2);
        string_q params;
        bool first = true;
        for (auto t : p->topics) {
            if (!first)
                params += extract(topic_2_Str(t),2);
            first = false;
        }
        params += data;
        // EXISTING_CODE
        // EXISTING_CODE

        if (topic_2_Str(p->topics[0]) % evt_BaseTransfered) {
            // event BaseTransfered(bytes32 _base, address indexed _to)
            // 0x8d2701dfa5297b68d0b7531cf9f8c4ac81517a6270519c81c6b16082797adc77
            CBaseTransferedEvent *a = new CBaseTransferedEvent;
            a->CLogEntry::operator=(*p);
            a->_base = "0x" + extract(data, 0*64, 64);
            a->_to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("BaseTransfered", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewBase) {
            // event NewBase(bytes32 _base, address indexed _address)
            // 0x514f075421f451de6bb4f10263e707d283f255473a3682a6427c0356727d528d
            CNewBaseEvent *a = new CNewBaseEvent;
            a->CLogEntry::operator=(*p);
            a->_base = "0x" + extract(data, 0*64, 64);
            a->_address = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("NewBase", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewHandle) {
            // event NewHandle(bytes32 _base, bytes32 _handle, address indexed _address)
            // 0xa5a4e093e832dd981aa7a9351bad0fdf82966fb32d32220c734c6e62536916b7
            CNewHandleEvent *a = new CNewHandleEvent;
            a->CLogEntry::operator=(*p);
            a->_base = "0x" + extract(data, 0*64, 64);
            a->_handle = "0x" + extract(data, 1*64, 64);
            a->_address = str_2_Addr(nTops > 3 ? topic_2_Str(p->topics[3]) : "");
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("NewHandle", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_OwnershipTransferred) {
            // event OwnershipTransferred(address indexed previousOwner, address indexed newOwner)
            // 0x8be0079c531659141344cd1fd0a4f28419497f9722a3daafe3b4186f6b6457e0
            COwnershipTransferredEvent *a = new COwnershipTransferredEvent;
            a->CLogEntry::operator=(*p);
            a->previousOwner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->newOwner = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("OwnershipTransferred", params, nItems, items);
            return a;

        }
        // fall through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return promoteToTokenEvent(p);
}

//-----------------------------------------------------------------------------
string_q sigs[] = {
    // Token support
    func_approve_qb,
    func_transferFrom_qb,
    func_transfer_qb,
    // Wallet support
    func_addOwner_qb,
    func_changeOwner_qb,
    func_changeRequirement_qb,
    func_confirm_qb,
    func_execute_qb,
    func_isOwner_qb,
    func_kill_qb,
    func_removeOwner_qb,
    func_resetSpentToday_qb,
    func_revoke_qb,
    func_setDailyLimit_qb,
    // Contract support
    func_baseRegistred,
    func_changePrice,
    func_doesOwnBase,
    func_findAddress,
    func_getBaseQuick,
    func_getPrice,
    func_handleIndex,
    func_isRegistered,
    func_owner,
    func_ownsBase,
    func_price,
    func_registerBase,
    func_registerHandle,
    func_transferBase,
    func_transferOwnership,
    func_withdraw,
};
size_t nSigs = sizeof(sigs) / sizeof(string_q);

//-----------------------------------------------------------------------------
string_q topics[] = {
    // Token support
    evt_Transfer_qb,
    evt_Approval_qb,
    // Wallet support
    evt_Confirmation_qb,
    evt_ConfirmationNeeded_qb,
    evt_Deposit_qb,
    evt_MultiTransact_qb,
    evt_OwnerAdded_qb,
    evt_OwnerChanged_qb,
    evt_OwnerRemoved_qb,
    evt_RequirementChanged_qb,
    evt_Revoke_qb,
    evt_SingleTransact_qb,
    // Contract support
    evt_BaseTransfered,
    evt_NewBase,
    evt_NewHandle,
    evt_OwnershipTransferred,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x25d94b021b69d9c01931ff40bd265cfc3d920f72
[{"constant":false,"inputs":[{"name":"_base","type":"bytes32"}],"name":"registerBase","outputs":[],"payable":true,"stateMutability":"payable","type":"function"},{"constant":true,"inputs":[{"name":"_base","type":"bytes32"}],"name":"isRegistered","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_base","type":"bytes32"},{"name":"_handle","type":"bytes32"}],"name":"findAddress","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_base","type":"bytes32"},{"name":"_addr","type":"address"}],"name":"doesOwnBase","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"withdraw","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"_base","type":"bytes32"},{"name":"_handle","type":"bytes32"},{"name":"_addr","type":"address"}],"name":"registerHandle","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"bytes32"},{"name":"","type":"bytes32"}],"name":"handleIndex","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"ownsBase","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_base","type":"bytes32"}],"name":"getBaseQuick","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"getPrice","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"price","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_price","type":"uint256"}],"name":"changePrice","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"","type":"bytes32"}],"name":"baseRegistred","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_base","type":"bytes32"},{"name":"_newAddress","type":"address"}],"name":"transferBase","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"inputs":[{"name":"_price","type":"uint256"},{"name":"_ethBase","type":"bytes32"},{"name":"_weldBase","type":"bytes32"}],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_base","type":"bytes32"},{"indexed":true,"name":"_address","type":"address"}],"name":"NewBase","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_base","type":"bytes32"},{"indexed":false,"name":"_handle","type":"bytes32"},{"indexed":true,"name":"_address","type":"address"}],"name":"NewHandle","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"_base","type":"bytes32"},{"indexed":true,"name":"_to","type":"address"}],"name":"BaseTransfered","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"previousOwner","type":"address"},{"indexed":true,"name":"newOwner","type":"address"}],"name":"OwnershipTransferred","type":"event"}]

ABI for addr : 0xfd495eeed737b002ea62cf0534e7707a9656ba19
[]

*/
