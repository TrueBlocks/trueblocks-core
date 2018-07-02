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
#include "revoke.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QRevoke, CTransaction);

//---------------------------------------------------------------------------
static string_q nextRevokeChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextRevokeChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QRevoke::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextRevokeChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRevokeChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const QRevoke *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QRevoke::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_operation" ) { _operation = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QRevoke::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QRevoke::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _operation;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QRevoke::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _operation;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, QRevokeArray& array) {
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
SFArchive& operator<<(SFArchive& archive, const QRevokeArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void QRevoke::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(QRevoke, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QRevoke, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevoke, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QRevoke, "_operation", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QRevoke, "schema");
    HIDE_FIELD(QRevoke, "deleted");
    HIDE_FIELD(QRevoke, "showing");

    builtIns.push_back(_biQRevoke);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRevokeChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const QRevoke *rev = (const QRevoke *)dataPtr;  // NOLINT
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
bool QRevoke::readBackLevel(SFArchive& archive) {

    CTransaction::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q QRevoke::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextRevokeChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_operation" ) return _operation;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QRevoke& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

