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
#include <algorithm>
#include "basetypes.h"

namespace qblocks {

    //----------------------------------------------------------------------
    #define ARRAY_CHUNK_SIZE 100

    //-------------------------------------------------------------------------
    typedef int  (*SEARCHFUNC)    (const void *ob1, const void *ob2);
    typedef int  (*SORTINGFUNC)   (const void *ob1, const void *ob2);
    typedef bool (*APPLYFUNC)     (string_q& line, void *data);

    //----------------------------------------------------------------------
    template<class TYPE>
    class SFArrayBase {
    public:
        size_t m_nCapacity;
        size_t m_nItems;
        TYPE  *m_Items;

    public:
        SFArrayBase(void);
        SFArrayBase(const SFArrayBase& cop);
        ~SFArrayBase(void);

        SFArrayBase& operator=(const SFArrayBase& cop);

              TYPE&  at        (size_t index);
        const TYPE&  operator[](size_t index) const;
              size_t capacity  (void) const { return m_nCapacity; }
              size_t size      (void) const { return m_nItems; }
              void   push_back (TYPE x);
              void   clear     (void);
              void   reserve   (size_t newSize);
              void   resize    (size_t newSize) { reserve(newSize); }

    private:
        void checkSize(size_t sizeNeeded);
        void duplicate(const SFArrayBase& cop);
        void initialize(size_t cap, size_t count, TYPE *values);
    };

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>::SFArrayBase(void) {
        initialize(0, 0, NULL);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>::SFArrayBase(const SFArrayBase<TYPE>& cop) {
        duplicate(cop);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>::~SFArrayBase(void) {
        clear();
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>& SFArrayBase<TYPE>::operator=(const SFArrayBase<TYPE>& cop) {
        clear();
        duplicate(cop);
        return *this;
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::initialize(size_t cap, size_t count, TYPE *values) {
        m_nCapacity = cap;
        m_nItems = count;
        m_Items  = values;
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::clear(void) {
        if (m_Items)
            delete [] m_Items;
        initialize(0, 0, NULL);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::duplicate(const SFArrayBase<TYPE>& cop) {
        checkSize(cop.capacity());
        for (size_t i = 0 ; i < cop.size() ; i++)
            m_Items[i] = cop.m_Items[i];
        m_nItems = cop.size();
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::reserve(size_t newSize) {
        checkSize(newSize);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::checkSize(size_t sizeNeeded) {
        if (sizeNeeded < m_nCapacity)
            return;

        // The user is requesting access to an index that is past range. We need to resize the array.
        size_t newSize = max(m_nCapacity + ARRAY_CHUNK_SIZE, sizeNeeded);
        TYPE *newArray = new TYPE[newSize];
        if (m_nItems) {
            // If there are any values in the source copy them over
            for (size_t i = 0 ; i < m_nItems ; i++)
                newArray[i] = m_Items[i];
            // Then clear out the old array
            if (m_Items)
                delete [] m_Items;
            m_Items = NULL;
        }
        initialize(newSize, m_nItems, newArray);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline TYPE& SFArrayBase<TYPE>::at(size_t index) {
        // TODO(tjayrush): This should definitly not grow the array. If we use this to grow the array,
        // when we switch to a native vector, this will break
        checkSize(index);
        if (index >= m_nItems)
            m_nItems = index+1;
        ASSERT(m_Items && index >= 0 && index <= m_nCapacity && index <= m_nItems);
        return m_Items[index];
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::push_back(TYPE x) {
        size_t index = size();
        checkSize(index);
        if (index >= m_nItems)
            m_nItems = index + 1;
        ASSERT(m_Items && index >= 0 && index <= m_nCapacity && index <= m_nItems);
        m_Items[index] = x;
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline const TYPE& SFArrayBase<TYPE>::operator[](size_t index) const {
        // This is the const version which means it's a get which means we should not be expecting
        // the array to grow. Does not appear to protect against accessing outside range though.
        ASSERT(index >= 0 && index <= m_nItems);
        return m_Items[index];
    }
}  // namespace qblocks

