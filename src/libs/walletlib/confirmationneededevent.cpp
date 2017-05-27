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
#include "confirmationneededevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QConfirmationNeededEvent, CLogEntry, curVersion);

//---------------------------------------------------------------------------
static SFString nextConfirmationneededeventChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextConfirmationneededeventChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void QConfirmationNeededEvent::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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
        ctx << getNextChunk(fmt, nextConfirmationneededeventChunk, this);
}

//---------------------------------------------------------------------------
SFString nextConfirmationneededeventChunk(const SFString& fieldIn, bool& force, const void *data)
{
    const QConfirmationNeededEvent *con = (const QConfirmationNeededEvent *)data;
    if (con)
    {
        // Give customized code a chance to override first
        SFString ret = nextConfirmationneededeventChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0]))
        {
            case 'd':
                if ( fieldIn % "data" ) return con->data;
                break;
            case 'i':
                if ( fieldIn % "initiator" ) return fromAddress(con->initiator);
                break;
            case 'o':
                if ( fieldIn % "operation" ) return con->operation;
                break;
            case 't':
                if ( fieldIn % "to" ) return fromAddress(con->to);
                break;
            case 'v':
                if ( fieldIn % "value" ) return asStringBN(con->value);
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextLogentryChunk(fieldIn, force, con);
        if (!ret.empty())
            return ret;
    }

    return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
bool QConfirmationNeededEvent::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0]))
    {
        case 'd':
            if ( fieldName % "data" ) { data = toLower(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "initiator" ) { initiator = toAddress(fieldValue); return true; }
            break;
        case 'o':
            if ( fieldName % "operation" ) { operation = toLower(fieldValue); return true; }
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
void QConfirmationNeededEvent::finishParse()
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QConfirmationNeededEvent::Serialize(SFArchive& archive)
{
    if (!archive.isReading())
        return ((const QConfirmationNeededEvent*)this)->SerializeC(archive);

    CLogEntry::Serialize(archive);

    archive >> operation;
    archive >> initiator;
    archive >> value;
    archive >> to;
    archive >> data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QConfirmationNeededEvent::SerializeC(SFArchive& archive) const
{
    CLogEntry::SerializeC(archive);

    archive << operation;
    archive << initiator;
    archive << value;
    archive << to;
    archive << data;

    return true;
}

//---------------------------------------------------------------------------
void QConfirmationNeededEvent::registerClass(void)
{
    static bool been_here=false;
    if (been_here) return;
    been_here=true;

    CLogEntry::registerClass();

    uint32_t fieldNum=1000;
    ADD_FIELD(QConfirmationNeededEvent, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(QConfirmationNeededEvent, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(QConfirmationNeededEvent, "operation", T_TEXT, ++fieldNum);
    ADD_FIELD(QConfirmationNeededEvent, "initiator", T_TEXT, ++fieldNum);
    ADD_FIELD(QConfirmationNeededEvent, "value", T_NUMBER, ++fieldNum);
    ADD_FIELD(QConfirmationNeededEvent, "to", T_TEXT, ++fieldNum);
    ADD_FIELD(QConfirmationNeededEvent, "data", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QConfirmationNeededEvent, "schema");
    HIDE_FIELD(QConfirmationNeededEvent, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextConfirmationneededeventChunk_custom(const SFString& fieldIn, bool& force, const void *data)
{
    const QConfirmationNeededEvent *con = (const QConfirmationNeededEvent *)data;
    if (con)
    {
        switch (tolower(fieldIn[0]))
        {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn,force,con);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool QConfirmationNeededEvent::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool QConfirmationNeededEvent::readBackLevel(SFArchive& archive)
{
    bool done=false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

