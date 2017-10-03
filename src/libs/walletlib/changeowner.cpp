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
#include "changeowner.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QChangeOwner, CTransaction);

//---------------------------------------------------------------------------
static SFString nextChangeownerChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextChangeownerChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QChangeOwner::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextChangeownerChunk, this);
}

//---------------------------------------------------------------------------
SFString nextChangeownerChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QChangeOwner *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QChangeOwner::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) { _from = toAddress(fieldValue); return true; }
            if ( fieldName % "_to" ) { _to = toAddress(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QChangeOwner::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QChangeOwner::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const QChangeOwner*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _from;
    archive >> _to;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QChangeOwner::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    archive << _from;
    archive << _to;

    return true;
}

//---------------------------------------------------------------------------
void QChangeOwner::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(QChangeOwner, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QChangeOwner, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QChangeOwner, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QChangeOwner, "_from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QChangeOwner, "_to", T_ADDRESS, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QChangeOwner, "schema");
    HIDE_FIELD(QChangeOwner, "deleted");
    HIDE_FIELD(QChangeOwner, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextChangeownerChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const QChangeOwner *cha = (const QChangeOwner *)dataPtr;
    if (cha) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, cha);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QChangeOwner::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QChangeOwner::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString QChangeOwner::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextChangeownerChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) return fromAddress(_from);
            if ( fieldName % "_to" ) return fromAddress(_to);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QChangeOwner& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

