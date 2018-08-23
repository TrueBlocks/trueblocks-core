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
#include "changerequirement.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(QChangeRequirement, CTransaction_Ex);

//---------------------------------------------------------------------------
static string_q nextChangerequirementChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextChangerequirementChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void QChangeRequirement::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextChangerequirementChunk, this);
}

//---------------------------------------------------------------------------
string_q nextChangerequirementChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const QChangeRequirement *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTransaction_Ex::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newRequired" ) { _newRequired = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void QChangeRequirement::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTransaction_Ex::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> _newRequired;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool QChangeRequirement::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CTransaction_Ex::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << _newRequired;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, QChangeRequirementArray& array) {
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
CArchive& operator<<(CArchive& archive, const QChangeRequirementArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void QChangeRequirement::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTransaction_Ex::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(QChangeRequirement, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(QChangeRequirement, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(QChangeRequirement, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(QChangeRequirement, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(QChangeRequirement, "_newRequired", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(QChangeRequirement, "schema");
    HIDE_FIELD(QChangeRequirement, "deleted");
    HIDE_FIELD(QChangeRequirement, "showing");
    HIDE_FIELD(QChangeRequirement, "cname");

    builtIns.push_back(_biQChangeRequirement);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextChangerequirementChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const QChangeRequirement *cha = reinterpret_cast<const QChangeRequirement *>(dataPtr);
    if (cha) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, cha);
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
bool QChangeRequirement::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q QChangeRequirement::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextChangerequirementChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case '_':
            if ( fieldName % "_newRequired" ) return bnu_2_Str(_newRequired);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTransaction_Ex::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const QChangeRequirement& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

