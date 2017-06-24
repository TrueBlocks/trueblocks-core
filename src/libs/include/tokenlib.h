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
  * This code was generated automatically from grabABI and makeClass from the
  * 'TokenLib' ABI file. You may edit the file,
  * but keep your edits inside the 'EXISTING_CODE' tags.
  *
  */
#include "etherlib.h"
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "approvalevent.h"
#include "transferevent.h"
#include "approve.h"
#include "transfer.h"
#include "transferfrom.h"
#include "tokenbal.h"

//------------------------------------------------------------------------
extern void tokenlib_init(void);
extern const CTransaction *promoteToToken(const CTransaction *p);
extern const CLogEntry *promoteToTokenEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const SFString func_approve_qb;
extern const SFString func_transfer_qb;
extern const SFString func_transferFrom_qb;

//------------------------------------------------------------------------
extern const SFString evt_Approval_qb;
extern const SFString evt_Transfer_qb;
