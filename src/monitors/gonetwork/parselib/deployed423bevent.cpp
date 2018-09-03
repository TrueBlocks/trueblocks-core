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
#include "deployed423bevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CDeployed423bEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextDeployed423beventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextDeployed423beventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CDeployed423bEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextDeployed423beventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextDeployed423beventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CDeployed423bEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CDeployed423bEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_price_start" ) { _price_start = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_price_constant1" ) { _price_constant1 = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_price_exponent1" ) { _price_exponent1 = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_price_constant2" ) { _price_constant2 = str_2_Wei(fieldValue); return true; }
            if ( fieldName % "_price_exponent2" ) { _price_exponent2 = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CDeployed423bEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CDeployed423bEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _price_start;
    archive >> _price_constant1;
    archive >> _price_exponent1;
    archive >> _price_constant2;
    archive >> _price_exponent2;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CDeployed423bEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _price_start;
    archive << _price_constant1;
    archive << _price_exponent1;
    archive << _price_constant2;
    archive << _price_exponent2;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CDeployed423bEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CDeployed423bEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CDeployed423bEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CDeployed423bEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "_price_start", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "_price_constant1", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "_price_exponent1", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "_price_constant2", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDeployed423bEvent, "_price_exponent2", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CDeployed423bEvent, "schema");
    HIDE_FIELD(CDeployed423bEvent, "deleted");
    HIDE_FIELD(CDeployed423bEvent, "showing");
    HIDE_FIELD(CDeployed423bEvent, "cname");

    builtIns.push_back(_biCDeployed423bEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextDeployed423beventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CDeployed423bEvent *dep = reinterpret_cast<const CDeployed423bEvent *>(dataPtr);
    if (dep) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, dep);
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
bool CDeployed423bEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CDeployed423bEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextDeployed423beventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_price_start" ) return bnu_2_Str(_price_start);
            if ( fieldName % "_price_constant1" ) return bnu_2_Str(_price_constant1);
            if ( fieldName % "_price_exponent1" ) return bnu_2_Str(_price_exponent1);
            if ( fieldName % "_price_constant2" ) return bnu_2_Str(_price_constant2);
            if ( fieldName % "_price_exponent2" ) return bnu_2_Str(_price_exponent2);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CDeployed423bEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

