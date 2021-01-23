/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "oacomponent.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COAComponent, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextOacomponentChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextOacomponentChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void COAComponent::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["oacomponent_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextOacomponentChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOacomponentChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COAComponent*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q COAComponent::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextOacomponentChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 's':
            if (fieldName % "schemas" || fieldName % "schemasCnt") {
                size_t cnt = schemas.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += schemas[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
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
bool COAComponent::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 's':
            if (fieldName % "schemas") {
                COASchema obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    schemas.push_back(obj);
                    obj = COASchema();  // reset
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COAComponent::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COAComponent::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> schemas;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COAComponent::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << schemas;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COAComponentArray& array) {
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
CArchive& operator<<(CArchive& archive, const COAComponentArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COAComponent::registerClass(void) {
    // only do this once
    if (HAS_FIELD(COAComponent, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(COAComponent, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(COAComponent, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(COAComponent, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(COAComponent, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(COAComponent, "schemas", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COAComponent, "schema");
    HIDE_FIELD(COAComponent, "deleted");
    HIDE_FIELD(COAComponent, "showing");
    HIDE_FIELD(COAComponent, "cname");

    builtIns.push_back(_biCOAComponent);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOacomponentChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const COAComponent* oac = reinterpret_cast<const COAComponent*>(dataPtr);
    if (oac) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, oac);
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
bool COAComponent::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const COAComponent& oac) {
    oac.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COAComponent& oac) {
    oac.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COAComponent& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* COAComponent::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "schemas") {
        if (index == NOPOS) {
            COASchema empty;
            ((COAComponent*)this)->schemas.push_back(empty);
            index = schemas.size() - 1;
        }
        if (index < schemas.size())
            return &schemas[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_OACOMPONENT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
