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
#include "person.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPerson, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextPersonChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextPersonChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CPerson::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextPersonChunk, this);
}

//---------------------------------------------------------------------------
SFString nextPersonChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CPerson *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CPerson::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "age" ) { age = toLong32u(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            if ( fieldName % "next" ) {
                Clear();
                next = new CPerson;
                if (next) {
                    char *p = cleanUpJson((char *)fieldValue.c_str());
                    uint32_t nFields = 0;
                    next->parseJson(p, nFields);
                    return true;
                }
                return false;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPerson::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPerson::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CPerson*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> name;
    archive >> age;
    next = NULL;
    bool has_next = false;
    archive >> has_next;
    if (has_next) {
        next = new CPerson;
        if (!next)
            return false;
        next->Serialize(archive);
    }
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPerson::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << age;
    archive << (next != NULL);
    if (next)
        next->SerializeC(archive);

    return true;
}

//---------------------------------------------------------------------------
void CPerson::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CPerson, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CPerson, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CPerson, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CPerson, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CPerson, "age", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPerson, "next", T_POINTER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPerson, "schema");
    HIDE_FIELD(CPerson, "deleted");
    HIDE_FIELD(CPerson, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextPersonChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CPerson *per = (const CPerson *)dataPtr;
    if (per) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, per);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CPerson::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CPerson::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CPerson::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextPersonChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "age" ) return asStringU(age);
            break;
        case 'n':
            if ( fieldName % "name" ) return name;
            if ( fieldName % "next" ) {
                if (next)
                    return next->Format();
                return "";
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPerson& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

