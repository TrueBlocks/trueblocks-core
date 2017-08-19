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
    #define TS_NUMERAL (1<<16)
    #define TS_STRING  (1<<17)
    #define TS_DATE    (1<<18)
    #define TS_ARRAY   (1<<19)
    #define TS_OBJECT  (1<<20)
    #define TS_POINTER (1<<21)

    #define T_DATE     ( 6 | TS_DATE   )
    #define T_TIME     ( 8 | TS_DATE   )
    #define T_BOOL     (10 | TS_NUMERAL)
    #define T_NUMBER   (12 | TS_NUMERAL)
    #define T_FLOAT    (14 | TS_NUMERAL)
    #define T_DOUBLE   (16 | TS_NUMERAL)
    #define T_WEI      (18 | TS_NUMERAL)
    #define T_GAS      (20 | TS_NUMERAL)
    #define T_ETHER    (22 | TS_NUMERAL)
    #define T_TEXT     (24 | TS_STRING )
    #define T_ADDRESS  (26 | TS_STRING )
    #define T_HASH     (28 | TS_STRING )
    #define T_BLOOM    (30 | TS_STRING )
    #define T_POINTER  (32 | TS_POINTER)
    #define T_OBJECT   (34 | TS_OBJECT )

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
