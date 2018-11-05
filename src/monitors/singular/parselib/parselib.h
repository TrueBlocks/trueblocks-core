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
#include "changebasevalue.h"
#include "depositrevenue.h"
#include "emergencycall.h"
#include "fund.h"
#include "issuetokens.h"
#include "setup.h"
#include "softwithdrawrevenuefor.h"
#include "updatestage.h"
#include "withdrawforworkshop.h"
#include "withdrawfunding.h"
#include "withdrawrevenue.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_CAP;
extern const string_q func_CROWDFUNDINGPERIOD;
extern const string_q func_TOKENLOCKINGPERIOD;
extern const string_q func_TOKENTARGET;
extern const string_q func_baseValue;
extern const string_q func_campaignEndedSuccessfully;
extern const string_q func_changeBaseValue;
extern const string_q func_depositRevenue;
extern const string_q func_emergencyCall;
extern const string_q func_fund;
extern const string_q func_fundBalance;
extern const string_q func_investments;
extern const string_q func_issueTokens;
extern const string_q func_mlastDay;
extern const string_q func_mspentToday;
extern const string_q func_owed;
extern const string_q func_owner;
extern const string_q func_revenueAtTimeOfWithdraw;
extern const string_q func_setup;
extern const string_q func_singularDTVCrowdfunding;
extern const string_q func_singularDTVFund;
extern const string_q func_singularDTVToken;
extern const string_q func_softWithdrawRevenueFor;
extern const string_q func_stage;
extern const string_q func_startDate;
extern const string_q func_totalRevenue;
extern const string_q func_twoYearsPassed;
extern const string_q func_updateStage;
extern const string_q func_valuePerShare;
extern const string_q func_withdrawForWorkshop;
extern const string_q func_withdrawFunding;
extern const string_q func_withdrawRevenue;
extern const string_q func_workshop;

//------------------------------------------------------------------------
// No events

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
