/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

class CBlockIndexItemStrip {
public:
    uint32_t bn;
    uint32_t ts;
    uint32_t cnt;
};

//-------------------------------------------------------------------------
bool forEveryBlockIndexItem(size_t fullMode, BLOCKINDEXFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
    if (!func)
        return false;

    CArchive finalBlockCache(READING_ARCHIVE);
    if (!finalBlockCache.Lock(finalBlockIndex_v2, modeReadOnly, LOCK_WAIT)) {
        cerr << "forEveryNonEmptyBlockOnDisc failed: " << finalBlockCache.LockFailure() << "\n";
        return false;
    }
    ASSERT(finalBlockCache.isOpen());

    uint64_t nItems = fileSize(finalBlockIndex_v2) / sizeof(CBlockIndexItemStrip);
    CBlockIndexItemStrip *items = new CBlockIndexItemStrip[nItems];
    if (items) {
        // read the entire full block index
        finalBlockCache.Read(items, sizeof(CBlockIndexItemStrip), nItems);
        finalBlockCache.Release();  // release it since we don't need it any longer

        for (uint64_t i = 0 ; i < nItems ; i = i + skip) {

            if (inRange((uint64_t)items[i].bn, start, start + count - 1)) {
                if ((fullMode & F_EMPTY) && items[i].cnt == 0) {
                    CBlockIndexItem ii;
                    ii.bn = items[i].bn;
                    ii.ts = items[i].ts;
                    ii.cnt = items[i].cnt;
                    if (!(*func)(ii, data)) {
                        delete [] items;
                        return false;
                    }
                }

                if ((fullMode & F_FULL) && items[i].cnt != 0) {
                    CBlockIndexItem ii;
                    ii.bn = items[i].bn;
                    ii.ts = items[i].ts;
                    ii.cnt = items[i].cnt;
                    if (!(*func)(ii, data)) {
                        delete [] items;
                        return false;
                    }
                }
            }
        }
        delete [] items;
    }
    return true;
}

#if 0
//#define OLD_FULL_BLOCKS
//-------------------------------------------------------------------------
bool forEveryNonEmptyBlockByNumber(UINT64VISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
    if (!func)
        return false;

#ifdef OLD_FULL_BLOCKS
    CArchive finalBlockCache(READING_ARCHIVE);
    if (!finalBlockCache.Lock(finalBlockIndex, modeReadOnly, LOCK_WAIT)) {
        cerr << "forEveryNonEmptyBlockOnDisc failed: " << finalBlockCache.LockFailure() << "\n";
        return false;
    }
    ASSERT(finalBlockCache.isOpen());

    uint64_t nItems = fileSize(finalBlockIndex) / sizeof(uint64_t);
    uint64_t *items = new uint64_t[nItems];
    if (items) {
        // read the entire full block index
        finalBlockCache.Read(items, sizeof(uint64_t), nItems);
        finalBlockCache.Release();  // release it since we don't need it any longer

        for (uint64_t i = 0 ; i < nItems ; i = i + skip) {
            // TODO(tjayrush): This should be a binary search not a scan. This is why it appears to wait
            uint64_t item = items[i];
            if (inRange(item, start, start + count - 1)) {
                bool ret = (*func)(items[i], data);
                if (!ret) {
                    // Cleanup and return if user tells us to
                    delete [] items;
                    return false;
                }
            } else {
                // do nothing
            }
        }
        delete [] items;
    }
    return true;
#else
    CArchive finalBlockCache(READING_ARCHIVE);
    if (!finalBlockCache.Lock(finalBlockIndex_v2, modeReadOnly, LOCK_WAIT)) {
        cerr << "forEveryNonEmptyBlockOnDisc failed: " << finalBlockCache.LockFailure() << "\n";
        return false;
    }
    ASSERT(finalBlockCache.isOpen());

    uint64_t nItems = fileSize(finalBlockIndex_v2) / sizeof(CBlockIndexItemStrip);
    CBlockIndexItemStrip *items = new CBlockIndexItemStrip[nItems];
    if (items) {
        // read the entire full block index
        finalBlockCache.Read(items, sizeof(CBlockIndexItemStrip), nItems);
        finalBlockCache.Release();  // release it since we don't need it any longer

        for (uint64_t i = 0 ; i < nItems ; i = i + skip) {
            // TODO(tjayrush): This should be a binary search not a scan. This is why it appears to wait
            uint64_t bn = items[i].bn;
            if (items[i].cnt && inRange(bn, start, start + count - 1)) {
                bool ret = (*func)(bn, data);
                if (!ret) {
                    // Cleanup and return if user tells us to
                    delete [] items;
                    return false;
                }
            } else {
                // do nothing
            }
        }
        delete [] items;
    }
    return true;
#endif
}

//-------------------------------------------------------------------------
bool forEveryEmptyBlockByNumber(UINT64VISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
    if (!func)
        return false;

#ifdef OLD_FULL_BLOCKS
    CArchive finalBlockCache(READING_ARCHIVE);
    if (!finalBlockCache.Lock(finalBlockIndex, modeReadOnly, LOCK_WAIT)) {
        cerr << "forEveryEmptyBlockOnDisc failed: " << finalBlockCache.LockFailure() << "\n";
        return false;
    }
    ASSERT(finalBlockCache.isOpen());

    uint64_t nItems = fileSize(finalBlockIndex) / sizeof(uint64_t) + 1;
    uint64_t *items = new uint64_t[nItems+2];
    if (!items) {
        cerr << "forEveryEmptyBlockOnDisc failed: could not allocate memory\n";
        return false;
    }

    finalBlockCache.Read(&items[0], sizeof(uint64_t), nItems);
    finalBlockCache.Release();

    CBlockRangeArray ranges;
    ranges.reserve(nItems * 35 / 100);  // less than 1/3 of blocks are empty

    uint64_t previous = (uint64_t)(start-1);
    uint64_t end = (start + count);
    for (size_t i = 0 ; i < nItems ; i++) {
        uint64_t current = items[i];
        if (start == 0 || (current >= start-1)) {
            int64_t diff = ((int64_t)current - (int64_t)previous) - 1;
            uint64_t udiff = (uint64_t)diff;
            if ((previous+1) <= (previous+udiff))
                ranges.push_back(make_pair(previous+1, min(end, current)));
        }
        previous = current;
        if (current >= end)
            break;
    }

    blknum_t next = NOPOS;
    for (auto range : ranges) {
        blknum_t st = (next == NOPOS ? range.first : (max(next, range.first)));
        for (blknum_t bn = st ; bn < range.second ; bn += skip) {
            if (!(*func)(bn, data)) {
                if (items)
                    delete [] items;
                return false;
            }
            next = bn + skip;
        }
    }
    if (items)
        delete [] items;
    return true;
#else
    CArchive finalBlockCache(READING_ARCHIVE);
    if (!finalBlockCache.Lock(finalBlockIndex_v2, modeReadOnly, LOCK_WAIT)) {
        cerr << "forEveryEmptyBlockOnDisc failed: " << finalBlockCache.LockFailure() << "\n";
        return false;
    }
    ASSERT(finalBlockCache.isOpen());

    uint64_t nItems = fileSize(finalBlockIndex_v2) / sizeof(CBlockIndexItemStrip) + 1;
    CBlockIndexItemStrip *items = new CBlockIndexItemStrip[nItems+2];
    if (!items) {
        cerr << "forEveryEmptyBlockOnDisc failed: could not allocate memory\n";
        return false;
    }

    finalBlockCache.Read(&items[0], sizeof(CBlockIndexItemStrip), nItems);
    finalBlockCache.Release();

    for (uint64_t i = 0 ; i < nItems ; i = i + skip) {
        // TODO(tjayrush): This should be a binary search not a scan. This is why it appears to wait
        uint64_t bn = items[i].bn;
        if (!items[i].cnt && inRange(bn, start, start + count - 1)) {
            bool ret = (*func)(bn, data);
            if (!ret) {
                // Cleanup and return if user tells us to
                delete [] items;
                return false;
            }
        } else {
            // do nothing
        }
    }

    return true;
#endif
}

//-------------------------------------------------------------------------
class CPassThru {
public:
    BLOCKVISITFUNC origFunc;
    void *origData;
};

//-------------------------------------------------------------------------
bool passThruFunction(uint64_t num, void *data) {
    CBlock block;
    getBlock(block, num);
    CPassThru *passThru = (CPassThru*)data;
    return passThru->origFunc(block, passThru->origData);
}

//-------------------------------------------------------------------------
bool forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
    CPassThru passThru;
    passThru.origFunc = func;
    passThru.origData = data;
    getCurlContext()->provider = "local";
    bool ret = forEveryNonEmptyBlockByNumber(passThruFunction, &passThru, start, count, skip);
    getCurlContext()->provider = "binary";
    return ret;
}

//-------------------------------------------------------------------------
bool forEveryEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip) {
    CPassThru passThru;
    passThru.origFunc = func;
    passThru.origData = data;
    getCurlContext()->provider = "local";
    bool ret = forEveryEmptyBlockByNumber(passThruFunction, &passThru, start, count, skip);
    getCurlContext()->provider = "binary";
    return ret;
}
#endif
