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
#include "isowner.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QIsOwner, CTransaction);

//---------------------------------------------------------------------------
static SFString nextIsownerChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextIsownerChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QIsOwner::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextIsownerChunk, this);
}

//---------------------------------------------------------------------------
SFString nextIsownerChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QIsOwner *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QIsOwner::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_addr" ) { _addr = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QIsOwner::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QIsOwner::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QIsOwner*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _addr;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QIsOwner::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    archive << _addr;

    return true;
}

//---------------------------------------------------------------------------
void QIsOwner::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QIsOwner, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QIsOwner, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QIsOwner, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QIsOwner, "_addr", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QIsOwner, "schema");
    HIDE_FIELD(QIsOwner, "deleted");
    HIDE_FIELD(QIsOwner, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextIsownerChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QIsOwner *iso = (const QIsOwner *)dataPtr;
    if (iso) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, iso);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QIsOwner::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QIsOwner::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QIsOwner::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextIsownerChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_addr" ) return fromAddress(_addr);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QIsOwner& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

