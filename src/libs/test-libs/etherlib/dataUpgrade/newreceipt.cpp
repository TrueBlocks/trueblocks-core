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
#include "newreceipt.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewReceipt, CBaseNode);

//---------------------------------------------------------------------------
extern SFString nextNewreceiptChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextNewreceiptChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CNewReceipt::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextNewreceiptChunk, this);
}

//---------------------------------------------------------------------------
SFString nextNewreceiptChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CNewReceipt *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "contractAddress" ) { contractAddress = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) { gasUsed = toGas(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "isError" ) { isError = toBool(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "logs" ) {
                char *p = (char *)fieldValue.c_str();
                while (p && *p) {
                    CLogEntry item;
                    uint32_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        logs[logs.getCount()] = item;
                }
                return true;
            }
            if ( fieldName % "logsBloom" ) { logsBloom = toBloom(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNewReceipt::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CNewReceipt*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> contractAddress;
    archive >> gasUsed;
    archive >> logs;
    archive >> logsBloom;
    archive >> isError;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    ((CNewReceipt*)this)->m_schema = getVersionNum();
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << contractAddress;
    archive << gasUsed;
    archive << logs;
    archive << logsBloom;
    archive << isError;

    return true;
}

//---------------------------------------------------------------------------
void CNewReceipt::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CNewReceipt, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewReceipt, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewReceipt, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewReceipt, "contractAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewReceipt, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CNewReceipt, "logs", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CNewReceipt, "logsBloom", T_BLOOM, ++fieldNum);
    ADD_FIELD(CNewReceipt, "isError", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewReceipt, "schema");
    HIDE_FIELD(CNewReceipt, "deleted");
    HIDE_FIELD(CNewReceipt, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextNewreceiptChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CNewReceipt *newp = (const CNewReceipt *)dataPtr;
    if (newp) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, newp);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CNewReceipt::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CNewReceipt::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CNewReceipt& newp) {
    newp.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CNewReceipt& newp) {
    newp.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CNewReceipt::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextNewreceiptChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "contractAddress" ) return fromAddress(contractAddress);
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) return fromGas(gasUsed);
            break;
        case 'i':
            if ( fieldName % "isError" ) return asString(isError);
            break;
        case 'l':
            if ( fieldName % "logs" || fieldName % "logsCnt" ) {
                uint32_t cnt = logs.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += logs[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if ( fieldName % "logsBloom" ) return bloom2Bytes(logsBloom);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNewReceipt& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CNewReceipt::getObjectAt(const SFString& fieldName, uint32_t index) const {
    if ( fieldName % "logs" && index < logs.getCount() )
        return &logs[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

