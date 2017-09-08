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
#include "singletransactevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QSingleTransactEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextSingletransacteventChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextSingletransacteventChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QSingleTransactEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextSingletransacteventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextSingletransacteventChunk(const SFString& fieldIn, bool& force, const void *data) {
    const QSingleTransactEvent *sin = (const QSingleTransactEvent *)data;
    if (sin) {
        // Give customized code a chance to override first
        SFString ret = nextSingletransacteventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'd':
                if ( fieldIn % "data" ) return sin->data;
                break;
            case 'o':
                if ( fieldIn % "owner" ) return fromAddress(sin->owner);
                break;
            case 't':
                if ( fieldIn % "to" ) return fromAddress(sin->to);
                break;
            case 'v':
                if ( fieldIn % "value" ) return asStringBN(sin->value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, sin);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QSingleTransactEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
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
void QSingleTransactEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QSingleTransactEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QSingleTransactEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> owner;
    archive >> value;
    archive >> to;
    archive >> data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QSingleTransactEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << owner;
    archive << value;
    archive << to;
    archive << data;

    return true;
}

//---------------------------------------------------------------------------
void QSingleTransactEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QSingleTransactEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "value", T_NUMBER, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "data", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QSingleTransactEvent, "schema");
    HIDE_FIELD(QSingleTransactEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextSingletransacteventChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const QSingleTransactEvent *sin = (const QSingleTransactEvent *)data;
    if (sin) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, sin);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QSingleTransactEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QSingleTransactEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

