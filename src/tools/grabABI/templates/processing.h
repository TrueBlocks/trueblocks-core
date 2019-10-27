#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */

// EXISTING_CODE
// EXISTING_CODE

//-----------------------------------------------------------------------
#include "etherlib.h"
#include "options.h"
#include "visitor.h"
#include "debug.h"

//-----------------------------------------------------------------------
extern void do_resize      (int dummy);
extern void myQuitHandler  (int s);
inline void myOnExitHandler(void) { myQuitHandler(1); }

// EXISTING_CODE
// EXISTING_CODE
