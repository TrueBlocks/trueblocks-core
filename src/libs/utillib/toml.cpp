/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"

#include "toml.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    CToml::CToml(void) {
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
    CToml::CTomlGroup *CToml::addGroup(const SFString& group, bool commented) {
        ASSERT(!findGroup(group));
        CTomlGroup *newGroup = new CTomlGroup;
        newGroup->comment = commented;
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
        SFString ret = getConfigStr(group, key, asString(def));
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
        setConfigStr(group, key, asString(value));
    }

    //-------------------------------------------------------------------------
    void CToml::setConfigBool(const SFString& group, const SFString& key, bool value) {
        setConfigStr(group, key, asString(value));
    }

    //---------------------------------------------------------------------------------------
    bool CToml::readFile(const SFString& filename) {
        // TODO(tjayrush): This is a very inadequate TOML parser. Should be replaced with full featured parser
        SFString curGroup;
        Clear();

        SFString contents = asciiFileToString(filename).Substitute("\\\n ", "\\\n")
                            .Substitute("\\\n", "").Substitute("\\\r\n", "");
        while (!contents.empty()) {
            SFString value = StripAny(nextTokenClear(contents, '\n'), " \t");
            bool comment = value.startsWith('#');
            if (comment)
                value = value.substr(1);
            if (!value.empty()) {
                if (value.startsWith('[')) {  // it's a group
                    value = StripAny(value.Substitute("[", "").Substitute("]", ""), " \t");
                    addGroup(value, comment);
                    curGroup = value;

                } else {
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

            SFString str = (first?EMPTY:"\n") + (group->comment?"#":EMPTY) + "[" + group->groupName + "]\n";
            LISTPOS kPos = group->keys.GetHeadPosition();
            while (kPos) {
                CTomlKey *key = group->keys.GetNext(kPos);
                str += (key->comment?"#":EMPTY) + key->keyName + "=" + key->value + "\n";
            }
            WriteLine(str);
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
    void CToml::setConfigStr(const SFString& group, const SFString& keyIn, const SFString& value) {
        bool comment = keyIn.startsWith('#');
        SFString key = (comment ? keyIn.substr(1) : keyIn);

        CTomlGroup *grp = findGroup(group);
        if (!grp) {
            addGroup(group, false);
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
            os << "[" << grp->groupName << (grp->comment?":comment ":"") << "]\n";
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
        comment = false;
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
        comment = group.comment;

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

}  // namespace qblocks
