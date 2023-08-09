#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "acctlib.h"

//-----------------------------------------------------------------------
extern bool loadTx_Func(CTraverser* trav, void* data);
extern bool pre_Func(CTraverser* trav, void* data);
extern bool post_Func(CTraverser* trav, void* data);
extern void start_Log(CTraverser* trav, void* data);
extern bool prog_Log(CTraverser* trav, void* data);
extern void end_Log(CTraverser* trav, void* data);

extern bool acct_Display(CTraverser* trav, void* data);
extern bool acct_PreFunc(CTraverser* trav, void* data);
class CTransactionTraverser : public CTraverser {
  public:
    CTransactionTraverser(void);
    ~CTransactionTraverser(void) {
    }
};
