#pragma once
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
#include "exportcontext.h"
#include "database.h"
#include "conversions.h"
#include "version.h"

namespace qblocks {

    #define READING_ARCHIVE true
    #define WRITING_ARCHIVE false

    class CBaseNode;
    typedef bool (*VISITARCHIVEFUNC)(CBaseNode& node, void *data);

    //-----------------------------------------------------------------------------------------
    class SFArchive : public CSharedResource {
    private:
        class SFArchiveHeader {
        public:
            // This data structure is sometimes written at the head of a
            // file. Don't remove it or change its size;
            uint32_t     m_version;
            timestamp_t  m_lastWritten;
        };

    public:
        SFArchiveHeader  m_header;
        bool             m_isReading;
        //VISITARCHIVEFUNC writeMsgFunc;
        //VISITARCHIVEFUNC readMsgFunc;

        SFArchive(bool isReading) : CSharedResource() {
            m_isReading        = isReading;
            m_header.m_version = getVersionNum();
            //writeMsgFunc     = NULL;
            //readMsgFunc      = NULL;
        }

        SFString getType(void) const override { return "SFArchive"; }
        
        bool isWriting(void) const {
            return !m_isReading;
        }

        bool isReading(void) const {
            return m_isReading;
        }

        void writeHeader(void) {
            Seek(0, SEEK_SET);
            m_header.m_lastWritten = toTimestamp(Now());
            operator<<(m_header.m_version);
            operator<<(m_header.m_lastWritten);
            bool unused = false;
            operator<<(unused);
        }

        void readHeader(void) {
            Seek(0, SEEK_SET);
            operator>>(m_header.m_version);
            operator>>(m_header.m_lastWritten);
            bool unused = false;
            operator>>(unused);
        }

        SFArchive& operator<<(bool b);
        SFArchive& operator<<(char c);
        SFArchive& operator<<(int d);
        SFArchive& operator<<(unsigned int d);
        SFArchive& operator<<(int64_t dw);
        SFArchive& operator<<(uint64_t dw);
        SFArchive& operator<<(float f);
        SFArchive& operator<<(double f);
        SFArchive& operator<<(const SFString& str);
        SFArchive& operator<<(const SFTime& tm);
        SFArchive& operator<<(const SFUintBN& bn);
        SFArchive& operator<<(const SFIntBN& bn);
        SFArchive& operator<<(const char *str);

        SFArchive& operator>>(bool& b);
        SFArchive& operator>>(char& c);
        SFArchive& operator>>(int& d);
        SFArchive& operator>>(unsigned int& d);
        SFArchive& operator>>(int64_t& dw);
        SFArchive& operator>>(uint64_t& dw);
        SFArchive& operator>>(float& f);
        SFArchive& operator>>(double& f);
        SFArchive& operator>>(SFString& str);
        SFArchive& operator>>(SFTime& tm);
        SFArchive& operator>>(SFUintBN& bn);
        SFArchive& operator>>(SFIntBN& bn);
    };

    extern SFArchive& operator<<(SFArchive& archive, const SFStringArray& array);
    extern SFArchive& operator<<(SFArchive& archive, const SFBigUintArray& array);
    extern SFArchive& operator<<(SFArchive& archive, const SFUintArray& array);

    extern SFArchive& operator>>(SFArchive& archive, SFStringArray& array);
    extern SFArchive& operator>>(SFArchive& archive, SFBigUintArray& array);
    extern SFArchive& operator>>(SFArchive& archive, SFUintArray& array);
}  // namespace qblocks
