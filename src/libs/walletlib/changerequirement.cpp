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
#include "changerequirement.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QChangeRequirement, CTransaction);

//---------------------------------------------------------------------------
static SFString nextChangerequirementChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextChangerequirementChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QChangeRequirement::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextChangerequirementChunk, this);
}

//---------------------------------------------------------------------------
SFString nextChangerequirementChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QChangeRequirement *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newRequired" ) { _newRequired = toWei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QChangeRequirement::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QChangeRequirement*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _newRequired;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    archive << _newRequired;

    return true;
}

//---------------------------------------------------------------------------
void QChangeRequirement::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QChangeRequirement, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QChangeRequirement, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QChangeRequirement, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QChangeRequirement, "_newRequired", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QChangeRequirement, "schema");
    HIDE_FIELD(QChangeRequirement, "deleted");
    HIDE_FIELD(QChangeRequirement, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextChangerequirementChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QChangeRequirement *cha = (const QChangeRequirement *)dataPtr;
    if (cha) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, cha);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QChangeRequirement::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QChangeRequirement::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QChangeRequirement::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextChangerequirementChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newRequired" ) return asStringBN(_newRequired);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QChangeRequirement& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

