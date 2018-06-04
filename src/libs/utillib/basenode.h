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

}  // namespace qblocks
