/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "grabABI.h"

    //-----------------------------------------------------------------------
    voidgrabABI _init(QUITHANDLER qh){
    etherlib_init(qh);    tokenlib_init(qh);
}

        //-----------------------------------------------------------------------
        // No functions

    //-----------------------------------------------------------------------
    const CTransaction* promoteToFunc(const CTransaction* p) {
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
    const CLogEntry* promoteToFunc Event(const CLogEntry* p) {
    size_t nTops = p->topics.size();
    if (nTops > 0) {    // the '0'th topic is the event signature
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
// EXISTING_CODE
<code>
                {
            // No events

    }
    // fall through
}
// EXISTING_CODE

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
const string_q topics[] = {
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
const size_t nTopics = sizeof(topics) / sizeof(string_q);

    /*
     */