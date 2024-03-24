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
#include "member.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CMember, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextMemberChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextMemberChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CMember::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["member_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextMemberChunk, this);
}

//---------------------------------------------------------------------------
string_q nextMemberChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CMember*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CMember::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextMemberChunk_custom(fieldName, this);
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
        case 'd':
            if (fieldName % "doc") {
                return doc == 0 ? "" : uint_2_Str(doc);
            }
            if (fieldName % "description") {
                return description;
            }
            break;
        case 'i':
            if (fieldName % "indexed") {
                return bool_2_Str_t(indexed);
            }
            if (fieldName % "internalType") {
                return internalType;
            }
            break;
        case 'm':
            if (fieldName % "memberFlags") {
                return memberFlags == 0 ? "" : uint_2_Str(memberFlags);
            }
            if (fieldName % "maxWidth") {
                return maxWidth == 0 ? "" : uint_2_Str(maxWidth);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 'p':
            if (fieldName % "precision") {
                return precision == 0 ? "" : uint_2_Str(precision);
            }
            break;
        case 's':
            if (fieldName % "strDefault") {
                return strDefault;
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                return value;
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
bool CMember::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
#define BOOL_ASSIGN_MASK(a, b) { if (str_2_Bool(fieldValue)) { a |= (b); } else { a &= uint64_t(~b); } }
    if (fieldName % "is_pointer")   { BOOL_ASSIGN_MASK(memberFlags, IS_POINTER);   return true; }
    if (fieldName % "is_array")     { BOOL_ASSIGN_MASK(memberFlags, IS_ARRAY);     return true; }
    if (fieldName % "is_object")    { BOOL_ASSIGN_MASK(memberFlags, IS_OBJECT);    return true; }
    if (fieldName % "is_builtin")   { BOOL_ASSIGN_MASK(memberFlags, IS_BUILTIN);   return true; }
    if (fieldName % "is_enabled")   { BOOL_ASSIGN_MASK(memberFlags, IS_ENABLED);   return true; }
    if (fieldName % "is_omitempty") { BOOL_ASSIGN_MASK(memberFlags, IS_OMITEMPTY); return true; }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "components") {
                CMember obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    components.push_back(obj);
                    obj = CMember();  // reset
                }
                return true;
            }
            break;
        case 'd':
            if (fieldName % "doc") {
                doc = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "description") {
                description = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "indexed") {
                indexed = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "internalType") {
                internalType = fieldValue;
                return true;
            }
            break;
        case 'm':
            if (fieldName % "memberFlags") {
                memberFlags = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "maxWidth") {
                maxWidth = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "precision") {
                precision = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "strDefault") {
                strDefault = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                value = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CMember::finishParse() {
    // EXISTING_CODE
    if (internalType.empty())
        internalType = type;
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CMember::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    // archive >> type;
    // archive >> name;
    // archive >> strDefault;
    // archive >> value;
    // archive >> indexed;
    // archive >> internalType;
    // archive >> components;
    // archive >> memberFlags;
    // archive >> precision;
    // archive >> maxWidth;
    // archive >> doc;
    // archive >> description;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMember::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    // archive << type;
    // archive << name;
    // archive << strDefault;
    // archive << value;
    // archive << indexed;
    // archive << internalType;
    // archive << components;
    // archive << memberFlags;
    // archive << precision;
    // archive << maxWidth;
    // archive << doc;
    // archive << description;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMember::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CMember copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CMemberArray& array) {
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
CArchive& operator<<(CArchive& archive, const CMemberArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CMember::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CMember, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CMember, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMember, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CMember, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CMember, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CMember, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "type");
    ADD_FIELD(CMember, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "name");
    ADD_FIELD(CMember, "strDefault", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "strDefault");
    ADD_FIELD(CMember, "value", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "value");
    ADD_FIELD(CMember, "indexed", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "indexed");
    ADD_FIELD(CMember, "internalType", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "internalType");
    ADD_FIELD(CMember, "components", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "components");
    ADD_FIELD(CMember, "memberFlags", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "memberFlags");
    ADD_FIELD(CMember, "precision", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "precision");
    ADD_FIELD(CMember, "maxWidth", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "maxWidth");
    ADD_FIELD(CMember, "doc", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "doc");
    ADD_FIELD(CMember, "description", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMember, "description");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CMember, "schema");
    HIDE_FIELD(CMember, "deleted");
    HIDE_FIELD(CMember, "showing");
    HIDE_FIELD(CMember, "cname");

    builtIns.push_back(_biCMember);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextMemberChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CMember* mem = reinterpret_cast<const CMember*>(dataPtr);
    if (mem) {
        switch (tolower(fieldIn[0])) {
                // EXISTING_CODE
            case 'i':
                if (fieldIn % "is_pointer")   return bool_2_Str_t(mem->memberFlags & IS_POINTER);
                if (fieldIn % "is_array")     return bool_2_Str_t(mem->memberFlags & IS_ARRAY);
                if (fieldIn % "is_object")    return bool_2_Str_t(mem->memberFlags & IS_OBJECT);
                if (fieldIn % "is_builtin")   return bool_2_Str_t(mem->memberFlags & IS_BUILTIN);
                if (fieldIn % "is_enabled")   return bool_2_Str_t(mem->memberFlags & IS_ENABLED);
                if (fieldIn % "is_omitempty") return bool_2_Str_t(mem->memberFlags & IS_OMITEMPTY);
                break;
            case 'v':
                if (fieldIn % "value") {
                    if (contains(mem->type, "tuple"))
                        return mem->value + "--tuple--";
                    return stripWhitespace(mem->value);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, mem);
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
bool CMember::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CMember& mem) {
    mem.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CMember& mem) {
    mem.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CMember& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CMember::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "components") {
        if (index == NOPOS) {
            CMember empty;
            ((CMember*)this)->components.push_back(empty);  // NOLINT
            index = components.size() - 1;
        }
        if (index < components.size())
            return &components[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_MEMBER = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CMember::isCalculated(void) const {
    return memberFlags & IS_CALCULATED;
}

void CMember::postProcessType(void) {
    if (startsWith(type, "double")) {
        precision = str_2_Uint(substitute(type, "double", "") == "" ? "5" : substitute(type, "double", ""));
        type = "double";
    }
    if (contains(type, "*") || contains(type, "Ptr"))
        memberFlags |= IS_POINTER;
    if (contains(type, "Array"))
        memberFlags |= IS_ARRAY;
    if (startsWith(type, 'C'))
        memberFlags |= IS_OBJECT;
    CStringArray builtinTypes = {
        "CStringArray", "CBlknumArray", "CAddressArray", "CBigUintArray", "CTopicArray",
    };
    for (auto b : builtinTypes) {
        if (type == b) {
            memberFlags |= IS_BUILTIN;
        }
    }
    type = substitute(type, "*", "");
}

//-----------------------------------------------------------------------
CMember::CMember(const string_q& n, const string_q& t, const string_q& v) {
    initialize();
    name = n;
    type = t;
    value = v;
}

//-----------------------------------------------------------------------
CMember::CMember(const string_q& n, const string_q& t, uint64_t v) {
    initialize();
    name = n;
    type = t;
    value = uint_2_Str(v);
}

//-----------------------------------------------------------------------
CMember::CMember(const string_q& n, const string_q& t, int64_t v) {
    initialize();
    name = n;
    type = t;
    value = int_2_Str(v);
}

//-----------------------------------------------------------------------
CMember::CMember(const string_q& n, const string_q& t, bool v) {
    initialize();
    name = n;
    type = t;
    value = bool_2_Str(v);
}

//-----------------------------------------------------------------------
CMember::CMember(const string_q& n, const string_q& t, biguint_t v) {
    initialize();
    name = n;
    type = t;
    value = bnu_2_Str(v);
}

//-----------------------------------------------------------------------
CMember::CMember(const string_q& n, const string_q& t, const CStringArray& array) {
    initialize();
    name = n;
    type = t;
    for (auto s : array)
        value += (s + "|");
}

//-----------------------------------------------------------------------
bool CMember::isValid(void) const {
    if (!(startsWith(type, "address") || startsWith(type, "bool") || startsWith(type, "string") ||
          startsWith(type, "bytes") || startsWith(type, "fixed") || startsWith(type, "uint") ||
          startsWith(type, "int") || startsWith(type, "tuple")))
        return false;
    if (startsWith(type, "bytes") && type != "bytes") {
        uint64_t n = str_2_Uint(substitute(type, "bytes", ""));
        return n > 0 && n <= 32;
    }
    return true;
}

//--------------------------------------------------------------------------------
string_q CMember::resolveType(void) const {
    if (contains(type, "tuple")) {
        ostringstream os;
        bool first = true;
        for (auto component : components) {
            os << (!first ? "," : "");
            os << component.resolveType();
            first = false;
        }
        return substitute(type, "tuple", "(" + os.str() + ")");
    }
    return type;
}
// EXISTING_CODE
}  // namespace qblocks
