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
IMPLEMENT_NODE(CTraceResult, CBaseNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextTraceresultChunk(const SFString& fieldIn, const void *data);
static SFString nextTraceresultChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void CTraceResult::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTraceresultChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTraceresultChunk(const SFString& fieldIn, const void *data) {
    const CTraceResult *tra = (const CTraceResult *)data;
    if (tra) {
        // Give customized code a chance to override first
        SFString ret = nextTraceresultChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'g':
                if ( fieldIn % "gasUsed" ) return asStringU(tra->gasUsed);
                break;
            case 'o':
                if ( fieldIn % "output" ) return tra->output;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, tra);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTraceResult::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'g':
            if ( fieldName % "gasUsed" ) { gasUsed = toUnsigned(fieldValue); return true; }
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

    if (!preSerialize(archive))
        return false;

    archive >> gasUsed;
    archive >> output;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceResult::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

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
    ADD_FIELD(CTraceResult, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CTraceResult, "output", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceResult, "schema");
    HIDE_FIELD(CTraceResult, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTraceresultChunk_custom(const SFString& fieldIn, const void *data) {
    const CTraceResult *tra = (const CTraceResult *)data;
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
bool CTraceResult::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTraceResult::readBackLevel(SFArchive& archive) {
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
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

