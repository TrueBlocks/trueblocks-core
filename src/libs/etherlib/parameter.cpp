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
IMPLEMENT_NODE(CParameter, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextParameterChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextParameterChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CParameter::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextParameterChunk, this);
}

//---------------------------------------------------------------------------
SFString nextParameterChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CParameter *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

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

    if (archive.isWriting())
        return ((const CParameter*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

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

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);
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
SFString nextParameterChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CParameter *par = (const CParameter *)dataPtr;
    if (par) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, par);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CParameter::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CParameter::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CParameter::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextParameterChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "indexed" ) return asString(indexed);
            if ( fieldName % "isPointer" ) return asString(isPointer);
            if ( fieldName % "isArray" ) return asString(isArray);
            if ( fieldName % "isObject" ) return asString(isObject);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            break;
        case 's':
            if ( fieldName % "strDefault" ) return strDefault;
            break;
        case 't':
            if ( fieldName % "type" ) return type;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CParameter& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

