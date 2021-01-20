#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
 /*
  *
  * This code was generated automatically from grabABI and makeClass You may 
  * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
  *
  */
#include "etherlib.h"
#include "tokenlib.h"
#include "walletlib.h"
#include "visitor.h"

//------------------------------------------------------------------------
extern void parselib_init(QUITHANDLER qh);
extern const CTransaction *promoteToFunc(const CTransaction *p);
extern const CLogEntry *promoteToEvent(const CLogEntry *p);

//------------------------------------------------------------------------
// No functions
//------------------------------------------------------------------------
// No events

//-----------------------------------------------------------------------------
extern const string_q sigs[];
extern const string_q topics[];

extern const size_t nSigs;
extern const size_t nTopics;

// EXISTING_CODE
// EXISTING_CODE