#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "database.h"
#include "biglib.h"

namespace qblocks {

    extern SFString cTeal, cOff;
    #define cleanFmt(str) ((str).Substitute("\\n\\\n", "\\n").Substitute("\n", "").Substitute("\\n", "\n").Substitute("\\t", "\t").Substitute("\\r", "\r"))
    //-------------------------------------------------------------------------
    class CToml : public CSharedResource {
    private:
        void Clear(void);
        // no default construction or copies
        CToml(void) {}
        CToml(const CToml& toml);
        CToml& operator= (const CToml& toml);

        //-------------------------------------------------------------------------
        // support class for toml file
        class CTomlKey {
        public:
            SFString keyName;
            SFString value;
            bool     comment;

            CTomlKey(void);
            CTomlKey(const CTomlKey& key);
            CTomlKey& operator=(const CTomlKey& key);
        };
        typedef SFList<CTomlKey*> CTomlKeyList;

        //-------------------------------------------------------------------------
        // support class for toml file
        class CTomlGroup {
        public:
            SFString  groupName;
            bool      isArray;
            bool      isComment;
            CTomlKeyList keys;

            CTomlGroup(void);
            CTomlGroup(const CTomlGroup& group);

            ~CTomlGroup(void);

            CTomlGroup& operator=(const CTomlGroup& group);

            CTomlKey *addKey(const SFString& keyName, const SFString& val, bool commented);
            CTomlKey *findKey(const SFString& keyName) const;

        private:
            void Clear(void);
            void Copy(const CTomlGroup& group);

        };
        typedef SFList<CTomlGroup*> CTomlGroupList;

    protected:
        CTomlGroup *addGroup(const SFString& group, bool commented, bool array);
        CTomlGroup *findGroup(const SFString& group) const;

        CTomlKey *addKey(const SFString& group, const SFString& key, const SFString& val, bool commented);
        CTomlKey *findKey(const SFString& group, const SFString& key) const;

    public:
        CTomlGroupList groups;

        CToml(const SFString& fileName);
        ~CToml(void);

        SFString getType(void) const override { return "CToml"; }

        SFString getConfigStr   (const SFString& group, const SFString& key, const SFString& def) const;
        uint64_t getConfigInt   (const SFString& group, const SFString& key, uint64_t def) const;
        SFUintBN getConfigBigInt(const SFString& group, const SFString& key, SFUintBN def) const;
        bool     getConfigBool  (const SFString& group, const SFString& key, bool def) const;
        SFString getDisplayStr  (bool terse, const SFString& def, const SFString& color=cTeal) const;

        void setConfigStr  (const SFString& group, const SFString& key, const SFString& value);
        void setConfigInt  (const SFString& group, const SFString& key, uint64_t value);
        void setConfigBool (const SFString& group, const SFString& key, bool value);

        bool writeFile(void);
        bool readFile(const SFString& filename);
        void mergeFile(CToml *tomlIn);

        friend ostream& operator<<(ostream& os, const CToml& tomlIn);
    };

    //-------------------------------------------------------------------------
    extern ostream& operator<<(ostream& os, const CToml& t);

}  // namespace qblocks
