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
#include "capreachedevent.h"
#include "fundtransferevent.h"
#include "ownershiptransferredevent.h"
#include "pauseevent.h"
#include "registrationstatuschangedevent.h"
#include "unpauseevent.h"
#include "allocatetokens.h"
#include "burn.h"
#include "buy.h"
#include "deactivate.h"
#include "decreaseapproval.h"
#include "enabletransfer.h"
#include "increaseapproval.h"
#include "ownersafewithdrawal.h"
#include "pause.h"
#include "registeruser.h"
#include "registerusers.h"
#include "setcap.h"
#include "setcrowdsale.h"
#include "setdeadline.h"
#include "terminate.h"
#include "transferownership.h"
#include "unpause.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_ADMINALLOWANCE;
extern const string_q func_CROWDSALEALLOWANCE;
extern const string_q func_GASLIMITINWEI;
extern const string_q func_INITIALSUPPLY;
extern const string_q func_RATE;
extern const string_q func_adminAddr;
extern const string_q func_adminAllowance;
extern const string_q func_allocateTokens;
extern const string_q func_amountRaised;
extern const string_q func_beneficiary;
extern const string_q func_burn;
extern const string_q func_buy;
extern const string_q func_cap;
extern const string_q func_capTime;
extern const string_q func_crowdSaleAddr;
extern const string_q func_crowdSaleAllowance;
extern const string_q func_currentTime;
extern const string_q func_deactivate;
extern const string_q func_deadline;
extern const string_q func_decreaseApproval;
extern const string_q func_enableTransfer;
extern const string_q func_fundingCap;
extern const string_q func_fundingCapReached;
extern const string_q func_increaseApproval;
extern const string_q func_mainsaleBalanceOf;
extern const string_q func_minContribution;
extern const string_q func_owner;
extern const string_q func_ownerSafeWithdrawal;
extern const string_q func_pause;
extern const string_q func_paused;
extern const string_q func_refundAmount;
extern const string_q func_registerUser;
extern const string_q func_registerUsers;
extern const string_q func_registry;
extern const string_q func_saleClosed;
extern const string_q func_setCap;
extern const string_q func_setCrowdsale;
extern const string_q func_setDeadline;
extern const string_q func_startTime;
extern const string_q func_terminate;
extern const string_q func_tokenReward;
extern const string_q func_transferEnabled;
extern const string_q func_transferOwnership;
extern const string_q func_unpause;

//------------------------------------------------------------------------
extern const string_q evt_Burn;
extern const string_q evt_CapReached;
extern const string_q evt_FundTransfer;
extern const string_q evt_OwnershipTransferred;
extern const string_q evt_Pause;
extern const string_q evt_RegistrationStatusChanged;
extern const string_q evt_Unpause;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
