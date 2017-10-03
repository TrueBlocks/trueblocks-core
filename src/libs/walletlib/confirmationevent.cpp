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
#include "confirmationevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QConfirmationEvent, CLogEntry);

//---------------------------------------------------------------------------
static SFString nextConfirmationeventChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextConfirmationeventChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QConfirmationEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextConfirmationeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextConfirmationeventChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QConfirmationEvent *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QConfirmationEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'o':
            if ( fieldName % "owner" ) { owner = toAddress(fieldValue); return true; }
            if ( fieldName % "operation" ) { operation = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QConfirmationEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QConfirmationEvent::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QConfirmationEvent*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> owner;
    archive >> operation;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QConfirmationEvent::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    archive << owner;
    archive << operation;

    return true;
}

//---------------------------------------------------------------------------
void QConfirmationEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QConfirmationEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QConfirmationEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QConfirmationEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QConfirmationEvent, "owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QConfirmationEvent, "operation", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QConfirmationEvent, "schema");
    HIDE_FIELD(QConfirmationEvent, "deleted");
    HIDE_FIELD(QConfirmationEvent, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextConfirmationeventChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QConfirmationEvent *con = (const QConfirmationEvent *)dataPtr;
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
bool QConfirmationEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QConfirmationEvent::readBackLevel(SFArchive& archive) {

    CLogEntry::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QConfirmationEvent::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextConfirmationeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'o':
            if ( fieldName % "owner" ) return fromAddress(owner);
            if ( fieldName % "operation" ) return operation;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QConfirmationEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

