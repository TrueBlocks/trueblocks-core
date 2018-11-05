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
    CClaimRemovedEvent::registerClass();
    CClaimSetEvent::registerClass();
    CRemoveClaim::registerClass();
    CSetClaim::registerClass();
    CSetSelfClaim::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_getClaim = "0xe1661eff";
const string_q func_registry = "0xe6d970aa";
const string_q func_removeClaim = "0xc7508ec7";
const string_q func_setClaim = "0x9918925d";
const string_q func_setSelfClaim = "0x9155b01a";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_removeClaim) {
            // function removeClaim(address issuer, address subject, bytes32 key)
            // 0xc7508ec7
            CRemoveClaim *a = new CRemoveClaim;
            a->CTransaction::operator=(*p);
            a->issuer = str_2_Addr(extract(params, 0*64, 64));
            a->subject = str_2_Addr(extract(params, 1*64, 64));
            a->key = extract(params, 2*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("removeClaim", params, nItems, items);
            return a;

        } else if (encoding == func_setClaim) {
            // function setClaim(address subject, bytes32 key, bytes32 value)
            // 0x9918925d
            CSetClaim *a = new CSetClaim;
            a->CTransaction::operator=(*p);
            a->subject = str_2_Addr(extract(params, 0*64, 64));
            a->key = extract(params, 1*64, 64);
            a->value = extract(params, 2*64, 64);
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("setClaim", params, nItems, items);
            return a;

        } else if (encoding == func_setSelfClaim) {
            // function setSelfClaim(bytes32 key, bytes32 value)
            // 0x9155b01a
            CSetSelfClaim *a = new CSetSelfClaim;
            a->CTransaction::operator=(*p);
            a->key = extract(params, 0*64, 64);
            a->value = extract(params, 1*64, 64);
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            a->articulatedTx = decodeRLP("setSelfClaim", params, nItems, items);
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
const string_q evt_ClaimRemoved = "0x7bc046ef9f2fccd4c99778d7e0fbed67ddb16e63e1a266894769de9516b8039a";
const string_q evt_ClaimSet = "0x8c9f893548e8429f9352aba6698e6f4dca2e390604f6e8c5881a7a505d94ae50";

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

        if (topic_2_Str(p->topics[0]) % evt_ClaimRemoved) {
            // event ClaimRemoved(address indexed issuer, address indexed subject, bytes32 indexed key, uint256 removedAt)
            // 0x7bc046ef9f2fccd4c99778d7e0fbed67ddb16e63e1a266894769de9516b8039a
            CClaimRemovedEvent *a = new CClaimRemovedEvent;
            a->CLogEntry::operator=(*p);
            a->issuer = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->subject = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->key = nTops > 3 ? topic_2_Str(p->topics[3]) : "";
            a->removedAt = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("ClaimRemoved", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ClaimSet) {
            // event ClaimSet(address indexed issuer, address indexed subject, bytes32 indexed key, bytes32 value, uint256 updatedAt)
            // 0x8c9f893548e8429f9352aba6698e6f4dca2e390604f6e8c5881a7a505d94ae50
            CClaimSetEvent *a = new CClaimSetEvent;
            a->CLogEntry::operator=(*p);
            a->issuer = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->subject = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->key = nTops > 3 ? topic_2_Str(p->topics[3]) : "";
            a->value = "0x" + extract(data, 0*64, 64);
            a->updatedAt = str_2_Wei("0x" + extract(data, 1*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("ClaimSet", params, nItems, items);
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
    func_getClaim,
    func_registry,
    func_removeClaim,
    func_setClaim,
    func_setSelfClaim,
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
    evt_ClaimRemoved,
    evt_ClaimSet,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xdb55d40684e7dc04655a9789937214b493a2c2c6
[{"constant":false,"inputs":[{"name":"key","type":"bytes32"},{"name":"value","type":"bytes32"}],"name":"setSelfClaim","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"subject","type":"address"},{"name":"key","type":"bytes32"},{"name":"value","type":"bytes32"}],"name":"setClaim","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"issuer","type":"address"},{"name":"subject","type":"address"},{"name":"key","type":"bytes32"}],"name":"removeClaim","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"issuer","type":"address"},{"name":"subject","type":"address"},{"name":"key","type":"bytes32"}],"name":"getClaim","outputs":[{"name":"","type":"bytes32"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"},{"name":"","type":"bytes32"}],"name":"registry","outputs":[{"name":"","type":"bytes32"}],"payable":false,"stateMutability":"view","type":"function"},{"anonymous":false,"inputs":[{"indexed":true,"name":"issuer","type":"address"},{"indexed":true,"name":"subject","type":"address"},{"indexed":true,"name":"key","type":"bytes32"},{"indexed":false,"name":"value","type":"bytes32"},{"indexed":false,"name":"updatedAt","type":"uint256"}],"name":"ClaimSet","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"issuer","type":"address"},{"indexed":true,"name":"subject","type":"address"},{"indexed":true,"name":"key","type":"bytes32"},{"indexed":false,"name":"removedAt","type":"uint256"}],"name":"ClaimRemoved","type":"event"}]

*/
