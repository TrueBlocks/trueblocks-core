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
#include "ownerchangedevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QOwnerChangedEvent, CLogEntry);

//---------------------------------------------------------------------------
static SFString nextOwnerchangedeventChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextOwnerchangedeventChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QOwnerChangedEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextOwnerchangedeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextOwnerchangedeventChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QOwnerChangedEvent *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QOwnerChangedEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "newOwner" ) { newOwner = toAddress(fieldValue); return true; }
            break;
        case 'o':
            if ( fieldName % "oldOwner" ) { oldOwner = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QOwnerChangedEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QOwnerChangedEvent::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QOwnerChangedEvent*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> oldOwner;
    archive >> newOwner;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QOwnerChangedEvent::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    archive << oldOwner;
    archive << newOwner;

    return true;
}

//---------------------------------------------------------------------------
void QOwnerChangedEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QOwnerChangedEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QOwnerChangedEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QOwnerChangedEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QOwnerChangedEvent, "oldOwner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QOwnerChangedEvent, "newOwner", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QOwnerChangedEvent, "schema");
    HIDE_FIELD(QOwnerChangedEvent, "deleted");
    HIDE_FIELD(QOwnerChangedEvent, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextOwnerchangedeventChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QOwnerChangedEvent *own = (const QOwnerChangedEvent *)dataPtr;
    if (own) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, own);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QOwnerChangedEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QOwnerChangedEvent::readBackLevel(SFArchive& archive) {

    CLogEntry::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QOwnerChangedEvent::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextOwnerchangedeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "newOwner" ) return fromAddress(newOwner);
            break;
        case 'o':
            if ( fieldName % "oldOwner" ) return fromAddress(oldOwner);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QOwnerChangedEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

