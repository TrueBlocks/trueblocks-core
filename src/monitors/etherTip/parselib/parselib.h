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
#include "changeofrulesevent.h"
#include "frozenfundsevent.h"
#include "membershipchangedevent.h"
#include "proposaladdedevent.h"
#include "proposaltalliedevent.h"
#include "votedevent.h"
#include "changemembership.h"
#include "changeunicorn.h"
#include "changevotingrules.h"
#include "executeproposal.h"
#include "freezeaccount.h"
#include "minttoken.h"
#include "newproposalinether.h"
#include "newproposalinwei.h"
#include "transferownership.h"
#include "vote.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_changeMembership;
extern const string_q func_changeUnicorn;
extern const string_q func_changeVotingRules;
extern const string_q func_checkProposalCode;
extern const string_q func_debatingPeriodInMinutes;
extern const string_q func_executeProposal;
extern const string_q func_freezeAccount;
extern const string_q func_frozenAccount;
extern const string_q func_majorityMargin;
extern const string_q func_memberId;
extern const string_q func_members;
extern const string_q func_minimumQuorum;
extern const string_q func_mintToken;
extern const string_q func_newProposalInEther;
extern const string_q func_newProposalInWei;
extern const string_q func_numProposals;
extern const string_q func_owner;
extern const string_q func_priceOfAUnicornInFinney;
extern const string_q func_proposals;
extern const string_q func_spentAllowance;
extern const string_q func_transferOwnership;
extern const string_q func_unicornAddress;
extern const string_q func_vote;

//------------------------------------------------------------------------
extern const string_q evt_ChangeOfRules;
extern const string_q evt_FrozenFunds;
extern const string_q evt_MembershipChanged;
extern const string_q evt_ProposalAdded;
extern const string_q evt_ProposalTallied;
extern const string_q evt_Voted;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
