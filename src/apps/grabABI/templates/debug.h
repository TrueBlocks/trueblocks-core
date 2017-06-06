#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */

//EXISTING_CODE
//EXISTING_CODE

#define MAX_TX 15
class CTransID {
public:
    blknum_t bn;
    blknum_t tx;
    CTransID(blknum_t b, blknum_t t) : bn(b), tx(t) { }
    CTransID(void) : bn(0), tx(0) {}
};
class CTransBuffer : public SFArrayBase<CTransID> {
private:
    uint32_t writeCursor;
    uint32_t readCursor;
public:
    CTransBuffer(void) : writeCursor(0), readCursor((uint32_t)NOPOS) {}
    void addItem(blknum_t b, blknum_t t) {
        SFArrayBase<CTransID>::operator[](writeCursor) = CTransID(b,t);
        writeCursor = (writeCursor + 1) % MAX_TX;
        if (readCursor == (uint32_t)NOPOS)
            readCursor = writeCursor;
    }
    void getPrev(blknum_t& b, blknum_t& t) {
        CTransID ti(0,0);
        ti = SFArrayBase<CTransID>::operator[](readCursor);
        b = ti.bn ; t = ti.tx;
        if (readCursor == 0)
            readCursor = MAX_TX-1;
        else
            readCursor--;
    }
};
extern CTransBuffer tBuffer;
