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
#include "openapi.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COpenApi, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextOpenapiChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextOpenapiChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void COpenApi::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["openapi_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextOpenapiChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOpenapiChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COpenApi*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q COpenApi::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextOpenapiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "components" || fieldName % "componentsCnt") {
                size_t cnt = components.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += components[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'i':
            if (fieldName % "info") {
                if (info == COAInfo())
                    return "{}";
                return info.Format();
            }
            break;
        case 'o':
            if (fieldName % "openapi") {
                return openapi;
            }
            break;
        case 'p':
            if (fieldName % "paths") {
                return paths;
            }
            break;
        case 's':
            if (fieldName % "servers" || fieldName % "serversCnt") {
                size_t cnt = servers.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += servers[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 't':
            if (fieldName % "tags" || fieldName % "tagsCnt") {
                size_t cnt = tags.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += tags[i].Format();
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

    string_q s;
    s = toUpper(string_q("info")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = info.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool COpenApi::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "components") {
                COAComponent obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    components.push_back(obj);
                    obj = COAComponent();  // reset
                }
                return true;
            }
            break;
        case 'i':
            if (fieldName % "info") {
                return info.parseJson3(fieldValue);
            }
            break;
        case 'o':
            if (fieldName % "openapi") {
                openapi = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "paths") {
                paths = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "servers") {
                COAServer obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    servers.push_back(obj);
                    obj = COAServer();  // reset
                }
                return true;
            }
            break;
        case 't':
            if (fieldName % "tags") {
                COATag obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    tags.push_back(obj);
                    obj = COATag();  // reset
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
void COpenApi::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COpenApi::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> openapi;
    archive >> info;
    archive >> servers;
    archive >> paths;
    archive >> components;
    archive >> tags;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COpenApi::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << openapi;
    archive << info;
    archive << servers;
    archive << paths;
    archive << components;
    archive << tags;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COpenApiArray& array) {
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
CArchive& operator<<(CArchive& archive, const COpenApiArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COpenApi::registerClass(void) {
    // only do this once
    if (HAS_FIELD(COpenApi, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(COpenApi, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(COpenApi, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(COpenApi, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(COpenApi, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(COpenApi, "openapi", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_OBJECT(COpenApi, "info", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(COAInfo));
    ADD_FIELD(COpenApi, "servers", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COpenApi, "paths", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COpenApi, "components", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COpenApi, "tags", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COpenApi, "schema");
    HIDE_FIELD(COpenApi, "deleted");
    HIDE_FIELD(COpenApi, "showing");
    HIDE_FIELD(COpenApi, "cname");

    builtIns.push_back(_biCOpenApi);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOpenapiChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const COpenApi* ope = reinterpret_cast<const COpenApi*>(dataPtr);
    if (ope) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, ope);
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
bool COpenApi::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const COpenApi& ope) {
    ope.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COpenApi& ope) {
    ope.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COpenApi& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* COpenApi::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "info")
        return &info;

    if (fieldName % "servers") {
        if (index == NOPOS) {
            COAServer empty;
            ((COpenApi*)this)->servers.push_back(empty);
            index = servers.size() - 1;
        }
        if (index < servers.size())
            return &servers[index];
    }

    if (fieldName % "components") {
        if (index == NOPOS) {
            COAComponent empty;
            ((COpenApi*)this)->components.push_back(empty);
            index = components.size() - 1;
        }
        if (index < components.size())
            return &components[index];
    }

    if (fieldName % "tags") {
        if (index == NOPOS) {
            COATag empty;
            ((COpenApi*)this)->tags.push_back(empty);
            index = tags.size() - 1;
        }
        if (index < tags.size())
            return &tags[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_OPENAPI = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
