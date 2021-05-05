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

#include <algorithm>
#include <vector>
#include "database.h"
#include "biglib.h"
#include "colors.h"

namespace qblocks {

//-------------------------------------------------------------------------
class CToml : public CSharedResource {
  private:
    // no default construction or copies
    CToml(void) {
    }
    CToml(const CToml& toml);
    CToml& operator=(const CToml& toml);

    //-------------------------------------------------------------------------
    // support class for toml file
    class CTomlKey {
      public:
        string_q keyName;
        string_q value;
        bool comment;
        bool deleted;

        CTomlKey(void);
        CTomlKey(const string_q& n, const string_q& v, bool c) : keyName(n), value(v), comment(c), deleted(false) {
        }
        CTomlKey(const CTomlKey& key);
        CTomlKey& operator=(const CTomlKey& key);
    };

    //-------------------------------------------------------------------------
    // support class for toml file
    class CTomlSection {
      public:
        string_q sectionName;
        bool isArray;
        bool isComment;
        vector<CTomlKey> keys;

        CTomlSection(void);
        CTomlSection(const string_q& n, bool a, bool c) : sectionName(n), isArray(a), isComment(c) {
        }
        CTomlSection(const CTomlSection& section);

        ~CTomlSection(void);

        CTomlSection& operator=(const CTomlSection& section);
        void addKey(const string_q& keyName, const string_q& val, bool commented);

      private:
        void clear(void);
        void copy(const CTomlSection& section);
    };

  protected:
    void addSection(const string_q& section, bool commented, bool array);
    void addKey(const string_q& section, const string_q& key, const string_q& val, bool commented);

    CTomlSection* findSection(const string_q& section) const;
    CTomlKey* findKey(const string_q& section, const string_q& key) const;

  public:
    void deleteKey(const string_q& section, const string_q& key);

  public:
    vector<CTomlSection> sections;

    explicit CToml(const string_q& fileName);
    ~CToml(void);

    void clear(void);

    string_q getType(void) const override {
        return "CToml";
    }

    string_q getConfigStr(const string_q& section, const string_q& key, const string_q& def) const;
    string_q getConfigJson(const string_q& section, const string_q& key, const string_q& def) const;
    uint64_t getConfigInt(const string_q& section, const string_q& key, uint64_t def) const;
    biguint_t getConfigBigInt(const string_q& section, const string_q& key, biguint_t def) const;
    bool getConfigBool(const string_q& section, const string_q& key, bool def) const;
    uint64_t getVersion(void) const;

    void setConfigArray(const string_q& section, const string_q& key, const string_q& value);
    void setConfigStr(const string_q& section, const string_q& key, const string_q& value);
    void setConfigInt(const string_q& section, const string_q& key, uint64_t value);
    void setConfigBool(const string_q& section, const string_q& key, bool value);

    bool writeFile(void);
    bool readFile(const string_q& filename);
    void mergeFile(CToml* tomlIn);

    bool isBackLevel(void) const;
    friend ostream& operator<<(ostream& os, const CToml& tomlIn);
};

//-------------------------------------------------------------------------
extern ostream& operator<<(ostream& os, const CToml& t);

}  // namespace qblocks
