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
#include "basetypes.h"
#include "sfarchive.h"
#include "filenames.h"

namespace qblocks {

///////////////////////////////////////////////////////////////////
CArchive& CArchive::operator<<(bool b) {
    Write(b);
    return *this;
}

CArchive& CArchive::operator<<(char c) {
    Write(c);
    return *this;
}

CArchive& CArchive::operator<<(int dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(unsigned int dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(int8_t dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(int16_t dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(int64_t dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(uint8_t dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(uint16_t dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(uint64_t dw) {
    Write(dw);
    return *this;
}

CArchive& CArchive::operator<<(float f) {
    Write(f);
    return *this;
}

CArchive& CArchive::operator<<(double f) {
    Write(f);
    return *this;
}

CArchive& CArchive::operator<<(const char* str) {
    string_q s = str;
    Write(s);
    return *this;
}

CArchive& CArchive::operator<<(const string_q& str) {
    Write(str);
    return *this;
}

CArchive& operator<<(CArchive& archive, const CStringArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        archive << array[i];
    return archive;
}

CArchive& operator<<(CArchive& archive, const CUintArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        archive << array[i];
    return archive;
}

// CArchive& operator<<(CArchive& archive, const CIntArray& array) {
//     uint64_t count = array.size();
//     archive << count;
//     for (size_t i = 0; i < array.size(); i++)
//         archive << array[i];
//     return archive;
// }

///////////////////////////////////////////////////////////////////
CArchive& CArchive::operator>>(bool& b) {
    Read(b);
    return *this;
}

CArchive& CArchive::operator>>(char& c) {
    Read(c);
    return *this;
}

CArchive& CArchive::operator>>(int& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(unsigned int& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(int8_t& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(int16_t& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(int64_t& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(uint8_t& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(uint16_t& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(uint64_t& dw) {
    Read(dw);
    return *this;
}

CArchive& CArchive::operator>>(float& f) {
    Read(f);
    return *this;
}

CArchive& CArchive::operator>>(double& f) {
    Read(f);
    return *this;
}

CArchive& CArchive::operator>>(string_q& str) {
    Read(str);
    return *this;
}

CArchive& operator>>(CArchive& archive, CStringArray& array) {
    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        string_q str;
        archive >> str;
        array.push_back(str);
    }
    return archive;
}

//----------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CUintArray& array) {
    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        uint64_t num;
        archive >> num;
        array.push_back(num);
    }
    return archive;
}

//----------------------------------------------------------------------
// CArchive& operator>>(CArchive& archive, CIntArray& array) {
//     uint64_t count;
//     archive >> count;
//     for (size_t i = 0; i < count; i++) {
//         int64_t num;
//         archive >> num;
//         array.push_back(num);
//     }
//     return archive;
// }

size_t asciiFileToBuffer(const string_q& fileName, vector<char>& buffer) {
    size_t len = fileSize(fileName);
    buffer.resize(len);
    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(fileName, modeReadOnly, LOCK_NOWAIT)) {
        archive.Read(buffer.data(), len, 1);
        archive.Release();
    }
    return buffer.size();
}

//----------------------------------------------------------------------
size_t asciiFileToString(const string_q& fileName, string_q& contents) {
    vector<char> buffer;
    asciiFileToBuffer(fileName, buffer);
    buffer.push_back('\0');  // not sure if this is needed or not. At worse it's redundant
    contents = buffer.data();
    return contents.size();
}

//----------------------------------------------------------------------
size_t asciiFileToLines(const string_q& fileName, CStringArray& lines) {
    string_q contents;
    asciiFileToString(fileName, contents);
    explode(lines, contents, '\n');
    return lines.size();
}

//----------------------------------------------------------------------
size_t asciiFileToLines(const string_q& fileName, CUintArray& lines) {
    string_q contents;
    asciiFileToString(fileName, contents);
    explode(lines, contents, '\n');
    return lines.size();
}

//----------------------------------------------------------------------
string_q asciiFileToString(const string_q& filename) {
    string_q ret;
    asciiFileToString(filename, ret);
    return ret;
}

//----------------------------------------------------------------------
size_t stringToAsciiFile(const string_q& fileName, const string_q& contents) {
    CSharedResource lock;
    if (lock.Lock(fileName, modeWriteCreate, LOCK_WAIT)) {
        lock.WriteLine(contents.c_str());
        lock.Release();
    } else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------
void CArchive::writeHeader(void) {
    Seek(0, SEEK_SET);
    m_header.m_lastWritten = 12;
    operator<<(m_header.m_version);
    operator<<(m_header.m_lastWritten);
    bool unused = false;
    operator<<(unused);
}

}  // namespace qblocks
