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
#include "balhistory.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBalHistory, CBaseNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextBalhistoryChunk(const SFString& fieldIn, const void *data);
static SFString nextBalhistoryChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void CBalHistory::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextBalhistoryChunk, this);
}

//---------------------------------------------------------------------------
SFString nextBalhistoryChunk(const SFString& fieldIn, const void *data) {
    const CBalHistory *bal = (const CBalHistory *)data;
    if (bal) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = bal->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextBalhistoryChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'b':
                if ( fieldIn % "balance" ) return asStringBN(bal->balance);
                break;
            case 'r':
                if ( fieldIn % "recordID" ) return bal->recordID;
                break;
            case 't':
                if ( fieldIn % "txDate" ) return bal->txDate.Format(FMT_JSON);
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, bal);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBalHistory::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "balance" ) { balance = toLong(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "recordID" ) { recordID = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "txDate" ) { txDate = snagDate(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBalHistory::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBalHistory::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CBalHistory*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> recordID;
    archive >> txDate;
    archive >> balance;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBalHistory::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << recordID;
    archive << txDate;
    archive << balance;

    return true;
}

//---------------------------------------------------------------------------
void CBalHistory::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CBalHistory, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalHistory, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBalHistory, "recordID", T_TEXT, ++fieldNum);
    ADD_FIELD(CBalHistory, "txDate", T_DATE, ++fieldNum);
    ADD_FIELD(CBalHistory, "balance", T_WEI, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBalHistory, "schema");
    HIDE_FIELD(CBalHistory, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextBalhistoryChunk_custom(const SFString& fieldIn, const void *data) {
    const CBalHistory *bal = (const CBalHistory *)data;
    if (bal) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, bal);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CBalHistory::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CBalHistory::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CBalHistory& bal) {
    bal.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CBalHistory& bal) {
    bal.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CBalHistory::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextBalhistoryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "balance" ) return asStringBN(balance);
            break;
        case 'r':
            if ( fieldName % "recordID" ) return recordID;
            break;
        case 't':
            if ( fieldName % "txDate" ) return txDate.Format(FMT_JSON);
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
}  // namespace qblocks

