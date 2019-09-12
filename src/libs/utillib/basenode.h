#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include "runtimeclass.h"

namespace qblocks {

    //----------------------------------------------------------------------------
    class CArchive;

    //----------------------------------------------------------------------------
    class CBaseNode {
    private:
        virtual char *parseJson1(char *s, size_t& nFields);

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
        virtual bool  parseJson3(string_q& str);
        virtual bool  parseJson4(string_q& str);
        virtual bool  parseCSV(const CStringArray& fields, string_q& str);
        virtual bool  parseText(const CStringArray& fields, string_q& str);
        virtual string_q toJson1(void) const;
        virtual string_q toJson(void) const;
        virtual string_q toJson(const string_q& fields) const;
        virtual string_q jsonFromArray(const CFieldDataArray& array) const;

    public:
        //TODO(tjayrush): global data
        static CRuntimeClass classCBaseNode;
        static CBaseNode *createObject(void);
        virtual CRuntimeClass *getRuntimeClass(void) const;
        virtual string_q getValueByName(const string_q& fieldName) const;
        virtual bool setValueByName(const string_q& fieldName, const string_q& fieldValue) { return false; }
        virtual bool Serialize(CArchive& archive);
        virtual bool SerializeC(CArchive& archive) const;
        virtual bool readBackLevel(CArchive& archive);
        virtual void finishParse(void) { }
        virtual void Format(ostream& ctx, const string_q& fmtIn, void *data = NULL) const { }
        virtual string_q Format(const string_q& fmtIn = "") const { return ""; }
        virtual const CBaseNode *getObjectAt(const string_q& name, size_t i) const { return NULL; }
        virtual const string_q   getStringAt(const string_q& fieldName, size_t i) const { return ""; }
        virtual bool showEmptyField(const string_q& fn) const { return true; }

        void doExport(ostream& os) const;

    protected:
        void initialize(void);
        void duplicate(const CBaseNode& bn);
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
    typedef string_q (*NEXTCHUNKFUNC)(const string_q& fieldIn, const void *data);

    //--------------------------------------------------------------------------------------------------------------
    extern string_q getNextChunk(string_q& fmtOut, NEXTCHUNKFUNC func, const void *data);
    extern string_q fldNotFound(const string_q& str);

    //--------------------------------------------------------------------------------------------------------------
    extern void preserveSpaces(string_q& str);
    extern void unpreserveSpaces(string_q& str);

}  // namespace qblocks
