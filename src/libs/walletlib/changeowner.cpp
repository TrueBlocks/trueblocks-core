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
IMPLEMENT_NODE(QChangeOwner, CTransaction, curVersion);

//---------------------------------------------------------------------------
static SFString nextChangeownerChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextChangeownerChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QChangeOwner::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
    if (!m_showing)
        return;

    if (fmtIn.empty())
    {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextChangeownerChunk, this);
}

//---------------------------------------------------------------------------
SFString nextChangeownerChunk(const SFString& fieldIn, bool& force, const void *data)
{
    const QChangeOwner *cha = (const QChangeOwner *)data;
    if (cha)
    {
        // Give customized code a chance to override first
        SFString ret = nextChangeownerChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0]))
        {
            case '_':
                if ( fieldIn % "_from" ) return fromAddress(cha->_from);
                if ( fieldIn % "_to" ) return fromAddress(cha->_to);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTransactionChunk(fieldIn, force, cha);
        if (!ret.empty())
            return ret;
    }

    return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
bool QChangeOwner::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0]))
    {
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
void QChangeOwner::finishParse()
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QChangeOwner::Serialize(SFArchive& archive)
{
    if (!archive.isReading())
        return ((const QChangeOwner*)this)->SerializeC(archive);

    CTransaction::Serialize(archive);

    archive >> _from;
    archive >> _to;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QChangeOwner::SerializeC(SFArchive& archive) const
{
    CTransaction::SerializeC(archive);

    archive << _from;
    archive << _to;

    return true;
}

//---------------------------------------------------------------------------
void QChangeOwner::registerClass(void)
{
    static bool been_here=false;
    if (been_here) return;
    been_here=true;

    CTransaction::registerClass();

    uint32_t fieldNum=1000;
    ADD_FIELD(QChangeOwner, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(QChangeOwner, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(QChangeOwner, "_from", T_TEXT, ++fieldNum);
    ADD_FIELD(QChangeOwner, "_to", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QChangeOwner, "schema");
    HIDE_FIELD(QChangeOwner, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextChangeownerChunk_custom(const SFString& fieldIn, bool& force, const void *data)
{
    const QChangeOwner *cha = (const QChangeOwner *)data;
    if (cha)
    {
        switch (tolower(fieldIn[0]))
        {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn,force,cha);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool QChangeOwner::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QChangeOwner::readBackLevel(SFArchive& archive)
{
    bool done=false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

