/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "abi.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextAbiChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAbiChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAbi::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAbiChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAbiChunk(const SFString& fieldIn, const void *dataPtr) {
    const CAbi *abi = (const CAbi *)dataPtr;
    if (abi) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = abi->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextAbiChunk_custom(fieldIn, dataPtr);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'a':
                if ( fieldIn % "abiByName" ) {
                    uint32_t cnt = abi->abiByName.getCount();
                    if (!cnt) return "";
                    SFString retS;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        retS += abi->abiByName[i].Format();
                        retS += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return retS;
                }
                if ( fieldIn % "abiByEncoding" ) {
                    uint32_t cnt = abi->abiByEncoding.getCount();
                    if (!cnt) return "";
                    SFString retS;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        retS += abi->abiByEncoding[i].Format();
                        retS += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return retS;
                }
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, abi);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAbi::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiByName" ) return true;
            if ( fieldName % "abiByEncoding" ) return true;
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAbi::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAbi::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CAbi*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> abiByName;
    archive >> abiByEncoding;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << abiByName;
    archive << abiByEncoding;

    return true;
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAbi, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbi, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "abiByName", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CAbi, "abiByEncoding", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbi, "schema");
    HIDE_FIELD(CAbi, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAbiChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CAbi *abi = (const CAbi *)dataPtr;
    if (abi) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, abi);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAbi::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAbi::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CAbi::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextAbiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiByName" ) {
                uint32_t cnt = abiByName.getCount();
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += abiByName[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if ( fieldName % "abiByEncoding" ) {
                uint32_t cnt = abiByEncoding.getCount();
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += abiByEncoding[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }
    return "";
#else
    return Format("[{"+toUpper(fieldName)+"}]");
#endif
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
int sortFuncTableByName(const void *ob1, const void *ob2) {
    CFunction *f1 = (CFunction*)ob1;
    CFunction *f2 = (CFunction*)ob2;
    return f2->name.compare(f1->name);
}

//---------------------------------------------------------------------------
int findByName(const void *rr1, const void *rr2) {
    CFunction *f1 = (CFunction*)rr1;
    CFunction *f2 = (CFunction*)rr2;
    return f2->name.compare(f1->name);
}

//---------------------------------------------------------------------------
int cleanCompare(const SFString& s1, const SFString& s2) {
    SFString ss1 = (s1.startsWith("0x") ? s1.substr(2,8) : s1.substr(0,8));
    SFString ss2 = (s2.startsWith("0x") ? s2.substr(2,8) : s2.substr(0,8));
    return ss2.compare(ss1);
}

//---------------------------------------------------------------------------
int sortFuncTableByEncoding(const void *ob1, const void *ob2) {
    CFunction *f1 = (CFunction*)ob1;
    CFunction *f2 = (CFunction*)ob2;
    return cleanCompare(f1->encoding, f2->encoding);
}

//---------------------------------------------------------------------------
int findByEncoding(const void *rr1, const void *rr2) {
    CFunction *f1 = (CFunction*)rr1;
    CFunction *f2 = (CFunction*)rr2;
    return cleanCompare(f1->encoding, f2->encoding);
}

//---------------------------------------------------------------------------
CFunction *CAbi::findFunctionByName(const SFString& name) {
    CFunction search;
    search.name = name;
    return abiByName.Find(&search, findByName);
}

//---------------------------------------------------------------------------
CFunction *CAbi::findFunctionByEncoding(const SFString& enc) {
    CFunction search;
    search.encoding = enc;
    return abiByEncoding.Find(&search, findByEncoding);
}

//---------------------------------------------------------------------------
void CAbi::clearABI(void) {
    abiByName.Clear();
    abiByEncoding.Clear();
}

//---------------------------------------------------------------------------
SFString abis[1000][2];
uint32_t nAbis = 0;

//---------------------------------------------------------------------------
void clearAbis(void) {
    nAbis = 0;
}

//---------------------------------------------------------------------------
SFString findEncoding(const SFString& addr, CFunction& func) {
    if (!nAbis) {
        SFString contents = asciiFileToString(configPath("abis/" + addr + ".abi"));
        while (!contents.empty()) {
            abis[nAbis][1] = nextTokenClear(contents, '\n');
            abis[nAbis][0] = nextTokenClear(abis[nAbis][1], '|');
            nAbis++;
        }
    }

    for (uint64_t i = 0 ; i < nAbis ; i++)
        if (abis[i][0] == func.name)
            return abis[i][1];
    return EMPTY;
}

//---------------------------------------------------------------------------
static bool getEncoding(const SFString& abiFilename, const SFString& addr, CFunction& func) {
    if (func.type != "function")
        return false;

    SFString fullName = func.name;  // we need the signature for ethabi
    func.name     = nextTokenClear(func.name, '(');  // Cleanup because we only need the name, not the signature
    func.encoding = findEncoding(addr, func);
    if (func.encoding.empty() && fileExists("/usr/local/bin/ethabi")) {
        // When we call ethabi, we want the full function declaration (if it's present)
        SFString cmd = "/usr/local/bin/ethabi encode function \"" +
                            abiFilename + "\" " + fullName.Substitute("(", "\\(").Substitute(")", "\\)");
        func.encoding = doCommand(cmd);
    }
    return !func.encoding.empty();
}

//---------------------------------------------------------------------------
bool CAbi::loadABIFromFile(const SFString& fileName) {

    SFString contents = asciiFileToString(fileName);
    ASSERT(!contents.empty());
    char *p = cleanUpJson((char *)contents.c_str());
    while (p && *p) {
        CFunction func;
        uint32_t nFields = 0;
        p = func.parseJson(p, nFields);
        if (nFields) {
            abiByName[abiByName.getCount()] = func;
            abiByEncoding[abiByEncoding.getCount()] = func;
        }
    }
    abiByName.Sort(sortFuncTableByName);
    abiByEncoding.Sort(sortFuncTableByEncoding);
    return abiByName.getCount();
}

//---------------------------------------------------------------------------
bool CAbi::loadABI(const SFString& addr) {
    // Already loaded?
    if (abiByName.getCount() && abiByEncoding.getCount())
        return true;

    SFString abiFilename = configPath("abis/" + addr + ".json");
    if (!fileExists(abiFilename))
        return false;

    cerr << "\tLoading abi file: " << abiFilename << "...\n";
    if (loadABIFromFile(abiFilename)) {

        SFString abis1;

        // TODO(tjayrush): this is wrong. We should remove the need to use external 'ethabi' code to get the encodings
        for (uint32_t i=0;i<abiByName.getCount();i++) {
            getEncoding(abiFilename, addr, abiByName[i]);
            abis1 += abiByName[i].Format("[{NAME}]|[{ENCODING}]\n");
        }

        // We need to do both since they are copies
        for (uint32_t i=0;i<abiByEncoding.getCount();i++) {
            getEncoding(abiFilename, addr, abiByEncoding[i]);
        }

        if (!fileExists(configPath("abis/" + addr + ".abi")) && !abis1.empty())
            stringToAsciiFile(configPath("abis/" + addr + ".abi"), abis1);

        if (verbose) {
            for (uint32_t i = 0 ; i < abiByName.getCount() ; i++) {
                CFunction *f = &abiByName[i];
                if (f->type == "function")
                    cerr << f->Format("[\"{NAME}|][{ENCODING}\"]").Substitute("\n", " ") << "\n";
            }
        }
    }
    return abiByName.getCount();
}

//---------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAbi& abi) {
    for (uint32_t i = 0 ; i < abi.abiByName.getCount() ; i++ ) {
        os << abi.abiByName[i].Format() << "\n";
    }
    for (uint32_t i = 0 ; i < abi.abiByEncoding.getCount() ; i++ ) {
        os << abi.abiByEncoding[i].Format() << "\n";
    }
    return os;
}
// EXISTING_CODE
}  // namespace qblocks

