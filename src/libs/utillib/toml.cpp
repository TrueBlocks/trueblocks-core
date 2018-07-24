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

#include "toml.h"
#include "conversions.h"

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
    }

    //-------------------------------------------------------------------------
    void CToml::addGroup(const string_q& group, bool commented, bool array) {
        if (findGroup(group))
            return;
        CTomlGroup newGroup(group, array, commented);
        groups.push_back(newGroup);
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup *CToml::findGroup(const string_q& group) const {
        for (size_t i = 0 ; i < groups.size() ; i++) {
            if (groups[i].groupName == group) {
                return &((CToml*)this)->groups[i];  // NOLINT
                }
            }
        return NULL;
    }

    //-------------------------------------------------------------------------
    void CToml::addKey(const string_q& group, const string_q& key, const string_q& val, bool commented) {
        CTomlGroup *grp = findGroup(group);
        if (grp)
            grp->addKey(key, val, commented);
        return;
    }

    //-------------------------------------------------------------------------
    CToml::CTomlKey *CToml::findKey(const string_q& group, const string_q& keyIn) const {
        CTomlGroup *grp = findGroup(group);
        if (grp) {
            for (size_t i = 0 ; i < grp->keys.size() ; i++)
                if (grp->keys[i].keyName == keyIn)
                    return &grp->keys[i];
        }
        return NULL;
    }

    //-------------------------------------------------------------------------
    uint64_t CToml::getConfigInt(const string_q& group, const string_q& key, uint64_t def) const {
        string_q ret = getConfigStr(group, key, uint_2_Str(def));
        return str_2_Uint(ret);
    }

    //-------------------------------------------------------------------------
    bool CToml::getConfigBool(const string_q& group, const string_q& key, bool def) const {
        string_q ret = getConfigStr(group, key, int_2_Str(def?1:0));
        replaceAny(ret, ";\t\n\r ", "");
        return ((ret == "true" || ret == "1") ? true : false);
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigInt(const string_q& group, const string_q& key, uint64_t value) {
        setConfigStr(group, key, int_2_Str((int64_t)value));
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigBool(const string_q& group, const string_q& key, bool value) {
        setConfigStr(group, key, int_2_Str(value));
    }

    //---------------------------------------------------------------------------------------
extern string_q stripFullLineComments(const string_q& inStr);
extern string_q collapseArrays(const string_q& inStr);
    bool CToml::readFile(const string_q& filename) {
        string_q curGroup;
        clear();

        string_q contents = asciiFileToString(filename);
        replaceAll(contents, "\\\n ", "\\\n");  // if ends with '\' + '\n' + space, make it just '\' + '\n'
        replaceAll(contents, "\\\n", "");       // if ends with '\' + '\n', its a continuation, so fold in
        replaceAll(contents, "\\\r\n", "");     // same for \r\n
        contents = collapseArrays(stripFullLineComments(contents));
        while (!contents.empty()) {
            string_q value = trimWhitespace(nextTokenClear(contents, '\n'));
            bool comment = startsWith(value, '#');
            if (comment)
                value = extract(value, 1);
            if (!value.empty()) {
                bool isArray = contains(value, "[[");
                if (startsWith(value, '[')) {  // it's a group
                    value = trimWhitespace(substitute(substitute(value, "[", ""), "]", ""));
                    addGroup(value, comment, isArray);
                    curGroup = value;

                } else {
                    if (curGroup.empty()) {
                        cerr << "key: " << value << " found outside of a controlling group. Quitting...\n";
                        cerr.flush();
                        exit(0);
                    }
                    string_q key = nextTokenClear(value, '=');  // value may be empty, but not whitespace
                    key   = trimWhitespace(key);
                    value = trimWhitespace(value);
                    addKey(curGroup, key, value, comment);
                }
            }
        }
        return true;
    }

    //---------------------------------------------------------------------------------------
    bool CToml::writeFile(void) {
        if (!Lock(m_filename, asciiWriteCreate, LOCK_CREATE)) {
            LockFailure();
            return false;
        }

        bool first = true;
        for (auto group : groups) {
            ostringstream os;
            os << (first?"":"\n") << (group.isComment?"#":"");
            os << (group.isArray?"[[":"[") << group.groupName << (group.isArray?"]]":"]") << "\n";
            for (auto key : group.keys)
                os << (key.comment ? "#" : "") << key.keyName << "=" << key.value << "\n";
            WriteLine(os.str().c_str());
            first = false;
        }
        Release();
        return true;
    }

    //---------------------------------------------------------------------------------------
    void CToml::mergeFile(CToml *tomlIn) {
        for (auto group : tomlIn->groups) {
            for (auto key : group.keys)
                setConfigStr(group.groupName, key.keyName, key.value);
        }
    }

    //---------------------------------------------------------------------------------------
    biguint_t CToml::getConfigBigInt(const string_q& group, const string_q& key, biguint_t def) const {
        string_q ret = getConfigStr(group, key, bnu_2_Str(def));
        string_q check = ret;
        replaceAny(check, "0123456789abcdefABCDEF", "");
        if (!check.empty()) {
            cerr << "Big int config item " << group << "::" << key << " is not an integer...returning zero.";
            return 0;
        }
        return str_2_BigUint(ret);
    }

    //---------------------------------------------------------------------------------------
    string_q CToml::getConfigStr(const string_q& group, const string_q& key, const string_q& def) const {
        CTomlKey *found = findKey(group, key);
        if (found && !found->comment)
            return found->value;
        return def;
    }

    //-------------------------------------------------------------------------
    string_q CToml::getDisplayStr(bool terse, const string_q& def, const string_q& color) const {
        string_q fmt = getConfigStr("display", (terse ? "terse" : "format"), "<not_set>");
        if (fmt == "<not_set>")
            fmt = def;
        if (!color.empty()) {
            replaceAll(fmt, "{", color+"{");
            replaceAll(fmt, "}", "}"+cOff);
        }

        string_q ret = substitute(fmt, "\\n\\\n", "\\n");
        ret = substitute(ret, "\n", "");
        ret = substitute(ret, "\\n", "\n");
        ret = substitute(ret, "\\t", "\t");
        ret = substitute(ret, "\\r", "\r");
        return ret;
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigStr(const string_q& group, const string_q& keyIn, const string_q& value) {
        bool comment = startsWith(keyIn, '#');
        string_q key = (comment ? extract(keyIn, 1) : keyIn);

        CTomlGroup *grp = findGroup(group);
        if (!grp) {
            addGroup(group, false, false);
            addKey(group, key, value, comment);

        } else {
            CTomlKey *found = findKey(group, key);
            if (found) {
                found->comment = comment;
                found->value = value;
            } else {
                addKey(group, key, value, comment);
            }
        }
    }

    //-------------------------------------------------------------------------
    ostream& operator<<(ostream& os, const CToml& tomlIn) {
        for (auto group : tomlIn.groups) {
            os << (group.isArray?"[[":"[");
            os << group.groupName;
            os << (group.isComment?":comment ":"");
            os << (group.isArray?"]]":"]");
            os << "\n";
            for (auto key : group.keys)
                os << "\t" << key.keyName << (key.comment?":comment":"") << "=" << key.value << "\n";;
        }
        return os;
    }

    //-------------------------------------------------------------------------
    CToml::CTomlKey::CTomlKey() : comment(false) {
    }

    //-------------------------------------------------------------------------
    CToml::CTomlKey::CTomlKey(const CTomlKey& key) : keyName(key.keyName), value(key.value), comment(key.comment) {
    }

    //-------------------------------------------------------------------------
    CToml::CTomlKey& CToml::CTomlKey::operator=(const CTomlKey& key) {
        keyName = key.keyName;
        value = key.value;
        comment = key.comment;
        return *this;
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup::CTomlGroup(void) {
        clear();
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup::CTomlGroup(const CTomlGroup& group) {
        copy(group);
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup::~CTomlGroup(void) {
        clear();
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup& CToml::CTomlGroup::operator=(const CTomlGroup& group) {
        copy(group);
        return *this;
    }

    //-------------------------------------------------------------------------
    void CToml::CTomlGroup::clear(void) {
        groupName = "";
        isComment = false;
        isArray = false;
        keys.clear();
    }

    //-------------------------------------------------------------------------
    void CToml::CTomlGroup::copy(const CTomlGroup& group) {
        clear();

        groupName = group.groupName;
        isComment = group.isComment;
        isArray   = group.isArray;
        keys.clear();
        for (auto key : group.keys)
            keys.push_back(key);
    }

    //---------------------------------------------------------------------------------------
    void CToml::CTomlGroup::addKey(const string_q& keyName, const string_q& val, bool commented) {
        CTomlKey key(keyName, val, commented);
        keys.push_back(key);
        return;
    }

    //---------------------------------------------------------------------------------------
    string_q stripFullLineComments(const string_q& inStr) {
        string_q str = inStr;
        string_q ret;
        while (!str.empty()) {
            string_q line = trimWhitespace(nextTokenClear(str, '\n'));
            if (line.length() && line[0] != '#') {
                ret += (line + "\n");
            }
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
