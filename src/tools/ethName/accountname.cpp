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
#include "accountname.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountName, CBaseNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextAccountnameChunk(const SFString& fieldIn, const void *data);
static SFString nextAccountnameChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void CAccountName::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextAccountnameChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAccountnameChunk(const SFString& fieldIn, const void *data) {
    const CAccountName *acc = (const CAccountName *)data;
    if (acc) {
        // Give customized code a chance to override first
        SFString ret = nextAccountnameChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'a':
                if ( fieldIn % "addr" ) return acc->addr;
                break;
            case 'd':
                if ( fieldIn % "description" ) return acc->description;
                break;
            case 'n':
                if ( fieldIn % "name" ) return acc->name;
                break;
            case 's':
                if ( fieldIn % "symbol" ) return acc->symbol;
                if ( fieldIn % "source" ) return acc->source;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, acc);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) { addr = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "description" ) { description = fieldValue; return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 's':
            if ( fieldName % "symbol" ) { symbol = fieldValue; return true; }
            if ( fieldName % "source" ) { source = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountName::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CAccountName*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> symbol;
    archive >> name;
    archive >> addr;
    archive >> source;
    archive >> description;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << symbol;
    archive << name;
    archive << addr;
    archive << source;
    archive << description;

    return true;
}

//---------------------------------------------------------------------------
void CAccountName::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAccountName, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountName, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccountName, "symbol", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "addr", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "source", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "description", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountName, "schema");
    HIDE_FIELD(CAccountName, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAccountnameChunk_custom(const SFString& fieldIn, const void *data) {
    const CAccountName *acc = (const CAccountName *)data;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAccountName::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAccountName::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CAccountName::CAccountName(SFString& strIn) {
    if (countOf('\t',strIn) == 2) {
        // previous format
        source = strIn;
        addr = toLower(nextTokenClear(source, '\t'));
        name = nextTokenClear(source, '\t');
    } else {
        description = strIn;
        symbol = nextTokenClear(description, '\t');
        name = nextTokenClear(description, '\t');
        addr = toLower(nextTokenClear(description, '\t'));
        source = nextTokenClear(description, '\t');
    }
}
// EXISTING_CODE

