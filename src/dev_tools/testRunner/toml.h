#pragma once
#include <algorithm>
#include <vector>
#include "database.h"

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
        string_q keyName;
        string_q value;

      public:
        CTomlKey(void);
        CTomlKey(const string_q& n, const string_q& v) : keyName(n), value(v) {
        }
        CTomlKey(const CTomlKey& key);
        CTomlKey& operator=(const CTomlKey& key);
        string_q getValue(void) const {
            return value;
        }
        void setValue(const string_q& v) {
            value = v;
        }
        string_q getKey(void) const {
            return keyName;
        }
        void setKey(const string_q& k) {
            keyName = k;
        }
    };

    //-------------------------------------------------------------------------
    // support class for toml file
    class CTomlComment : public CTomlKey {
      public:
        CTomlComment(void);
        CTomlComment(const string_q& v) {
            setValue(v);
        }
    };

    //-------------------------------------------------------------------------
    // support class for toml file
    class CTomlSection {
      public:
        string_q sectionName;
        vector<CTomlKey> keys;

        CTomlSection(void);
        CTomlSection(const string_q& n) : sectionName(n) {
        }
        CTomlSection(const CTomlSection& section);

        ~CTomlSection(void);

        CTomlSection& operator=(const CTomlSection& section);
        void addKey(const string_q& keyName, const string_q& val);

      private:
        void clear(void);
        void copy(const CTomlSection& section);
    };

  protected:
    string_q addSection(const string_q& section);
    void addKey(const string_q& section, const string_q& key, const string_q& val);

    CTomlSection* findSection(const string_q& section) const;
    CTomlKey* findKey(const string_q& section, const string_q& key) const;

    vector<CTomlSection> sections;

    void clear(void);
    string_q getType(void) const override {
        return "CToml";
    }

    bool readFile(const string_q& filename);

  public:
    explicit CToml(const string_q& fileName);
    ~CToml(void);

    string_q getConfigStr(const string_q& section, const string_q& key, const string_q& def) const;
    uint64_t getVersion(void) const;
    void setConfigStr(const string_q& section, const string_q& key, const string_q& value);
    void mergeFile(CToml* tomlIn);
};
