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
#include "burnevent.h"
#include "lognoteevent.h"
#include "logsetauthorityevent.h"
#include "logsetownerevent.h"
#include "mintevent.h"
#include "approve.h"
#include "burn.h"
#include "burn9f8f.h"
#include "mint.h"
#include "mint9f8f.h"
#include "move.h"
#include "pull.h"
#include "push.h"
#include "setauthority.h"
#include "setname.h"
#include "setowner.h"
#include "start.h"
#include "stop.h"
#include "processing.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh=defaultQuitHandler);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
extern const string_q func_approve;
extern const string_q func_authority;
extern const string_q func_burn;
extern const string_q func_burn9f8f;
extern const string_q func_mint;
extern const string_q func_mint9f8f;
extern const string_q func_move;
extern const string_q func_owner;
extern const string_q func_pull;
extern const string_q func_push;
extern const string_q func_setAuthority;
extern const string_q func_setName;
extern const string_q func_setOwner;
extern const string_q func_start;
extern const string_q func_stop;
extern const string_q func_stopped;

//------------------------------------------------------------------------
extern const string_q evt_Burn;
extern const string_q evt_LogNote;
extern const string_q evt_LogSetAuthority;
extern const string_q evt_LogSetOwner;
extern const string_q evt_Mint;

//-----------------------------------------------------------------------------
extern string_q sigs[];
extern string_q topics[];

extern size_t nSigs;
extern size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE
