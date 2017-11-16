#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "runtimeclass.h"

namespace qblocks {

    //----------------------------------------------------------------------------
    class CExportContext;
    class SFArchive;

    //----------------------------------------------------------------------------
    class CBaseNode {
    public:
        uint64_t m_deleted;
        uint64_t m_schema;
        uint64_t m_showing;

    public:
        const CBaseNode *pParent;

        CBaseNode(void);
        virtual ~CBaseNode(void);

        bool isDeleted(void) const;
        void setDeleted(bool val);

        virtual bool isKindOf(const CRuntimeClass* pClass) const;
        virtual char *parseJson(char *s, uint32_t& nFields);
        virtual char *parseJson(char *s);
        virtual char *parseCSV(char *s, uint32_t& nFields, const SFString *fields);
        virtual char *parseText(char *s, uint32_t& nFields, const SFString *fields);
        virtual SFString toJson1(void) const;
        virtual SFString toJson(void) const;
        virtual SFString toJson(const SFString& fields) const;
        virtual SFString toJson(const CFieldList *fields) const;
        virtual SFString toJsonFldList(const CFieldList *fieldList) const;

    public:
        static CRuntimeClass classCBaseNode;
        static CBaseNode *CreateObject(void);
        virtual CRuntimeClass *getRuntimeClass(void) const;
        virtual SFString getValueByName(const SFString& fieldName) const;
        virtual bool setValueByName(const SFString& fieldName, const SFString& fieldValue) { return false; }
        virtual bool Serialize(SFArchive& archive);
        virtual bool SerializeC(SFArchive& archive) const;
        virtual bool readBackLevel(SFArchive& archive);
        virtual void finishParse(void) { }
        virtual void Format(CExportContext& ctx, const SFString& fmtIn, void *data = NULL) const { }
        virtual SFString Format(const SFString& fmtIn = "") const { return ""; }
        virtual const CBaseNode *getObjectAt(const SFString& name, uint32_t i) const { return NULL; }
        virtual const SFString   getStringAt(const SFString& name, uint32_t i) const { return ""; }
        virtual bool handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data = NULL) const {
            return false;
        }

        void doExport(ostream& os) const;

    protected:
        void Init(void);
        void Copy(const CBaseNode& bn);
    };

    //------------------------------------------------------------------
    template<class T>
    T RandomValue(T a, T b) {
        T range = (a > b ? a - b : b - a);
        if (range == 0)
            return a;
        return min(a, b) + (((T)rand()) % range);
    }

    //-------------------------------------------------------------------------
    template<class T>
    inline bool inRange(T val, T mn, T mx) {
        return (val >= mn && val <= mx);
    }

    //-------------------------------------------------------------------------
    extern char *cleanUpJson(char *s);

    //--------------------------------------------------------------------------------------------------------------
    typedef SFString (*NEXTCHUNKFUNC)(const SFString& fieldIn, const void *data);

    //--------------------------------------------------------------------------------------------------------------
    SFString getNextChunk(SFString& fmtOut, NEXTCHUNKFUNC func, const void *data);
    SFString fldNotFound(const SFString& str);

#define byzantiumBlock 4370000
}  // namespace qblocks
