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
    CBurnEvent::registerClass();
    CFreezeEvent::registerClass();
    CUnfreezeEvent::registerClass();
    CBurn::registerClass();
    CFreeze::registerClass();
    CUnfreeze::registerClass();
    CWithdrawEther::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_burn = "0x42966c68";
const string_q func_freeze = "0xd7a78db8";
const string_q func_freezeOf = "0xcd4217c1";
const string_q func_owner = "0x8da5cb5b";
const string_q func_unfreeze = "0x6623fc46";
const string_q func_withdrawEther = "0x3bed33ce";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_burn) {
            // function burn(uint256 _value)
            // 0x42966c68
            CBurn *a = new CBurn;
            a->CTransaction::operator=(*p);
            a->_value = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("burn", params, nItems, items);
            return a;

        } else if (encoding == func_freeze) {
            // function freeze(uint256 _value)
            // 0xd7a78db8
            CFreeze *a = new CFreeze;
            a->CTransaction::operator=(*p);
            a->_value = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("freeze", params, nItems, items);
            return a;

        } else if (encoding == func_unfreeze) {
            // function unfreeze(uint256 _value)
            // 0x6623fc46
            CUnfreeze *a = new CUnfreeze;
            a->CTransaction::operator=(*p);
            a->_value = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("unfreeze", params, nItems, items);
            return a;

        } else if (encoding == func_withdrawEther) {
            // function withdrawEther(uint256 amount)
            // 0x3bed33ce
            CWithdrawEther *a = new CWithdrawEther;
            a->CTransaction::operator=(*p);
            a->amount = str_2_Wei("0x" + extract(params, 0*64, 64));
            items[nItems++] = "uint256";
            a->articulatedTx = decodeRLP("withdrawEther", params, nItems, items);
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
const string_q evt_Burn = "0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5";
const string_q evt_Freeze = "0xf97a274face0b5517365ad396b1fdba6f68bd3135ef603e44272adba3af5a1e0";
const string_q evt_Unfreeze = "0x2cfce4af01bcb9d6cf6c84ee1b7c491100b8695368264146a94d71e10a63083f";

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

        if (topic_2_Str(p->topics[0]) % evt_Burn) {
            // event Burn(address indexed from, uint256 value)
            // 0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5
            CBurnEvent *a = new CBurnEvent;
            a->CLogEntry::operator=(*p);
            a->from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Burn", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Freeze) {
            // event Freeze(address indexed from, uint256 value)
            // 0xf97a274face0b5517365ad396b1fdba6f68bd3135ef603e44272adba3af5a1e0
            CFreezeEvent *a = new CFreezeEvent;
            a->CLogEntry::operator=(*p);
            a->from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Freeze", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Unfreeze) {
            // event Unfreeze(address indexed from, uint256 value)
            // 0x2cfce4af01bcb9d6cf6c84ee1b7c491100b8695368264146a94d71e10a63083f
            CUnfreezeEvent *a = new CUnfreezeEvent;
            a->CLogEntry::operator=(*p);
            a->from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Unfreeze", params, nItems, items);
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
    func_burn,
    func_freeze,
    func_freezeOf,
    func_owner,
    func_unfreeze,
    func_withdrawEther,
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
    evt_Burn,
    evt_Freeze,
    evt_Unfreeze,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xb8c77482e45f1f44de1745f52c74426c631bdd52
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"amount","type":"uint256"}],"name":"withdrawEther","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_value","type":"uint256"}],"name":"burn","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_value","type":"uint256"}],"name":"unfreeze","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"freezeOf","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_value","type":"uint256"}],"name":"freeze","outputs":[{"name":"success","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"inputs":[{"name":"initialSupply","type":"uint256"},{"name":"tokenName","type":"string"},{"name":"decimalUnits","type":"uint8"},{"name":"tokenSymbol","type":"string"}],"payable":false,"type":"constructor"},{"payable":true,"type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Burn","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Freeze","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Unfreeze","type":"event"}]

ABI for addr : 0x00c5e04176d95a286fcce0e68c683ca0bfec8454
[]

*/
