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
#include "basetypes.h"
#include "runtimeclass.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    CRuntimeClass::CRuntimeClass(void) {
        m_FieldList = new CFieldList;
    }

    //-------------------------------------------------------------------------
    CFieldList *CRuntimeClass::GetFieldList(void) const {
        return m_FieldList;
    }

    //-------------------------------------------------------------------------
    CRuntimeClass::~CRuntimeClass(void) {
        if (m_FieldList) {
            LISTPOS p = m_FieldList->GetHeadPosition();
            while (p) {
                CFieldData *field = m_FieldList->GetNext(p);
                delete field;
            }
            m_FieldList->RemoveAll();
            delete m_FieldList;
            m_FieldList = NULL;
        }
    }

    //-------------------------------------------------------------------------
    char *CRuntimeClass::getClassNamePtr(void) const {
        return m_ClassName;
    }

    //-------------------------------------------------------------------------
    bool CRuntimeClass::IsDerivedFrom(const CRuntimeClass* pBaseClass) const {
        const CRuntimeClass* pClassThis = this;
        while (pClassThis != NULL) {
            if (pClassThis == pBaseClass)
                return true;
            pClassThis = pClassThis->m_BaseClass;
        }
        return false;
    }

    //-------------------------------------------------------------------------
    CBaseNode *CRuntimeClass::CreateObject(void) {
        if (m_CreateFunc)
            return (*m_CreateFunc)();
        return NULL;
    }

    //-------------------------------------------------------------------------
    CFieldData *CRuntimeClass::findField(const string_q& fieldName) {
        for (auto field : newList)
            if (field->getName() == fieldName)
                return field;
        return NULL;
    }

    //-------------------------------------------------------------------------
    bool CRuntimeClass::isFieldHidden(const string_q& fieldName) {
        const CFieldData *f = findField(fieldName);
        if (f)
            return f->isHidden();
        return false;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::AddField(const string_q& fieldName, size_t dataType, size_t fieldID) {
        CFieldData *field = new CFieldData(fieldName, fieldID, dataType);
        if (field) {
            m_FieldList->AddTail(field);
            newList.push_back(field);
        }
    }

    //-------------------------------------------------------------------------
    bool compareByName(const CFieldData *v1, const CFieldData *v2 ) {
        return v1->getName() < v2->getName();
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::sortFieldList(void) {
        sort(newList.begin(), newList.end(), compareByName);
        if (m_FieldList) {
            *m_FieldList = CFieldList();
            for (auto field : newList)
                m_FieldList->AddTail(field);
        }
        return;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::hideAllFields(void) {
        for (auto field : newList)
            field->setHidden(true);
        return;
    }

    //-------------------------------------------------------------------------
    void CRuntimeClass::showAllFields(void) {
        for (auto field : newList)
            field->setHidden(false);
        return;
    }

    //-------------------------------------------------------------------------
    bool CRuntimeClass::forEveryField(FIELDVISITFUNC func, void *data) {
        if (!func)
            return true;

        for (auto field : newList) {
            if (!(*func)(field, data))
                return false;
        }

        return true;
    }

    //-------------------------------------------------------------------------
    CBuiltIn::CBuiltIn(CRuntimeClass *pClass, const string_q& className, size_t size, PFNV createFunc, CRuntimeClass *pBase) {
        m_pClass = pClass;
        string_q copy = className;
        if (!copy.empty())
            pClass->m_ClassName = strdup(copy.c_str());
        pClass->m_ObjectSize = size;
        pClass->m_BaseClass  = pBase;
        pClass->m_FieldList  = NULL;
        pClass->m_CreateFunc = createFunc;
    }
}  // namespace qblocks
