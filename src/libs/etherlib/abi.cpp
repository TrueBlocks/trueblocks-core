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
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode);

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
    if (dataPtr)
        return ((const CAbi *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAbi::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiByName" ) {
                char *p = (char *)fieldValue.c_str();
                while (p && *p) {
                    CFunction item;
                    uint32_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        abiByName[abiByName.getCount()] = item;
                }
                return true;
            }
            if ( fieldName % "abiByEncoding" ) {
                char *p = (char *)fieldValue.c_str();
                while (p && *p) {
                    CFunction item;
                    uint32_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        abiByEncoding[abiByEncoding.getCount()] = item;
                }
                return true;
            }
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

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> abiByName;
    archive >> abiByEncoding;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);
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
    ADD_FIELD(CAbi, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "abiByName", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CAbi, "abiByEncoding", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbi, "schema");
    HIDE_FIELD(CAbi, "deleted");
    HIDE_FIELD(CAbi, "showing");

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

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CAbi::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAbiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiByName" || fieldName % "abiByNameCnt" ) {
                uint32_t cnt = abiByName.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += abiByName[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if ( fieldName % "abiByEncoding" || fieldName % "abiByEncodingCnt" ) {
                uint32_t cnt = abiByEncoding.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
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

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAbi& item) {
    // EXISTING_CODE
    for (uint32_t i = 0 ; i < item.abiByName.getCount() ; i++ ) {
        os << item.abiByName[i].Format() << "\n";
    }
    for (uint32_t i = 0 ; i < item.abiByEncoding.getCount() ; i++ ) {
        os << item.abiByEncoding[i].Format() << "\n";
    }
    { return os; }
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAbi::getObjectAt(const SFString& name, uint32_t i) const {
    if ( name % "abiByName" && i < abiByName.getCount() )
        return &abiByName[i];
    if ( name % "abiByEncoding" && i < abiByEncoding.getCount() )
        return &abiByEncoding[i];
    return NULL;
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
        SFString contents = asciiFileToString(blockCachePath("abis/" + addr + ".abi"));
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

    SFString abiFilename = blockCachePath("abis/" + addr + ".json");
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

        if (!fileExists(blockCachePath("abis/" + addr + ".abi")) && !abis1.empty())
            stringToAsciiFile(blockCachePath("abis/" + addr + ".abi"), abis1);

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
// EXISTING_CODE
}  // namespace qblocks

