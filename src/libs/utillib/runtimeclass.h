#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "fielddata.h"

namespace qblocks {

    class CBaseNode;
    typedef CBaseNode* (*PFNV)(void);

    //----------------------------------------------------------------------------
    class CRuntimeClass;
    class CBuiltIn {
    public:
        const CRuntimeClass *m_pClass;
        CBuiltIn(CRuntimeClass *pClass, const SFString& className, uint32_t size, PFNV createFunc, CRuntimeClass *pBase);
    };

    //----------------------------------------------------------------------------
    class CRuntimeClass {
    public:
        char *m_ClassName;
        uint32_t m_ObjectSize;
        PFNV m_CreateFunc;
        CRuntimeClass *m_BaseClass;
        CFieldList *m_FieldList;

    public:
        virtual ~CRuntimeClass(void);

        char *getClassNamePtr(void) const;
        bool IsDerivedFrom(const CRuntimeClass* pBaseClass) const;
        void AddField(const SFString& fieldName, uint32_t dataType, uint32_t fieldID);
        void ClearFieldList(void);
        SFString listOfFields(char sep = '|') const;
        CFieldList *GetFieldList(void) const { return m_FieldList; }
        void Initialize(const SFString& protoName);
        void hideAllFields(void);
        void showAllFields(void);
        void sortFieldList(void);

        CBaseNode *CreateObject(void) {
            if (m_CreateFunc)
                return (*m_CreateFunc)();
            return NULL;
        }

        virtual CFieldData *FindField(const SFString& fieldName) {
            if (m_FieldList) {
                LISTPOS pos = m_FieldList->GetHeadPosition();
                while (pos) {
                    CFieldData *field = m_FieldList->GetNext(pos);
                    if (field->m_fieldName == fieldName)
                        return field;
                }
            }
            return NULL;
        }
        bool isFieldHidden(const SFString& fieldName) {
            CFieldData *f = FindField(fieldName);
            if (f)
                return f->isHidden();
            return false;
        }
    };

    //------------------------------------------------------------
#define GETRUNTIME_CLASS(CLASS_NAME) \
(&CLASS_NAME::class##CLASS_NAME)

    //------------------------------------------------------------
#define DECLARE_NODE(CLASS_NAME) \
public: \
static CRuntimeClass class##CLASS_NAME; \
static CBaseNode *CreateObject(void); \
CRuntimeClass *getRuntimeClass(void) const override; \
SFString getValueByName(const SFString& fieldName) const override; \
bool setValueByName(const SFString& fieldName, const SFString& fieldValue) override; \
bool Serialize(SFArchive& archive) override; \
bool SerializeC(SFArchive& archive) const override; \
void finishParse(void) override; \
bool handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data = NULL) const override; \
void Format(CExportContext& ctx, const SFString& fmtIn, void *data = NULL) const override; \
SFString Format(const SFString& fmtIn = "") const override { \
    CStringExportContext ctx; Format(ctx, fmtIn, NULL); return ctx.str; } \
SFString getClassName(void) const; \
static void registerClass(void)

    //------------------------------------------------------------
#define IMPLEMENT_NODE(CLASS_NAME, BASECLASS_NAME) \
CRuntimeClass CLASS_NAME::class##CLASS_NAME; \
CRuntimeClass *CLASS_NAME::getRuntimeClass(void) const { return &CLASS_NAME::class##CLASS_NAME; } \
SFString CLASS_NAME::getClassName(void) const { return CLASS_NAME::class##CLASS_NAME.getClassNamePtr(); } \
CBaseNode* CLASS_NAME::CreateObject(void) { return new CLASS_NAME; } \
static CBuiltIn _bi##CLASS_NAME(&CLASS_NAME::class##CLASS_NAME, #CLASS_NAME, sizeof(CLASS_NAME), \
CLASS_NAME::CreateObject, GETRUNTIME_CLASS(BASECLASS_NAME));

    //------------------------------------------------------------
#define ADD_FIELD(CLASS_NAME, FIELD_NAME, FIELD_TYPE, FIELD_ID) \
GETRUNTIME_CLASS(CLASS_NAME)->AddField(FIELD_NAME, FIELD_TYPE, FIELD_ID);

    //------------------------------------------------------------
#define HIDE_FIELD(CLASS_NAME, FIELD_NAME) {\
CFieldData *f = GETRUNTIME_CLASS(CLASS_NAME)->FindField(FIELD_NAME); if (f) { f->setHidden(true); }\
}

    //------------------------------------------------------------
#define SUBFIELD_FMT(a, sf, b) SFString("[\"") + SFString(sf) + SFString("\": \"{") + \
toUpper(SFString(a)) + "::" + toUpper(SFString(sf)) + "}\"" + (b ? ", ]" : "]")

    //------------------------------------------------------------
#define UNHIDE_FIELD(CLASS_NAME, FIELD_NAME) {\
CFieldData *f = GETRUNTIME_CLASS(CLASS_NAME)->FindField(FIELD_NAME); if (f) { f->setHidden(false); }\
}

    //------------------------------------------------------------
#define RENAME_FIELD(CLASS_NAME, OLD_NAME, NEW_NAME) {\
CFieldData *f = GETRUNTIME_CLASS(CLASS_NAME)->FindField(OLD_NAME); if (f) { f->setName(NEW_NAME); }\
}

    //------------------------------------------------------------
#define HIDE_ALL_FIELDS(CLASS_NAME) \
GETRUNTIME_CLASS(CLASS_NAME)->hideAllFields();

    //------------------------------------------------------------
#define SHOW_ALL_FIELDS(CLASS_NAME) \
GETRUNTIME_CLASS(CLASS_NAME)->showAllFields();

    //------------------------------------------------------------
    // Archive array containers
#define IMPLEMENT_ARCHIVE_ARRAY(ARRAY_CLASS) \
inline SFArchive& operator>>(SFArchive& archive, ARRAY_CLASS& array) \
{ \
uint64_t count; \
archive >> count; \
array.Grow((uint32_t)(count+1)); \
for (uint32_t i = 0 ; i < count ; i++) \
{ \
array[i].Serialize(archive); \
} \
return archive; \
}

    //------------------------------------------------------------
#define IMPLEMENT_ARCHIVE_ARRAY_C(ARRAY_CLASS) \
inline SFArchive& operator<<(SFArchive& archive, const ARRAY_CLASS& array) \
{ \
uint64_t count = array.getCount(); \
archive << count; \
for (uint32_t i = 0 ; i < array.getCount() ; i++) \
array[i].SerializeC(archive); \
return archive; \
}

    //------------------------------------------------------------
    // Archive list containers
#define IMPLEMENT_ARCHIVE_LIST(LIST_CLASS) \
inline SFArchive& operator<<(SFArchive& archive, LIST_CLASS& array) \
{ \
return archive; \
} \
inline SFArchive& operator>>(SFArchive& archive, LIST_CLASS& array) \
{ \
return archive; \
}

    //---------------------------------------------------------------------------
    extern SFString nextBasenodeChunk(const SFString& fieldIn, const CBaseNode *node);
}  // namespace qblocks
