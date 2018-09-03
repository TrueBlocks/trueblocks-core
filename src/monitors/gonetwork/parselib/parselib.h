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

#include "auctionendedevent.h"
#include "auctionstartedevent.h"
#include "bidsubmissionevent.h"
#include "claimedtokensevent.h"
#include "deployedevent.h"
#include "deployed423bevent.h"
#include "privatesalewhitelistedevent.h"
#include "publicsalewhitelistedevent.h"
#include "removedfromprivatesalewhitelistevent.h"
#include "removedfrompublicsalewhitelistevent.h"
#include "setupevent.h"
#include "tokensdistributedevent.h"
#include "addtoprivatesalewhitelist.h"
#include "addtopublicsalewhitelist.h"
#include "bid.h"
#include "distributegotokens.h"
#include "finalizeauction.h"
#include "removefromprivatesalewhitelist.h"
#include "removefrompublicsalewhitelist.h"
#include "setup.h"
#include "startauction.h"

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
