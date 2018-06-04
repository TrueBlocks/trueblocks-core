#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "abirecord.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CABIRecord, CBaseNode);

//---------------------------------------------------------------------------
extern SFString nextAbirecordChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAbirecordChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CABIRecord::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAbirecordChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAbirecordChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CABIRecord *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CABIRecord::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiitem" ) { abiitem = fieldValue; return true; }
            break;
        case 'c':
            if ( fieldName % "constant" ) { constant = toBool(fieldValue); return true; }
            if ( fieldName % "count" ) { count = toUnsigned(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = fieldValue; return true; }
            break;
        case 'i':
            if ( fieldName % "inputcnt" ) { inputcnt = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 'o':
            if ( fieldName % "output0" ) { output0 = fieldValue; return true; }
            break;
        case 's':
            if ( fieldName % "signature" ) { signature = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "type" ) { type = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CABIRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CABIRecord::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CABIRecord*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> name;
    archive >> type;
    archive >> constant;
    archive >> inputcnt;
    archive >> output0;
    archive >> abiitem;
    archive >> count;
    archive >> hash;
    archive >> signature;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CABIRecord::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << type;
    archive << constant;
    archive << inputcnt;
    archive << output0;
    archive << abiitem;
    archive << count;
    archive << hash;
    archive << signature;

    return true;
}

//---------------------------------------------------------------------------
void CABIRecord::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CABIRecord, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CABIRecord, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CABIRecord, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CABIRecord, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CABIRecord, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CABIRecord, "constant", T_BOOL, ++fieldNum);
    ADD_FIELD(CABIRecord, "inputcnt", T_NUMBER, ++fieldNum);
    ADD_FIELD(CABIRecord, "output0", T_TEXT, ++fieldNum);
    ADD_FIELD(CABIRecord, "abiitem", T_TEXT, ++fieldNum);
    ADD_FIELD(CABIRecord, "count", T_NUMBER, ++fieldNum);
    ADD_FIELD(CABIRecord, "hash", T_TEXT, ++fieldNum);
    ADD_FIELD(CABIRecord, "signature", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CABIRecord, "schema");
    HIDE_FIELD(CABIRecord, "deleted");
    HIDE_FIELD(CABIRecord, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAbirecordChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CABIRecord *abi = (const CABIRecord *)dataPtr;
    if (abi) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, abi);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CABIRecord::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CABIRecord::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CABIRecord& abi) {
    abi.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CABIRecord& abi) {
    abi.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFString CABIRecord::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAbirecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "abiitem" ) return abiitem;
            break;
        case 'c':
            if ( fieldName % "constant" ) return asString(constant);
            if ( fieldName % "count" ) return asStringU(count);
            break;
        case 'h':
            if ( fieldName % "hash" ) return hash;
            break;
        case 'i':
            if ( fieldName % "inputcnt" ) return asStringU(inputcnt);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            break;
        case 'o':
            if ( fieldName % "output0" ) return output0;
            break;
        case 's':
            if ( fieldName % "signature" ) return signature;
            break;
        case 't':
            if ( fieldName % "type" ) return type;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CABIRecord& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

