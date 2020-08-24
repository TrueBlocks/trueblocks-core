/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
void CToml::addSection(const string_q& section, bool commented, bool array) {
    if (findSection(section))
        return;
    CTomlSection newSection(section, array, commented);
    sections.push_back(newSection);
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
void CToml::addKey(const string_q& section, const string_q& key, const string_q& val, bool commented) {
    CTomlSection* grp = findSection(section);
    if (grp)
        grp->addKey(key, val, commented);
    return;
}

//-------------------------------------------------------------------------
CToml::CTomlKey* CToml::findKey(const string_q& section, const string_q& keyIn) const {
    CTomlSection* grp = findSection(section);
    if (grp) {
        for (size_t i = 0; i < grp->keys.size(); i++)
            if (grp->keys[i].keyName == keyIn)
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
extern string_q stripFullLineComments(const string_q& inStr);
extern string_q collapseArrays(const string_q& inStr);
bool CToml::readFile(const string_q& filename) {
    string_q curSection;
    clear();

    string_q contents;
    asciiFileToString(filename, contents);
    if (!contains(contents, "[version]")) {
        addSection("version", false, false);
        addKey("version", "current", getVersionStr(false, false), false);
    }
    replaceAll(contents, "\\\n ", "\\\n");  // if ends with '\' + '\n' + space, make it just '\' + '\n'
    replaceAll(contents, "\\\n", "");       // if ends with '\' + '\n', its a continuation, so fold in
    replaceAll(contents, "\\\r\n", "");     // same for \r\n
    contents = collapseArrays(stripFullLineComments(contents));
    while (!contents.empty()) {
        string_q value = trimWhitespace(nextTokenClear(contents, '\n'));
        bool comment = startsWith(value, '#');
        if (comment)
            value = trim(extract(value, 1));
        if (!value.empty()) {
            bool isArray = contains(value, "[[");
            if (startsWith(value, '[')) {  // it's a section
                value = trim(trimWhitespace(substitute(substitute(value, "[", ""), "]", "")), '\"');
                addSection(value, comment, isArray);
                curSection = value;

            } else {
                if (curSection.empty()) {
                    string_q section = "root-level";
                    addSection(section, false, false);
                    curSection = section;
                }
                string_q key = nextTokenClear(value, '=');  // value may be empty, but not whitespace
                addKey(curSection, trimWhitespace(key), trimWhitespace(value), comment);
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
    WriteLine(os.str().c_str());
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
    for (auto section : tomlIn->sections) {
        for (auto key : section.keys) {
            setConfigStr(section.sectionName, key.keyName, "\"" + key.value + "\"");
            if (key.deleted)
                deleteKey(section.sectionName, key.keyName);
            if (key.comment) {
                CTomlKey* k = findKey(section.sectionName, key.keyName);
                if (k)
                    k->comment = true;
            }
        }
    }
}

//---------------------------------------------------------------------------------------
biguint_t CToml::getConfigBigInt(const string_q& section, const string_q& key, biguint_t def) const {
    string_q ret = getConfigStr(section, key, bnu_2_Str(def));
    string_q check = ret;
    replaceAny(check, "0123456789abcdefABCDEF", "");
    if (!check.empty()) {
        LOG_WARN("Big int config item ", section, "::", key, " is not an integer...returning zero.");
        return 0;
    }
    return str_2_BigUint(ret);
}

//---------------------------------------------------------------------------------------
string_q CToml::getConfigStr(const string_q& section, const string_q& key, const string_q& def) const {
    string_q env = getEnvStr(toUpper(section + "_" + key));
    if (!env.empty())
        return env;
    CTomlKey* found = findKey(section, key);
    if (found && !found->comment)
        return found->value;
    return def;
}

//---------------------------------------------------------------------------------------
string_q CToml::getConfigJson(const string_q& section, const string_q& key, const string_q& def) const {
    return getConfigStr(section, key, def);
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
void CToml::setConfigArray(const string_q& section, const string_q& key, const string& value) {
    setConfigStr(section, key, value);
    CToml::CTomlSection* grp = findSection(section);
    if (grp)
        grp->isArray = true;
}

//-------------------------------------------------------------------------
void CToml::setConfigStr(const string_q& section, const string_q& keyIn, const string_q& value) {
    bool comment = startsWith(keyIn, '#');
    string_q key = (comment ? extract(keyIn, 1) : keyIn);

    CTomlSection* grp = findSection(section);
    if (!grp) {
        addSection(section, false, false);
        addKey(section, key, value, comment);

    } else {
        CTomlKey* found = findKey(section, key);
        if (found) {
            found->comment = comment;
            found->value = value;
        } else {
            addKey(section, key, value, comment);
        }
    }
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CToml& tomlIn) {
    bool first = true;
    for (auto section : tomlIn.sections) {
        if (!first)
            os << endl;
        os << (section.isComment ? "# " : "");
        os << (section.isArray ? "[[" : "[");
        os << section.sectionName;
        os << (section.isArray ? "]]" : "]");
        os << endl;
        for (auto key : section.keys) {
            os << (key.comment || section.isComment || key.deleted ? "# " : "");
            if ((!key.value.empty() && isNumeral(key.value)) || (key.value == "true" || key.value == "false")) {
                os << key.keyName << " = " << key.value;
            } else {
                string val = substitute(key.value, "\"", "\\\"");
                if (key.keyName == "list" && !contains(val, '[')) {
                    val = "\"\"" + substitute(val, "|", "|\\\n    ") + "\"\"";
                    os << key.keyName << " = "
                       << "\"" << val << "\"";
                } else if (key.keyName == "list") {
                    val = substitute(val, "\\\"", "\"");
                    val = substitute(val, "[", "[\n   ");
                    val = substitute(val, "]", "\n]");
                    val = substitute(val, "},", "},\n    ");
                    val = substitute(val, ":", "=");
                    val = substitute(val, " \n", "\n");
                    val = substitute(val, "\n\n", "\n");
                    os << key.keyName << " = " << val;
                } else {
                    os << key.keyName << " = "
                       << "\"" << val << "\"";
                }
            }
            os << (key.deleted ? " (deleted)" : "");
            os << endl;
        }
        first = false;
    }
    return os;
}

//-------------------------------------------------------------------------
CToml::CTomlKey::CTomlKey() : comment(false) {
}

//-------------------------------------------------------------------------
CToml::CTomlKey::CTomlKey(const CTomlKey& key)
    : keyName(key.keyName), value(key.value), comment(key.comment), deleted(key.deleted) {
}

//-------------------------------------------------------------------------
CToml::CTomlKey& CToml::CTomlKey::operator=(const CTomlKey& key) {
    keyName = key.keyName;
    value = key.value;
    comment = key.comment;
    deleted = key.deleted;
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
    isComment = false;
    isArray = false;
    keys.clear();
}

//-------------------------------------------------------------------------
void CToml::CTomlSection::copy(const CTomlSection& section) {
    clear();

    sectionName = section.sectionName;
    isComment = section.isComment;
    isArray = section.isArray;
    keys.clear();
    for (auto key : section.keys)
        keys.push_back(key);
}

//---------------------------------------------------------------------------------------
void CToml::CTomlSection::addKey(const string_q& keyName, const string_q& val, bool commented) {
    string_q str = substitute(val, "\"\"\"", "");
    if (endsWith(str, '\"'))
        replaceReverse(str, "\"", "");
    if (startsWith(str, '\"'))
        replace(str, "\"", "");
    str = substitute(str, "\\\"", "\"");  // unescape
    str = substitute(str, "\\#", "#");    // unescape
    CTomlKey key(keyName, str, commented);
    key.deleted = contains(val, "(deleted)");
    keys.push_back(key);
    return;
}

//---------------------------------------------------------------------------------------
void CToml::deleteKey(const string_q& section, const string_q& key) {
    CToml::CTomlKey* kPtr = findKey(section, key);
    if (kPtr)
        kPtr->deleted = true;
}

//---------------------------------------------------------------------------------------
string_q stripFullLineComments(const string_q& inStr) {
    string_q str = inStr;
    string_q ret;
    while (!str.empty()) {
        string_q line = trimWhitespace(nextTokenClear(str, '\n'));
        if (contains(line, '=') || contains(line, '[') || (line.length() && line[0] != '#'))
            ret += (line + "\n");
    }
    return ret;
}

//---------------------------------------------------------------------------------------
string_q collapseArrays(const string_q& inStr) {
    if (!contains(inStr, "[[") && !contains(inStr, "]]"))
        return inStr;

    string_q ret;
    string_q str = substitute(inStr, "  ", " ");
    replace(str, "[[", "`");
    string_q front = nextTokenClear(str, '`');
    str = "[[" + str;
    str = substitute(str, "[[", "<array>");
    str = substitute(str, "]]", "</array>\n<name>");
    str = substitute(str, "[", "</name>\n<value>");
    str = substitute(str, "]", "</value>\n<name>");
    replaceReverse(str, "<name>", "");
    replaceAll(str, "<name>\n", "<name>");
    replaceAll(str, " = </name>", "</name>");
    while (contains(str, "</array>")) {
        string_q array = snagFieldClear(str, "array");
        string_q vals;
        while (contains(str, "</value>")) {
            string_q name = substitute(substitute(snagFieldClear(str, "name"), "=", ""), "\n", "");
            string_q value = substitute(substitute(snagFieldClear(str, "value"), "\n", ""), "=", ":");
            vals += name + " = [ " + value + " ]\n";
        }
        string_q line = "[[" + array + "]]\n" + vals;
        ret += line;
    }
    return front + ret;
}
}  // namespace qblocks
