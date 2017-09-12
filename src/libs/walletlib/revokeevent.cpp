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
#include "revokeevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QRevokeEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextRevokeeventChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextRevokeeventChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QRevokeEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextRevokeeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextRevokeeventChunk(const SFString& fieldIn, const void *dataPtr) {
    const QRevokeEvent *rev = (const QRevokeEvent *)dataPtr;
    if (rev) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = rev->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextRevokeeventChunk_custom(fieldIn, dataPtr);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'o':
                if ( fieldIn % "owner" ) return fromAddress(rev->owner);
                if ( fieldIn % "operation" ) return rev->operation;
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, rev);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QRevokeEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
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
void QRevokeEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QRevokeEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QRevokeEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> owner;
    archive >> operation;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QRevokeEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << owner;
    archive << operation;

    return true;
}

//---------------------------------------------------------------------------
void QRevokeEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QRevokeEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QRevokeEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevokeEvent, "owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QRevokeEvent, "operation", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QRevokeEvent, "schema");
    HIDE_FIELD(QRevokeEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextRevokeeventChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QRevokeEvent *rev = (const QRevokeEvent *)dataPtr;
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
bool QRevokeEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QRevokeEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QRevokeEvent::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextRevokeeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case 'o':
            if ( fieldName % "owner" ) return fromAddress(owner);
            if ( fieldName % "operation" ) return operation;
            break;
    }
    return "";
#else
    return Format("[{"+toUpper(fieldName)+"}]");
#endif
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

