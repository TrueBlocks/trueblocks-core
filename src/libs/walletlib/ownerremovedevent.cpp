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
#include "ownerremovedevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QOwnerRemovedEvent, CLogEntry, curVersion);

//---------------------------------------------------------------------------
static SFString nextOwnerremovedeventChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextOwnerremovedeventChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QOwnerRemovedEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextOwnerremovedeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextOwnerremovedeventChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QOwnerRemovedEvent *own = (const QOwnerRemovedEvent *)data;
    if (own) {
        // Give customized code a chance to override first
        SFString ret = nextOwnerremovedeventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
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
bool QOwnerRemovedEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'o':
            if ( fieldName % "oldOwner" ) { oldOwner = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QOwnerRemovedEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QOwnerRemovedEvent::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const QOwnerRemovedEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> oldOwner;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QOwnerRemovedEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << oldOwner;

    return true;
}

//---------------------------------------------------------------------------
void QOwnerRemovedEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QOwnerRemovedEvent, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(QOwnerRemovedEvent, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(QOwnerRemovedEvent, "oldOwner", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QOwnerRemovedEvent, "schema");
    HIDE_FIELD(QOwnerRemovedEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextOwnerremovedeventChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QOwnerRemovedEvent *own = (const QOwnerRemovedEvent *)data;
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
bool QOwnerRemovedEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QOwnerRemovedEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

