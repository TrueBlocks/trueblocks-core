#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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

  public:
    static CRuntimeClass classCBaseNode;
    static CBaseNode* createObject(void);
    virtual CRuntimeClass* getRuntimeClass(void) const;
    virtual string_q getValueByName(const string_q& fieldName) const;
    virtual bool setValueByName(const string_q& fieldName, const string_q& fieldValue);
    virtual void finishParse(void) {
    }
    virtual const CBaseNode* getObjectAt(const string_q& name, size_t i) const {
        return NULL;
    }
    virtual const string_q getStringAt(const string_q& fieldName, size_t i) const {
        return "";
    }

  protected:
    void initialize(void);
    void duplicate(const CBaseNode& bn);
};

//------------------------------------------------------------------
template <class T>
T RandomValue(T a, T b) {
    T range = (a > b ? a - b : b - a);
    if (range == 0)
        return a;
    return min(a, b) + (((T)rand()) % range);
}

//-------------------------------------------------------------------------
template <class T>
inline bool inRange(T val, T mn, T mx) {
    return (val >= mn && val <= mx);
}

//-------------------------------------------------------------------------
extern char* cleanUpJson(char* s);

//--------------------------------------------------------------------------------------------------------------
typedef string_q (*NEXTCHUNKFUNC)(const string_q& fieldIn, const void* data);

//--------------------------------------------------------------------------------------------------------------
extern string_q getNextChunk(string_q& fmtOut, NEXTCHUNKFUNC func, const void* data);
extern string_q fldNotFound(const string_q& str);

}  // namespace qblocks
