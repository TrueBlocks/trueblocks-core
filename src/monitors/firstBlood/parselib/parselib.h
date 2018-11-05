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
#include "allocatebountyandecosystemtokensevent.h"
#include "allocatefoundertokensevent.h"
#include "buyevent.h"
#include "withdrawevent.h"
#include "allocatebountyandecosystemtokens.h"
#include "allocatefoundertokens.h"
#include "buy.h"
#include "buyrecipient.h"
#include "changefounder.h"
#include "halt.h"
#include "unhalt.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_allocateBountyAndEcosystemTokens;
extern const string_q func_allocateFounderTokens;
extern const string_q func_bountyAllocated;
extern const string_q func_bountyAllocation;
extern const string_q func_buy;
extern const string_q func_buyRecipient;
extern const string_q func_changeFounder;
extern const string_q func_ecosystemAllocated;
extern const string_q func_ecosystemAllocation;
extern const string_q func_endBlock;
extern const string_q func_etherCap;
extern const string_q func_founder;
extern const string_q func_founderAllocated;
extern const string_q func_founderAllocation;
extern const string_q func_founderLockup;
extern const string_q func_halt;
extern const string_q func_halted;
extern const string_q func_presaleEtherRaised;
extern const string_q func_presaleTokenSupply;
extern const string_q func_price;
extern const string_q func_signer;
extern const string_q func_startBlock;
extern const string_q func_testPrice;
extern const string_q func_transferLockup;
extern const string_q func_unhalt;

//------------------------------------------------------------------------
extern const string_q evt_AllocateBountyAndEcosystemTokens;
extern const string_q evt_AllocateFounderTokens;
extern const string_q evt_Buy;
extern const string_q evt_Withdraw;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
