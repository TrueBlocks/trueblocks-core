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
#include "emergencystopactivatedevent.h"
#include "tokenmintevent.h"
#include "tokenswapoverevent.h"
#include "buytokens.h"
#include "disabletokenswaplock.h"
#include "mintreserve.h"
#include "minttokens.h"
#include "settokencontract.h"
#include "starttokenswap.h"
#include "stoptoken.h"
#include "stoptokenswap.h"
#include "withdrawreserve.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_MAXETH;
extern const string_q func_PRESALELENGTH;
extern const string_q func_SWAPLENGTH;
extern const string_q func_TRANSITIONWINDOW;
extern const string_q func_amountRaised;
extern const string_q func_buyTokens;
extern const string_q func_creationTime;
extern const string_q func_currentSwapRate;
extern const string_q func_disableTokenSwapLock;
extern const string_q func_getOwner;
extern const string_q func_mintReserve;
extern const string_q func_mintTokens;
extern const string_q func_mlastDay;
extern const string_q func_mspentToday;
extern const string_q func_setTokenContract;
extern const string_q func_startTokenSwap;
extern const string_q func_stopToken;
extern const string_q func_stopTokenSwap;
extern const string_q func_tokenCap;
extern const string_q func_tokenCtr;
extern const string_q func_tokenSwap;
extern const string_q func_transferStop;
extern const string_q func_walletAddress;
extern const string_q func_withdrawReserve;

//------------------------------------------------------------------------
extern const string_q evt_EmergencyStopActivated;
extern const string_q evt_TokenMint;
extern const string_q evt_TokenSwapOver;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
