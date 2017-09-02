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
IMPLEMENT_NODE(QOwnerChangedEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextOwnerchangedeventChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextOwnerchangedeventChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QOwnerChangedEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextOwnerchangedeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextOwnerchangedeventChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QOwnerChangedEvent *own = (const QOwnerChangedEvent *)data;
    if (own) {
        // Give customized code a chance to override first
        SFString ret = nextOwnerchangedeventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'n':
                if ( fieldIn % "newOwner" ) return fromAddress(own->newOwner);
                break;
            case 'o':
                if ( fieldIn % "oldOwner" ) return fromAddress(own->oldOwner);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, own);
        if (!ret.empty())
            return ret;
    }

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

    CLogEntry::Serialize(archive);

    archive >> oldOwner;
    archive >> newOwner;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QOwnerChangedEvent::SerializeC(SFArchive& archive) const {
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
    ADD_FIELD(QOwnerChangedEvent, "oldOwner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QOwnerChangedEvent, "newOwner", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QOwnerChangedEvent, "schema");
    HIDE_FIELD(QOwnerChangedEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextOwnerchangedeventChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QOwnerChangedEvent *own = (const QOwnerChangedEvent *)data;
    if (own) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, own);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QOwnerChangedEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QOwnerChangedEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

