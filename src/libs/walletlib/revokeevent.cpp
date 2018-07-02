/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include <algorithm>
#include "revokeevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QRevokeEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextRevokeeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextRevokeeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QRevokeEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextRevokeeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRevokeeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QRevokeEvent *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QRevokeEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'o':
            if ( fieldName % "owner" ) { owner = toAddress(fieldValue); return true; }
            if ( fieldName % "operation" ) { operation = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QRevokeEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QRevokeEvent::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> owner;
    archive >> operation;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QRevokeEvent::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << owner;
    archive << operation;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, QRevokeEventArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const QRevokeEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void QRevokeEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(QRevokeEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QRevokeEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevokeEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevokeEvent, "owner", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QRevokeEvent, "operation", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QRevokeEvent, "schema");
    HIDE_FIELD(QRevokeEvent, "deleted");
    HIDE_FIELD(QRevokeEvent, "showing");

    builtIns.push_back(_biQRevokeEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRevokeeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const QRevokeEvent *rev = (const QRevokeEvent *)dataPtr;  // NOLINT
    if (rev) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rev);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool QRevokeEvent::readBackLevel(SFArchive& archive) {

    CLogEntry::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q QRevokeEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextRevokeeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'o':
            if ( fieldName % "owner" ) return fromAddress(owner);
            if ( fieldName % "operation" ) return operation;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QRevokeEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

