#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
    const CRuntimeClass* m_pClass;
    CBuiltIn(CRuntimeClass* pClass, const string_q& className, size_t size, PFNV createFunc, CRuntimeClass* pBase);
    friend bool operator<(const CBuiltIn& v1, const CBuiltIn& v2);
    bool operator==(const CBuiltIn& item) const;
    bool operator!=(const CBuiltIn& item) const {
        return !operator==(item);
    }
};
CBaseNode* createObjectOfType(const string_q& className);
extern vector<CBuiltIn> builtIns;

//----------------------------------------------------------------------------
typedef bool (*FIELDVISITFUNC)(const CFieldData& fld, void* data);

//----------------------------------------------------------------------------
class CRuntimeClass {
  public:
    char* m_ClassName;
    size_t m_ObjectSize;
    PFNV m_CreateFunc;
    CRuntimeClass* m_BaseClass;
    CFieldDataArray fieldList;

  public:
    CRuntimeClass(void);
    virtual ~CRuntimeClass(void);

    char* getClassNamePtr(void) const;
    bool isDerivedFrom(const CRuntimeClass* pBaseClass) const;
    void initialize(const string_q& protoName);
    CBaseNode* createObject(void);

    void addField(const string_q& fieldName, size_t dataType, size_t fieldID);
    void hideAllFields(void);
    void showAllFields(void);
    void sortFieldList(void);
    bool forEveryField(FIELDVISITFUNC func, void* data);

    CFieldData* findField(const string_q& fieldName);
    bool isFieldHidden(const string_q& fieldName);
};

//---------------------------------------------------------------------------
extern string_q nextBasenodeChunk(const string_q& fieldIn, const CBaseNode* node);

//------------------------------------------------------------
#define GETRUNTIME_CLASS(CLASS_NAME) (&CLASS_NAME::class##CLASS_NAME)

//------------------------------------------------------------
// TODO(tjayrush): global data
#define DECLARE_NODE(CLASS_NAME)                                                                                       \
  public:                                                                                                              \
    static CRuntimeClass class##CLASS_NAME;                                                                            \
    static CBaseNode* createObject(void);                                                                              \
    static void registerClass(void);                                                                                   \
    CRuntimeClass* getRuntimeClass(void) const override;                                                               \
    string_q getValueByName(const string_q& fieldName) const override;                                                 \
    bool setValueByName(const string_q& fieldName, const string_q& fieldValue) override;                               \
    void finishParse(void) override;                                                                                   \
    bool Serialize(CArchive& archive) override;                                                                        \
    bool SerializeC(CArchive& archive) const override;                                                                 \
    void Format(ostream& ctx, const string_q& fmtIn, void* data = NULL) const override;                                \
    string_q Format(const string_q& fmtIn = "") const override {                                                       \
        stringstream ctx;                                                                                              \
        Format(ctx, fmtIn, NULL);                                                                                      \
        return ctx.str();                                                                                              \
    }                                                                                                                  \
    string_q getClassName(void) const;

//------------------------------------------------------------
// TODO(tjayrush): global data
#define IMPLEMENT_NODE(CLASS_NAME, BASECLASS_NAME)                                                                     \
    static CBuiltIn _bi##CLASS_NAME(&CLASS_NAME::class##CLASS_NAME, #CLASS_NAME, sizeof(CLASS_NAME),                   \
                                    CLASS_NAME::createObject, GETRUNTIME_CLASS(BASECLASS_NAME));                       \
    CRuntimeClass CLASS_NAME::class##CLASS_NAME;                                                                       \
    CRuntimeClass* CLASS_NAME::getRuntimeClass(void) const {                                                           \
        return &CLASS_NAME::class##CLASS_NAME;                                                                         \
    }                                                                                                                  \
    string_q CLASS_NAME::getClassName(void) const {                                                                    \
        return CLASS_NAME::class##CLASS_NAME.getClassNamePtr();                                                        \
    }                                                                                                                  \
    CBaseNode* CLASS_NAME::createObject(void) {                                                                        \
        return new CLASS_NAME;                                                                                         \
    }

//------------------------------------------------------------
#define ADD_FIELD(CLASS_NAME, FIELD_NAME, FIELD_TYPE, FIELD_ID)                                                        \
    GETRUNTIME_CLASS(CLASS_NAME)->addField(FIELD_NAME, FIELD_TYPE, FIELD_ID);

//------------------------------------------------------------
#define SUBFIELD_FMT(a, sf, b)                                                                                         \
    string_q("[\"") + string_q(sf) + string_q("\": \"{") + toUpper(string_q(a)) + "::" + toUpper(string_q(sf)) +       \
        "}\"" + (b ? ", ]" : "]")

//------------------------------------------------------------
#define HAS_FIELD(CLASS_NAME, FIELD_NAME) (GETRUNTIME_CLASS(CLASS_NAME)->findField(FIELD_NAME) != NULL)

//------------------------------------------------------------
#define HIDE_FIELD(CLASS_NAME, FIELD_NAME)                                                                             \
    {                                                                                                                  \
        CFieldData* f = GETRUNTIME_CLASS(CLASS_NAME)->findField(FIELD_NAME);                                           \
        if (f)                                                                                                         \
            f->setHidden(true);                                                                                        \
    }

//------------------------------------------------------------
#define HIDE_PARENT_FIELD(CLASS_NAME, FIELD_NAME)                                                                      \
    {                                                                                                                  \
        CRuntimeClass* p = GETRUNTIME_CLASS(CLASS_NAME)->m_BaseClass;                                                  \
        if (p) {                                                                                                       \
            CFieldData* f = p->findField(FIELD_NAME);                                                                  \
            if (f)                                                                                                     \
                f->setHidden(true);                                                                                    \
        }                                                                                                              \
    }

//------------------------------------------------------------
#define UNHIDE_FIELD(CLASS_NAME, FIELD_NAME)                                                                           \
    {                                                                                                                  \
        CFieldData* f = GETRUNTIME_CLASS(CLASS_NAME)->findField(FIELD_NAME);                                           \
        if (f)                                                                                                         \
            f->setHidden(false);                                                                                       \
    }

//------------------------------------------------------------
#define SHOW_FIELD(CLASS_NAME, FIELD_NAME)                                                                             \
    {                                                                                                                  \
        CFieldData* f = GETRUNTIME_CLASS(CLASS_NAME)->findField(FIELD_NAME);                                           \
        if (f)                                                                                                         \
            f->setHidden(false);                                                                                       \
    }

//------------------------------------------------------------
#define HIDE_ALL_FIELDS(CLASS_NAME)                                                                                    \
    { GETRUNTIME_CLASS(CLASS_NAME)->hideAllFields(); }
#define SHOW_ALL_FIELDS(CLASS_NAME)                                                                                    \
    { GETRUNTIME_CLASS(CLASS_NAME)->showAllFields(); }

//------------------------------------------------------------
#define RENAME_FIELD(CLASS_NAME, OLD_NAME, NEW_NAME)                                                                   \
    {                                                                                                                  \
        CFieldData* f = GETRUNTIME_CLASS(CLASS_NAME)->findField(OLD_NAME);                                             \
        if (f)                                                                                                         \
            f->setName(NEW_NAME);                                                                                      \
    }

//------------------------------------------------------------
#define IS_HIDDEN(CLASS_NAME, FIELD_NAME) GETRUNTIME_CLASS(CLASS_NAME)->isFieldHidden(FIELD_NAME)

}  // namespace qblocks
