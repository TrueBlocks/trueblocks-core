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
#include "confirmationevent.h"
#include "escapehatchblackistedtokenevent.h"
#include "escapehatchcalledevent.h"
#include "executionevent.h"
#include "executionfailureevent.h"
#include "fundssentevent.h"
#include "owneradditionevent.h"
#include "ownerremovalevent.h"
#include "ownershipremovedevent.h"
#include "ownershiprequestedevent.h"
#include "ownershiptransferredevent.h"
#include "requirementchangeevent.h"
#include "revocationevent.h"
#include "submissionevent.h"
#include "acceptownership.h"
#include "changehatchescapecaller.h"
#include "changeowner.h"
#include "changeownership.h"
#include "confirmtransaction.h"
#include "escapehatch.h"
#include "executetransaction.h"
#include "fill.h"
#include "finalizefunding.h"
#include "onapprove.h"
#include "ontransfer.h"
#include "proposeownership.h"
#include "proxypayment.h"
#include "removeownership.h"
#include "replaceowner.h"
#include "revokeconfirmation.h"
#include "seal.h"
#include "setvault.h"
#include "submittransaction.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_MAXOWNERCOUNT;
extern const string_q func_acceptOwnership;
extern const string_q func_beneficiary;
extern const string_q func_changeHatchEscapeCaller;
extern const string_q func_changeOwner;
extern const string_q func_changeOwnership;
extern const string_q func_confirmTransaction;
extern const string_q func_confirmations;
extern const string_q func_endFundingTime;
extern const string_q func_escapeHatch;
extern const string_q func_escapeHatchCaller;
extern const string_q func_escapeHatchDestination;
extern const string_q func_executeTransaction;
extern const string_q func_fill;
extern const string_q func_finalizeFunding;
extern const string_q func_getConfirmationCount;
extern const string_q func_getConfirmations;
extern const string_q func_getOwner;
extern const string_q func_getOwners;
extern const string_q func_getTransactionCount;
extern const string_q func_getTransactionIds;
extern const string_q func_isConfirmed;
extern const string_q func_isTokenEscapable;
extern const string_q func_maximumFunding;
extern const string_q func_newOwnerCandidate;
extern const string_q func_onApprove;
extern const string_q func_onTransfer;
extern const string_q func_owner;
extern const string_q func_owners;
extern const string_q func_proposeOwnership;
extern const string_q func_proxyPayment;
extern const string_q func_removeOwnership;
extern const string_q func_replaceOwner;
extern const string_q func_required;
extern const string_q func_revokeConfirmation;
extern const string_q func_seal;
extern const string_q func_sealed;
extern const string_q func_setVault;
extern const string_q func_startFundingTime;
extern const string_q func_submitTransaction;
extern const string_q func_tokenContract;
extern const string_q func_totalCollected;
extern const string_q func_transactionCount;
extern const string_q func_transactions;
extern const string_q func_vaultAddress;

//------------------------------------------------------------------------
extern const string_q evt_Confirmation;
extern const string_q evt_EscapeHatchBlackistedToken;
extern const string_q evt_EscapeHatchCalled;
extern const string_q evt_Execution;
extern const string_q evt_ExecutionFailure;
extern const string_q evt_FundsSent;
extern const string_q evt_OwnerAddition;
extern const string_q evt_OwnerRemoval;
extern const string_q evt_OwnershipRemoved;
extern const string_q evt_OwnershipRequested;
extern const string_q evt_OwnershipTransferred;
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
