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
    CMigratedEvent::registerClass();
    CPauseEvent::registerClass();
    CUnpauseEvent::registerClass();
    CMigrateBalance::registerClass();
    CMigrateBalances::registerClass();
    CPause::registerClass();
    CTransferOwnership::registerClass();
    CUnpause::registerClass();
    tokenlib_init();
}

//-----------------------------------------------------------------------
const string_q func_initialized = "0x158ef93e";
const string_q func_legacyRepContract = "0xb85e0aca";
const string_q func_migrateBalance = "0x2988e36b";
const string_q func_migrateBalances = "0x4b92738e";
const string_q func_owner = "0x8da5cb5b";
const string_q func_pause = "0x8456cb59";
const string_q func_paused = "0x5c975abb";
const string_q func_targetSupply = "0x2a1eafd9";
const string_q func_transferOwnership = "0xf2fde38b";
const string_q func_unpause = "0x3f4ba83a";

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q items[256];
        size_t nItems = 0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_migrateBalance) {
            // function migrateBalance(address _holder)
            // 0x2988e36b
            CMigrateBalance *a = new CMigrateBalance;
            a->CTransaction::operator=(*p);
            a->_holder = str_2_Addr(extract(params, 0*64, 64));
            items[nItems++] = "address";
            a->articulatedTx = decodeRLP("migrateBalance", params, nItems, items);
            return a;

        } else if (encoding == func_migrateBalances) {
            // function migrateBalances(address[] _holders)
            // 0x4b92738e
            CMigrateBalances *a = new CMigrateBalances;
            a->CTransaction::operator=(*p);
            while (!params.empty()) {
                string_q val = extract(params, 0, 64);
                params = extract(params, 64);
                a->_holders.push_back(val);
            }
            items[nItems++] = "address[]";
            a->articulatedTx = decodeRLP("migrateBalances", params, nItems, items);
            return a;

        } else if (encoding == func_pause) {
            // function pause()
            // 0x8456cb59
            CPause *a = new CPause;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("pause", params, nItems, items);
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

        } else if (encoding == func_unpause) {
            // function unpause()
            // 0x3f4ba83a
            CUnpause *a = new CUnpause;
            a->CTransaction::operator=(*p);
            a->articulatedTx = decodeRLP("unpause", params, nItems, items);
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
const string_q evt_Migrated = "0x8b80bd19aea7b735bc6d75db8d6adbe18b28c30d62b3555245eb67b2340caedc";
const string_q evt_Pause = "0x6985a02210a168e66602d3235cb6db0e70f92b3ba4d376a33c0f3d9434bff625";
const string_q evt_Unpause = "0x7805862f689e2f13df9f062ff482ad3ad112aca9e0847911ed832e158c525b33";

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

        if (topic_2_Str(p->topics[0]) % evt_Migrated) {
            // event Migrated(address indexed holder, uint256 amount)
            // 0x8b80bd19aea7b735bc6d75db8d6adbe18b28c30d62b3555245eb67b2340caedc
            CMigratedEvent *a = new CMigratedEvent;
            a->CLogEntry::operator=(*p);
            a->holder = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            a->articulatedLog = decodeRLP("Migrated", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Pause) {
            // event Pause()
            // 0x6985a02210a168e66602d3235cb6db0e70f92b3ba4d376a33c0f3d9434bff625
            CPauseEvent *a = new CPauseEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("Pause", params, nItems, items);
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Unpause) {
            // event Unpause()
            // 0x7805862f689e2f13df9f062ff482ad3ad112aca9e0847911ed832e158c525b33
            CUnpauseEvent *a = new CUnpauseEvent;
            a->CLogEntry::operator=(*p);
            a->articulatedLog = decodeRLP("Unpause", params, nItems, items);
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
    func_initialized,
    func_legacyRepContract,
    func_migrateBalance,
    func_migrateBalances,
    func_owner,
    func_pause,
    func_paused,
    func_targetSupply,
    func_transferOwnership,
    func_unpause,
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
    evt_Migrated,
    evt_Pause,
    evt_Unpause,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xe94327d07fc17907b4db788e5adf2ed424addff6
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"initialized","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_holder","type":"address"}],"name":"migrateBalance","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"targetSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"unpause","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_holders","type":"address[]"}],"name":"migrateBalances","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"paused","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[],"name":"pause","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"type":"function"},{"constant":true,"inputs":[],"name":"legacyRepContract","outputs":[{"name":"","type":"address"}],"payable":false,"type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"type":"function"},{"inputs":[{"name":"_legacyRepContract","type":"address"},{"name":"_amountUsedToFreeze","type":"uint256"},{"name":"_accountToSendFrozenRepTo","type":"address"}],"payable":false,"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"holder","type":"address"},{"indexed":false,"name":"amount","type":"uint256"}],"name":"Migrated","type":"event"},{"anonymous":false,"inputs":[],"name":"Pause","type":"event"},{"anonymous":false,"inputs":[],"name":"Unpause","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"}]

ABI for addr : 0x7e614ec62cfd5761f20a9c5a2fe2bc0ac7431918
[]

ABI for addr : 0x48c80f1f4d53d5951e5d5438b54cba84f29f32a5
[]

*/
