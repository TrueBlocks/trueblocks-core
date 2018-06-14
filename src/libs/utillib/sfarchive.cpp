/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "basetypes.h"
#include "biglib.h"
#include "sfarchive.h"

namespace qblocks {

    ///////////////////////////////////////////////////////////////////
    SFArchive& SFArchive::operator<<(bool b) {
        Write(b);
        return *this;
    }

    SFArchive& SFArchive::operator<<(char c) {
        Write(c);
        return *this;
    }

    SFArchive& SFArchive::operator<<(int dw) {
        Write(dw);
        return *this;
    }

    SFArchive& SFArchive::operator<<(unsigned int dw) {
        Write(dw);
        return *this;
    }

    SFArchive& SFArchive::operator<<(int64_t dw) {
        Write(dw);
        return *this;
    }

    SFArchive& SFArchive::operator<<(uint64_t dw) {
        Write(dw);
        return *this;
    }

    SFArchive& SFArchive::operator<<(float f) {
        Write(f);
        return *this;
    }

    SFArchive& SFArchive::operator<<(double f) {
        Write(f);
        return *this;
    }

    SFArchive& SFArchive::operator<<(const char *str) {
        string_q s = str;
        Write(s);
        return *this;
    }

    SFArchive& SFArchive::operator<<(const string_q& str) {
        Write(str);
        return *this;
    }

    SFArchive& operator<<(SFArchive& archive, const CStringArray& array) {
        uint64_t count = array.size();
        archive << count;
        for (uint32_t i = 0 ; i < array.size() ; i++)
            archive << array[i];
        return archive;
    }

    SFArchive& operator<<(SFArchive& archive, const SFBigUintArray& array) {
        uint64_t count = array.size();
        archive << count;
        for (uint32_t i = 0 ; i < array.size() ; i++)
            archive << array[i];
        return archive;
    }

    SFArchive& operator<<(SFArchive& archive, const SFUintArray& array) {
        uint64_t count = array.size();
        archive << count;
        for (uint32_t i = 0 ; i < array.size() ; i++)
            archive << array[i];
        return archive;
    }

    SFArchive& SFArchive::operator<<(const SFUintBN& bn) {
        *this << bn.capacity;
        *this << bn.len;
        for (size_t i=0 ; i < bn.len ; i++)
            *this << (uint64_t)bn.blk[i];
        return *this;
    }

    SFArchive& SFArchive::operator<<(const SFIntBN& bn) {
        *this << (const unsigned int)bn.sign;
        *this << bn.mag;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////
    SFArchive& SFArchive::operator>>(bool& b) {
        Read(b);
        return *this;
    }

    SFArchive& SFArchive::operator>>(char& c) {
        Read(c);
        return *this;
    }

    SFArchive& SFArchive::operator>>(int& dw) {
        Read(dw);
        return *this;
    }

    SFArchive& SFArchive::operator>>(unsigned int& dw) {
        Read(dw);
        return *this;
    }

    SFArchive& SFArchive::operator>>(int64_t& dw) {
        Read(dw);
        return *this;
    }

    SFArchive& SFArchive::operator>>(uint64_t& dw) {
        Read(dw);
        return *this;
    }

    SFArchive& SFArchive::operator>>(float& f) {
        Read(f);
        return *this;
    }

    SFArchive& SFArchive::operator>>(double& f) {
        Read(f);
        return *this;
    }

    SFArchive& SFArchive::operator>>(string_q& str) {
        Read(str);
        return *this;
    }

    SFArchive& operator>>(SFArchive& archive, CStringArray& array) {
        uint64_t count;
        archive >> count;
        for (uint32_t i = 0 ; i < count ; i++) {
            string_q str;
            archive >> str;
            array.push_back(str);
        }
        return archive;
    }

    SFArchive& operator>>(SFArchive& archive, SFBigUintArray& array) {
        uint64_t count;
        archive >> count;
        for (uint32_t i = 0 ; i < count ; i++) {
            SFUintBN num;
            archive >> num;
            array.push_back(num);
        }
        return archive;
    }

    SFArchive& operator>>(SFArchive& archive, SFUintArray& array) {
        uint64_t count;
        archive >> count;
        for (uint32_t i = 0 ; i < count ; i++) {
            uint64_t num;
            archive >> num;
            array.push_back(num);
        }
        return archive;
    }

    SFArchive& SFArchive::operator>>(SFUintBN& bn) {
        // Note: I experimented with writing out
        // the blk in one Read/Write but it was
        // always slower on my machine
        unsigned int size;
        *this >> size;
        bn.allocate(size);
        bn.capacity = size;
        *this >> bn.len;
        for (size_t i=0 ; i < bn.len ; i++)
            *this >> bn.blk[i];
        return *this;
    }

    SFArchive& SFArchive::operator>>(SFIntBN& bn) {
        *this >> bn.sign;
        *this >> bn.mag;
        return *this;
    }

    //----------------------------------------------------------------------
    uint64_t appendToAsciiFile(const string_q& fileName, const string_q& addContents) {
        SFArchive asciiCache(WRITING_ARCHIVE);
        if (asciiCache.Lock(fileName, asciiWriteAppend, LOCK_NOWAIT)) {
            asciiCache.WriteLine(addContents.c_str());
            asciiCache.Release();
        }
        return fileSize(fileName);
    }

}  // namespace qblocks
