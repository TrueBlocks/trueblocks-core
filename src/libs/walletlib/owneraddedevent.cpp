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
#include "owneraddedevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QOwnerAddedEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextOwneraddedeventChunk(const SFString& fieldIn, const void *data);
static SFString nextOwneraddedeventChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void QOwnerAddedEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextOwneraddedeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextOwneraddedeventChunk(const SFString& fieldIn, const void *data) {
    const QOwnerAddedEvent *own = (const QOwnerAddedEvent *)data;
    if (own) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = own->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextOwneraddedeventChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'n':
                if ( fieldIn % "newOwner" ) return fromAddress(own->newOwner);
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, own);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QOwnerAddedEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "newOwner" ) { newOwner = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QOwnerAddedEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QOwnerAddedEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QOwnerAddedEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> newOwner;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QOwnerAddedEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << newOwner;

    return true;
}

//---------------------------------------------------------------------------
void QOwnerAddedEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QOwnerAddedEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QOwnerAddedEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QOwnerAddedEvent, "newOwner", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QOwnerAddedEvent, "schema");
    HIDE_FIELD(QOwnerAddedEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextOwneraddedeventChunk_custom(const SFString& fieldIn, const void *data) {
    const QOwnerAddedEvent *own = (const QOwnerAddedEvent *)data;
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
bool QOwnerAddedEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QOwnerAddedEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QOwnerAddedEvent::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextOwneraddedeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "newOwner" ) return fromAddress(newOwner);
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

