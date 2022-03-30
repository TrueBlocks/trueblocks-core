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
#include "biglib.h"
#include "sfarchive.h"
#include "filenames.h"
#include "logging.h"

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

CArchive& CArchive::operator<<(const biguint_t& bn) {
    *this << bn.capacity;
    *this << bn.len;
    for (size_t i = 0; i < bn.len; i++)
        *this << (uint64_t)bn.blk[i];
    return *this;
}

CArchive& CArchive::operator<<(const bigint_t& bn) {
    *this << (const unsigned int)bn.sign;
    *this << bn.mag;
    return *this;
}

CArchive& CArchive::operator<<(const time_q& date) {
    *this << date_2_Ts(date);
    return *this;
}

CArchive& operator<<(CArchive& archive, const CStringArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        archive << array[i];
    return archive;
}

CArchive& operator<<(CArchive& archive, const CBigUintArray& array) {
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

CArchive& operator<<(CArchive& archive, const CIntArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        archive << array[i];
    return archive;
}

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

CArchive& CArchive::operator>>(biguint_t& bn) {
    // Note: I experimented with writing out
    // the blk in one Read/Write but it was
    // always slower on my machine
    unsigned int size;
    *this >> size;
    bn.allocate(size);
    bn.capacity = size;
    *this >> bn.len;
    for (size_t i = 0; i < bn.len; i++)
        *this >> bn.blk[i];
    return *this;
}

CArchive& CArchive::operator>>(bigint_t& bn) {
    *this >> bn.sign;
    *this >> bn.mag;
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

CArchive& operator>>(CArchive& archive, CBigUintArray& array) {
    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        biguint_t num;
        archive >> num;
        array.push_back(num);
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
CArchive& operator>>(CArchive& archive, CIntArray& array) {
    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        int64_t num;
        archive >> num;
        array.push_back(num);
    }
    return archive;
}

CArchive& CArchive::operator>>(time_q& date) {
    timestamp_t ts;
    *this >> ts;
    date = ts_2_Date(ts);
    return *this;
}

//----------------------------------------------------------------------
bool forEveryLineInAsciiFile(const string_q& filenameIn, CHARPTRFUNC func, void* data) {
    if (!func)
        return false;

    CFilename filename(filenameIn);
    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(filename.getFullPath(), modeReadOnly, LOCK_NOWAIT)) {
#define MAX_LINE (4096 * 1000)
        char buffer[MAX_LINE];
        bool done = false;
        while (!done) {
            bzero(buffer, MAX_LINE);
            done = (archive.ReadLine(buffer, MAX_LINE) == NULL);
            if (!done)
                done = !(*func)(buffer, data);  // returns true to continue
        }
        archive.Release();
        return done;
    }
    return true;
}

//----------------------------------------------------------------------
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
size_t asciiFileToMap(const string_q& fileName, CNameValueMap& theMap) {
    CStringArray lines;
    asciiFileToLines(fileName, lines);
    for (auto value : lines) {
        string_q key = nextTokenClear(value, ',');
        theMap[key] = value;
    }
    return theMap.size();
}

//----------------------------------------------------------------------
string_q asciiFileToString(const string_q& filename) {
    string_q ret;
    asciiFileToString(filename, ret);
    return ret;
}

//----------------------------------------------------------------------
size_t stringToAsciiFile(const string_q& fileName, const string_q& contents) {
    CAsciiFile lock;
    if (lock.Lock(fileName, modeWriteCreate, LOCK_WAIT)) {
        lock.WriteLine(contents.c_str());
        lock.Release();
    } else {
        string_q dName = fileName;
        if (isTestMode())
            dName = "--filename--";
        LOG_WARN("Could not open file: ", dName);
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------
bool appendFile(const string_q& toFile, const string_q& fromFile) {
    ofstream output;
    output.open(toFile, ios::out | ios::app);
    if (!output.is_open())
        return false;

    ifstream input(fromFile, ios::in);
    if (!input.is_open()) {
        output.close();
        return false;
    }

    output << input.rdbuf();
    output.flush();
    input.close();

    return true;
}

//----------------------------------------------------------------------
size_t appendToAsciiFile(const string_q& fileName, const string_q& addContents) {
    CArchive asciiCache(WRITING_ARCHIVE);
    if (asciiCache.Lock(fileName, modeWriteAppend, LOCK_WAIT)) {
        asciiCache.WriteLine(addContents.c_str());
        asciiCache.Release();
    }
    return fileSize(fileName);
}

//----------------------------------------------------------------------
string_q linesToString(const CStringArray& lines, char sep) {
    ostringstream os;
    for (auto line : lines)
        os << line << (sep != 0 ? string_q(1, sep) : "");
    return os.str();
}

//----------------------------------------------------------------------
size_t linesToAsciiFile(const string_q& fileName, const CStringArray& lines, char sep) {
    CArchive asciiCache(WRITING_ARCHIVE);
    if (asciiCache.Lock(fileName, modeWriteAppend, LOCK_WAIT)) {
        for (auto line : lines)
            asciiCache.WriteLine((line + (sep != 0 ? string_q(1, sep) : "")).c_str());
        asciiCache.Release();
    }
    return fileSize(fileName);
}

//----------------------------------------------------------------------
void CArchive::writeHeader(void) {
    Seek(0, SEEK_SET);
    m_header.m_lastWritten = date_2_Ts(Now());
    operator<<(m_header.m_version);
    operator<<(m_header.m_lastWritten);
    bool unused = false;
    operator<<(unused);
}

//-----------------------------------------------------------------------
bool CArchive::needsUpgrade(bool arrayFile) {
    if (!isOpen())
        return false;

    long t = Tell();  // so we can put it back
    uint64_t schema = 0;
    if (arrayFile) {
        Seek(sizeof(uint64_t) * 2, SEEK_SET);  // skip to the third 64 bit int which is schema (first is count, second
                                               // is first items deleted flag
    } else {
        Seek(sizeof(uint64_t), SEEK_SET);  // skip to the second 64 bit int which is schema
    }
    this->Read(schema);
    Seek(t, SEEK_SET);  // go back where we started

    return schema < getVersionNum();
}

}  // namespace qblocks
