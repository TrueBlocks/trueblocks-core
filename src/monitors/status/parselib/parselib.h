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

#include "claimedtokensevent.h"
#include "controllerchangedevent.h"
#include "newclonetokenevent.h"
#include "acceptownership.h"
#include "changecontroller.h"
#include "changeowner.h"
#include "claimtokens.h"
#include "createclonetoken.h"
#include "createclonetokena1c9.h"
#include "destroytokens.h"
#include "enabletransfers.h"
#include "generatetokens.h"
#include "onapprove.h"
#include "ontransfer.h"
#include "proxypayment.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
