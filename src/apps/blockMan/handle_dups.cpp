/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

int sortByNum(const void *v1, const void *v2) {
    blknum_t b1 = *(blknum_t*)v1;
    blknum_t b2 = *(blknum_t*)v2;
    return (int)(b1 - b2);
}

//----------------------------------------------------------------------------------
void handle_remove_dups(COptions& options) {

#if 0
    cerr << "Opening\n";
    CArchive fullBlockCache(READING_ARCHIVE);
    if (!fullBlockCache.Lock(fullBlockIndex, binaryReadOnly, LOCK_WAIT)) {
        cerr << "handle_remove_dups failed: " << fullBlockCache.LockFailure() << "\n";
        return;
    }
    ASSERT(fullBlockCache.isOpen());

    uint64_t nItems = fileSize(fullBlockIndex) / sizeof(uint64_t) + 1;
    uint64_t *items = new uint64_t[nItems+2];
    if (!items) {
        cerr << "handle_remove_dups failed: could not allocate memory\n";
        return;
    }

    cerr << "Reading\n";
    fullBlockCache.Read(&items[0], sizeof(uint64_t), nItems);
    fullBlockCache.Release();

    qsort(items, nItems, sizeof(uint64_t), sortByNum);

#if 1
    size_t cnt=nItems;
#else
    cerr << "De-duping\n";
    size_t cnt=0;
    for (size_t i = 0 ; i < nItems-1 ; i++) {
        if (items[i] != items[i+1])
            items[cnt++] = items[i];
    }
    if (items[cnt] != items[nItems-1])
        items[cnt++] = items[nItems-1];
#endif

    cerr << "Listing\n";
    for (size_t i = 0 ; i < cnt ; i++) {
        cout << items[i] << endl;
    }
    cerr.flush();
    cout.flush();

#if 0
    cerr << "Reopening\n";
    CArchive fullBlockCache1(WRITING_ARCHIVE);
    if (!fullBlockCache1.Lock(fullBlockIndex, binaryWriteCreate, LOCK_WAIT)) {
        cerr << "handle_remove_dups failed: " << fullBlockCache1.LockFailure() << "\n";
        return;
    }
    ASSERT(fullBlockCache1.isOpen());
    fullBlockCache1.Write(&items[0], sizeof(uint64_t), cnt);
    fullBlockCache1.Release();
#endif
#endif
}
