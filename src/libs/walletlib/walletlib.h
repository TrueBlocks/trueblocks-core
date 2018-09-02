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
#include "tokenlib.h"
#include "confirmationevent.h"
#include "confirmationneededevent.h"
#include "depositevent.h"
#include "multitransactevent.h"
#include "owneraddedevent.h"
#include "ownerchangedevent.h"
#include "ownerremovedevent.h"
#include "requirementchangedevent.h"
#include "revokeevent.h"
#include "singletransactevent.h"
#include "addowner.h"
#include "changeowner.h"
#include "changerequirement.h"
#include "confirm.h"
#include "execute.h"
#include "isowner.h"
#include "kill.h"
#include "removeowner.h"
#include "resetspenttoday.h"
#include "revoke.h"
#include "setdailylimit.h"

//------------------------------------------------------------------------
extern void walletlib_init(void);
extern const CTransaction *promoteToWallet(const CTransaction *p);
extern const CLogEntry *promoteToWalletEvent(const CLogEntry *p);
extern bool articulateWallet(CTransaction *p);
extern bool articulateWalletEvent(CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_addOwner_qb;
extern const string_q func_changeOwner_qb;
extern const string_q func_changeRequirement_qb;
extern const string_q func_confirm_qb;
extern const string_q func_execute_qb;
extern const string_q func_hasConfirmed_qb;
extern const string_q func_isOwner_qb;
extern const string_q func_kill_qb;
extern const string_q func_mdailyLimit_qb;
extern const string_q func_mnumOwners_qb;
extern const string_q func_mrequired_qb;
extern const string_q func_removeOwner_qb;
extern const string_q func_resetSpentToday_qb;
extern const string_q func_revoke_qb;
extern const string_q func_setDailyLimit_qb;

//------------------------------------------------------------------------
extern const string_q evt_Confirmation_qb;
extern const string_q evt_ConfirmationNeeded_qb;
extern const string_q evt_Deposit_qb;
extern const string_q evt_MultiTransact_qb;
extern const string_q evt_OwnerAdded_qb;
extern const string_q evt_OwnerChanged_qb;
extern const string_q evt_OwnerRemoved_qb;
extern const string_q evt_RequirementChanged_qb;
extern const string_q evt_Revoke_qb;
extern const string_q evt_SingleTransact_qb;

// EXISTING_CODE
// EXISTING_CODE
