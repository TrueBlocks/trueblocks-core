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
#include "revoke.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QRevoke, CTransaction);

//---------------------------------------------------------------------------
static SFString nextRevokeChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextRevokeChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QRevoke::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextRevokeChunk, this);
}

//---------------------------------------------------------------------------
SFString nextRevokeChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QRevoke *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QRevoke::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_operation" ) { _operation = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QRevoke::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QRevoke::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QRevoke*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _operation;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QRevoke::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    archive << _operation;

    return true;
}

//---------------------------------------------------------------------------
void QRevoke::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QRevoke, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QRevoke, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevoke, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevoke, "_operation", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QRevoke, "schema");
    HIDE_FIELD(QRevoke, "deleted");
    HIDE_FIELD(QRevoke, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextRevokeChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QRevoke *rev = (const QRevoke *)dataPtr;
    if (rev) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rev);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QRevoke::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QRevoke::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QRevoke::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextRevokeChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_operation" ) return _operation;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QRevoke& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

