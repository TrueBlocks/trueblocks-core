/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "person.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPerson, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPersonChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPersonChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPerson::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["person_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPersonChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPersonChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPerson*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPerson::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPersonChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "age") {
                return uint_2_Str(age);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            if (fieldName % "next") {
                if (next)
                    return next->Format();
                return "";
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CPerson::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "age") {
                age = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            if (fieldName % "next") {
                // This drops memory, so we comment it out for now
                // clear();
                // next = new CPerson;
                // if (next) {
                //     string_q str = fieldValue;
                //     return next->parseJson3(str);
                // }
                return false;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPerson::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPerson::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> name;
    archive >> age;
    next = NULL;
    bool has_next = false;
    archive >> has_next;
    if (has_next) {
        string_q className;
        archive >> className;
        next = reinterpret_cast<CPerson*>(createObjectOfType(className));
        if (!next)
            return false;
        next->Serialize(archive);
    }
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPerson::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << age;
    archive << (next != NULL);
    if (next) {
        archive << next->getRuntimeClass()->getClassNamePtr();
        next->SerializeC(archive);
    }
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPersonArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPersonArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPerson::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPerson, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPerson, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPerson, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPerson, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPerson, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPerson, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPerson, "age", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CPerson, "next", T_POINTER | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPerson, "schema");
    HIDE_FIELD(CPerson, "deleted");
    HIDE_FIELD(CPerson, "showing");
    HIDE_FIELD(CPerson, "cname");

    builtIns.push_back(_biCPerson);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPersonChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPerson* per = reinterpret_cast<const CPerson*>(dataPtr);
    if (per) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, per);
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
bool CPerson::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPerson& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPerson::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "next")
        return next;
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PERSON = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
