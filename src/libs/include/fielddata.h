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
    #define TS_LABEL   (1<<10)
    #define TS_DATE    (1<<12)
    #define TS_ARRAY   (1<<14)
    #define TS_NUMERAL (1<<15)
    #define TS_OBJECT  (1<<16)

    #define T_DATE     (4 | TS_DATE)
    #define T_TIME     (5 | TS_DATE)

    #define T_TEXT     (10)
    #define T_BOOL     (12)
    #define T_NUMBER   (14 | TS_NUMERAL)
    #define T_BLOOM    (16)
    #define T_POINTER  (18)
    #define T_FLOAT    T_NUMBER
    #define T_DOUBLE   T_NUMBER

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
    };

    //-------------------------------------------------------------------------
    class CFieldList : public SFList<CFieldData*> {
    public:
        CFieldList(void) : SFList<CFieldData*>() { }
        const CFieldData *getFieldByID(uint32_t id) const;
        const CFieldData *getFieldByName(const SFString& name) const;
    };
}  // namespace qblocks
