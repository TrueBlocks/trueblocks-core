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
#include "basetypes.h"
#include "sfos.h"

namespace qblocks {

    //----------------------------------------------------------------------------
    class CExportOptions {
    public:
        size_t lev, spcs;
        bool noFrst;
        char tab, nl;
        bool quoteNums;
        bool hexNums;
        bool hashesOnly;
        bool colored;
        bool asEther;
        bool asDollars;
        bool asWei;
        bool isParity;
        CExportOptions(void) {
            noFrst = false;
            lev = 0; spcs = 2;
            tab = ' '; nl = '\n';
            quoteNums = false;
            hexNums = false;
            hashesOnly = false;
            colored = false;
            asEther = false;
            asDollars = false;
            asWei = true;
            isParity = false;
        }
    };
    extern CExportOptions& expContext(void);
    extern void incIndent(void);
    extern void decIndent(void);
    extern string_q indent(void);

    class CExportContext {
    public:
        size_t   nTabs;
        string_q fmt;
        char     tCh;

        CExportContext(void);
        virtual ~CExportContext(void) { }

        virtual CExportContext& operator<<(bool b);
        virtual CExportContext& operator<<(char c);
        virtual CExportContext& operator<<(int64_t dw);
        virtual CExportContext& operator<<(uint64_t ui);
        virtual CExportContext& operator<<(uint32_t sz);
        virtual CExportContext& operator<<(int i);
        virtual CExportContext& operator<<(float f);
        virtual CExportContext& operator<<(double f);
        virtual CExportContext& operator<<(const char *str);
        virtual CExportContext& operator<<(const string_q& str);

        virtual string_q tabs(size_t add = 0) { return string_q(nTabs + add, tCh); }
        virtual string_q inc(void) { string_q ret = string_q(nTabs, tCh); nTabs++; return ret; }
        virtual string_q dec(void) { nTabs--; return string_q(nTabs, tCh); }

        virtual void setOutput(void *output) = 0;
        virtual void* getOutput(void) const = 0;
        virtual void Output(const string_q& str) = 0;
        virtual void flush(void) = 0;
    };

    // Handy for generating code into strings
    class CStringExportContext : public CExportContext {
    public:
        string_q str;

        CStringExportContext(void) {}

        void  setOutput(void *output);
        void* getOutput(void) const { return NULL; }
        void  Output(const string_q& s);
        void  flush(void) { }  // do nothing

        operator string_q(void) const;
    };

    inline CStringExportContext::operator string_q(void) const {
        return str;
    }
}  // namespace qblocks
