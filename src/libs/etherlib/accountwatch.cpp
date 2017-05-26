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
#include "accountwatch.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountWatch, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextAccountwatchChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextAccountwatchChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CAccountWatch::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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
        ctx << getNextChunk(fmt, nextAccountwatchChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAccountwatchChunk(const SFString& fieldIn, bool& force, const void *data)
{
    const CAccountWatch *acc = (const CAccountWatch *)data;
    if (acc)
    {
        // Give customized code a chance to override first
        SFString ret = nextAccountwatchChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0]))
        {
            case 'a':
                if ( fieldIn % "address" ) return fromAddress(acc->address);
                break;
            case 'c':
                if ( fieldIn % "color" ) return acc->color;
                break;
            case 'd':
                if ( fieldIn % "disabled" ) return asString(acc->disabled);
                break;
            case 'f':
                if ( fieldIn % "firstBlock" ) return asStringU(acc->firstBlock);
                break;
            case 'i':
                if ( fieldIn % "index" ) return asStringU(acc->index);
                break;
            case 'n':
                if ( fieldIn % "name" ) return acc->name;
                if ( fieldIn % "nodeis" ) { expContext().noFrst=true; return acc->nodeis.Format(); }
                break;
            case 'q':
                if ( fieldIn % "qbis" ) { expContext().noFrst=true; return acc->qbis.Format(); }
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, acc);
        if (!ret.empty())
            return ret;
    }

    return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
    // EXISTING_CODE
    if (fieldName % "qbis") {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        qbis.parseJson(p,nFields);
        return true;

    } else if (fieldName % "nodeis") {
        char *p = (char *)(const char*)fieldValue;
        uint32_t nFields=0;
        nodeis.parseJson(p,nFields);
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0]))
    {
        case 'a':
            if ( fieldName % "address" ) { address = toAddress(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "color" ) { color = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "disabled" ) { disabled = toBool(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "firstBlock" ) { firstBlock = toUnsigned(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "index" ) { index = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            if ( fieldName % "nodeis" ) { /* nodeis = fieldValue; */ return false; }
            break;
        case 'q':
            if ( fieldName % "qbis" ) { /* qbis = fieldValue; */ return false; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountWatch::finishParse()
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::Serialize(SFArchive& archive)
{
    if (!archive.isReading())
        return ((const CAccountWatch*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> index;
    archive >> address;
    archive >> name;
    archive >> color;
    archive >> firstBlock;
    archive >> disabled;
    archive >> qbis;
    archive >> nodeis;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::SerializeC(SFArchive& archive) const
{
    if (!preSerializeC(archive))
        return false;

    archive << index;
    archive << address;
    archive << name;
    archive << color;
    archive << firstBlock;
    archive << disabled;
    archive << qbis;
    archive << nodeis;

    return true;
}

//---------------------------------------------------------------------------
void CAccountWatch::registerClass(void)
{
    static bool been_here=false;
    if (been_here) return;
    been_here=true;

    uint32_t fieldNum=1000;
    ADD_FIELD(CAccountWatch, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CAccountWatch, "index", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "address", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "color", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "firstBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "disabled", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "qbis", T_TEXT|TS_OBJECT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "nodeis", T_TEXT|TS_OBJECT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountWatch, "schema");
    HIDE_FIELD(CAccountWatch, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAccountwatchChunk_custom(const SFString& fieldIn, bool& force, const void *data)
{
    const CAccountWatch *acc = (const CAccountWatch *)data;
    if (acc)
    {
        switch (tolower(fieldIn[0]))
        {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn,force,acc);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CAccountWatch::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAccountWatch::readBackLevel(SFArchive& archive)
{
    bool done=false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CAccountWatch::getWatch(const CToml& toml, uint32_t n, bool fromFile) {
    index = n;
    address = toLower(toml.getConfigStr("watches", "address_"+asString(n), ""));
    if (!address.startsWith("0x"))
        address = "0x" + address;
    if (address.length() != 42)
        return false;
    name = toml.getConfigStr("watches", "name_"+asString(n), "");
    if (name.empty())
        return false;
    color = convertColor(toml.getConfigStr("watches", "color_"+asString(n), ""));
    if (color.empty())
        return false;
    firstBlock = toml.getConfigInt("watches", "firstBlock_"+asString(n), 0);
    disabled = toml.getConfigBool("watches", "disabled_"+asString(n), false);
    qbis.begBal = toml.getConfigBigInt("watches", "start_bal_"+asString(n), 0);
    qbis.endBal = qbis.begBal;
    nodeis.begBal = getBalance(address, firstBlock-1, fromFile);
    nodeis.endBal = nodeis.begBal;
    return true;
}
// EXISTING_CODE
}  // namespace qblocks

