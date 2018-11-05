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
#include "migratedevent.h"
#include "pauseevent.h"
#include "unpauseevent.h"
#include "migratebalance.h"
#include "migratebalances.h"
#include "pause.h"
#include "transferownership.h"
#include "unpause.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_initialized;
extern const string_q func_legacyRepContract;
extern const string_q func_migrateBalance;
extern const string_q func_migrateBalances;
extern const string_q func_owner;
extern const string_q func_pause;
extern const string_q func_paused;
extern const string_q func_targetSupply;
extern const string_q func_transferOwnership;
extern const string_q func_unpause;

//------------------------------------------------------------------------
extern const string_q evt_Migrated;
extern const string_q evt_Pause;
extern const string_q evt_Unpause;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
