#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
 /*
  *
  * This code was generated automatically from grabABI and makeClass You may 
  * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
  *
  */
#include "etherlib.h"
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "approvalevent.h"
#include "transferevent.h"
#include "approve.h"
#include "approveandcall.h"
#include "transfer.h"
#include "transferfrom.h"

//------------------------------------------------------------------------
extern void tokenlib_init(void);
extern const CTransaction *promoteToToken(const CTransaction *p);
extern const CLogEntry *promoteToTokenEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const SFString func_allowance_qb;
extern const SFString func_approve_qb;
extern const SFString func_approveAndCall_qb;
extern const SFString func_balanceOf_qb;
extern const SFString func_decimals_qb;
extern const SFString func_name_qb;
extern const SFString func_symbol_qb;
extern const SFString func_totalSupply_qb;
extern const SFString func_transfer_qb;
extern const SFString func_transferFrom_qb;
extern const SFString func_version_qb;

//------------------------------------------------------------------------
extern const SFString evt_Approval_qb;
extern const SFString evt_Transfer_qb;

// EXISTING_CODE
//-----------------------------------------------------------------------
inline bool isTokenRelated(const CTransaction *trans) {
    return (trans->input.startsWith(func_allowance_qb) ||
            trans->input.startsWith(func_approve_qb) ||
            trans->input.startsWith(func_approveAndCall_qb) ||
            trans->input.startsWith(func_balanceOf_qb) ||
            trans->input.startsWith(func_decimals_qb) ||
            trans->input.startsWith(func_name_qb) ||
            trans->input.startsWith(func_symbol_qb) ||
            trans->input.startsWith(func_totalSupply_qb) ||
            trans->input.startsWith(func_transfer_qb) ||
            trans->input.startsWith(func_transferFrom_qb) ||
            trans->input.startsWith(func_version_qb));
}
// EXISTING_CODE
