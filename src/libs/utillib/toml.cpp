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
    CToml::CToml(const SFString& fileName) : CSharedResource() {
        setFilename(fileName);
        if (!fileName.empty())
            readFile(fileName);
    }

    //-------------------------------------------------------------------------
    CToml::~CToml(void) {
        Clear();
    }

    //-------------------------------------------------------------------------
    void CToml::Clear(void) {
        LISTPOS gPos = groups.GetHeadPosition();
        while (gPos) {
            CTomlGroup *group = groups.GetNext(gPos);
            delete group;
        }
        groups.RemoveAll();
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup *CToml::addGroup(const SFString& group, bool commented, bool array) {
        ASSERT(!findGroup(group));
        CTomlGroup *newGroup = new CTomlGroup;
        newGroup->isArray = array;
        newGroup->isComment = commented;
        newGroup->groupName = group;
        groups.AddTail(newGroup);
        return newGroup;
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup *CToml::findGroup(const SFString& group) const {
        LISTPOS gPos = groups.GetHeadPosition();
        while (gPos) {
            CTomlGroup *grp = groups.GetNext(gPos);
            if (grp->groupName == group)
                return grp;
        }
        return NULL;
    }

    //-------------------------------------------------------------------------
    CToml::CTomlKey *CToml::addKey(const SFString& group, const SFString& key, const SFString& val, bool commented) {
        CTomlGroup *grp = findGroup(group);
        ASSERT(grp);
        return grp->addKey(key, val, commented);
    }

    //-------------------------------------------------------------------------
    CToml::CTomlKey *CToml::findKey(const SFString& group, const SFString& key) const {
        CTomlGroup *grp = findGroup(group);
        if (grp)
            return grp->findKey(key);
        return NULL;
    }

    //-------------------------------------------------------------------------
    uint64_t CToml::getConfigInt(const SFString& group, const SFString& key, uint64_t def) const {
        SFString ret = getConfigStr(group, key, asStringU(def));
        return toLong32u(ret);
    }

    //-------------------------------------------------------------------------
    bool CToml::getConfigBool(const SFString& group, const SFString& key, bool def) const {
        SFString ret = getConfigStr(group, key, asString(def?1:0));
        ret.ReplaceAny(";\t\n\r ", "");
        return ((ret == "true" || ret == "1") ? true : false);
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigInt(const SFString& group, const SFString& key, uint64_t value) {
        setConfigStr(group, key, asString((int64_t)value));
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigBool(const SFString& group, const SFString& key, bool value) {
        setConfigStr(group, key, asString(value));
    }

    //---------------------------------------------------------------------------------------
extern SFString stripFullLineComments(const SFString& inStr);
extern SFString collapseArrays(const SFString& inStr);
    bool CToml::readFile(const SFString& filename) {
        SFString curGroup;
        Clear();

        SFString contents = asciiFileToString(filename)
                            .Substitute("\\\n ", "\\\n").Substitute("\\\n", "").Substitute("\\\r\n", "");
        contents = collapseArrays(stripFullLineComments(contents));
        while (!contents.empty()) {
            SFString value = StripAny(nextTokenClear(contents, '\n'), " \t");
            bool comment = value.startsWith('#');
            if (comment)
                value = value.substr(1);
            if (!value.empty()) {
                bool isArray = value.Contains("[[");
                if (value.startsWith('[')) {  // it's a group
                    value = StripAny(value.Substitute("[", "").Substitute("]", ""), " \t");
                    addGroup(value, comment, isArray);
                    curGroup = value;

                } else {
                    if (curGroup.empty()) {
                        cerr << "key: " << value << " found outside of a controlling group. Quitting...\n";
                        cerr.flush();
                        exit(0);
                    }
                    SFString key = nextTokenClear(value, '=');  // value may be empty, but not whitespace
                    key = StripAny(key, " \t");
                    value = StripAny(value, " \t");
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
        LISTPOS gPos = groups.GetHeadPosition();
        while (gPos) {
            CTomlGroup *group = groups.GetNext(gPos);
            ASSERT(group);

            ostringstream os;
            os << (first?"":"\n") << (group->isComment?"#":"");
            os << (group->isArray?"[[":"[") << group->groupName << (group->isArray?"]]":"]") << "\n";
            LISTPOS kPos = group->keys.GetHeadPosition();
            while (kPos) {
                CTomlKey *key = group->keys.GetNext(kPos);
                os << (key->comment?"#":EMPTY) << key->keyName << "=" << key->value << "\n";
            }
            WriteLine(os.str().c_str());
            first = false;
        }
        Release();
        return true;
    }

    //---------------------------------------------------------------------------------------
    void CToml::mergeFile(CToml *tomlIn) {
        LISTPOS gPos = tomlIn->groups.GetHeadPosition();
        while (gPos) {
            CTomlGroup *group = tomlIn->groups.GetNext(gPos);
            LISTPOS kPos = group->keys.GetHeadPosition();
            while (kPos) {
                CTomlKey *key = group->keys.GetNext(kPos);
                setConfigStr(group->groupName, key->keyName, key->value);
            }
        }
    }

    SFUintBN CToml::getConfigBigInt(const SFString& group, const SFString& key, SFUintBN def) const {
        SFString ret = getConfigStr(group, key, to_string(def).c_str());
        SFString check = ret;
        check.ReplaceAny("0123456789abcdefABCDEF", "");
        if (!check.empty()) {
            cerr << "Big int config item " << group << "::" << key << " is not an integer...returning zero.";
            return 0;
        }
        return str2BigUint(ret);
    }

    //---------------------------------------------------------------------------------------
    SFString CToml::getConfigStr(const SFString& group, const SFString& key, const SFString& def) const {
        SFString ret = def;
        CTomlGroup *grp = findGroup(group);
        if (grp) {
            CTomlKey *k = grp->findKey(key);
            if (k && !k->comment)
                ret = k->value;
        }
        return ret;
    }

    //-------------------------------------------------------------------------
    SFString CToml::getDisplayStr(bool terse, const SFString& def, const SFString& color) const {
        SFString fmt = getConfigStr("display", (terse ? "terse" : "format"), "<not_set>");
        if (fmt == "<not_set>")
            fmt = def;
        if (!color.empty()) {
            fmt.ReplaceAll("{", color+"{");
            fmt.ReplaceAll("}", "}"+cOff);
        }
        return cleanFmt(fmt);
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigStr(const SFString& group, const SFString& keyIn, const SFString& value) {
        bool comment = keyIn.startsWith('#');
        SFString key = (comment ? keyIn.substr(1) : keyIn);

        CTomlGroup *grp = findGroup(group);
        if (!grp) {
            addGroup(group, false, false);
            addKey(group, key, value, comment);

        } else {
            CTomlKey *k = grp->findKey(key);
            if (k) {
                // last in wins
                k->comment = comment;
                k->value = value;
            } else {
                addKey(group, key, value, comment);
            }
        }
    }

    //-------------------------------------------------------------------------
    ostream& operator<<(ostream& os, const CToml& tomlIn) {
        LISTPOS gPos = tomlIn.groups.GetHeadPosition();
        while (gPos) {
            const CToml::CTomlGroup *grp = tomlIn.groups.GetNext(gPos);
            os << (grp->isArray?"[[":"[") << grp->groupName << (grp->isComment?":comment ":"") << (grp->isArray?"]]":"]") << "\n";
            LISTPOS kPos = grp->keys.GetHeadPosition();
            while (kPos) {
                const CToml::CTomlKey *key = grp->keys.GetNext(kPos);
                os << "\t" << key->keyName << (key->comment?":comment":"") << "=" << key->value << "\n";;
            }
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
        Clear();
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup::CTomlGroup(const CTomlGroup& group) {
        Copy(group);
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup::~CTomlGroup(void) {
        Clear();
    }

    //-------------------------------------------------------------------------
    CToml::CTomlGroup& CToml::CTomlGroup::operator=(const CTomlGroup& group) {
        Copy(group);
        return *this;
    }

    //-------------------------------------------------------------------------
    void CToml::CTomlGroup::Clear(void) {
        groupName = EMPTY;
        isComment = false;
        isArray = false;
        LISTPOS kPos = keys.GetHeadPosition();
        while (kPos) {
            CTomlKey *key = keys.GetNext(kPos);
            delete key;
        }
        keys.RemoveAll();
    }

    //-------------------------------------------------------------------------
    void CToml::CTomlGroup::Copy(const CTomlGroup& group) {
        Clear();

        groupName = group.groupName;
        isComment = group.isComment;
        isArray   = group.isArray;

        LISTPOS kPos = group.keys.GetHeadPosition();
        while (kPos) {
            CTomlKey *orig = group.keys.GetNext(kPos);
            CTomlKey *copy = new CTomlKey(*orig);
            keys.AddTail(copy);
        }
    }

    //---------------------------------------------------------------------------------------
    CToml::CTomlKey *CToml::CTomlGroup::addKey(const SFString& keyName, const SFString& val, bool commented) {
        CTomlKey *key = new CTomlKey;
        key->comment = commented;
        key->keyName = keyName;
        key->value = val;
        keys.AddTail(key);
        return key;
    }

    //---------------------------------------------------------------------------------------
    CToml::CTomlKey *CToml::CTomlGroup::findKey(const SFString& keyName) const {
        LISTPOS kPos = keys.GetHeadPosition();
        while (kPos) {
            CTomlKey *key = keys.GetNext(kPos);
            SFString name = key->keyName;
            if (name == keyName)
                return key;
        }
        return NULL;
    }

    //---------------------------------------------------------------------------------------
    SFString stripFullLineComments(const SFString& inStr) {
        SFString str = inStr;
        SFString ret;
        while (!str.empty()) {
            SFString line = StripAny(nextTokenClear(str, '\n'), " \t\n\r");
            if (line.length() && line[0] != '#') {
                ret += (line + "\n");
            }
        }
        return ret;
    }

    //---------------------------------------------------------------------------------------
    SFString collapseArrays(const SFString& inStr) {
        if (!inStr.Contains("[[") && !inStr.Contains("]]"))
            return inStr;

        SFString ret;
        SFString str = inStr.Substitute("  "," ");
        str.Replace("[[","`");
        SFString front = nextTokenClear(str, '`');
        str = "[[" + str;
        str = str.Substitute("[[","<array>");
        str = str.Substitute("]]","</array>\n<name>");
        str = str.Substitute("[","</name>\n<value>");
        str = str.Substitute("]","</value>\n<name>");
        str.ReplaceReverse("<name>","");
        str.ReplaceAll("<name>\n","<name>");
        str.ReplaceAll(" = </name>","</name>");
        while (str.Contains("</array>")) {
            SFString array = snagFieldClear(str,"array");
            SFString vals;
            while (str.Contains("</value>")) {
                SFString name = snagFieldClear(str,"name").Substitute("=","").Substitute("\n","");
                SFString value = snagFieldClear(str,"value").Substitute("\n","").Substitute("=",":");
                vals += name + " = [ " + value + " ]\n";
            }
            SFString line = "[[" + array + "]]\n" + vals;
            ret += line;
        }
        return front + ret;
    }
}  // namespace qblocks
