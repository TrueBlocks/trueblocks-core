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
#include "namevalue.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNameValue, CBaseNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextNamevalueChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextNamevalueChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CNameValue::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextNamevalueChunk, this);
}

//---------------------------------------------------------------------------
SFString nextNamevalueChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CNameValue *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNameValue::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNameValue::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNameValue::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CNameValue*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> name;
    archive >> value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNameValue::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << name;
    archive << value;

    return true;
}

//---------------------------------------------------------------------------
void CNameValue::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CNameValue, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNameValue, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNameValue, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CNameValue, "value", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNameValue, "schema");
    HIDE_FIELD(CNameValue, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextNamevalueChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CNameValue *nam = (const CNameValue *)dataPtr;
    if (nam) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, nam);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CNameValue::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CNameValue::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CNameValue::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextNamevalueChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "name" ) return name;
            break;
        case 'v':
            if ( fieldName % "value" ) return value;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNameValue& item) {
    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

