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
#include "confirmedevent.h"
#include "newownerevent.h"
#include "puzzledevent.h"
#include "requestedevent.h"
#include "revokedevent.h"
#include "certify.h"
#include "drain.h"
#include "puzzle.h"
#include "request.h"
#include "revoke.h"
#include "setdelegate.h"
#include "setfee.h"
#include "setowner.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_certified;
extern const string_q func_certify;
extern const string_q func_delegate;
extern const string_q func_drain;
extern const string_q func_fee;
extern const string_q func_get;
extern const string_q func_getAddress;
extern const string_q func_getUint;
extern const string_q func_owner;
extern const string_q func_puzzle;
extern const string_q func_request;
extern const string_q func_revoke;
extern const string_q func_setDelegate;
extern const string_q func_setFee;
extern const string_q func_setOwner;

//------------------------------------------------------------------------
extern const string_q evt_Confirmed;
extern const string_q evt_NewOwner;
extern const string_q evt_Puzzled;
extern const string_q evt_Requested;
extern const string_q evt_Revoked;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
