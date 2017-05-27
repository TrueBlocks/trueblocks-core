/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "acctTree.h"

//-----------------------------------------------------------------------------
bool acctTree_Infix::contains(const SFString& _key) const {
    uint32_t l1 = _key.length();
    uint32_t l2 = m_prefix.length();
    const char *s1 = (const char*)_key;
    const char *s2 = (const char*)m_prefix;
    bool found = !memcmp(s1, s2, l2);

    return
    l1 >= l2 && found;
}

//-----------------------------------------------------------------------------
bool acctTree_Leaf::contains(const SFString& _key) const {
    uint32_t l1 = _key.length();
    uint32_t l2 = m_prefix.length();
    const char *s1 = (const char*)_key;
    const char *s2 = (const char*)m_prefix;
    bool found = !memcmp(s1, s2, l1);

    return l1 == l2 && found;
}
