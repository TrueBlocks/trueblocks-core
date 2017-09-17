#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

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
            uint32_t     m_unused;
            timestamp_t  m_lastWritten;
        };

    public:
        SFArchiveHeader  m_header;
        bool             m_isReading;
        const CBaseNode  *pParent;
        //VISITARCHIVEFUNC writeMsgFunc;
        //VISITARCHIVEFUNC readMsgFunc;

        SFArchive(bool isReading) {
            m_isReading       = isReading;
            m_header.m_unused = getVersionNum();
            pParent           = NULL;
            //writeMsgFunc    = NULL;
            //readMsgFunc     = NULL;
        }

        bool isWriting(void) const {
            return !m_isReading;
        }

        bool isReading(void) const {
            return m_isReading;
        }

        void writeHeader(void) {
            Seek(0, SEEK_SET);
            m_header.m_lastWritten = toTimestamp(Now());
            operator<<(m_header.m_unused);
            operator<<(m_header.m_lastWritten);
            bool unused = false;
            operator<<(unused);
        }

        void readHeader(void) {
            Seek(0, SEEK_SET);
            operator>>(m_header.m_unused);
            operator>>(m_header.m_lastWritten);
            bool unused = false;
            operator>>(unused);
        }

        void resetForWriting(void) {
            if (isOpen())
                Release();
            m_isReading = false;
        }

        void resetForReading(void) {
            if (isOpen())
                Release();
            m_isReading = true;
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
