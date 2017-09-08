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
#include "depositevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QDepositEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextDepositeventChunk(const SFString& fieldIn, const void *data);
static SFString nextDepositeventChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void QDepositEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextDepositeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextDepositeventChunk(const SFString& fieldIn, const void *data) {
    const QDepositEvent *dep = (const QDepositEvent *)data;
    if (dep) {
        // Give customized code a chance to override first
        SFString ret = nextDepositeventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'f':
                if ( fieldIn % "from" ) return fromAddress(dep->from);
                break;
            case 'v':
                if ( fieldIn % "value" ) return asStringBN(dep->value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, dep);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QDepositEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'f':
            if ( fieldName % "from" ) { from = toAddress(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QDepositEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QDepositEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QDepositEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> from;
    archive >> value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QDepositEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << from;
    archive << value;

    return true;
}

//---------------------------------------------------------------------------
void QDepositEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QDepositEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QDepositEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QDepositEvent, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QDepositEvent, "value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QDepositEvent, "schema");
    HIDE_FIELD(QDepositEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextDepositeventChunk_custom(const SFString& fieldIn, const void *data) {
    const QDepositEvent *dep = (const QDepositEvent *)data;
    if (dep) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, dep);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QDepositEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QDepositEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

