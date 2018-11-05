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
#include "bidsubmissionevent.h"
#include "confirmationevent.h"
#include "dailylimitchangeevent.h"
#include "executionevent.h"
#include "executionfailureevent.h"
#include "owneradditionevent.h"
#include "ownerremovalevent.h"
#include "refundreceivedevent.h"
#include "refundsubmissionevent.h"
#include "requirementchangeevent.h"
#include "revocationevent.h"
#include "submissionevent.h"
#include "bid.h"
#include "bidproxy.h"
#include "calccurrenttokenprice.h"
#include "changedailylimit.h"
#include "changesettings.h"
#include "claimproxy.h"
#include "claimtokens.h"
#include "confirmtransaction.h"
#include "contribute.h"
#include "executetransaction.h"
#include "refund.h"
#include "replaceowner.h"
#include "revokeconfirmation.h"
#include "setup.h"
#include "startauction.h"
#include "submittransaction.h"
#include "transfer.h"
#include "updatestage.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_AUCTIONSTARTED;
extern const string_q func_MAXOWNERCOUNT;
extern const string_q func_MAXTOKENSSOLD;
extern const string_q func_TRADINGSTARTED;
extern const string_q func_WAITINGPERIOD;
extern const string_q func_bid;
extern const string_q func_bidProxy;
extern const string_q func_bids;
extern const string_q func_calcCurrentTokenPrice;
extern const string_q func_calcMaxWithdraw;
extern const string_q func_calcStopPrice;
extern const string_q func_calcTokenPrice;
extern const string_q func_ceiling;
extern const string_q func_changeDailyLimit;
extern const string_q func_changeSettings;
extern const string_q func_claimProxy;
extern const string_q func_claimTokens;
extern const string_q func_confirmTransaction;
extern const string_q func_confirmations;
extern const string_q func_contribute;
extern const string_q func_contributions;
extern const string_q func_dailyLimit;
extern const string_q func_dutchAuction;
extern const string_q func_endTime;
extern const string_q func_executeTransaction;
extern const string_q func_finalPrice;
extern const string_q func_getConfirmationCount;
extern const string_q func_getConfirmations;
extern const string_q func_getOwners;
extern const string_q func_getTransactionCount;
extern const string_q func_getTransactionIds;
extern const string_q func_gnosisToken;
extern const string_q func_isConfirmed;
extern const string_q func_lastDay;
extern const string_q func_owner;
extern const string_q func_owners;
extern const string_q func_priceFactor;
extern const string_q func_refund;
extern const string_q func_replaceOwner;
extern const string_q func_required;
extern const string_q func_revokeConfirmation;
extern const string_q func_setup;
extern const string_q func_spentToday;
extern const string_q func_stage;
extern const string_q func_startAuction;
extern const string_q func_startBlock;
extern const string_q func_submitTransaction;
extern const string_q func_totalBalance;
extern const string_q func_totalContributions;
extern const string_q func_totalReceived;
extern const string_q func_totalTokens;
extern const string_q func_transactionCount;
extern const string_q func_transactions;
extern const string_q func_transfer;
extern const string_q func_updateStage;
extern const string_q func_wallet;

//------------------------------------------------------------------------
extern const string_q evt_BidSubmission;
extern const string_q evt_Confirmation;
extern const string_q evt_DailyLimitChange;
extern const string_q evt_Execution;
extern const string_q evt_ExecutionFailure;
extern const string_q evt_OwnerAddition;
extern const string_q evt_OwnerRemoval;
extern const string_q evt_RefundReceived;
extern const string_q evt_RefundSubmission;
extern const string_q evt_RequirementChange;
extern const string_q evt_Revocation;
extern const string_q evt_Submission;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
