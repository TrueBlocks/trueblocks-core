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
#include "lognoteevent.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLogNoteEvent, CLogEntry);

//---------------------------------------------------------------------------
static string_q nextLognoteeventChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextLognoteeventChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CLogNoteEvent::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextLognoteeventChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLognoteeventChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLogNoteEvent *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CLogNoteEvent::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CLogEntry::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "bar" ) { bar = toLower(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "foo" ) { foo = toLower(fieldValue); return true; }
            if ( fieldName % "fax" ) { fax = toLower(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "guy" ) { guy = str_2_Addr(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "sig" ) { sig = toLower(fieldValue); return true; }
            break;
        case 'w':
            if ( fieldName % "wad" ) { wad = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CLogNoteEvent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLogNoteEvent::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CLogEntry::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> sig;
    archive >> guy;
    archive >> foo;
    archive >> bar;
    archive >> wad;
    archive >> fax;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLogNoteEvent::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CLogEntry::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << sig;
    archive << guy;
    archive << foo;
    archive << bar;
    archive << wad;
    archive << fax;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLogNoteEventArray& array) {
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
CArchive& operator<<(CArchive& archive, const CLogNoteEventArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLogNoteEvent::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CLogEntry::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CLogNoteEvent, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "sig", T_TEXT, ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "guy", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "foo", T_TEXT, ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "bar", T_TEXT, ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "wad", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLogNoteEvent, "fax", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLogNoteEvent, "schema");
    HIDE_FIELD(CLogNoteEvent, "deleted");
    HIDE_FIELD(CLogNoteEvent, "showing");
    HIDE_FIELD(CLogNoteEvent, "cname");

    builtIns.push_back(_biCLogNoteEvent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLognoteeventChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CLogNoteEvent *log = reinterpret_cast<const CLogNoteEvent *>(dataPtr);
    if (log) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, log);
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
bool CLogNoteEvent::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CLogNoteEvent::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextLognoteeventChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "bar" ) return bar;
            break;
        case 'f':
            if ( fieldName % "foo" ) return foo;
            if ( fieldName % "fax" ) return fax;
            break;
        case 'g':
            if ( fieldName % "guy" ) return addr_2_Str(guy);
            break;
        case 's':
            if ( fieldName % "sig" ) return sig;
            break;
        case 'w':
            if ( fieldName % "wad" ) return bnu_2_Str(wad);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CLogEntry::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLogNoteEvent& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

