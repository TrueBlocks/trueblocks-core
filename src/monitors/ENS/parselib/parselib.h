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
#include "transferregistrars.h"
#include "unsealbid.h"
#include "unsealbid6090.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_acceptRegistrarTransfer;
extern const string_q func_cancelBid;
extern const string_q func_ens;
extern const string_q func_entries;
extern const string_q func_eraseNode;
extern const string_q func_finalizeAuction;
extern const string_q func_getAllowedTime;
extern const string_q func_invalidateName;
extern const string_q func_isAllowed;
extern const string_q func_launchLength;
extern const string_q func_newBid;
extern const string_q func_registryStarted;
extern const string_q func_releaseDeed;
extern const string_q func_rootNode;
extern const string_q func_sealedBids;
extern const string_q func_shaBid;
extern const string_q func_startAuction;
extern const string_q func_startAuctions;
extern const string_q func_startAuctionsAndBid;
extern const string_q func_state;
extern const string_q func_transfer;
extern const string_q func_transferRegistrars;
extern const string_q func_unsealBid;
extern const string_q func_unsealBid6090;

//------------------------------------------------------------------------
extern const string_q evt_AuctionStarted;
extern const string_q evt_BidRevealed;
extern const string_q evt_HashInvalidated;
extern const string_q evt_HashRegistered;
extern const string_q evt_HashReleased;
extern const string_q evt_NewBid;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
