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

namespace qblocks {

    class CBaseNode;
    typedef bool (*VISITARCHIVEFUNC)(CBaseNode& node, void *data);

    //-----------------------------------------------------------------------------------------
    class SFArchive : public CSharedResource {
    private:
        class SFArchiveHeader {
        public:
            bool         m_writeDeleted;
            uint32_t     m_archiveSchema;
            timestamp_t  m_lastWritten;
        };

    public:
        SFArchiveHeader  m_header;
        bool             m_isReading;
        const CBaseNode  *pParent;
        //VISITARCHIVEFUNC writeMsgFunc;
        //VISITARCHIVEFUNC readMsgFunc;

        SFArchive(bool isReading, uint32_t schema, bool writeDeleted) {
            m_isReading              = isReading;
            m_header.m_archiveSchema = schema;
            m_header.m_writeDeleted  = writeDeleted;
            pParent                  = NULL;
            //writeMsgFunc           = NULL;
            //readMsgFunc            = NULL;
        }

        bool writeDeleted(void) const {
            return (m_header.m_writeDeleted);
        }

        bool isWriting(void) const {
            return !m_isReading;
        }

        bool isReading(void) const {
            return m_isReading;
        }

        void writeHeader(void) {
            Seek(0, SEEK_SET);
            m_header.m_lastWritten = toTimeStamp(Now());
            operator<<(m_header.m_archiveSchema);
            operator<<(m_header.m_lastWritten);
            operator<<(m_header.m_writeDeleted);
        }

        void readHeader(void) {
            Seek(0, SEEK_SET);
            operator>>(m_header.m_archiveSchema);
            operator>>(m_header.m_lastWritten);
            operator>>(m_header.m_writeDeleted);
        }

        bool isSchema(uint32_t testSchema) {
            return (m_header.m_archiveSchema == testSchema);
        }

        void resetForWriting(void) {
            if (isOpen())
                Release();
            m_isReading = false;
            // no reason to change these
            // m_archiveSchema = NO_SCHEMA;
            // m_writeDeleted = true;
        }

        void resetForReading(void) {
            if (isOpen())
                Release();
            m_isReading = true;
            // no reason to change these
            // m_archiveSchema = NO_SCHEMA;
            // m_writeDeleted = true;
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

    extern SFArchive& operator>>(SFArchive& archive, SFStringArray& array);
    extern SFArchive& operator>>(SFArchive& archive, SFBigUintArray& array);
}  // namespace qblocks
