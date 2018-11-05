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
#include "basetransferedevent.h"
#include "newbaseevent.h"
#include "newhandleevent.h"
#include "ownershiptransferredevent.h"
#include "changeprice.h"
#include "getbasequick.h"
#include "registerbase.h"
#include "registerhandle.h"
#include "transferbase.h"
#include "transferownership.h"
#include "withdraw.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_baseRegistred;
extern const string_q func_changePrice;
extern const string_q func_doesOwnBase;
extern const string_q func_findAddress;
extern const string_q func_getBaseQuick;
extern const string_q func_getPrice;
extern const string_q func_handleIndex;
extern const string_q func_isRegistered;
extern const string_q func_owner;
extern const string_q func_ownsBase;
extern const string_q func_price;
extern const string_q func_registerBase;
extern const string_q func_registerHandle;
extern const string_q func_transferBase;
extern const string_q func_transferOwnership;
extern const string_q func_withdraw;

//------------------------------------------------------------------------
extern const string_q evt_BaseTransfered;
extern const string_q evt_NewBase;
extern const string_q evt_NewHandle;
extern const string_q evt_OwnershipTransferred;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
