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
#include "parameter.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CParameter, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextParameterChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextParameterChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CParameter::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextParameterChunk, this);
}

//---------------------------------------------------------------------------
SFString nextParameterChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CParameter *par = (const CParameter *)data;
    if (par) {
        // Give customized code a chance to override first
        SFString ret = nextParameterChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'i':
                if ( fieldIn % "indexed" ) return asString(par->indexed);
                if ( fieldIn % "isPointer" ) return asString(par->isPointer);
                if ( fieldIn % "isArray" ) return asString(par->isArray);
                if ( fieldIn % "isObject" ) return asString(par->isObject);
                break;
            case 'n':
                if ( fieldIn % "name" ) return par->name;
                break;
            case 's':
                if ( fieldIn % "strDefault" ) return par->strDefault;
                break;
            case 't':
                if ( fieldIn % "type" ) return par->type;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, par);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CParameter::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "indexed" ) { indexed = toBool(fieldValue); return true; }
            if ( fieldName % "isPointer" ) { isPointer = toBool(fieldValue); return true; }
            if ( fieldName % "isArray" ) { isArray = toBool(fieldValue); return true; }
            if ( fieldName % "isObject" ) { isObject = toBool(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 's':
            if ( fieldName % "strDefault" ) { strDefault = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "type" ) { type = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CParameter::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CParameter::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CParameter*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> indexed;
    archive >> name;
    archive >> type;
    archive >> isPointer;
    archive >> isArray;
    archive >> isObject;
    archive >> strDefault;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CParameter::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << indexed;
    archive << name;
    archive << type;
    archive << isPointer;
    archive << isArray;
    archive << isObject;
    archive << strDefault;

    return true;
}

//---------------------------------------------------------------------------
void CParameter::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CParameter, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CParameter, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CParameter, "indexed", T_BOOL, ++fieldNum);
    ADD_FIELD(CParameter, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CParameter, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CParameter, "isPointer", T_BOOL, ++fieldNum);
    ADD_FIELD(CParameter, "isArray", T_BOOL, ++fieldNum);
    ADD_FIELD(CParameter, "isObject", T_BOOL, ++fieldNum);
    ADD_FIELD(CParameter, "strDefault", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CParameter, "schema");
    HIDE_FIELD(CParameter, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextParameterChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CParameter *par = (const CParameter *)data;
    if (par) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, par);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CParameter::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CParameter::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

