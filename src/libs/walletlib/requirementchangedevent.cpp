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
#include "requirementchangedevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QRequirementChangedEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextRequirementchangedeventChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextRequirementchangedeventChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QRequirementChangedEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextRequirementchangedeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextRequirementchangedeventChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QRequirementChangedEvent *req = (const QRequirementChangedEvent *)data;
    if (req) {
        // Give customized code a chance to override first
        SFString ret = nextRequirementchangedeventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'n':
                if ( fieldIn % "newRequirement" ) return asStringBN(req->newRequirement);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, req);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QRequirementChangedEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "newRequirement" ) { newRequirement = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QRequirementChangedEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QRequirementChangedEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QRequirementChangedEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> newRequirement;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QRequirementChangedEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << newRequirement;

    return true;
}

//---------------------------------------------------------------------------
void QRequirementChangedEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QRequirementChangedEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QRequirementChangedEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRequirementChangedEvent, "newRequirement", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QRequirementChangedEvent, "schema");
    HIDE_FIELD(QRequirementChangedEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextRequirementchangedeventChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QRequirementChangedEvent *req = (const QRequirementChangedEvent *)data;
    if (req) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, req);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QRequirementChangedEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QRequirementChangedEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

