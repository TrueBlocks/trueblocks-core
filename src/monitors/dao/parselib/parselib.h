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
#include "allowedrecipientchangedevent.h"
#include "createdtokenevent.h"
#include "fuelingtodateevent.h"
#include "newcuratorevent.h"
#include "proposaladdedevent.h"
#include "proposaltalliedevent.h"
#include "refundevent.h"
#include "votedevent.h"
#include "changeallowedrecipients.h"
#include "changeproposaldeposit.h"
#include "createtokenproxy.h"
#include "executeproposal.h"
#include "getmyreward.h"
#include "halveminquorum.h"
#include "newcontract.h"
#include "newproposal.h"
#include "receiveether.h"
#include "refund.h"
#include "retrievedaoreward.h"
#include "splitdao.h"
#include "transferfromwithoutreward.h"
#include "transferwithoutreward.h"
#include "trusteewithdraw.h"
#include "unblockme.h"
#include "vote.h"
#include "withdraw.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_DAOpaidOut;
extern const string_q func_DAOrewardAccount;
extern const string_q func_actualBalance;
extern const string_q func_allowedRecipients;
extern const string_q func_blocked;
extern const string_q func_changeAllowedRecipients;
extern const string_q func_changeProposalDeposit;
extern const string_q func_checkProposalCode;
extern const string_q func_closingTime;
extern const string_q func_createTokenProxy;
extern const string_q func_curator;
extern const string_q func_daoCreator;
extern const string_q func_divisor;
extern const string_q func_executeProposal;
extern const string_q func_extraBalance;
extern const string_q func_getMyReward;
extern const string_q func_getNewDAOAddress;
extern const string_q func_halveMinQuorum;
extern const string_q func_isFueled;
extern const string_q func_lastTimeMinQuorumMet;
extern const string_q func_mainDAO;
extern const string_q func_minQuorumDivisor;
extern const string_q func_minTokensToCreate;
extern const string_q func_newContract;
extern const string_q func_newProposal;
extern const string_q func_numberOfProposals;
extern const string_q func_paidOut;
extern const string_q func_privateCreation;
extern const string_q func_proposalDeposit;
extern const string_q func_proposals;
extern const string_q func_receiveEther;
extern const string_q func_refund;
extern const string_q func_retrieveDAOReward;
extern const string_q func_rewardAccount;
extern const string_q func_rewardToken;
extern const string_q func_splitDAO;
extern const string_q func_totalRewardToken;
extern const string_q func_transferFromWithoutReward;
extern const string_q func_transferWithoutReward;
extern const string_q func_trustee;
extern const string_q func_trusteeWithdraw;
extern const string_q func_unblockMe;
extern const string_q func_vote;
extern const string_q func_withdraw;

//------------------------------------------------------------------------
extern const string_q evt_AllowedRecipientChanged;
extern const string_q evt_CreatedToken;
extern const string_q evt_FuelingToDate;
extern const string_q evt_NewCurator;
extern const string_q evt_ProposalAdded;
extern const string_q evt_ProposalTallied;
extern const string_q evt_Refund;
extern const string_q evt_Voted;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
