/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "makeClass.h"
#include "pairFile.h"

//------------------------------------------------------------------------------------------------------------
bool CToml1::readFile(const SFString& dataFile) {
    SFString contents = Strip(asciiFileToString(dataFile), '\n').Substitute("\t", "").Substitute("\r", "");
    while (!contents.empty()) {
        SFString value = nextTokenClear(contents, '\n');
        SFString fieldName = nextTokenClear(value, ':');
        if (!fieldName.startsWith(';'))
            values[getFieldID(fieldName)] = value;
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
SFString CToml1::fields[50];
uint32_t CToml1::nFields = 0;

//------------------------------------------------------------------------------------------------------------
void CToml1::loadFields(uint32_t nItems, const SFString *items) {
    for (int i = 0 ; i < nItems ; i++)
        fields[i] = items[i];
    nFields = nItems;
}

//------------------------------------------------------------------------------------------------------------
uint32_t CToml1::getFieldID(const SFString& fieldName) const {
    for (int i = 0 ; i < CToml1::nFields ; i++)
        if (CToml1::fields[i] % fieldName)
            return i;
    cout << "unknown field: " << fieldName << "....quiting\n";
    exit(0);
}

//------------------------------------------------------------------------------------------------------------
const SFString CToml1::getConfigStr(const SFString& unused, const SFString& fieldName, const SFString& def) const {
    SFString ret = values[getFieldID(fieldName)];
    if (ret == "")
        ret = def;
    return ret;
}

//------------------------------------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CToml1& t)
{
    for (int i = 0 ; i < t.nFields ; i++) {
        SFString fieldName = "  " + t.fields[i].Right(23) + ":";
        if (fieldName.length() < 23)
            fieldName += SFString(' ', 23-fieldName.length());
        fieldName += " [VALUE]\n";
        os << convertTypes(fieldName.Substitute("[VALUE]", t.values[i]).Substitute("|", "\n\t\t\t"));
    }
    return os;
}
