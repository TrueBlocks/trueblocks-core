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
IMPLEMENT_NODE(QRequirementChangedEvent, CLogEntry);

//---------------------------------------------------------------------------
static SFString nextRequirementchangedeventChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextRequirementchangedeventChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QRequirementChangedEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextRequirementchangedeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextRequirementchangedeventChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QRequirementChangedEvent *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

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
SFString nextRequirementchangedeventChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QRequirementChangedEvent *req = (const QRequirementChangedEvent *)dataPtr;
    if (req) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, req);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QRequirementChangedEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
SFString QRequirementChangedEvent::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextRequirementchangedeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "newRequirement" ) return asStringBN(newRequirement);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QRequirementChangedEvent& item) {
    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

