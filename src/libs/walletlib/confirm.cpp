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
#include "confirm.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QConfirm, CTransaction);

//---------------------------------------------------------------------------
static SFString nextConfirmChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextConfirmChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QConfirm::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextConfirmChunk, this);
}

//---------------------------------------------------------------------------
SFString nextConfirmChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QConfirm *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QConfirm::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_h" ) { _h = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QConfirm::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QConfirm::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QConfirm*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _h;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QConfirm::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    archive << _h;

    return true;
}

//---------------------------------------------------------------------------
void QConfirm::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QConfirm, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QConfirm, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QConfirm, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QConfirm, "_h", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QConfirm, "schema");
    HIDE_FIELD(QConfirm, "deleted");
    HIDE_FIELD(QConfirm, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextConfirmChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QConfirm *con = (const QConfirm *)dataPtr;
    if (con) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, con);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QConfirm::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QConfirm::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QConfirm::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextConfirmChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_h" ) return _h;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QConfirm& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

