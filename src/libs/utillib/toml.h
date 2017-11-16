#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "database.h"
#include "biglib.h"

namespace qblocks {

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

        SFString getConfigStr(const SFString& group, const SFString& key, const SFString& def) const;
        uint64_t getConfigInt(const SFString& group, const SFString& key, uint64_t def) const;
        SFUintBN getConfigBigInt(const SFString& group, const SFString& key, SFUintBN def) const;
        bool getConfigBool(const SFString& group, const SFString& key, bool def) const;

        void setConfigStr(const SFString& group, const SFString& key, const SFString& value);
        void setConfigInt(const SFString& group, const SFString& key, uint64_t value);
        void setConfigBool(const SFString& group, const SFString& key, bool value);

        bool writeFile(void);
        bool readFile(const SFString& filename);
        void mergeFile(CToml *tomlIn);

        friend ostream& operator<<(ostream& os, const CToml& tomlIn);
    };

    //-------------------------------------------------------------------------
    extern ostream& operator<<(ostream& os, const CToml& t);

}  // namespace qblocks
