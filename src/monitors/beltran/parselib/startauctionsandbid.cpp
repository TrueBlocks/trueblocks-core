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
#include "startauctionsandbid.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CStartAuctionsAndBid, CTransaction);

//---------------------------------------------------------------------------
static string_q nextStartauctionsandbidChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextStartauctionsandbidChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CStartAuctionsAndBid::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextStartauctionsandbidChunk, this);
}

//---------------------------------------------------------------------------
string_q nextStartauctionsandbidChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CStartAuctionsAndBid *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CStartAuctionsAndBid::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'h':
            if ( fieldName % "hashes" ) {
                string_q str = fieldValue;
                while (!str.empty()) {
                    hashes.push_back(nextTokenClear(str, ','));
                }
                return true;
            }
            break;
        case 's':
            if ( fieldName % "sealedBid" ) { sealedBid = toLower(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CStartAuctionsAndBid::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CStartAuctionsAndBid::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> hashes;
    archive >> sealedBid;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CStartAuctionsAndBid::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << hashes;
    archive << sealedBid;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CStartAuctionsAndBidArray& array) {
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
CArchive& operator<<(CArchive& archive, const CStartAuctionsAndBidArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CStartAuctionsAndBid::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CStartAuctionsAndBid, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CStartAuctionsAndBid, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CStartAuctionsAndBid, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CStartAuctionsAndBid, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CStartAuctionsAndBid, "hashes", T_TEXT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CStartAuctionsAndBid, "sealedBid", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CStartAuctionsAndBid, "schema");
    HIDE_FIELD(CStartAuctionsAndBid, "deleted");
    HIDE_FIELD(CStartAuctionsAndBid, "showing");
    HIDE_FIELD(CStartAuctionsAndBid, "cname");

    builtIns.push_back(_biCStartAuctionsAndBid);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextStartauctionsandbidChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CStartAuctionsAndBid *sta = reinterpret_cast<const CStartAuctionsAndBid *>(dataPtr);
    if (sta) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, sta);
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
bool CStartAuctionsAndBid::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CStartAuctionsAndBid::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextStartauctionsandbidChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'h':
            if ( fieldName % "hashes" || fieldName % "hashesCnt" ) {
                size_t cnt = hashes.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += ("\"" + hashes[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indent() : "\n");
                }
                return retS;
            }
            break;
        case 's':
            if ( fieldName % "sealedBid" ) return sealedBid;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CStartAuctionsAndBid& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CStartAuctionsAndBid::getStringAt(const string_q& name, size_t i) const {
    if ( name % "hashes" && i < hashes.size() )
        return (hashes[i]);
    return "";
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

