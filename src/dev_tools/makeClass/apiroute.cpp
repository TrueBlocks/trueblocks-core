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
#include "apiroute.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CApiRoute, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextApirouteChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextApirouteChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CApiRoute::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["apiroute_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextApirouteChunk, this);
}

//---------------------------------------------------------------------------
string_q nextApirouteChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CApiRoute*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CApiRoute::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextApirouteChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "commands" || fieldName % "commandsCnt") {
                size_t cnt = commands.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += commands[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'r':
            if (fieldName % "route") {
                return route;
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
bool CApiRoute::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "commands") {
                CCommandOption obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    commands.push_back(obj);
                    obj = CCommandOption();  // reset
                }
                return true;
            }
            break;
        case 'r':
            if (fieldName % "route") {
                route = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CApiRoute::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CApiRoute::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> route;
    archive >> commands;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CApiRoute::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << route;
    archive << commands;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CApiRouteArray& array) {
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
CArchive& operator<<(CArchive& archive, const CApiRouteArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CApiRoute::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CApiRoute, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CApiRoute, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CApiRoute, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CApiRoute, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CApiRoute, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CApiRoute, "route", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CApiRoute, "commands", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CApiRoute, "schema");
    HIDE_FIELD(CApiRoute, "deleted");
    HIDE_FIELD(CApiRoute, "showing");
    HIDE_FIELD(CApiRoute, "cname");

    builtIns.push_back(_biCApiRoute);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextApirouteChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CApiRoute* api = reinterpret_cast<const CApiRoute*>(dataPtr);
    if (api) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, api);
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
bool CApiRoute::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CApiRoute& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CApiRoute::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "commands") {
        if (index == NOPOS) {
            CCommandOption empty;
            ((CApiRoute*)this)->commands.push_back(empty);
            index = commands.size() - 1;
        }
        if (index < commands.size())
            return &commands[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APIROUTE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
