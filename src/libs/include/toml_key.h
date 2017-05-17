#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "list.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    class CTomlKey {
    public:
        SFString keyName;
        SFString value;
        bool     comment;

        CTomlKey(void);
        CTomlKey(const CTomlKey& key);
        CTomlKey& operator=(const CTomlKey& key);
    };
    typedef SFList<CTomlKey*> CTomlKeyList;

    //-------------------------------------------------------------------------
    inline CTomlKey::CTomlKey() : comment(false) {
    }

    //-------------------------------------------------------------------------
    inline CTomlKey::CTomlKey(const CTomlKey& key) : keyName(key.keyName), value(key.value), comment(key.comment) {
    }

    //-------------------------------------------------------------------------
    inline CTomlKey& CTomlKey::operator=(const CTomlKey& key) {
        keyName = key.keyName;
        value = key.value;
        comment = key.comment;
        return *this;
    }
}
