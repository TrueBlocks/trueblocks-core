/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "runtimeclass.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    CRuntimeClass::~CRuntimeClass(void)
    {
        ClearFieldList();
        ASSERT(!m_FieldList);
    }

    //-------------------------------------------------------------------------
    char *CRuntimeClass::getClassNamePtr(void) const
    {
        return m_ClassName;
    }

    //-------------------------------------------------------------------------
    bool CRuntimeClass::IsDerivedFrom(const CRuntimeClass* pBaseClass) const
    {
        const CRuntimeClass* pClassThis = this;
        while (pClassThis != NULL)
        {
            if (pClassThis == pBaseClass)
                return true;
            pClassThis = pClassThis->m_BaseClass;
        }
        return false;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::Initialize(const SFString& protoName)
    {
        SFString copy = protoName;
        if (!copy.empty())
            m_ClassName = strdup(copy.c_str());

        m_ObjectSize    = 0;
        // Signifies that we were created with Initialize so we can cleanup (i.e. the class is not builtin)
        m_classSchema   = 0xFFFA;
        m_BaseClass     = NULL;
        m_FieldList     = NULL;
        m_CreateFunc    = NULL;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::ClearFieldList(void)
    {
        if (m_FieldList)
        {
            LISTPOS p = m_FieldList->GetHeadPosition();
            while (p)
            {
                CFieldData *field = m_FieldList->GetNext(p);
                delete field;
            }
            m_FieldList->RemoveAll();
            delete m_FieldList;
            m_FieldList = NULL;
        }
    }

    //-------------------------------------------------------------------------
    SFString CRuntimeClass::listOfFields(char sep) const
    {
        SFString ret;
        if (m_FieldList)
        {
            LISTPOS p = m_FieldList->GetHeadPosition();
            while (p)
            {
                CFieldData *field = m_FieldList->GetNext(p);
                ret += field->m_fieldName + sep;
            }
        }
        return ret;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::hideAllFields(void)
    {
        if (m_FieldList)
        {
            LISTPOS p = m_FieldList->GetHeadPosition();
            while (p)
            {
                CFieldData *field = m_FieldList->GetNext(p);
                field->setHidden(true);
            }
        }
        return;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::AddField(const SFString& fieldName, uint32_t dataType, uint32_t fieldID)
    {
        if (!m_FieldList)
            m_FieldList = new CFieldList;
        ASSERT(m_FieldList);

        CFieldData *field = new CFieldData;
        if (field)
        {
            field->m_fieldName = fieldName;
            field->m_type = dataType;
            field->m_fieldID = fieldID;
            m_FieldList->AddTail(field);
        }
    }

    //-------------------------------------------------------------------------
    CBuiltIn::CBuiltIn(CRuntimeClass *pClass, const SFString& className, uint32_t size, PFNV createFunc, CRuntimeClass *pBase, uint32_t schema)
    {
        m_pClass = pClass;
        SFString copy = className;
        if (!copy.empty())
            pClass->m_ClassName = strdup(copy.c_str());
        
        pClass->m_ObjectSize    = size;
        // Signifies that we were created with Initialize so we can cleanup (i.e. the class is not builtin)
        pClass->m_classSchema   = schema;
        pClass->m_BaseClass     = pBase;
        pClass->m_FieldList     = NULL;
        pClass->m_CreateFunc    = createFunc;
    }
    
}
