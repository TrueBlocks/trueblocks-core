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
#include "database.h"
#include "conversions.h"
#include "version.h"
#include "sftime.h"

namespace qblocks {

#define READING_ARCHIVE true
#define WRITING_ARCHIVE false

class CBaseNode;
typedef bool (*VISITARCHIVEFUNC)(CBaseNode& node, void* data);

//-----------------------------------------------------------------------------------------
class CArchive : public CSharedResource {
  private:
    class CArchiveHeader {
      public:
        // This data structure is sometimes written at the head of a
        // file. Don't remove it or change its size;
        uint32_t m_version;
        timestamp_t m_lastWritten;
    };

  public:
    CArchiveHeader m_header;
    bool m_isReading;
    // VISITARCHIVEFUNC writeMsgFunc;
    // VISITARCHIVEFUNC readMsgFunc;

    explicit CArchive(bool isReading) : CSharedResource() {
        m_isReading = isReading;
        m_header.m_version = getVersionNum();
        // writeMsgFunc     = NULL;
        // readMsgFunc      = NULL;
    }

    string_q getType(void) const override {
        return "CArchive";
    }

    bool isWriting(void) const {
        return !m_isReading;
    }

    bool isReading(void) const {
        return m_isReading;
    }

    void writeHeader(void);
    bool needsUpgrade(bool arrayFile);

    CArchive& operator<<(bool b);
    CArchive& operator<<(char c);
    CArchive& operator<<(int d);
    CArchive& operator<<(unsigned int d);
    CArchive& operator<<(int8_t dw);
    CArchive& operator<<(uint8_t dw);
    CArchive& operator<<(int16_t dw);
    CArchive& operator<<(uint16_t dw);
    CArchive& operator<<(int64_t dw);
    CArchive& operator<<(uint64_t dw);
    CArchive& operator<<(float f);
    CArchive& operator<<(double f);
    CArchive& operator<<(const string_q& str);
    CArchive& operator<<(const biguint_t& bn);
    CArchive& operator<<(const bigint_t& bn);
    CArchive& operator<<(const char* str);
    CArchive& operator<<(const time_q& date);

    CArchive& operator>>(bool& b);
    CArchive& operator>>(char& c);
    CArchive& operator>>(int& d);
    CArchive& operator>>(unsigned int& d);
    CArchive& operator>>(int8_t& dw);
    CArchive& operator>>(uint8_t& dw);
    CArchive& operator>>(int16_t& dw);
    CArchive& operator>>(uint16_t& dw);
    CArchive& operator>>(int64_t& dw);
    CArchive& operator>>(uint64_t& dw);
    CArchive& operator>>(float& f);
    CArchive& operator>>(double& f);
    CArchive& operator>>(string_q& str);
    CArchive& operator>>(biguint_t& bn);
    CArchive& operator>>(bigint_t& bn);
    CArchive& operator>>(time_q& date);
};

extern CArchive& operator<<(CArchive& archive, const CStringArray& array);
extern CArchive& operator<<(CArchive& archive, const CBigUintArray& array);
extern CArchive& operator<<(CArchive& archive, const CUintArray& array);
extern CArchive& operator<<(CArchive& archive, const CIntArray& array);

extern CArchive& operator>>(CArchive& archive, CStringArray& array);
extern CArchive& operator>>(CArchive& archive, CBigUintArray& array);
extern CArchive& operator>>(CArchive& archive, CUintArray& array);
extern CArchive& operator>>(CArchive& archive, CIntArray& array);

}  // namespace qblocks
