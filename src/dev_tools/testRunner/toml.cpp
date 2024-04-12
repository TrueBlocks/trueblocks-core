#include "utillib.h"
#include "toml.h"

CToml::CToml(const string_q& fileName) : CSharedResource() {
    setFilename(fileName);
    if (!fileName.empty())
        readFile(fileName);
}

CToml::~CToml(void) {
    clear();
}

void CToml::clear(void) {
    sections.clear();
}

string_q CToml::addSection(const string_q& section) {
    if (findSection(section))
        return section;
    CTomlSection newSection(section);
    sections.push_back(newSection);
    return section;
}

CToml::CTomlSection* CToml::findSection(const string_q& section) const {
    for (size_t i = 0; i < sections.size(); i++) {
        if (sections[i].sectionName == section) {
            return (CToml::CTomlSection*)&sections[i];
        }
    }
    return NULL;
}

void CToml::addKey(const string_q& section, const string_q& key, const string_q& val) {
    CTomlSection* grp = findSection(section);
    if (grp)
        grp->addKey(key, val);
    return;
}

CToml::CTomlKey* CToml::findKey(const string_q& section, const string_q& keyIn) const {
    CTomlSection* grp = findSection(section);
    if (grp) {
        for (size_t i = 0; i < grp->keys.size(); i++)
            if (grp->keys[i].getKey() == keyIn)
                return &grp->keys[i];
    }
    return NULL;
}

bool CToml::readFile(const string_q& filename) {
    string_q curSection;
    clear();

    string_q contents = asciiFileToString(filename);
    if (!contains(contents, "[version]")) {
        addSection("version");
        addKey("version", "current", "2.5.8-release");
    }

    while (!contents.empty()) {
        string_q value = trimWhitespace(nextTokenClear(contents, '\n'));
        if (value.empty())
            value = "#";

        if (startsWith(value, '[')) {  // it's a section
            value = trim(trimWhitespace(substitute(substitute(value, "[", ""), "]", "")), '\"');
            curSection = addSection(value);

        } else {
            if (curSection.empty()) {
                string_q section = "root-level";
                curSection = addSection(section);
            }
            if (!startsWith(value, '#')) {
                string_q key = nextTokenClear(value, '=');
                addKey(curSection, trimWhitespace(key), trimWhitespace(value));
            }
        }
    }

    return true;
}

void CToml::mergeFile(CToml* tomlIn) {
    for (auto section : tomlIn->sections) {
        for (auto key : section.keys) {
            setConfigStr(section.sectionName, key.getKey(), "\"" + key.getValue() + "\"");
        }
    }
}

string_q getEnvStr(const string_q& name) {
    char* sss = getenv(name.c_str());
    return (sss ? string_q(sss) : string_q(""));
}

string_q CToml::getConfigStr(const string_q& section, const string_q& key, const string_q& def) const {
    string_q theKey = toUpper("TB_" + substitute(section, ".", "_") + "_" + key);
    string_q env = getEnvStr(theKey);
    if (!env.empty()) {
        return env;

    } else {
        // Backward compatibility -- old style
        theKey = toUpper(section + "_" + key);
        env = getEnvStr(theKey);
        if (!env.empty())
            return env;
    }

    CTomlKey* found = findKey(section, key);
    if (found)
        return found->getValue();

    return def;
}

uint64_t CToml::getVersion(void) const {
    // handle older ways of stroring version. Note: after 0.6.0, always stored as [version]current
    return 20000 + 500 + 8;
}

void CToml::setConfigStr(const string_q& section, const string_q& keyIn, const string_q& value) {
    if (startsWith(keyIn, '#'))
        return;

    string_q key = keyIn;
    CTomlSection* grp = findSection(section);
    if (!grp) {
        addSection(section);
        addKey(section, key, value);
    } else {
        CTomlKey* found = findKey(section, key);
        if (found) {
            found->setValue(value);
        } else {
            addKey(section, key, value);
        }
    }
}

CToml::CTomlKey::CTomlKey() {
}

CToml::CTomlKey::CTomlKey(const CTomlKey& key) : keyName(key.keyName), value(key.value) {
}

CToml::CTomlKey& CToml::CTomlKey::operator=(const CTomlKey& key) {
    keyName = key.keyName;
    value = key.value;
    return *this;
}

CToml::CTomlSection::CTomlSection(void) {
    clear();
}

CToml::CTomlSection::CTomlSection(const CTomlSection& section) {
    copy(section);
}

CToml::CTomlSection::~CTomlSection(void) {
    clear();
}

CToml::CTomlSection& CToml::CTomlSection::operator=(const CTomlSection& section) {
    copy(section);
    return *this;
}

void CToml::CTomlSection::clear(void) {
    sectionName = "";
    keys.clear();
}

void CToml::CTomlSection::copy(const CTomlSection& section) {
    clear();

    sectionName = section.sectionName;
    keys.clear();
    for (auto key : section.keys)
        keys.push_back(key);
}

void CToml::CTomlSection::addKey(const string_q& keyName, const string_q& val) {
    string_q str = substitute(val, "\"\"\"", "");
    if (endsWith(str, '\"'))
        replaceReverse(str, "\"", "");
    if (startsWith(str, '\"'))
        replace(str, "\"", "");
    str = substitute(str, "\\\"", "\"");  // unescape
    str = substitute(str, "\\#", "#");    // unescape
    CTomlKey key(keyName, str);
    keys.push_back(key);
    return;
}
