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
#include "basetypes.h"

#include "toml.h"
#include "conversions.h"
#include "version.h"
#include "sfstring.h"
#include "logging.h"

namespace qblocks {

//-------------------------------------------------------------------------
CToml::CToml(const string_q& fileName) : CSharedResource() {
    setFilename(fileName);
    if (!fileName.empty())
        readFile(fileName);
}

//-------------------------------------------------------------------------
CToml::~CToml(void) {
    clear();
}

//-------------------------------------------------------------------------
void CToml::clear(void) {
    sections.clear();
}

//-------------------------------------------------------------------------
string_q CToml::addSection(const string_q& section) {
    if (findSection(section))
        return section;
    CTomlSection newSection(section);
    sections.push_back(newSection);
    return section;
}

//-------------------------------------------------------------------------
CToml::CTomlSection* CToml::findSection(const string_q& section) const {
    for (size_t i = 0; i < sections.size(); i++) {
        if (sections[i].sectionName == section) {
            return (CToml::CTomlSection*)&sections[i];
        }
    }
    return NULL;
}

//-------------------------------------------------------------------------
void CToml::addKey(const string_q& section, const string_q& key, const string_q& val) {
    CTomlSection* grp = findSection(section);
    if (grp)
        grp->addKey(key, val);
    return;
}

//-------------------------------------------------------------------------
void CToml::addComment(const string_q& section, const string_q& val) {
    CTomlSection* grp = findSection(section);
    if (grp)
        grp->addComment(val);
    return;
}

//-------------------------------------------------------------------------
CToml::CTomlKey* CToml::findKey(const string_q& section, const string_q& keyIn) const {
    CTomlSection* grp = findSection(section);
    if (grp) {
        for (size_t i = 0; i < grp->keys.size(); i++)
            if (grp->keys[i].getKey() == keyIn)
                return &grp->keys[i];
    }
    return NULL;
}

//-------------------------------------------------------------------------
uint64_t CToml::getConfigInt(const string_q& section, const string_q& key, uint64_t def) const {
    string_q ret = getConfigStr(section, key, uint_2_Str(def));
    return str_2_Uint(ret);
}

//-------------------------------------------------------------------------
bool CToml::getConfigBool(const string_q& section, const string_q& key, bool def) const {
    string_q ret = getConfigStr(section, key, int_2_Str(def ? 1 : 0));
    replaceAny(ret, ";\t\n\r ", "");
    return ((ret == "true" || ret == "1") ? true : false);
}

//---------------------------------------------------------------------------------------
string_q foldContinuations(const string_q& strIn) {
    string_q ret = strIn;
    replaceAll(ret, "\\\n ", "\\\n");  // if ends with '\' + '\n' + space, make it just '\' + '\n'
    replaceAll(ret, "\\\n", "");       // if ends with '\' + '\n', its a continuation, so fold in
    replaceAll(ret, "\\\r\n", "");     // same for \r\n
    return ret;
}

//---------------------------------------------------------------------------------------
bool CToml::readFile(const string_q& filename) {
    string_q curSection;
    clear();

    string_q contents = asciiFileToString(filename);
    contents = foldContinuations(contents);

    if (!contains(contents, "[version]")) {
        // return unused
        addSection("version");
        addKey("version", "current", getVersionStr(false, false));
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
            } else {
                addComment(curSection, value);
            }
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------
bool is_str(const string_q& str) {
    if (str.empty())
        return true;
    if (startsWith(str, '['))
        return false;
    if (isAddress(str))
        return true;
    if (isdigit(str[0]))
        return false;
    if (str == "true" || str == "false")
        return false;
    return true;
}

//---------------------------------------------------------------------------------------
string_q escape_quotes(const string_q& str) {
    string_q res;
    for (auto it = str.begin(); it != str.end(); ++it) {
        if (*it == '"')
            res += "\\";
        res += *it;
    }
    return res;
}

//---------------------------------------------------------------------------------------
bool CToml::writeFile(void) {
    if (!Lock(m_filename, modeWriteCreate, LOCK_CREATE)) {
        LockFailure();
        return false;
    }
    ostringstream os;
    os << *this;
    WriteLine(substitute(os.str(), "\n\n\n", "\n\n").c_str());
    Release();
    return true;
}

//---------------------------------------------------------------------------------------
bool CToml::isBackLevel(void) const {
    if (getVersion() < getVersionNum(0, 6, 2))
        return true;
    // This is where we would handle future upgrades
    return false;
}

//---------------------------------------------------------------------------------------
void CToml::mergeFile(CToml* tomlIn) {
    for (auto section : tomlIn->sections)
        for (auto key : section.keys)
            setConfigStr(section.sectionName, key.getKey(), "\"" + key.getValue() + "\"");
}

//---------------------------------------------------------------------------------------
string_q CToml::getConfigStr(const string_q& section, const string_q& key, const string_q& def) const {
    string_q env = getEnvStr(toUpper(section + "_" + key));
    if (!env.empty())
        return env;
    CTomlKey* found = findKey(section, key);
    if (found)
        return found->getValue();
    return def;
}

//-------------------------------------------------------------------------
uint64_t CToml::getVersion(void) const {
    // handle older ways of stroring version. Note: after 0.6.0, always stored as [version]current
    string_q value = getConfigStr("version", "current", getConfigStr("", "version", "0.0.0"));
    uint16_t v1 = (uint16_t)str_2_Uint(nextTokenClear(value, '.'));
    uint16_t v2 = (uint16_t)str_2_Uint(nextTokenClear(value, '.'));
    uint16_t v3 = (uint16_t)str_2_Uint(nextTokenClear(value, '.'));
    return getVersionNum(v1, v2, v3);
}

//-------------------------------------------------------------------------
void CToml::setConfigInt(const string_q& section, const string_q& key, uint64_t value) {
    setConfigStr(section, key, int_2_Str((int64_t)value));
}

//-------------------------------------------------------------------------
void CToml::setConfigBool(const string_q& section, const string_q& key, bool value) {
    setConfigStr(section, key, bool_2_Str(value));
}

//-------------------------------------------------------------------------
void CToml::setConfigStr(const string_q& section, const string_q& keyIn, const string_q& value) {
    if (startsWith(keyIn, '#'))
        return;

    string_q key = keyIn;
    CTomlSection* grp = findSection(section);
    if (!grp) {
        // return unused
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

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CToml& tomlIn) {
    bool first = true;
    for (auto section : tomlIn.sections) {
        if (!first)
            os << endl;
        os << "[";
        os << section.sectionName;
        os << "]";
        os << endl;
        for (auto key : section.keys) {
            string_q value = key.getValue();
            if ((!value.empty() && isNumeral(value)) || (value == "true" || value == "false")) {
                os << key.getKey() << " = " << value;

            } else {
                if (key.getKey().empty()) {
                    if (value != "#")
                        os << value;  // a comment

                } else {
                    string val = substitute(value, "\"", "\\\"");
                    os << key.getKey() << " = "
                       << "\"" << val << "\"";
                }
            }
            os << endl;
        }
        first = false;
    }
    return os;
}

//-------------------------------------------------------------------------
CToml::CTomlKey::CTomlKey() {
}

//-------------------------------------------------------------------------
CToml::CTomlKey::CTomlKey(const CTomlKey& key) : keyName(key.keyName), value(key.value) {
}

//-------------------------------------------------------------------------
CToml::CTomlKey& CToml::CTomlKey::operator=(const CTomlKey& key) {
    keyName = key.keyName;
    value = key.value;
    return *this;
}

//-------------------------------------------------------------------------
CToml::CTomlSection::CTomlSection(void) {
    clear();
}

//-------------------------------------------------------------------------
CToml::CTomlSection::CTomlSection(const CTomlSection& section) {
    copy(section);
}

//-------------------------------------------------------------------------
CToml::CTomlSection::~CTomlSection(void) {
    clear();
}

//-------------------------------------------------------------------------
CToml::CTomlSection& CToml::CTomlSection::operator=(const CTomlSection& section) {
    copy(section);
    return *this;
}

//-------------------------------------------------------------------------
void CToml::CTomlSection::clear(void) {
    sectionName = "";
    keys.clear();
}

//-------------------------------------------------------------------------
void CToml::CTomlSection::copy(const CTomlSection& section) {
    clear();

    sectionName = section.sectionName;
    keys.clear();
    for (auto key : section.keys)
        keys.push_back(key);
}

//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
void CToml::CTomlSection::addComment(const string_q& val) {
    CTomlComment comment(val);
    keys.push_back(comment);
    return;
}
}  // namespace qblocks
