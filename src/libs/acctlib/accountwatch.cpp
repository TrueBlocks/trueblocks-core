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
#include "accountwatch.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountWatch, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextAccountwatchChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAccountwatchChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAccountWatch::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAccountwatchChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAccountwatchChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CAccountWatch *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "qbis") {
        char *p = (char *)fieldValue.c_str();
        uint32_t nFields = 0;
        qbis.parseJson(p, nFields);
        return true;
    }
    if (fieldName % "balance") {
        qbis.endBal = qbis.begBal = toWei(fieldValue);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = toAddress(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "color" ) { color = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "deepScan" ) { deepScan = toBool(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) { firstBlock = toUnsigned(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "id" ) { id = toLong32u(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "lastBlock" ) { lastBlock = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            if ( fieldName % "nodeBal" ) { nodeBal = toWei(fieldValue); return true; }
            break;
        case 'q':
            if ( fieldName % "qbis" ) { /* qbis = fieldValue; */ return false; }
            break;
        case 's':
            if ( fieldName % "status" ) { status = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountWatch::finishParse() {
    // EXISTING_CODE
    bloom = makeBloom(address);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CAccountWatch*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> id;
    archive >> address;
    archive >> name;
    archive >> color;
    archive >> firstBlock;
    archive >> lastBlock;
    archive >> status;
    archive >> deepScan;
    archive >> qbis;
    archive >> nodeBal;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << id;
    archive << address;
    archive << name;
    archive << color;
    archive << firstBlock;
    archive << lastBlock;
    archive << status;
    archive << deepScan;
    archive << qbis;
    archive << nodeBal;

    return true;
}

//---------------------------------------------------------------------------
void CAccountWatch::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAccountWatch, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "id", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAccountWatch, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "color", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "firstBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "lastBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "status", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deepScan", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "qbis", T_OBJECT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "nodeBal", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountWatch, "schema");
    HIDE_FIELD(CAccountWatch, "deleted");
    HIDE_FIELD(CAccountWatch, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAccountwatchChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CAccountWatch *acc = (const CAccountWatch *)dataPtr;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAccountWatch::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAccountWatch::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CAccountWatch::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAccountwatchChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return fromAddress(address);
            break;
        case 'c':
            if ( fieldName % "color" ) return color;
            break;
        case 'd':
            if ( fieldName % "deepScan" ) return asString(deepScan);
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) return asStringU(firstBlock);
            break;
        case 'i':
            if ( fieldName % "id" ) return asStringU(id);
            break;
        case 'l':
            if ( fieldName % "lastBlock" ) return asStringU(lastBlock);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            if ( fieldName % "nodeBal" ) return fromWei(nodeBal);
            break;
        case 'q':
            if ( fieldName % "qbis" ) { expContext().noFrst=true; return qbis.Format(); }
            break;
        case 's':
            if ( fieldName % "status" ) return status;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    SFString s;
    s = toUpper(SFString("qbis")) + "::";
    if (fieldName.Contains(s)) {
        SFString f = fieldName;
        f.ReplaceAll(s,"");
        f = qbis.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountWatch& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAccountWatch::getObjectAt(const SFString& fieldName, uint32_t index) const {
    if ( fieldName % "qbis" )
        return &qbis;
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
SFString CAccountWatch::displayName(bool useColor, bool terse, uint32_t w1, uint32_t w2) const {
    if (address == "others") {
        return padRight(name, w1 + w2 + 1);
    }

    if (terse) {
        uint64_t len = name.length();
        uint64_t need = 42 - len - 6; // " (" and "...)"
        return (useColor ? color : "") + name.substr(0,42-6) + " (" + address.substr(0,need) + "...)" + (useColor ? cOff : "");
    }

    return padRight(name.substr(0,w1),w1) + " " + address.substr(0,w2) + " ";
}

//-----------------------------------------------------------------------
SFUintBN getNodeBal(CBalanceHistoryArray& history, const SFAddress& addr, blknum_t blockNum) {

    if (fileExists("./cache/balances.txt")) {
        if (history.getCount() == 0) { // do not collapse
            SFString contents = asciiFileToString("./cache/balances.txt");
            while (!contents.empty()) {
                SFString line = nextTokenClear(contents, '\n');
                SFString bn = nextTokenClear(line, '\t');
                SFString addr1 = nextTokenClear(line, '\t');
                SFString bal = nextTokenClear(line, '\t');
                if (addr1 == addr) {
                    uint32_t cnt = history.getCount();
                    history[cnt].bn      = toUnsigned(bn);
                    history[cnt].balance = toWei(bal);
                }
            }
        }

        SFUintBN ret = 0;
        for (uint32_t i = 0 ; i < history.getCount() ; i++) {
            if (history[i].bn > blockNum) {
                return ret;
            }
            ret = history[i].balance;
        }
        if (ret > 0) {
            return ret;
        }
    }
    return getBalance(addr, blockNum, false);
}
// EXISTING_CODE
}  // namespace qblocks

