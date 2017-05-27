#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "utillib.h"
#include "etherlib.h"

//---------------------------------------------------------------------------------------------------
class CSlurperApp
{
private:
	CToml toml;
	CWebAPI api;

public:
	CAccount theAccount;

    CSlurperApp(void) : toml() { };
    ~CSlurperApp(void) { }
private:
};

//---------------------------------------------------------------------------------------------------
extern void findBlockRange(const SFString& contents, uint32_t& minBlock, uint32_t& maxBlock);
