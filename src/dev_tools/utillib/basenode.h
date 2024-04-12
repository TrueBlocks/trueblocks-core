#pragma once
/*
 */
#include <algorithm>
#include "runtimeclass.h"

namespace qblocks {

class CBaseNode {
  public:
    CBaseNode(void);
    virtual ~CBaseNode(void);
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

inline CBaseNode::CBaseNode(void) {
}

inline CBaseNode::~CBaseNode(void) {
}

}  // namespace qblocks
