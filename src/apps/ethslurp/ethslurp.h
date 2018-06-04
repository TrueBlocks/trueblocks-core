#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include "etherlib.h"
#include "webapi.h"
#include "account.h"
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
