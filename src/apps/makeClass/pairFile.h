#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

//-------------------------------------------------------------------------
class CToml1 {
public:
    static SFString fields[50];
    static uint32_t nFields;
    static void loadFields(uint32_t nItems, const SFString *items);

    SFString values[50];
    bool readFile(const SFString& filename);
    uint32_t getFieldID(const SFString& fieldName) const;
    const SFString getConfigStr(const SFString& unused, const SFString& fieldName, const SFString& def) const;
    bool getConfigBool(const SFString& group, const SFString& key, bool def) const;
    bool isDisabled(void) const { return getConfigStr("", "disabled", "") == "true"; }
};

//-------------------------------------------------------------------------
extern ostream& operator<<(ostream& os, const CToml1& t);

//-------------------------------------------------------------------------
inline bool CToml1::getConfigBool(const SFString& group, const SFString& key, bool def) const {
    SFString ret = getConfigStr(group, key, asString(def?1:0));
    ret.ReplaceAny(";\t\n\r ", "");
    return ((ret == "true" || ret == "1") ? true : false);
}
