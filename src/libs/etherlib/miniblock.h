#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

namespace qblocks {

//------------------------------------------------------------------------
class CMiniBlock
{
public:
    SFUint32 blockNumber;
    SFUint32 timestamp;
    SFUint32 firstTrans;
    SFUint32 nTrans;

    CMiniBlock(CBlock *block) {
        bzero(this, sizeof(CMiniBlock));
        blockNumber = block->blockNumber;
        timestamp = block->timestamp;
        firstTrans = 0;
        nTrans = 0;
    }

    void toBlock(CBlock& block) const {
        block.blockNumber = blockNumber;
        block.timestamp = timestamp;
        return;
    }

    SFString Format(void) const {
        CStringExportContext ctx;
        ctx << "blockNumber: " << blockNumber << " timestamp: " << timestamp << " firstTrans: " << firstTrans << " nTrans: " << nTrans;
        return ctx.str;
    }

    bool operator==(const CBlock& b) const {
        if (b.blockNumber != blockNumber) return false;
        if (b.timestamp != timestamp) return false;
        if (b.transactions.getCount() != nTrans) return false;
        return true;
    }

    CMiniBlock() { }
};

//------------------------------------------------------------------------
class CMiniTrans
{
public:
    SFUint32  index;
    SFUint32  gasUsed;
    SFUint32  gasPrice;
    SFUint32  gas;
    SFUint32  isError;
    char      from [41];
    char      to   [41];
    char      value[41];

    CMiniTrans(CTransaction *t)
    {
        bzero(this, sizeof(CMiniTrans));
        index = t->transactionIndex;
        gasUsed = t->receipt.gasUsed;
        gasPrice = t->gasPrice;
        gas = canonicalWeiL(t->gas);
        isError = t->isError;
        strncpy(from,  (const char*)fromAddress(t->from).substr(2), 40);from [40]='\0';
        strncpy(to,    (const char*)fromAddress(t->to).substr(2),   40);to   [40]='\0';
        strncpy(value, (const char*)fromWei(t->value),              40);value[40]='\0';
    }

    void toTrans(CTransaction& trans) const {
        trans.transactionIndex = index;
        trans.gas = gas;
        trans.receipt.gasUsed = gasUsed;
        trans.gasPrice = gasPrice;
        trans.isError = isError;
        trans.from = toAddress(from);
        trans.to = toAddress(to);
        trans.value = toWei(value);
        return;
    }

    SFString Format(void) const {
        CStringExportContext ctx;
        ctx << "index: " << index
        << " from: " << from
        << " to: " << to
        << " value: " << value
        << " isError: " << isError
        << " gas: " << gas
        << " gasUsed: " << gasUsed
        << " gasPrice: " << gasPrice;
        return ctx.str;
    }

    CMiniTrans() {}
};
}  // namespace qblocks
