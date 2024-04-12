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
#include "sftime.h"

namespace qblocks {

#define READING_ARCHIVE true
#define WRITING_ARCHIVE false

class CArchive : public CSharedResource {
  private:
    class CArchiveHeader {
      public:
        // This data structure is sometimes written at the head of a
        // file. Don't remove it or change its size;
        uint32_t m_version;
        int64_t m_lastWritten;
    };

  public:
    CArchiveHeader m_header;
    bool m_isReading;

    explicit CArchive(bool isReading) : CSharedResource() {
        m_isReading = isReading;
        m_header.m_version = 20000 + 500 + 8;
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
    CArchive& operator<<(const char* str);

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
};

}  // namespace qblocks
