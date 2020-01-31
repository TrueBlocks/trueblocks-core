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

#define DEBUGGER_ON
// EXISTING_CODE
// EXISTING_CODE

#ifdef DEBUGGER_ON
#define MAX_TX 15
//-----------------------------------------------------------------
class CTransID {
  public:
    blknum_t bn;
    blknum_t tx;
    hash_t hash;
    CTransID(blknum_t b, blknum_t t, const hash_t& h) : bn(b), tx(t), hash(h) {
    }
    CTransID(void) : bn(0), tx(0), hash("") {
    }
};

//-----------------------------------------------------------------
class CTransBuffer : public vector<CTransID> {
  public:
    uint32_t writeCursor;
    uint32_t readCursor;

  public:
    CTransBuffer(void) : writeCursor(0), readCursor((uint32_t)NOPOS) {
        resize(15);
    }
    void addItem(blknum_t b, blknum_t t, const hash_t& h) {
        at(writeCursor) = CTransID(b, t, h);
        writeCursor = (writeCursor + 1) % MAX_TX;
        if (readCursor == (uint32_t)NOPOS)
            readCursor = writeCursor;
    }
    void getPrev(blknum_t& b, blknum_t& t, hash_t& h) {
        CTransID ti(0, 0, "");
        ti = vector<CTransID>::operator[](readCursor);
        b = ti.bn;
        t = ti.tx;
        h = ti.hash;
        if (readCursor == 0)
            readCursor = MAX_TX - 1;
        else
            readCursor--;
    }
};

//-----------------------------------------------------------------
extern CTransBuffer tBuffer;
extern bool debugFile(void);
#endif

// EXISTING_CODE
// EXISTING_CODE