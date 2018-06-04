#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "utillib.h"
#include "etherlib.h"

//---------------------------------------------------------------------------------------------------
class CSlurperApp {
private:
    CToml toml;
    CWebAPI api;

public:
    CAccount theAccount;

    CSlurperApp(void) : toml("") { }
    ~CSlurperApp(void) { }
private:
};

//---------------------------------------------------------------------------------------------------
extern void findBlockRange(const SFString& contents, uint32_t& minBlock, uint32_t& maxBlock);
