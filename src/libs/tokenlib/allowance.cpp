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
#include "allowance.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QAllowance, CTransaction);

//---------------------------------------------------------------------------
static SFString nextAllowanceChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAllowanceChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QAllowance::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAllowanceChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAllowanceChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QAllowance *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QAllowance::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_owner" ) { _owner = toAddress(fieldValue); return true; }
            if ( fieldName % "_spender" ) { _spender = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QAllowance::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QAllowance::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QAllowance*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _owner;
    archive >> _spender;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QAllowance::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _owner;
    archive << _spender;

    return true;
}

//---------------------------------------------------------------------------
void QAllowance::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QAllowance, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QAllowance, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QAllowance, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QAllowance, "_owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QAllowance, "_spender", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QAllowance, "schema");
    HIDE_FIELD(QAllowance, "deleted");
    HIDE_FIELD(QAllowance, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAllowanceChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QAllowance *all = (const QAllowance *)dataPtr;
    if (all) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, all);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QAllowance::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QAllowance::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QAllowance::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAllowanceChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_owner" ) return fromAddress(_owner);
            if ( fieldName % "_spender" ) return fromAddress(_spender);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QAllowance& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

