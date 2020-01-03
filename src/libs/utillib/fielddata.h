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
#include <vector>

namespace qblocks {

//-------------------------------------------------------------------------
#define TS_NUMERAL (1 << 16)
#define TS_STRING (1 << 17)
#define TS_DATE (1 << 18)
#define TS_ARRAY (1 << 19)
#define TS_OBJECT (1 << 20)
#define TS_POINTER (1 << 21)
#define TS_BIGNUM (1 << 22)

#define T_DATE (5 | TS_DATE)
#define T_TIME (10 | TS_DATE)
#define T_TIMESTAMP (15 | TS_NUMERAL)
#define T_BOOL (20 | TS_NUMERAL)
#define T_NUMBER (25 | TS_NUMERAL)
#define T_BLOCKNUM (T_NUMBER)
#define T_UNUMBER (T_NUMBER)
#define T_UINT256 (T_NUMBER)
#define T_INT256 (T_NUMBER)
#define T_DOUBLE (30 | TS_NUMERAL)
#define T_WEI (35 | TS_NUMERAL | TS_BIGNUM)
#define T_GAS (40 | TS_NUMERAL | TS_BIGNUM)
#define T_ETHER (45 | TS_NUMERAL | TS_BIGNUM)
#define T_TEXT (50 | TS_STRING)
//#define T_BLOOM (T_TEXT)
#define T_ADDRESS (55 | TS_STRING)
#define T_HASH (60 | TS_STRING)
#define T_IPFSHASH (T_HASH)
#define T_BLOOM (65 | TS_STRING)
#define T_POINTER (70 | TS_POINTER)
#define T_OBJECT (75 | TS_OBJECT)

//-------------------------------------------------------------------------
class CFieldData {
  private:
    string_q m_fieldName;
    size_t m_fieldID;
    uint64_t m_fieldType;
    bool m_hidden;

  public:
    CFieldData(void) : m_fieldID(0), m_fieldType(0), m_hidden(false) {
    }
    CFieldData(const string_q& fn, size_t id, uint64_t t)
        : m_fieldName(fn), m_fieldID(id), m_fieldType(t), m_hidden(false) {
    }
    CFieldData(const CFieldData& cp) {
        m_fieldName = cp.m_fieldName;
        m_fieldID = cp.m_fieldID;
        m_fieldType = cp.m_fieldType;
        m_hidden = cp.m_hidden;
    }
    CFieldData& operator=(const CFieldData& cp) {
        m_fieldName = cp.m_fieldName;
        m_fieldID = cp.m_fieldID;
        m_fieldType = cp.m_fieldType;
        m_hidden = cp.m_hidden;
        return *this;
    }
    bool isHidden(void) const {
        return m_hidden;
    }
    void setHidden(bool hide) {
        m_hidden = hide;
    }

    bool isObject(void) const {
        return m_fieldType & TS_OBJECT;
    }
    bool isArray(void) const {
        return m_fieldType & TS_ARRAY;
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
    void setName(const string_q& str) {
        m_fieldName = str;
    }
    void setType(uint64_t type) {
        m_fieldType = type;
    }

    bool operator==(const CFieldData& data) {
        if (m_fieldName != data.m_fieldName)
            return false;
        if (m_fieldID != data.m_fieldID)
            return false;
        if (m_fieldType != data.m_fieldType)
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
    friend ostream& operator<<(ostream& os, const CFieldData& item);

    friend class CRuntimeClass;
    friend class CFieldList;
    friend class CBaseNode;
};
typedef vector<CFieldData> CFieldDataArray;

//-----------------------------------------------------------------------
extern void manageFields(const string_q& formatStr);
extern void manageFields(const string_q& listIn, bool show);
extern string_q baseTypeName(uint64_t type);
extern string_q fieldTypeStr(uint64_t type);
}  // namespace qblocks
