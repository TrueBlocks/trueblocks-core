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
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "burnevent.h"
#include "mintevent.h"
#include "mintfinishedevent.h"
#include "minttimelockedevent.h"
#include "ownershiptransferredevent.h"
#include "addexcluded.h"
#include "allowmovetokens.h"
#include "burn.h"
#include "decreaseapproval.h"
#include "finishminting.h"
#include "increaseapproval.h"
#include "mint.h"
#include "minttimelocked.h"
#include "returnfrozenfreefunds.h"
#include "transferownership.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_addExcluded;
extern const string_q func_allowMoveTokens;
extern const string_q func_burn;
extern const string_q func_decreaseApproval;
extern const string_q func_finishMinting;
extern const string_q func_frozenFunds;
extern const string_q func_increaseApproval;
extern const string_q func_mint;
extern const string_q func_mintTimelocked;
extern const string_q func_mintingFinished;
extern const string_q func_owner;
extern const string_q func_paused;
extern const string_q func_returnFrozenFreeFunds;
extern const string_q func_transferOwnership;

//------------------------------------------------------------------------
extern const string_q evt_Burn;
extern const string_q evt_Mint;
extern const string_q evt_MintFinished;
extern const string_q evt_MintTimelocked;
extern const string_q evt_OwnershipTransferred;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
