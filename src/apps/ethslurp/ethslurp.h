#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <algorithm>
#include "etherlib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
class CSlurperApp {
private:
    CToml toml;
    CWebAPI api;

public:
    CAccount theAccount;

    CSlurperApp(void) : toml("") { }
    ~CSlurperApp(void) { }

    bool Initialize(COptions& options, SFString& message);
    bool Slurp(COptions& options, SFString& message);
    bool Filter(COptions& options, SFString& message);
    bool Display(COptions& options, SFString& message);

private:
    void buildDisplayStrings(COptions& options);
    SFString getFormatString(COptions& options, const SFString& name, bool ignoreBlank);
};

//---------------------------------------------------------------------------------------------------
extern bool establishFolders(CToml& toml);
extern void findBlockRange(const SFString& contents, uint32_t& minBlock, uint32_t& maxBlock);

extern CFileExportContext outScreen;
