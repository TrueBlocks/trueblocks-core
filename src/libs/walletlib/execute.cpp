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
#include "execute.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QExecute, CTransaction, dataSchema());

//---------------------------------------------------------------------------
static SFString nextExecuteChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextExecuteChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QExecute::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextExecuteChunk, this);
}

//---------------------------------------------------------------------------
SFString nextExecuteChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QExecute *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QExecute::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_to" ) { _to = toAddress(fieldValue); return true; }
            if ( fieldName % "_value" ) { _value = toUnsigned(fieldValue); return true; }
            if ( fieldName % "_data" ) { _data = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QExecute::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QExecute::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const QExecute*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _to;
    archive >> _value;
    archive >> _data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QExecute::SerializeC(SFArchive& archive) const {
    CTransaction::SerializeC(archive);

    archive << _to;
    archive << _value;
    archive << _data;

    return true;
}

//---------------------------------------------------------------------------
void QExecute::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QExecute, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QExecute, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QExecute, "_to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QExecute, "_value", T_NUMBER, ++fieldNum);
    ADD_FIELD(QExecute, "_data", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QExecute, "schema");
    HIDE_FIELD(QExecute, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextExecuteChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QExecute *exe = (const QExecute *)dataPtr;
    if (exe) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, exe);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QExecute::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QExecute::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QExecute::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextExecuteChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_to" ) return fromAddress(_to);
            if ( fieldName % "_value" ) return asStringBN(_value);
            if ( fieldName % "_data" ) return _data;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

