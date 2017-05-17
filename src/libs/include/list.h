#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "basetypes.h"

namespace qblocks {

    //----------------------------------------------------------------------
    #define ARRAY_CHUNK_SIZE 100

    //-------------------------------------------------------------------------
    typedef int  (*SEARCHFUNC)    (const void *ob1, const void *ob2);
    typedef int  (*SORTINGFUNC)   (const void *ob1, const void *ob2);
    typedef int  (*DUPLICATEFUNC) (const void *ob1, const void *ob2);
    typedef bool (*APPLYFUNC)     (SFString& line, void *data);

    //----------------------------------------------------------------------
    template<class TYPE>
    class SFArrayBase {
    protected:
        uint32_t m_nSize;
        uint32_t m_nValues;
        TYPE  *m_Values;

    public:
        SFArrayBase(void);
        SFArrayBase(const SFArrayBase& copy);
        ~SFArrayBase(void);

        SFArrayBase& operator=(const SFArrayBase& copy);

        TYPE& operator[](uint32_t index);
        const TYPE& operator[](uint32_t index) const;

        void addValue(TYPE val) { operator[](m_nValues) = val; }
        uint32_t getCount(void) const { return m_nValues; }
        uint32_t getSize(void) const { return m_nSize; }

        void Sort(SORTINGFUNC func) { qsort(&m_Values[0], m_nValues, sizeof(TYPE), func); }
        TYPE *Find(TYPE *key, SEARCHFUNC func) {
            return reinterpret_cast<TYPE*>(bsearch(key, &m_Values[0], m_nValues, sizeof(TYPE), func));
        }

        void Clear(void);
        void Grow(uint32_t sizeNeeded);

    private:
        void CheckSize(uint32_t sizeNeeded);
        void Copy(const SFArrayBase& copy);
        void Init(uint32_t size, uint32_t count, TYPE *values);
    };

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>::SFArrayBase(void) {
        Init(0, 0, NULL);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>::SFArrayBase(const SFArrayBase<TYPE>& copy) {
        Copy(copy);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>::~SFArrayBase(void) {
        Clear();
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline SFArrayBase<TYPE>& SFArrayBase<TYPE>::operator=(const SFArrayBase<TYPE>& copy) {
        Clear();
        Copy(copy);
        return *this;
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::Init(uint32_t size, uint32_t count, TYPE *values) {
        m_nSize   = size;
        m_nValues = count;
        m_Values  = values;
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::Clear(void) {
        if (m_Values)
            delete [] m_Values;
        Init(0, 0, NULL);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::Copy(const SFArrayBase<TYPE>& copy) {
        CheckSize(copy.getSize());
        for (uint32_t i = 0 ; i < copy.getCount() ; i++)
            m_Values[i] = copy.m_Values[i];
        m_nValues = copy.getCount();
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::Grow(uint32_t sizeNeeded) {
        CheckSize(sizeNeeded);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline void SFArrayBase<TYPE>::CheckSize(uint32_t sizeNeeded) {
        if (sizeNeeded < m_nSize)
            return;

        // The user is requesting access to an index that is past range. We need to grow the array.
        uint32_t newSize = MAX(m_nSize + ARRAY_CHUNK_SIZE, sizeNeeded);
        TYPE *newArray = new TYPE[newSize];
        if (m_nValues) {
            ASSERT(m_Values);
            // If there are any values in the source copy them over
            for (uint32_t i = 0 ; i < m_nValues ; i++)
                newArray[i] = m_Values[i];
            // Then clear out the old array
            if (m_Values)
                delete [] m_Values;
            m_Values = NULL;
        }
        Init(newSize, m_nValues, newArray);
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline TYPE& SFArrayBase<TYPE>::operator[](uint32_t index) {
        // This is the non-const version which means we may
        // have to grow the array
        CheckSize(index);
        if (index >= m_nValues)
            m_nValues = index+1;
        ASSERT(m_Values && index >= 0 && index <= m_nSize && index <= m_nValues);
        return m_Values[index];
    }

    //----------------------------------------------------------------------
    template<class TYPE>
    inline const TYPE& SFArrayBase<TYPE>::operator[](uint32_t index) const {
        // This is the const version which means it's a get which means we should not be expecting
        // the array to grow. Does not appear to protect against accessing outside range though.
        ASSERT(index >= 0 && index <= m_nValues);
        return m_Values[index];
    }

    struct xLISTPOS__ { int unused; };
    typedef xLISTPOS__* LISTPOS;

    //----------------------------------------------------------------------
    template<class TYPE>
    class SFListNode {
    private:
        // Don't allow default construction, copy construction or assignment
        SFListNode(void) {}
        SFListNode(const SFListNode& node) {}
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
        uint32_t m_Count;
        SFListNode<TYPE> *m_Head;
        SFListNode<TYPE> *m_Tail;

    public:
        SFList(void);
        SFList(const SFList& l);
        ~SFList(void);

        SFList& operator=(const SFList& l);

        uint32_t GetCount(void) const { return m_Count; }
        TYPE GetHead(void) const { return (TYPE)(m_Head->m_Data); }
        TYPE GetTail(void) const { return (TYPE)(m_Tail->m_Data); }

        LISTPOS GetHeadPosition (void) const { return (LISTPOS)m_Head; }
        LISTPOS GetTailPosition (void) const { return (LISTPOS)m_Tail; }

        void setHead(SFListNode<TYPE> *newHead) { m_Head = newHead; }
        void setTail(SFListNode<TYPE> *newTail) { m_Tail = newTail; }

        void AddToList(TYPE item) { AddTail(item); }
        bool empty(void) const { return (m_Head == NULL); }

        TYPE GetNext(LISTPOS& rPosition) const;
        TYPE GetPrev(LISTPOS& rPosition) const;
        LISTPOS Find(TYPE item) const;
        TYPE FindAt(TYPE item) const;
        TYPE FindAt(LISTPOS pos) const;

        void AddHead(TYPE item);
        void AddTail(TYPE item);
        void AddToList(const SFList& l);
        bool AddSorted(TYPE item, SORTINGFUNC sortFunc, DUPLICATEFUNC dupFunc = NULL);

        void InsertBefore(LISTPOS pos, TYPE item);
        void InsertAfter(LISTPOS pos, TYPE item);

        TYPE RemoveAt(LISTPOS pos);
        TYPE RemoveHead(void);
        TYPE RemoveTail(void);
        void RemoveAll(void);
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
    TYPE SFList<TYPE>::FindAt(TYPE probe) const {
        LISTPOS pos = GetHeadPosition();
        while (pos) {
            TYPE ob = GetNext(pos);
            if (ob == probe)
                return ob;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    TYPE SFList<TYPE>::FindAt(LISTPOS probe) const {
        LISTPOS pos = GetHeadPosition();
        while (pos) {
            LISTPOS prev = pos;
            TYPE ob = GetNext(pos);
            if (prev == probe)
                return ob;
        }
        return NULL;
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
    inline void SFList<TYPE>::AddHead(TYPE data) {
        SFListNode<TYPE> *node = new SFListNode<TYPE>(data);

        ASSERT(node);
        ASSERT(!m_Head || m_Head->m_Prev == NULL);
        ASSERT(!m_Tail || m_Tail->m_Next == NULL);

        node->m_Next = m_Head;
        node->m_Prev = NULL;

        if (!m_Head) {
            ASSERT(!m_Tail);
            m_Head = m_Tail = node;
        } else {
            ASSERT(m_Tail);
            m_Head->m_Prev = node;
            m_Head = node;
        }
        m_Count++;
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
    inline void SFList<TYPE>::InsertBefore(LISTPOS pos, TYPE data) {
        SFListNode<TYPE> *node = new SFListNode<TYPE>(data);
        SFListNode<TYPE> *before = (SFListNode<TYPE> *)pos;

        ASSERT(node && before);

        node->m_Prev = before->m_Prev;
        node->m_Next = before;

        if (before->m_Prev)
            before->m_Prev->m_Next = node;
        before->m_Prev = node;

        ASSERT(m_Head && m_Tail);  // We would have used AddTail otherwise
        if (before == m_Head)
            m_Head = node;

        m_Count++;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    inline void SFList<TYPE>::InsertAfter(LISTPOS pos, TYPE data) {
        SFListNode<TYPE> *node  = new SFListNode<TYPE>(data);
        SFListNode<TYPE> *after = (SFListNode<TYPE> *)pos;

        ASSERT(node && after);

        node->m_Prev = after;
        node->m_Next = after->m_Next;

        if (after->m_Next)
            after->m_Next->m_Prev = node;
        after->m_Next = node;

        ASSERT(m_Head && m_Tail);  // We would have used AddTail otherwise
        if (after == m_Tail)
            m_Tail = node;

        m_Count++;
    }

    //---------------------------------------------------------------------
    template<class TYPE>
    void SFList<TYPE>::AddToList(const SFList<TYPE>& l) {
        LISTPOS pos = l.GetHeadPosition();
        while (pos) {
            TYPE ob = l.GetNext(pos);
            AddToList(ob);
        }
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
    inline TYPE SFList<TYPE>::GetPrev(LISTPOS& pos) const {
        SFListNode<TYPE> *node = (SFListNode<TYPE> *)pos;
        ASSERT(node);
        pos = (LISTPOS)((node->m_Prev != m_Tail) ? node->m_Prev : NULL);

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

    //---------------------------------------------------------------------
    template<class TYPE>
    inline TYPE SFList<TYPE>::RemoveAt(LISTPOS pos) {
        SFListNode<TYPE> *node = (SFListNode<TYPE> *)pos;

        ASSERT(node);
        ASSERT(!m_Head || m_Head->m_Prev == NULL);
        ASSERT(!m_Tail || m_Tail->m_Next == NULL);

        TYPE data = (TYPE)(node->m_Data);

        if (!m_Head) {
            ASSERT(!m_Tail);
            delete node;
            return data;
        }
        ASSERT(m_Tail);

        if (m_Head == node)
            m_Head = m_Head->m_Next;

        if (m_Tail == node)
            m_Tail = m_Tail->m_Prev;

        if (node->m_Prev)
            node->m_Prev->m_Next = node->m_Next;

        if (node->m_Next)
            node->m_Next->m_Prev = node->m_Prev;

        m_Count--;

        delete node;
        return data;
    }

    //---------------------------------------------------------------------
    // stack use
    template<class TYPE>
    inline TYPE SFList<TYPE>::RemoveHead(void) {
        return RemoveAt((LISTPOS)m_Head);
    }

    //---------------------------------------------------------------------
    // queue use
    template<class TYPE>
    inline TYPE SFList<TYPE>::RemoveTail(void) {
        return RemoveAt((LISTPOS)m_Tail);
    }

    //-----------------------------------------------------------------------------
    // return true of added, false otherwise so caller can free allocated memory if any
    template<class TYPE>
    inline bool SFList<TYPE>::AddSorted(TYPE item, SORTINGFUNC sortFunc, DUPLICATEFUNC dupFunc) {
        if (!item)
            return false;

        if (sortFunc) {
            // Sort it in (if told to)...
            LISTPOS ePos = GetHeadPosition();
            while (ePos) {
                LISTPOS lastPos = ePos;
                TYPE test = GetNext(ePos);
                
                bool isDup = dupFunc && (dupFunc)(item, test);
                if (isDup) {
                    // caller must free this memory or drop it
                    return false;
                }
                
                if ((sortFunc)(item, test) < 0) {
                    InsertBefore(lastPos, item);
                    return true;
                }
            }
        }
        
        // ...else just add it to the end
        AddToList(item);
        return true;
    }
    
    //---------------------------------------------------------------------------------
    template<class TYPE>
    class SFStack : public SFList<TYPE> {
    public:
        SFStack(void) {}
        ~SFStack(void) { while (Peek()) delete Pop(); }
        
        void Push(TYPE val) { SFList<TYPE>::AddHead(val); }
        TYPE Peek(void) { return SFList<TYPE>::empty() ? NULL : SFList<TYPE>::GetHead(); }
        TYPE Pop(void) { return SFList<TYPE>::RemoveHead(); }
    };
    
    //---------------------------------------------------------------------------------
    template<class TYPE>
    class SFUniqueList : public SFList<TYPE> {
        SORTINGFUNC   sortFunc;
        DUPLICATEFUNC compFunc;
    public:
        SFUniqueList(SORTINGFUNC sFunc, DUPLICATEFUNC cFunc) {
            sortFunc = sFunc;
            compFunc = cFunc;
        }
        bool AddItemUnique(TYPE item) {
            return SFList<TYPE>::AddSorted(item, sortFunc, compFunc);
        }
    };
    
    //-----------------------------------------------------------------------------------------
    inline int sortByStringValue(const void *rr1, const void *rr2) {
        SFString n1 = * reinterpret_cast<const SFString*>(rr1);
        SFString n2 = * reinterpret_cast<const SFString*>(rr2);
        return n1.Icompare(n2);
    }
    
    //-----------------------------------------------------------------------------------------
    inline int compareStringValue(const void *rr1, const void *rr2) {
        // return true of these are the same string
        return !sortByStringValue(rr1, rr2);
    }
    
    //----------------------------------------------------------------------------------------
    class SFUniqueStringList : public SFUniqueList<const SFString&> {
    public:
        SFUniqueStringList(void) : SFUniqueList(sortByStringValue, compareStringValue) { }
    };
}
