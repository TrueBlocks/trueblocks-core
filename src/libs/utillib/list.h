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
        // TODO: This should definitly not grow the array. If we use this to grow the array, 
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

    struct xLISTPOS__ { int unused; };
    typedef xLISTPOS__* LISTPOS;

    //----------------------------------------------------------------------
    template<class TYPE>
    class SFListNode {
    private:
        // Don't allow default construction, copy construction or assignment
        SFListNode(void) { }
        SFListNode(const SFListNode& node) { }
        SFListNode& operator=(const SFListNode& node) { return *this; }

    public:
        SFListNode *m_Next;
        SFListNode *m_Prev;
        TYPE m_Data;

        explicit SFListNode(TYPE d) { m_Next = NULL; m_Prev = NULL;  m_Data = d; }
        ~SFListNode(void) { m_Next = NULL; m_Prev = NULL;  }

        SFListNode *Next(void) const { return m_Next; }
        SFListNode *Prev(void) const { return m_Prev; }
    };

    //----------------------------------------------------------------------
    template<class TYPE>
    class SFList {
    protected:
        size_t m_Count;
        SFListNode<TYPE> *m_Head;
        SFListNode<TYPE> *m_Tail;

    public:
        SFList(void);
        SFList(const SFList& l);
        ~SFList(void);

        SFList& operator=(const SFList& l);

        size_t size(void) const { return m_Count; }
        bool empty(void) const { return (m_Head == NULL); }

        LISTPOS GetHeadPosition (void) const { return (LISTPOS)m_Head; }
        TYPE    GetNext         (LISTPOS& rPosition) const;
        LISTPOS Find            (TYPE item) const;
        void    AddTail         (TYPE item);
        void    RemoveAll       (void);
    };

    //---------------------------------------------------------------------
    template<class TYPE>
    inline SFList<TYPE>::SFList(void) {
        m_Head  = NULL;
        m_Tail  = NULL;
        m_Count = 0;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    SFList<TYPE>::SFList(const SFList<TYPE>& l) {
        m_Head  = NULL;
        m_Tail  = NULL;
        m_Count = 0;

        LISTPOS pos = l.GetHeadPosition();
        while (pos) {
            TYPE ob = l.GetNext(pos);
            AddTail(ob);
        }
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    inline SFList<TYPE>::~SFList(void) {
        RemoveAll();
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    SFList<TYPE>& SFList<TYPE>::operator=(const SFList<TYPE>& l) {
        RemoveAll();

        LISTPOS pos = l.GetHeadPosition();
        while (pos) {
            TYPE ob = l.GetNext(pos);
            AddTail(ob);
        }
        return *this;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    LISTPOS SFList<TYPE>::Find(TYPE probe) const {
        LISTPOS pos = GetHeadPosition();
        while (pos) {
            LISTPOS last = pos;
            TYPE ob = GetNext(pos);
            if (ob == probe)
                return last;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    inline void SFList<TYPE>::AddTail(TYPE data) {
        SFListNode<TYPE> *node = new SFListNode<TYPE>(data);

        ASSERT(node);
        ASSERT(!m_Head || m_Head->m_Prev == NULL);
        ASSERT(!m_Tail || m_Tail->m_Next == NULL);

        node->m_Next = NULL;
        node->m_Prev = m_Tail;

        if (!m_Head) {
            ASSERT(!m_Tail);
            m_Head = m_Tail = node;
        } else {
            ASSERT(m_Tail);
            m_Tail->m_Next = node;
            m_Tail = node;
        }
        m_Count++;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    inline TYPE SFList<TYPE>::GetNext(LISTPOS& pos) const {
        SFListNode<TYPE> *node = (SFListNode<TYPE> *)pos;
        ASSERT(node);
        pos = (LISTPOS)((node->m_Next != m_Head) ? node->m_Next : NULL);

        return (TYPE)(node->m_Data);
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    inline void SFList<TYPE>::RemoveAll(void) {
        SFListNode<TYPE> *node = m_Head;
        while (node) {
            SFListNode<TYPE> *n = ((node->m_Next != m_Head) ? node->m_Next : NULL);
            if (node == m_Head)
                m_Head = NULL;
            delete node;
            node = n;
        }

        m_Head      = NULL;
        m_Tail      = NULL;
        m_Count     = 0;
    }

    //-------------------------------------------------------------------------
    class CFieldData;
    class CFieldList : public SFList<CFieldData*> {
    public:
        CFieldList(void) : SFList<CFieldData*>() { }
    };
}  // namespace qblocks

