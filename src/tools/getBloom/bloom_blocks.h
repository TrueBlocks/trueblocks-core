#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
