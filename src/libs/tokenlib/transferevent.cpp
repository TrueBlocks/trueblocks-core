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
#include "transferevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QTransferEvent, CLogEntry, dataSchema());

//---------------------------------------------------------------------------
static SFString nextTransfereventChunk(const SFString& fieldIn, const void *data);
static SFString nextTransfereventChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void QTransferEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextTransfereventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTransfereventChunk(const SFString& fieldIn, const void *data) {
    const QTransferEvent *tra = (const QTransferEvent *)data;
    if (tra) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = tra->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextTransfereventChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case '_':
                if ( fieldIn % "_from" ) return fromAddress(tra->_from);
                if ( fieldIn % "_to" ) return fromAddress(tra->_to);
                if ( fieldIn % "_value" ) return asStringBN(tra->_value);
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, tra);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QTransferEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) { _from = toAddress(fieldValue); return true; }
            if ( fieldName % "_to" ) { _to = toAddress(fieldValue); return true; }
            if ( fieldName % "_value" ) { _value = toUnsigned(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QTransferEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QTransferEvent::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QTransferEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> _from;
    archive >> _to;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QTransferEvent::SerializeC(SFArchive& archive) const {
    CLogEntry::SerializeC(archive);

    archive << _from;
    archive << _to;
    archive << _value;

    return true;
}

//---------------------------------------------------------------------------
void QTransferEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QTransferEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QTransferEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QTransferEvent, "_from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QTransferEvent, "_to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QTransferEvent, "_value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QTransferEvent, "schema");
    HIDE_FIELD(QTransferEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTransfereventChunk_custom(const SFString& fieldIn, const void *data) {
    const QTransferEvent *tra = (const QTransferEvent *)data;
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QTransferEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QTransferEvent::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QTransferEvent::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextTransfereventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) return fromAddress(_from);
            if ( fieldName % "_to" ) return fromAddress(_to);
            if ( fieldName % "_value" ) return asStringBN(_value);
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

