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
#include "cancelorder.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCancelOrder, CTransaction);

//---------------------------------------------------------------------------
static string_q nextCancelorderChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextCancelorderChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CCancelOrder::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextCancelorderChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCancelorderChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCancelOrder *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CCancelOrder::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountGet" ) { amountGet = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "amountGive" ) { amountGive = str_2_Wei(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "expires" ) { expires = str_2_Wei(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nonce" ) { nonce = str_2_Wei(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "r" ) { r = toLower(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "s" ) { s = toLower(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "tokenGet" ) { tokenGet = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "tokenGive" ) { tokenGive = str_2_Addr(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "v" ) { v = (uint32_t)str_2_Uint(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCancelOrder::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCancelOrder::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> tokenGet;
    archive >> amountGet;
    archive >> tokenGive;
    archive >> amountGive;
    archive >> expires;
    archive >> nonce;
    archive >> v;
    archive >> r;
    archive >> s;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCancelOrder::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << tokenGet;
    archive << amountGet;
    archive << tokenGive;
    archive << amountGive;
    archive << expires;
    archive << nonce;
    archive << v;
    archive << r;
    archive << s;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCancelOrderArray& array) {
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
CArchive& operator<<(CArchive& archive, const CCancelOrderArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCancelOrder::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CCancelOrder, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CCancelOrder, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CCancelOrder, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CCancelOrder, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CCancelOrder, "tokenGet", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CCancelOrder, "amountGet", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCancelOrder, "tokenGive", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CCancelOrder, "amountGive", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCancelOrder, "expires", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCancelOrder, "nonce", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCancelOrder, "v", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCancelOrder, "r", T_TEXT, ++fieldNum);
    ADD_FIELD(CCancelOrder, "s", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCancelOrder, "schema");
    HIDE_FIELD(CCancelOrder, "deleted");
    HIDE_FIELD(CCancelOrder, "showing");
    HIDE_FIELD(CCancelOrder, "cname");

    builtIns.push_back(_biCCancelOrder);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCancelorderChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CCancelOrder *can = reinterpret_cast<const CCancelOrder *>(dataPtr);
    if (can) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, can);
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
bool CCancelOrder::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CCancelOrder::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextCancelorderChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountGet" ) return bnu_2_Str(amountGet);
            if ( fieldName % "amountGive" ) return bnu_2_Str(amountGive);
            break;
        case 'e':
            if ( fieldName % "expires" ) return bnu_2_Str(expires);
            break;
        case 'n':
            if ( fieldName % "nonce" ) return bnu_2_Str(nonce);
            break;
        case 'r':
            if ( fieldName % "r" ) return r;
            break;
        case 's':
            if ( fieldName % "s" ) return s;
            break;
        case 't':
            if ( fieldName % "tokenGet" ) return addr_2_Str(tokenGet);
            if ( fieldName % "tokenGive" ) return addr_2_Str(tokenGive);
            break;
        case 'v':
            if ( fieldName % "v" ) return uint_2_Str(v);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCancelOrder& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

