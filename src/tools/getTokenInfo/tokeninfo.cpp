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
#include "tokeninfo.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTokenInfo, CAccountWatch);

//---------------------------------------------------------------------------
extern string_q nextTokeninfoChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextTokeninfoChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTokenInfo::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTokeninfoChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTokeninfoChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTokenInfo *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTokenInfo::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CAccountWatch::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) { addr = str_2_Addr(fieldValue); return true; }
            break;
        case 'd':
            if ( fieldName % "decimals" ) { decimals = str_2_Uint(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "holders" ) {
                string_q str = fieldValue;
                while (!str.empty()) {
                    holders.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        case 's':
            if ( fieldName % "symbol" ) { symbol = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "totalSupply" ) { totalSupply = str_2_Wei(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "version" ) { version = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTokenInfo::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTokenInfo::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CAccountWatch::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> addr;
    archive >> totalSupply;
    archive >> decimals;
    archive >> version;
    archive >> symbol;
//    archive >> holders;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTokenInfo::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CAccountWatch::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << addr;
    archive << totalSupply;
    archive << decimals;
    archive << version;
    archive << symbol;
//    archive << holders;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTokenInfoArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTokenInfoArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTokenInfo::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CAccountWatch::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CTokenInfo, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTokenInfo, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTokenInfo, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTokenInfo, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CTokenInfo, "addr", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTokenInfo, "totalSupply", T_WEI, ++fieldNum);
    ADD_FIELD(CTokenInfo, "decimals", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTokenInfo, "version", T_TEXT, ++fieldNum);
    ADD_FIELD(CTokenInfo, "symbol", T_TEXT, ++fieldNum);
    ADD_FIELD(CTokenInfo, "holders", T_ADDRESS|TS_ARRAY, ++fieldNum);
    HIDE_FIELD(CTokenInfo, "holders");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTokenInfo, "schema");
    HIDE_FIELD(CTokenInfo, "deleted");
    HIDE_FIELD(CTokenInfo, "showing");
    HIDE_FIELD(CTokenInfo, "cname");

    builtIns.push_back(_biCTokenInfo);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTokeninfoChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CTokenInfo *tok = reinterpret_cast<const CTokenInfo *>(dataPtr);
    if (tok) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tok);
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
bool CTokenInfo::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTokenInfo& tok) {
    tok.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTokenInfo& tok) {
    tok.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CTokenInfo::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextTokeninfoChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) return addr_2_Str(addr);
            break;
        case 'd':
            if ( fieldName % "decimals" ) return uint_2_Str(decimals);
            break;
        case 'h':
            if ( fieldName % "holders" || fieldName % "holdersCnt" ) {
                size_t cnt = holders.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += ("\"" + holders[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indent() : "\n");
                }
                return retS;
            }
            break;
        case 's':
            if ( fieldName % "symbol" ) return symbol;
            break;
        case 't':
            if ( fieldName % "totalSupply" ) return wei_2_Str(totalSupply);
            break;
        case 'v':
            if ( fieldName % "version" ) return version;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CAccountWatch::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTokenInfo& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CTokenInfo::getStringAt(const string_q& fieldName, size_t i) const {
    if ( name % "holders" && i < holders.size() )
        return (holders[i]);
    return "";
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

