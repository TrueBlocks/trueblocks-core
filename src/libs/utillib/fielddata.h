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
    #define TS_NUMERAL  (1<<16)
    #define TS_STRING   (1<<17)
    #define TS_DATE     (1<<18)
    #define TS_ARRAY    (1<<19)
    #define TS_OBJECT   (1<<20)
    #define TS_POINTER  (1<<21)
    #define TS_BIGNUM   (1<<22)

    #define T_DATE      ( 5 | TS_DATE   )
    #define T_TIME      (10 | TS_DATE   )
    #define T_TIMESTAMP (15 | TS_NUMERAL)
    #define T_BOOL      (20 | TS_NUMERAL)
    #define T_NUMBER    (25 | TS_NUMERAL)
    #define T_DOUBLE    (30 | TS_NUMERAL)
    #define T_WEI       (35 | TS_NUMERAL | TS_BIGNUM)
    #define T_GAS       (40 | TS_NUMERAL | TS_BIGNUM)
    #define T_ETHER     (45 | TS_NUMERAL | TS_BIGNUM)
    #define T_TEXT      (50 | TS_STRING )
    #define T_ADDRESS   (55 | TS_STRING )
    #define T_HASH      (60 | TS_STRING )
    #define T_BLOOM     (65 | TS_STRING )
    #define T_POINTER   (70 | TS_POINTER)
    #define T_OBJECT    (75 | TS_OBJECT )

    //-------------------------------------------------------------------------
    class CFieldData {
    private:
        SFString m_fieldName;
        uint32_t m_fieldID;
        uint32_t m_fieldType;
        bool     m_hidden;

    public:
        CFieldData(void) : m_fieldID(0), m_fieldType(0), m_hidden(false) { }

        bool isHidden(void) const { return m_hidden; }
        void setHidden(bool hide) { m_hidden = hide; }

        bool isObject(void) const { return m_fieldType & TS_OBJECT; }
        bool isArray (void) const { return m_fieldType & TS_ARRAY;  }

        SFString getName(void) const { return m_fieldName; }
        uint32_t getID  (void) const { return m_fieldID;   }
        uint32_t getType(void) const { return m_fieldType; }
        void     setName(const SFString& str) { m_fieldName = str; }

        bool operator==(const CFieldData& data) {
            if (m_fieldName != data.m_fieldName) return false;
            if (m_fieldID != data.m_fieldID) return false;
            if (m_fieldType != data.m_fieldType) return false;
            if (m_hidden != data.m_hidden) return false;
            return true;
        }
        bool operator!=(const CFieldData& data) { return !operator==(data); }

        friend class CRuntimeClass;
        friend class CFieldList;
        friend class CBaseNode;
        friend int sortFieldsByName(const void *v1, const void *v2);
    };

    //-------------------------------------------------------------------------
    class CFieldList : public SFList<CFieldData*> {
    public:
        CFieldList(void) : SFList<CFieldData*>() { }
        const CFieldData *getFieldByID(uint32_t id) const;
        const CFieldData *getFieldByName(const SFString& name) const;
    };
}  // namespace qblocks
