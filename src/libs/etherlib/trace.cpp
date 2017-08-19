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
#include "trace.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTrace, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextTraceChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextTraceChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CTrace::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextTraceChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTraceChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CTrace *tra = (const CTrace *)data;
    if (tra) {
        // Give customized code a chance to override first
        SFString ret = nextTraceChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'a':
                if ( fieldIn % "action" ) { expContext().noFrst=true; return tra->action.Format(); }
                break;
            case 'b':
                if ( fieldIn % "blockHash" ) return fromHash(tra->blockHash);
                if ( fieldIn % "blockNumber" ) return asStringU(tra->blockNumber);
                break;
            case 'e':
                if ( fieldIn % "error" ) return tra->error;
                break;
            case 'r':
                if ( fieldIn % "result" ) { expContext().noFrst=true; return tra->result.Format(); }
                break;
            case 's':
                if ( fieldIn % "subtraces" ) return asStringU(tra->subtraces);
                break;
            case 't':
                if ( fieldIn % "traceAddress" ) {
                    uint32_t cnt = tra->traceAddress.getCount();
                    if (!cnt) return "";
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        ret += indent() + ("\"" + tra->traceAddress[i] + "\"");
                        ret += ((i < cnt-1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                if ( fieldIn % "transactionHash" ) return fromHash(tra->transactionHash);
                if ( fieldIn % "transactionPosition" ) return asStringU(tra->transactionPosition);
                if ( fieldIn % "type" ) return tra->type;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, tra);
        if (!ret.empty())
            return ret;
    }

    SFString s;
    s = toUpper(SFString("action")) + "::";
    if (fieldIn.Contains(s)) {
        SFString f = fieldIn;
        f.ReplaceAll(s,"");
        f = tra->action.Format("[{"+f+"}]");
        return f;
    }
    s = toUpper(SFString("result")) + "::";
    if (fieldIn.Contains(s)) {
        SFString f = fieldIn;
        f.ReplaceAll(s,"");
        f = tra->result.Format("[{"+f+"}]");
        return f;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTrace::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    if (fieldName % "action") {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        action.parseJson(p,nFields);
        return true;

    } else if (fieldName % "result") {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        result.parseJson(p,nFields);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "action" ) { /* action = fieldValue; */ return false; }
            break;
        case 'b':
            if ( fieldName % "blockHash" ) { blockHash = toHash(fieldValue); return true; }
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "error" ) { error = fieldValue; return true; }
            break;
        case 'r':
            if ( fieldName % "result" ) { /* result = fieldValue; */ return false; }
            break;
        case 's':
            if ( fieldName % "subtraces" ) { subtraces = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "traceAddress" ) return true;
            if ( fieldName % "transactionHash" ) { transactionHash = toHash(fieldValue); return true; }
            if ( fieldName % "transactionPosition" ) { transactionPosition = toUnsigned(fieldValue); return true; }
            if ( fieldName % "type" ) { type = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTrace::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTrace::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CTrace*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> blockHash;
    archive >> blockNumber;
    archive >> subtraces;
    archive >> traceAddress;
    archive >> transactionHash;
    archive >> transactionPosition;
    archive >> type;
    archive >> error;
    archive >> action;
    archive >> result;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTrace::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << blockHash;
    archive << blockNumber;
    archive << subtraces;
    archive << traceAddress;
    archive << transactionHash;
    archive << transactionPosition;
    archive << type;
    archive << error;
    archive << action;
    archive << result;

    return true;
}

//---------------------------------------------------------------------------
void CTrace::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTrace, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTrace, "blockHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTrace, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "subtraces", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "traceAddress", T_ADDRESS|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CTrace, "transactionHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTrace, "transactionPosition", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrace, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrace, "error", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrace, "action", T_OBJECT, ++fieldNum);
    ADD_FIELD(CTrace, "result", T_OBJECT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTrace, "schema");
    HIDE_FIELD(CTrace, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTraceChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CTrace *tra = (const CTrace *)data;
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

    return "";
}

//---------------------------------------------------------------------------
bool CTrace::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTrace::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CTrace::isError(void) const {
    return !error.empty();
}
// EXISTING_CODE
}  // namespace qblocks

