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
#include "approvalforallevent.h"
#include "transferevent.h"
#include "approve.h"
#include "approveandcall.h"
#include "ownerof.h"
#include "safetransferfrom.h"
#include "safetransferfromtoke.h"
#include "setapprovalforall.h"
#include "transfer.h"
#include "transferfrom.h"

//------------------------------------------------------------------------
extern void tokenlib_init(void);
extern const CTransaction *promoteToToken(const CTransaction *p);
extern const CLogEntry *promoteToTokenEvent(const CLogEntry *p);
extern bool articulateToken(CTransaction *p);
extern bool articulateTokenEvent(CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_allowance_qb;
extern const string_q func_approve_qb;
extern const string_q func_approveAndCall_qb;
extern const string_q func_balanceOf_qb;
extern const string_q func_decimals_qb;
extern const string_q func_exists_qb;
extern const string_q func_getApproved_qb;
extern const string_q func_isApprovedForAll_qb;
extern const string_q func_name_qb;
extern const string_q func_ownerOf_qb;
extern const string_q func_safeTransferFrom_qb;
extern const string_q func_safeTransferFromToke_qb;
extern const string_q func_setApprovalForAll_qb;
extern const string_q func_symbol_qb;
extern const string_q func_totalSupply_qb;
extern const string_q func_transfer_qb;
extern const string_q func_transferFrom_qb;
extern const string_q func_version_qb;

//------------------------------------------------------------------------
extern const string_q evt_Approval_qb;
extern const string_q evt_ApprovalForAll_qb;
extern const string_q evt_Transfer_qb;

// EXISTING_CODE
//-----------------------------------------------------------------------
inline bool isTokenRelated(const CTransaction *trans) {
    return (
            startsWith(trans->input, func_transfer_qb) ||
            startsWith(trans->input, func_transferFrom_qb) ||
            startsWith(trans->input, func_approve_qb) ||
            startsWith(trans->input, func_safeTransferFrom_qb) ||
            startsWith(trans->input, func_safeTransferFromToke_qb) ||
            startsWith(trans->input, func_setApprovalForAll_qb) ||
            startsWith(trans->input, func_approveAndCall_qb) ||
            startsWith(trans->input, func_allowance_qb) ||
            startsWith(trans->input, func_balanceOf_qb) ||
            startsWith(trans->input, func_decimals_qb) ||
            startsWith(trans->input, func_name_qb) ||
            startsWith(trans->input, func_exists_qb) ||
            startsWith(trans->input, func_getApproved_qb) ||
            startsWith(trans->input, func_isApprovedForAll_qb) ||
            startsWith(trans->input, func_ownerOf_qb) ||
            startsWith(trans->input, func_symbol_qb) ||
            startsWith(trans->input, func_totalSupply_qb) ||
            startsWith(trans->input, func_version_qb)
    );
}
// EXISTING_CODE
