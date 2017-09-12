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
#include "transferfrom.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QTransferFrom, CTransaction, dataSchema());

//---------------------------------------------------------------------------
static SFString nextTransferfromChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTransferfromChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QTransferFrom::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTransferfromChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTransferfromChunk(const SFString& fieldIn, const void *dataPtr) {
    const QTransferFrom *tra = (const QTransferFrom *)dataPtr;
    if (tra) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = tra->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextTransferfromChunk_custom(fieldIn, dataPtr);
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
        ret = nextTransactionChunk(fieldIn, tra);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QTransferFrom::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
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
void QTransferFrom::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QTransferFrom::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QTransferFrom*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _from;
    archive >> _to;
    archive >> _value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QTransferFrom::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _from;
    archive << _to;
    archive << _value;

    return true;
}

//---------------------------------------------------------------------------
void QTransferFrom::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QTransferFrom, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QTransferFrom, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QTransferFrom, "_from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QTransferFrom, "_to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QTransferFrom, "_value", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QTransferFrom, "schema");
    HIDE_FIELD(QTransferFrom, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTransferfromChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QTransferFrom *tra = (const QTransferFrom *)dataPtr;
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
bool QTransferFrom::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QTransferFrom::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QTransferFrom::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextTransferfromChunk_custom(fieldName, this);
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

