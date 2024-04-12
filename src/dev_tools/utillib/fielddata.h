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
#include <vector>

namespace qblocks {

class CRuntimeClass;
//-------------------------------------------------------------------------
class CFieldData {
  private:
    string_q m_fieldName;
    size_t m_fieldID;
    uint64_t m_fieldType;
    const CRuntimeClass* m_pType;
    bool m_hidden;

  public:
    CFieldData(void) : m_fieldID(0), m_fieldType(0), m_pType(NULL), m_hidden(false) {
    }
    CFieldData(const string_q& fn, size_t id, uint64_t t, const CRuntimeClass* pType = NULL)
        : m_fieldName(fn), m_fieldID(id), m_fieldType(t), m_pType(pType), m_hidden(false) {
    }
    CFieldData(const CFieldData& cp) {
        m_fieldName = cp.m_fieldName;
        m_fieldID = cp.m_fieldID;
        m_fieldType = cp.m_fieldType;
        m_pType = cp.m_pType;
        m_hidden = cp.m_hidden;
    }
    CFieldData& operator=(const CFieldData& cp) {
        m_fieldName = cp.m_fieldName;
        m_fieldID = cp.m_fieldID;
        m_fieldType = cp.m_fieldType;
        m_pType = cp.m_pType;
        m_hidden = cp.m_hidden;
        return *this;
    }

    string_q getName(void) const {
        return m_fieldName;
    }
    size_t getID(void) const {
        return m_fieldID;
    }
    uint64_t getType(void) const {
        return m_fieldType;
    }
    const CRuntimeClass* getObjType(void) const {
        return m_pType;
    }
    void setName(const string_q& str) {
        m_fieldName = str;
    }
    void setType(uint64_t type) {
        m_fieldType = type;
    }
    void setObjType(const CRuntimeClass* pClass) {
        m_pType = pClass;
    }

    bool operator==(const CFieldData& data) {
        if (m_fieldName != data.m_fieldName)
            return false;
        if (m_fieldID != data.m_fieldID)
            return false;
        if (m_fieldType != data.m_fieldType)
            return false;
        if (m_pType != data.m_pType)
            return false;
        if (m_hidden != data.m_hidden)
            return false;
        return true;
    }
    bool operator!=(const CFieldData& data) {
        return !operator==(data);
    }
    friend bool operator<(const CFieldData& v1, const CFieldData& v2) {
        return v1.getName() < v2.getName();
    }

    friend class CRuntimeClass;
    friend class CFieldList;
};
typedef vector<CFieldData> CFieldDataArray;
}  // namespace qblocks
