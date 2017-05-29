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
#include "traceaction.h"
#include "trace.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceAction, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextTraceactionChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextTraceactionChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CTraceAction::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextTraceactionChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTraceactionChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CTraceAction *tra = (const CTraceAction *)data;
    if (tra) {
        // Give customized code a chance to override first
        SFString ret = nextTraceactionChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'c':
                if ( fieldIn % "callType" ) return tra->callType;
                break;
            case 'f':
                if ( fieldIn % "from" ) return fromAddress(tra->from);
                break;
            case 'g':
                if ( fieldIn % "gas" ) return asStringU(tra->gas);
                break;
            case 'i':
                if ( fieldIn % "input" ) return tra->input;
                break;
            case 't':
                if ( fieldIn % "to" ) return fromAddress(tra->to);
                break;
            case 'v':
                if ( fieldIn % "value" ) return asStringBN(tra->value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, tra);
        if (!ret.empty())
            return ret;
    }

    return "Field not found: [{" + fieldIn + "}]\n";
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "callType" ) { callType = fieldValue; return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gas" ) { gas = toUnsigned(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "input" ) { input = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "to" ) { to = toAddress(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTraceAction::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CTraceAction*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> callType;
    archive >> from;
    archive >> gas;
    archive >> input;
    archive >> to;
    archive >> value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceAction::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << callType;
    archive << from;
    archive << gas;
    archive << input;
    archive << to;
    archive << value;

    return true;
}

//---------------------------------------------------------------------------
void CTraceAction::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTraceAction, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CTraceAction, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CTraceAction, "callType", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "from", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "gas", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceAction, "input", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "to", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceAction, "value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceAction, "schema");
    HIDE_FIELD(CTraceAction, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTraceactionChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CTraceAction *tra = (const CTraceAction *)data;
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, tra);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CTraceAction::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTraceAction::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CTraceAction& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CTraceAction& tra) {
    tra.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

