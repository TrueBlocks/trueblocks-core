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
#include "multitransactevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QMultiTransactEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextMultitransacteventChunk(const SFString& fieldIn, const void *data);
static SFString nextMultitransacteventChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void QMultiTransactEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextMultitransacteventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextMultitransacteventChunk(const SFString& fieldIn, const void *data) {
    const QMultiTransactEvent *mul = (const QMultiTransactEvent *)data;
    if (mul) {
        // Give customized code a chance to override first
        SFString ret = nextMultitransacteventChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'd':
                if ( fieldIn % "data" ) return mul->data;
                break;
            case 'o':
                if ( fieldIn % "owner" ) return fromAddress(mul->owner);
                if ( fieldIn % "operation" ) return mul->operation;
                break;
            case 't':
                if ( fieldIn % "to" ) return fromAddress(mul->to);
                break;
            case 'v':
                if ( fieldIn % "value" ) return asStringBN(mul->value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, mul);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QMultiTransactEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'd':
            if ( fieldName % "data" ) { data = toLower(fieldValue); return true; }
            break;
        case 'o':
            if ( fieldName % "owner" ) { owner = toAddress(fieldValue); return true; }
            if ( fieldName % "operation" ) { operation = toLower(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "to" ) { to = toAddress(fieldValue); return true; }
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
void QMultiTransactEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QMultiTransactEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QMultiTransactEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> owner;
    archive >> operation;
    archive >> value;
    archive >> to;
    archive >> data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QMultiTransactEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << owner;
    archive << operation;
    archive << value;
    archive << to;
    archive << data;

    return true;
}

//---------------------------------------------------------------------------
void QMultiTransactEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QMultiTransactEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QMultiTransactEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QMultiTransactEvent, "owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QMultiTransactEvent, "operation", T_TEXT, ++fieldNum);
    ADD_FIELD(QMultiTransactEvent, "value", T_NUMBER, ++fieldNum);
    ADD_FIELD(QMultiTransactEvent, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QMultiTransactEvent, "data", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QMultiTransactEvent, "schema");
    HIDE_FIELD(QMultiTransactEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextMultitransacteventChunk_custom(const SFString& fieldIn, const void *data) {
    const QMultiTransactEvent *mul = (const QMultiTransactEvent *)data;
    if (mul) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, mul);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QMultiTransactEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QMultiTransactEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

