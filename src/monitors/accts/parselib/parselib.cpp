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
    tokenlib_init();
}

//-----------------------------------------------------------------------
// No functions

//-----------------------------------------------------------------------
const CTransaction *promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        // EXISTING_CODE
        // EXISTING_CODE

        {
            // No functions
        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return promoteToToken(p);
}

//-----------------------------------------------------------------------
// No events

//-----------------------------------------------------------------------
const CLogEntry *promoteToEvent(const CLogEntry *p) {

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        // EXISTING_CODE
        // EXISTING_CODE

        {
            // No events
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
    // No functions
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
    // No events
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xd31f381ad54386895852f311fc3be2008c88cb27
[]

ABI for addr : 0x85979379cb90d63fe944763b42b9ec4b49573ea7
[]

ABI for addr : 0xfd6c5ac853391486d25ddd5cefb1f7040ee31c2c
[]

ABI for addr : 0xf1aa581f353005ba3765b81bf52d6b1c488c2767
[]

ABI for addr : 0xb97073b754660bb356dfe12f78ae366d77dbc80f
[]

ABI for addr : 0xa92e6abc1156b6ed4948cc962cb60f250d422c68
[]

ABI for addr : 0x00ad1a7b0ef4d3c5070d73b9412b6ed22efb7533
[]

ABI for addr : 0x654e7f49b474e2f5ac29cc5f2f0d41c8a93d6d0a
[]

ABI for addr : 0x00a3819199113fc6a6e6ba1298afde7377e2009b
[]

ABI for addr : 0x31f54a9d0c9817bb9e6ca1d17ca20d8ea718bc66
[]

ABI for addr : 0x00a6ef05a497236a1ea611857f52884593f63f8c
[]

*/
