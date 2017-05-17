#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 *------------------------------------------------------------------------*/
#include "basetypes.h"

namespace qblocks {

    //-------------------------------------------------------------------------------
    template <class BaseType>
    class SFBigNumStore {
    public:
        static const unsigned int N;
        unsigned int capacity;
        unsigned int len;
        BaseType *blk;

        SFBigNumStore(void);
        SFBigNumStore(const SFBigNumStore<BaseType>& x);
        SFBigNumStore<BaseType>& operator=(const SFBigNumStore<BaseType>& x);

        explicit SFBigNumStore(unsigned int size);
        SFBigNumStore(const BaseType *b, unsigned int len);
        ~SFBigNumStore();

        void allocate(unsigned int size);
        void allocateAndCopy(unsigned int size);

        bool operator==(const SFBigNumStore<BaseType>& x) const;
        bool operator!=(const SFBigNumStore<BaseType>& x) const { return !operator==(x); }
    };

    //-------------------------------------------------------------------------------
    template <class BaseType>
    const unsigned int SFBigNumStore<BaseType>::N = 8 * sizeof(BaseType);

    //-------------------------------------------------------------------------------
    template <class BaseType>
    inline SFBigNumStore<BaseType>::SFBigNumStore(void) : capacity(0), len(0) {
        blk = 0;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    inline SFBigNumStore<BaseType>::SFBigNumStore(unsigned int c) : capacity(c), len(0) {
        blk = (capacity > 0) ? (new BaseType[capacity]) : 0;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    SFBigNumStore<BaseType>& SFBigNumStore<BaseType>::operator=(const SFBigNumStore<BaseType> &x) {
        if (this == &x)
            return *this;

        len = x.len;
        allocate(len);
        for (unsigned int i = 0; i < len; i++)
            blk[i] = x.blk[i];
        return *this;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    SFBigNumStore<BaseType>::SFBigNumStore(const SFBigNumStore<BaseType> &x) : len(x.len) {
        capacity = len;
        blk = new BaseType[capacity];
        for (unsigned int i = 0; i < len; i++)
            blk[i] = x.blk[i];
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    SFBigNumStore<BaseType>::SFBigNumStore(const BaseType *b, unsigned int len) : capacity(len), len(len) {
#if 1
        blk = new BaseType[capacity];
        for (unsigned int i = 0; i < len; i++)
            blk[i] = b[i];
#else
        blk = new BaseType[capacity];
        for (unsigned int i = 0; i < len; i++)
            blk[i] = (b ? b[i] : 0);
#endif
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    inline SFBigNumStore<BaseType>::~SFBigNumStore() {
        if (blk)
            delete [] blk;
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    void SFBigNumStore<BaseType>::allocate(unsigned int size) {
        if (size > capacity) {
            if (blk)
                delete [] blk;
            capacity = size;
            blk = new BaseType[capacity];
        }
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    void SFBigNumStore<BaseType>::allocateAndCopy(unsigned int size) {
        if (size > capacity) {
            BaseType *oldBlk = blk;
            capacity = size;
            blk = new BaseType[capacity];
            for (unsigned int i = 0; i < len; i++)
                blk[i] = oldBlk[i];
            delete [] oldBlk;
        }
    }

    //-------------------------------------------------------------------------------
    template <class BaseType>
    bool SFBigNumStore<BaseType>::operator==(const SFBigNumStore<BaseType> &x) const {
        if (len != x.len)
            return false;
        
        for (unsigned int i = 0; i < len; i++)
            if (blk[i] != x.blk[i])
                return false;
        return true;
    }
    
    //-------------------------------------------------------------------------------
#define thisIsMe(a) (this == &a)
    
}
using namespace qblocks;
