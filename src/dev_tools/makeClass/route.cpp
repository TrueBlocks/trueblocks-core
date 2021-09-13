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
#include "route.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRoute, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextRouteChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextRouteChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CRoute::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["route_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextRouteChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRouteChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CRoute*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CRoute::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextRouteChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'h':
            if (fieldName % "helpText") {
                return helpText;
            }
            if (fieldName % "hotKey") {
                return hotKey;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                return path;
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
bool CRoute::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'h':
            if (fieldName % "helpText") {
                helpText = fieldValue;
                return true;
            }
            if (fieldName % "hotKey") {
                hotKey = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                path = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CRoute::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CRoute::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> path;
    archive >> helpText;
    archive >> hotKey;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRoute::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << path;
    archive << helpText;
    archive << hotKey;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRoute::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CRoute copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRouteArray& array) {
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
CArchive& operator<<(CArchive& archive, const CRouteArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CRoute::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CRoute, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CRoute, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CRoute, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CRoute, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CRoute, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CRoute, "path", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRoute, "helpText", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRoute, "hotKey", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CRoute, "schema");
    HIDE_FIELD(CRoute, "deleted");
    HIDE_FIELD(CRoute, "showing");
    HIDE_FIELD(CRoute, "cname");

    builtIns.push_back(_biCRoute);

    // EXISTING_CODE
    ADD_FIELD(CRoute, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRoute, "exact", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRoute, "component", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRouteChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CRoute* rou = reinterpret_cast<const CRoute*>(dataPtr);
    if (rou) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'c':
                if (fieldIn % "component") {
                    CStringArray parts;
                    explode(parts, rou->path, '/');
                    ostringstream os;
                    if (parts.size() > 1) {
                        os << toProper(parts[1]) << "View";
                    } else {
                        os << "DashboardView";
                    }
                    return os.str();
                }
                break;
            case 'e':
                if (fieldIn % "exact") {
                    return contains(rou->path, ":") ? "false" : "true";
                }
                break;
            case 'n':
                if (fieldIn % "name") {
                    bool isTemplate = contains(rou->path, ":");
                    CStringArray parts;
                    explode(parts, substitute(substitute(rou->path, "-", "/"), ":", "/"), '/');
                    ostringstream os;
                    for (auto part : parts)
                        os << toProper(part);
                    if (os.str().empty())
                        os << "Root";
                    if (isTemplate)
                        os << "Template";
                    else
                        os << "Location";
                    return os.str();
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, rou);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CRoute::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CRoute& rou) {
    rou.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRoute& rou) {
    rou.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CRoute& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ROUTE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
