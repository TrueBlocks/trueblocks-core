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
#include "reclaimmisplacedtokensevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CReclaimMisplacedTokensEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextReclaimmisplacedtokenseventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextReclaimmisplacedtokenseventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CReclaimMisplacedTokensEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextReclaimmisplacedtokenseventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextReclaimmisplacedtokenseventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CReclaimMisplacedTokensEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CReclaimMisplacedTokensEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) { _from = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "_tokenAmount" ) { _tokenAmount = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CReclaimMisplacedTokensEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CReclaimMisplacedTokensEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _from;
    archive >> _tokenAmount;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReclaimMisplacedTokensEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _from;
    archive << _tokenAmount;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CReclaimMisplacedTokensEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CReclaimMisplacedTokensEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CReclaimMisplacedTokensEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CReclaimMisplacedTokensEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CReclaimMisplacedTokensEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CReclaimMisplacedTokensEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CReclaimMisplacedTokensEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CReclaimMisplacedTokensEvent, "_from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CReclaimMisplacedTokensEvent, "_tokenAmount", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CReclaimMisplacedTokensEvent, "schema");
    HIDE_FIELD(CReclaimMisplacedTokensEvent, "deleted");
    HIDE_FIELD(CReclaimMisplacedTokensEvent, "showing");
    HIDE_FIELD(CReclaimMisplacedTokensEvent, "cname");

    builtIns.push_back(_biCReclaimMisplacedTokensEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextReclaimmisplacedtokenseventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CReclaimMisplacedTokensEvent *rec = reinterpret_cast<const CReclaimMisplacedTokensEvent *>(dataPtr);
    if (rec) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rec);
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
bool CReclaimMisplacedTokensEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CReclaimMisplacedTokensEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextReclaimmisplacedtokenseventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_from" ) return addr_2_Str(_from);
            if ( fieldName % "_tokenAmount" ) return bnu_2_Str(_tokenAmount);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CReclaimMisplacedTokensEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

