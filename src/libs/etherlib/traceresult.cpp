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
#include "traceresult.h"
#include "trace.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceResult, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextTraceresultChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTraceresultChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTraceResult::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTraceresultChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTraceresultChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTraceResult *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTraceResult::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = toAddress(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "code" ) { code = fieldValue; return true; }
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) { gasUsed = toGas(fieldValue); return true; }
            break;
        case 'o':
            if ( fieldName % "output" ) { output = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTraceResult::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTraceResult::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CTraceResult*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> code;
    archive >> gasUsed;
    archive >> output;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceResult::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << code;
    archive << gasUsed;
    archive << output;

    return true;
}

//---------------------------------------------------------------------------
void CTraceResult::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTraceResult, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceResult, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTraceResult, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTraceResult, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTraceResult, "code", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceResult, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CTraceResult, "output", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceResult, "schema");
    HIDE_FIELD(CTraceResult, "deleted");
    HIDE_FIELD(CTraceResult, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTraceresultChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CTraceResult *tra = (const CTraceResult *)dataPtr;
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTraceResult::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTraceResult::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CTraceResult& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CTraceResult& tra) {
    tra.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CTraceResult::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTraceresultChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return fromAddress(address);
            break;
        case 'c':
            if ( fieldName % "code" ) return code;
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) return fromGas(gasUsed);
            break;
        case 'o':
            if ( fieldName % "output" ) return output;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraceResult& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

