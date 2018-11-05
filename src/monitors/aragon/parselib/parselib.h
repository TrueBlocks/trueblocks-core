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
#include "newclonetokenevent.h"
#include "newtokengrantevent.h"
#include "changecontroller.h"
#include "changevestingwhitelister.h"
#include "createclonetoken.h"
#include "destroytokens.h"
#include "enabletransfers.h"
#include "generatetokens.h"
#include "grantvestedtokens.h"
#include "onapprove.h"
#include "ontransfer.h"
#include "proxypayment.h"
#include "revoketokengrant.h"
#include "setcancreategrants.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_balanceOfAt;
extern const string_q func_changeController;
extern const string_q func_changeVestingWhitelister;
extern const string_q func_controller;
extern const string_q func_createCloneToken;
extern const string_q func_creationBlock;
extern const string_q func_destroyTokens;
extern const string_q func_enableTransfers;
extern const string_q func_generateTokens;
extern const string_q func_grantVestedTokens;
extern const string_q func_grants;
extern const string_q func_lastTokenIsTransferableDate;
extern const string_q func_onApprove;
extern const string_q func_onTransfer;
extern const string_q func_parentSnapShotBlock;
extern const string_q func_parentToken;
extern const string_q func_proxyPayment;
extern const string_q func_revokeTokenGrant;
extern const string_q func_sale;
extern const string_q func_setCanCreateGrants;
extern const string_q func_spendableBalanceOf;
extern const string_q func_token;
extern const string_q func_tokenFactory;
extern const string_q func_tokenGrant;
extern const string_q func_tokenGrantsCount;
extern const string_q func_totalSupplyAt;
extern const string_q func_transferableTokens;
extern const string_q func_transfersEnabled;

//------------------------------------------------------------------------
extern const string_q evt_NewCloneToken;
extern const string_q evt_NewTokenGrant;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
