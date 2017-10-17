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
IMPLEMENT_NODE(QSingleTransactEvent, CLogEntry);

//---------------------------------------------------------------------------
static SFString nextSingletransacteventChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextSingletransacteventChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QSingleTransactEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextSingletransacteventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextSingletransacteventChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QSingleTransactEvent *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

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

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> owner;
    archive >> value;
    archive >> to;
    archive >> data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QSingleTransactEvent::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
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
    ADD_FIELD(QSingleTransactEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "value", T_NUMBER, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QSingleTransactEvent, "data", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QSingleTransactEvent, "schema");
    HIDE_FIELD(QSingleTransactEvent, "deleted");
    HIDE_FIELD(QSingleTransactEvent, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextSingletransacteventChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QSingleTransactEvent *sin = (const QSingleTransactEvent *)dataPtr;
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
bool QSingleTransactEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QSingleTransactEvent::readBackLevel(SFArchive& archive) {

    CLogEntry::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QSingleTransactEvent::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextSingletransacteventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'd':
            if ( fieldName % "data" ) return data;
            break;
        case 'o':
            if ( fieldName % "owner" ) return fromAddress(owner);
            break;
        case 't':
            if ( fieldName % "to" ) return fromAddress(to);
            break;
        case 'v':
            if ( fieldName % "value" ) return asStringBN(value);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QSingleTransactEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

