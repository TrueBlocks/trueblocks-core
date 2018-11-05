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
    CConfirmedEvent::registerClass();
    CNewOwnerEvent::registerClass();
    CPuzzledEvent::registerClass();
    CRequestedEvent::registerClass();
    CRevokedEvent::registerClass();
    CCertify::registerClass();
    CDrain::registerClass();
    CPuzzle::registerClass();
    CRequest::registerClass();
    CRevoke::registerClass();
    CSetDelegate::registerClass();
    CSetFee::registerClass();
    CSetOwner::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_certified = "0xcc1d4c02";
const string_q func_certify = "0x14253887";
const string_q func_delegate = "0xc89e4361";
const string_q func_drain = "0x9890220b";
const string_q func_fee = "0xddca3f43";
const string_q func_get = "0xfc2525ab";
const string_q func_getAddress = "0x5283f339";
const string_q func_getUint = "0xca4cbf67";
const string_q func_owner = "0x8da5cb5b";
const string_q func_puzzle = "0x3da5c3ce";
const string_q func_request = "0x338cdca1";
const string_q func_revoke = "0x74a8f103";
const string_q func_setDelegate = "0xca5eb5e1";
const string_q func_setFee = "0x69fe0e2d";
const string_q func_setOwner = "0x13af4035";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_certify) {
            // function certify(address _who)
            // 0x14253887
            CCertify *a = new CCertify;
            a->CTransaction::operator=(*p);
            a->_who = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("certify", params, nItems, items);
            return a;

        } else if (encoding == func_drain) {
            // function drain()
            // 0x9890220b
            CDrain *a = new CDrain;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("drain", params, nItems, items);
            return a;

        } else if (encoding == func_puzzle) {
            // function puzzle(address _who, bytes32 _puzzle)
            // 0x3da5c3ce
            CPuzzle *a = new CPuzzle;
            a->CTransaction::operator=(*p);
            a->_who = str_2_Addr(extract(params, 0*64, 64));
            a->_puzzle = extract(params, 1*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("puzzle", params, nItems, items);
            return a;

        } else if (encoding == func_request) {
            // function request()
            // 0x338cdca1
            CRequest *a = new CRequest;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("request", params, nItems, items);
            return a;

        } else if (encoding == func_revoke) {
            // function revoke(address _who)
            // 0x74a8f103
            CRevoke *a = new CRevoke;
            a->CTransaction::operator=(*p);
            a->_who = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("revoke", params, nItems, items);
            return a;

        } else if (encoding == func_setDelegate) {
            // function setDelegate(address _new)
            // 0xca5eb5e1
            CSetDelegate *a = new CSetDelegate;
            a->CTransaction::operator=(*p);
            a->_new = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setDelegate", params, nItems, items);
            return a;

        } else if (encoding == func_setFee) {
            // function setFee(uint256 _new)
            // 0x69fe0e2d
            CSetFee *a = new CSetFee;
            a->CTransaction::operator=(*p);
            a->_new = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("setFee", params, nItems, items);
            return a;

        } else if (encoding == func_setOwner) {
            // function setOwner(address _new)
            // 0x13af4035
            CSetOwner *a = new CSetOwner;
            a->CTransaction::operator=(*p);
            a->_new = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("setOwner", params, nItems, items);
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
const string_q evt_Confirmed = "0xd415b905d4dd806bfba99a7a0e6351bd0c9db3a9912add21c0e6bef4479f673f";
const string_q evt_NewOwner = "0x70aea8d848e8a90fb7661b227dc522eb6395c3dac71b63cb59edd5c9899b2364";
const string_q evt_Puzzled = "0xa9a343b39eac85ffb326e93ecd46785b814e72dc9f2b33bb0b4a315ba2859f43";
const string_q evt_Requested = "0x039f711c9c18dd815b225b1424855e6118e746c6b5d688907f10c4dd29ebe92a";
const string_q evt_Revoked = "0xb6fa8b8bd5eab60f292eca876e3ef90722275b785309d84b1de113ce0b8c4e74";

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

        if (topic_2_Str(p->topics[0]) % evt_Confirmed) {
            // event Confirmed(address indexed who)
            // 0xd415b905d4dd806bfba99a7a0e6351bd0c9db3a9912add21c0e6bef4479f673f
            CConfirmedEvent *a = new CConfirmedEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("Confirmed", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewOwner) {
            // event NewOwner(address indexed old, address indexed current)
            // 0x70aea8d848e8a90fb7661b227dc522eb6395c3dac71b63cb59edd5c9899b2364
            CNewOwnerEvent *a = new CNewOwnerEvent;
            a->CLogEntry::operator=(*p);
            a->old = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->current = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            items[nItems++] = "address";
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("NewOwner", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Puzzled) {
            // event Puzzled(address indexed who, bytes32 puzzle)
            // 0xa9a343b39eac85ffb326e93ecd46785b814e72dc9f2b33bb0b4a315ba2859f43
            CPuzzledEvent *a = new CPuzzledEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->puzzle = "0x" + extract(data, 0*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            a->articulatedLog = decodeRLP("Puzzled", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Requested) {
            // event Requested(address indexed who)
            // 0x039f711c9c18dd815b225b1424855e6118e746c6b5d688907f10c4dd29ebe92a
            CRequestedEvent *a = new CRequestedEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("Requested", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Revoked) {
            // event Revoked(address indexed who)
            // 0xb6fa8b8bd5eab60f292eca876e3ef90722275b785309d84b1de113ce0b8c4e74
            CRevokedEvent *a = new CRevokedEvent;
            a->CLogEntry::operator=(*p);
            a->who = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            items[nItems++] = "address";
            a->articulatedLog = decodeRLP("Revoked", params, nItems, items);
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
    func_certified,
    func_certify,
    func_delegate,
    func_drain,
    func_fee,
    func_get,
    func_getAddress,
    func_getUint,
    func_owner,
    func_puzzle,
    func_request,
    func_revoke,
    func_setDelegate,
    func_setFee,
    func_setOwner,
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
    evt_Confirmed,
    evt_NewOwner,
    evt_Puzzled,
    evt_Requested,
    evt_Revoked,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0x9ae98746eb8a0aeee5ff2b6b15875313a986f103
[{"constant":false,"inputs":[{"name":"_new","type":"address"}],"name":"setOwner","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_who","type":"address"}],"name":"certify","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"request","outputs":[],"payable":true,"type":"function"},{"constant":false,"inputs":[{"name":"_who","type":"address"},{"name":"_puzzle","type":"bytes32"}],"name":"puzzle","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_who","type":"address"},{"name":"_field","type":"string"}],"name":"getAddress","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_new","type":"uint256"}],"name":"setFee","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_who","type":"address"}],"name":"revoke","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_code","type":"bytes32"}],"name":"confirm","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"drain","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"delegate","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_who","type":"address"},{"name":"_field","type":"string"}],"name":"getUint","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_new","type":"address"}],"name":"setDelegate","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_who","type":"address"}],"name":"certified","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"fee","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_who","type":"address"},{"name":"_field","type":"string"}],"name":"get","outputs":[{"name":"","type":"bytes32"}],"payable":false,"type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"Requested","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"},{"indexed":false,"name":"puzzle","type":"bytes32"}],"name":"Puzzled","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"Confirmed","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"who","type":"address"}],"name":"Revoked","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"old","type":"address"},{"indexed":true,"name":"current","type":"address"}],"name":"NewOwner","type":"event"}]

ABI for addr : 0x00433f38561cc8799077c7e4e3cd6d0d777bd0e0
[]

*/
