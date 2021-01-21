#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
 /*
  *
  * This code was generated automatically from grabABI and makeClass You may 
  * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
  *
  */
#include "etherlib.h"
#include "tokenlib.h"
#include "walletlib.h"
#include "constructor.h"
#include "approve.h"
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
#include "transfer.h"
#include "transferfrom.h"
#include "transferfromwithoutreward.h"
#include "transferwithoutreward.h"
#include "unblockme.h"
#include "vote.h"
#include "allowedrecipientchangedevent.h"
#include "approvalevent.h"
#include "createdtokenevent.h"
#include "fuelingtodateevent.h"
#include "newcuratorevent.h"
#include "proposaladdedevent.h"
#include "proposaltalliedevent.h"
#include "refundevent.h"
#include "transferevent.h"
#include "votedevent.h"
#include "visitor.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_;
extern const string_q func_DAOpaidOut;
extern const string_q func_DAOrewardAccount;
extern const string_q func_actualBalance;
extern const string_q func_allowance;
extern const string_q func_allowedRecipients;
extern const string_q func_approve;
extern const string_q func_balanceOf;
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
extern const string_q func_totalSupply;
extern const string_q func_transfer;
extern const string_q func_transferFrom;
extern const string_q func_transferFromWithoutReward;
extern const string_q func_transferWithoutReward;
extern const string_q func_unblockMe;
extern const string_q func_vote;

//------------------------------------------------------------------------
extern const string_q evt_AllowedRecipientChanged;
extern const string_q evt_Approval;
extern const string_q evt_CreatedToken;
extern const string_q evt_FuelingToDate;
extern const string_q evt_NewCurator;
extern const string_q evt_ProposalAdded;
extern const string_q evt_ProposalTallied;
extern const string_q evt_Refund;
extern const string_q evt_Transfer;
extern const string_q evt_Voted;

//-----------------------------------------------------------------------------
extern const string_q sigs[];
extern const string_q topics[];

extern const size_t nSigs;
extern const size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE