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
#include "trade.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTrade, CTransaction);

//---------------------------------------------------------------------------
static string_q nextTradeChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextTradeChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTrade::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTradeChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTradeChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTrade *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTrade::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountGet" ) { amountGet = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "amountGive" ) { amountGive = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "amount" ) { amount = str_2_Wei(fieldValue); return true; }
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
        case 'u':
            if ( fieldName % "user" ) { user = str_2_Addr(fieldValue); return true; }
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
void CTrade::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTrade::Serialize(CArchive& archive) {

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
    archive >> user;
    archive >> v;
    archive >> r;
    archive >> s;
    archive >> amount;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTrade::SerializeC(CArchive& archive) const {

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
    archive << user;
    archive << v;
    archive << r;
    archive << s;
    archive << amount;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTradeArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTradeArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTrade::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CTrade, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrade, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTrade, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTrade, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CTrade, "tokenGet", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTrade, "amountGet", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrade, "tokenGive", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTrade, "amountGive", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrade, "expires", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrade, "nonce", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrade, "user", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTrade, "v", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTrade, "r", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrade, "s", T_TEXT, ++fieldNum);
    ADD_FIELD(CTrade, "amount", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTrade, "schema");
    HIDE_FIELD(CTrade, "deleted");
    HIDE_FIELD(CTrade, "showing");
    HIDE_FIELD(CTrade, "cname");

    builtIns.push_back(_biCTrade);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTradeChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CTrade *tra = reinterpret_cast<const CTrade *>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
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
bool CTrade::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CTrade::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextTradeChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "amountGet" ) return bnu_2_Str(amountGet);
            if ( fieldName % "amountGive" ) return bnu_2_Str(amountGive);
            if ( fieldName % "amount" ) return bnu_2_Str(amount);
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
        case 'u':
            if ( fieldName % "user" ) return addr_2_Str(user);
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
ostream& operator<<(ostream& os, const CTrade& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

