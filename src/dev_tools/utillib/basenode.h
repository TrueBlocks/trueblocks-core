#pragma once
/*

*/
#include <algorithm>
#include "runtimeclass.h"

namespace qblocks {

class CBaseNode {
  public:
    uint64_t m_deleted;
    uint64_t m_schema;
    uint64_t m_showing;

  public:
    CBaseNode(void);
    virtual ~CBaseNode(void);
    bool isDeleted(void) const;
    void setDeleted(bool val);

  public:
    static CRuntimeClass classCBaseNode;
    virtual CRuntimeClass* getRuntimeClass(void) const;
    virtual const CBaseNode* getObjectAt(const string_q& name, size_t i) const {
        return NULL;
    }
    virtual const string_q getStringAt(const string_q& fieldName, size_t i) const {
        return "";
    }

  protected:
    void initialize(void);
    void duplicate(const CBaseNode& bn);
};

template <class T>
T RandomValue(T a, T b) {
    T range = (a > b ? a - b : b - a);
    if (range == 0)
        return a;
    return min(a, b) + (((T)rand()) % range);
}

template <class T>
inline bool inRange(T val, T mn, T mx) {
    return (val >= mn && val <= mx);
}

}  // namespace qblocks
