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

#include "auctionstartedevent.h"
#include "bidrevealedevent.h"
#include "hashinvalidatedevent.h"
#include "hashregisteredevent.h"
#include "hashreleasedevent.h"
#include "newbidevent.h"
#include "acceptregistrartransfer.h"
#include "cancelbid.h"
#include "erasenode.h"
#include "finalizeauction.h"
#include "invalidatename.h"
#include "newbid.h"
#include "releasedeed.h"
#include "startauction.h"
#include "startauctions.h"
#include "startauctionsandbid.h"
#include "transfer.h"
#include "transferanyerc20token.h"
#include "transferownership.h"
#include "transferregistrars.h"
#include "unsealbid.h"
#include "unsealbid0122.h"

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
