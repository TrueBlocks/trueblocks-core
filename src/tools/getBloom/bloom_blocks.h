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
#include "etherlib.h"

#define IMPLEMENT_UNUSED(CLASS_NAME) \
inline void CLASS_NAME::finishParse(void) {} \
inline bool CLASS_NAME::Serialize(SFArchive& archive) {return true;} \
inline bool CLASS_NAME::SerializeC(SFArchive& archive) const {return true;} \
inline bool CLASS_NAME::handleCustomFormat(CExportContext& ctx, SFString const& fieldName, void*data) const {return false;}

//-----------------------------------------------------------------------------
class CBloomReceipt : public CBaseNode {
public:
    SFString logsBloom;
    DECLARE_NODE(CBloomReceipt);
    CBloomReceipt(void);
};
IMPLEMENT_UNUSED(CBloomReceipt);

//-----------------------------------------------------------------------------
class CBloomTrans : public CBaseNode {
public:
    SFString hash;
    blknum_t transactionIndex;
    CBloomReceipt receipt;
    DECLARE_NODE(CBloomTrans);
    CBloomTrans(void);
    const CBaseNode *getObjectAt(const SFString& name, uint32_t i) const override;
};
IMPLEMENT_UNUSED(CBloomTrans);

//-----------------------------------------------------------------------------
class CBloomBlock : public CBaseNode {
public:
    SFString logsBloom;
    blknum_t number;
    SFArrayBase<CBloomTrans> transactions;
    DECLARE_NODE(CBloomBlock);
    CBloomBlock(void);
    const CBaseNode *getObjectAt(const SFString& name, uint32_t i) const override;
};
IMPLEMENT_UNUSED(CBloomBlock);
