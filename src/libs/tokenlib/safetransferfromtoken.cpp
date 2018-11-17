/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "safetransferfromtoken.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QSafeTransferFromToken, CTransaction);

//---------------------------------------------------------------------------
static string_q nextSafetransferfromtokeChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextSafetransferfromtokeChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QSafeTransferFromToken::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextSafetransferfromtokeChunk, this);
}

//---------------------------------------------------------------------------
string_q nextSafetransferfromtokeChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const QSafeTransferFromToken *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QSafeTransferFromToken::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) { _from = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_to" ) { _to = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_tokenId" ) { _tokenId = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_data" ) { _data = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QSafeTransferFromToken::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QSafeTransferFromToken::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _from;
    archive >> _to;
    archive >> _tokenId;
    archive >> _data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QSafeTransferFromToken::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _from;
    archive << _to;
    archive << _tokenId;
    archive << _data;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, QSafeTransferFromTokenArray& array) {
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
CArchive& operator<<(CArchive& archive, const QSafeTransferFromTokenArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void QSafeTransferFromToken::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(QSafeTransferFromToken, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "_from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "_to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "_tokenId", T_NUMBER, ++fieldNum);
    ADD_FIELD(QSafeTransferFromToken, "_data", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QSafeTransferFromToken, "schema");
    HIDE_FIELD(QSafeTransferFromToken, "deleted");
    HIDE_FIELD(QSafeTransferFromToken, "showing");
    HIDE_FIELD(QSafeTransferFromToken, "cname");

    builtIns.push_back(_biQSafeTransferFromToken);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextSafetransferfromtokeChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const QSafeTransferFromToken *saf = reinterpret_cast<const QSafeTransferFromToken *>(dataPtr);
    if (saf) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, saf);
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
bool QSafeTransferFromToken::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q QSafeTransferFromToken::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextSafetransferfromtokeChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) return addr_2_Str(_from);
            if ( fieldName % "_to" ) return addr_2_Str(_to);
            if ( fieldName % "_tokenId" ) return bnu_2_Str(_tokenId);
            if ( fieldName % "_data" ) return _data;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QSafeTransferFromToken& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

