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
#include "parameter.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CParameter, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextParameterChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextParameterChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CParameter::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["parameter_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextParameterChunk, this);
}

//---------------------------------------------------------------------------
string_q nextParameterChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CParameter*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CParameter::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextParameterChunk_custom(fieldName, this);
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
            if (fieldName % "indexed") {
                return bool_2_Str_t(indexed);
            }
            if (fieldName % "internalType") {
                return internalType;
            }
            if (fieldName % "is_flags") {
                return uint_2_Str(is_flags);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            if (fieldName % "no_write") {
                return bool_2_Str_t(no_write);
            }
            break;
        case 's':
            if (fieldName % "str_default") {
                return str_default;
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
bool CParameter::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // clang-format off
#define BOOL_ASSIGN_MASK(a, b) { if (str_2_Bool(fieldValue)) { a |= (b); } else { a &= uint64_t(~b); } }
    if (fieldName % "is_pointer") { BOOL_ASSIGN_MASK(is_flags, IS_POINTER); return true; }
    if (fieldName % "is_array")   { BOOL_ASSIGN_MASK(is_flags, IS_ARRAY);   return true; }
    if (fieldName % "is_object")  { BOOL_ASSIGN_MASK(is_flags, IS_OBJECT);  return true; }
    if (fieldName % "is_builtin") { BOOL_ASSIGN_MASK(is_flags, IS_BUILTIN); return true; }
    if (fieldName % "is_minimal") { BOOL_ASSIGN_MASK(is_flags, IS_MINIMAL); return true; }
    if (fieldName % "is_enabled") { BOOL_ASSIGN_MASK(is_flags, IS_ENABLED); return true; }
    // clang-format on
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "components") {
                CParameter obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    components.push_back(obj);
                    obj = CParameter();  // reset
                }
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
            if (fieldName % "is_flags") {
                is_flags = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            if (fieldName % "no_write") {
                no_write = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "str_default") {
                str_default = fieldValue;
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
void CParameter::finishParse() {
    // EXISTING_CODE
    if (internalType.empty())
        internalType = type;
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CParameter::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> type;
    archive >> name;
    archive >> str_default;
    archive >> value;
    archive >> indexed;
    archive >> internalType;
    archive >> components;
    archive >> no_write;
    archive >> is_flags;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CParameter::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << name;
    archive << str_default;
    archive << value;
    archive << indexed;
    archive << internalType;
    archive << components;
    archive << no_write;
    archive << is_flags;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CParameterArray& array) {
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
CArchive& operator<<(CArchive& archive, const CParameterArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CParameter::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CParameter, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CParameter, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CParameter, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CParameter, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CParameter, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CParameter, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "str_default", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "value", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "indexed", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "internalType", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "components", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "no_write", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "is_flags", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CParameter, "schema");
    HIDE_FIELD(CParameter, "deleted");
    HIDE_FIELD(CParameter, "showing");
    HIDE_FIELD(CParameter, "cname");

    builtIns.push_back(_biCParameter);

    // EXISTING_CODE
    ADD_FIELD(CParameter, "is_pointer", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "is_array", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "is_object", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "is_builtin", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "is_minimal", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CParameter, "is_enabled", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CParameter, "is_enabled");
    HIDE_FIELD(CParameter, "is_flags");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextParameterChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CParameter* par = reinterpret_cast<const CParameter*>(dataPtr);
    if (par) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // clang-format off
            case 'i':
                if (fieldIn % "is_pointer") return bool_2_Str_t(par->is_flags & IS_POINTER);
                if (fieldIn % "is_array")   return bool_2_Str_t(par->is_flags & IS_ARRAY);
                if (fieldIn % "is_object")  return bool_2_Str_t(par->is_flags & IS_OBJECT);
                if (fieldIn % "is_builtin") return bool_2_Str_t(par->is_flags & IS_BUILTIN);
                if (fieldIn % "is_minimal") return bool_2_Str_t(par->is_flags & IS_MINIMAL);
                if (fieldIn % "is_enabled") return bool_2_Str_t(par->is_flags & IS_ENABLED);
                break;
            case 'v':
                if (fieldIn % "value") {
                    if (contains(par->type, "tuple"))
                        return par->value + "--tuple--";
                    return stripWhitespace(par->value);
                }
                break;
            // clang-format off
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, par);
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
bool CParameter::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 8, 2)) {
        archive >> type;
        archive >> name;
        archive >> str_default;
        archive >> value;
        archive >> indexed;
        archive >> internalType;
        archive >> components;
        archive >> no_write;
        archive >> is_flags;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CParameter& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CParameter::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "components") {
        if (index == NOPOS) {
            CParameter empty;
            ((CParameter*)this)->components.push_back(empty);
            index = components.size() - 1;
        }
        if (index < components.size())
            return &components[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PARAMETER =
    "[{TYPE}]\t"
    "[{NAME}]\t"
    "[{STR_DEFAULT}]\t"
    "[{VALUE}]\t"
    "[{INTERNALTYPE}]\t"
    "[{INDEXED}]\t"
    "[{IS_POINTER}]\t"
    "[{IS_ARRAY}]\t"
    "[{IS_OBJECT}]\t"
    "[{IS_BUILTIN}]\t"
    "[{NO_WRITE}]\t"
    "[{IS_MINIMAL}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
CParameter::CParameter(string_q& textIn) {
    initialize();

    replaceAll(textIn, " *", "* ");                    // cleanup
    replaceAll(textIn, "address[]", "CAddressArray");  // cleanup

    if (contains(textIn, "nowrite")) {
        no_write = true;
        if (contains(textIn, "-min"))
            is_flags |= IS_MINIMAL;
        replace(textIn, " (nowrite)", "");
        replace(textIn, " (nowrite-min)", "");
    }

    if (contains(textIn, "=")) {
        str_default = textIn;
        textIn = nextTokenClear(str_default, '=');
    }

    type = nextTokenClear(textIn, ' ');
    if (contains(type, "*") || contains(type, "Ptr"))
        is_flags |= IS_POINTER;
    if (contains(type, "Array"))
        is_flags |= IS_ARRAY;
    if (startsWith(type, 'C'))
        is_flags |= IS_OBJECT;
    CStringArray builtinTypes = {
        "CStringArray", "CBlockNumArray", "CAddressArray", "CBigUintArray", "CTopicArray",
    };
    for (auto b : builtinTypes) {
        if (type == b) {
            is_flags |= IS_BUILTIN;
        }
    }

    type = substitute(type, "*", "");
    name = textIn;
}

//-----------------------------------------------------------------------
CParameter::CParameter(const string_q& n, const string_q& t, const string_q& v) {
    initialize();
    name = n;
    type = t;
    value = v;
}

//-----------------------------------------------------------------------
CParameter::CParameter(const string_q& n, const string_q& t, uint64_t v) {
    initialize();
    name = n;
    type = t;
    value = uint_2_Str(v);
}

//-----------------------------------------------------------------------
CParameter::CParameter(const string_q& n, const string_q& t, int64_t v) {
    initialize();
    name = n;
    type = t;
    value = int_2_Str(v);
}

//-----------------------------------------------------------------------
CParameter::CParameter(const string_q& n, const string_q& t, bool v) {
    initialize();
    name = n;
    type = t;
    value = bool_2_Str(v);
}

//-----------------------------------------------------------------------
CParameter::CParameter(const string_q& n, const string_q& t, biguint_t v) {
    initialize();
    name = n;
    type = t;
    value = bnu_2_Str(v);
}

//-----------------------------------------------------------------------
CParameter::CParameter(const string_q& n, const string_q& t, const CStringArray& array) {
    initialize();
    name = n;
    type = t;
    for (auto s : array)
        value += (s + "|");
}

//-----------------------------------------------------------------------
string_q CParameter::getFunctionAssign(uint64_t which) const {
    string_q ass;
    if (contains(type, "[") && contains(type, "]")) {
        const char* STR_ASSIGNARRAY =
            "\t\t\twhile (!params.empty()) {\n"
            "\t\t\t\tstring_q val = extract(params, 0, 64);\n"
            "\t\t\t\tparams = extract(params, 64);\n"
            "\t\t\t\ta->[{NAME}].push_back(val);\n"
            "\t\t\t}\n";
        return Format(STR_ASSIGNARRAY);
    }

    // clang-format off
           if (         type == "uint")    { ass = "str_2_Wei(\"0x\" + [{VAL}]);";
    } else if (         type == "uint256") { ass = "str_2_Wei(\"0x\" + [{VAL}]);";
    } else if (contains(type, "gas"))      { ass = "str_2_Gas([{VAL}]);";
    } else if (contains(type, "uint64"))   { ass = "str_2_Uint([{VAL}]);";
    } else if (contains(type, "uint"))     { ass = "(uint32_t)str_2_Uint([{VAL}]);";
    } else if (contains(type, "int"))      { ass = "str_2_Int([{VAL}]);";
    } else if (contains(type, "bool"))     { ass = "str_2_Int([{VAL}]);";
    } else if (contains(type, "address"))  { ass = "str_2_Addr([{VAL}]);";
    } else                                 { ass = "[{VAL}];";
    }
    // clang-format on

    replace(ass, "[{VAL}]", "extract(params, " + uint_2_Str(which) + "*64" + (type == "bytes" ? "" : ", 64") + ")");
    return Format("\t\t\ta->[{NAME}] = " + ass + "\n");
}

//-----------------------------------------------------------------------
string_q CParameter::getEventAssign(uint64_t which, uint64_t nIndexed) const {
    string_q ass;

    // clang-format off
           if (         type == "uint")    { ass = "str_2_Wei([{VAL}]);";
    } else if (         type == "uint256") { ass = "str_2_Wei([{VAL}]);";
    } else if (contains(type, "gas"))      { ass = "str_2_Gas([{VAL}]);";
    } else if (contains(type, "uint64"))   { ass = "str_2_Uint([{VAL}]);";
    } else if (contains(type, "uint"))     { ass = "(uint32_t)str_2_Uint([{VAL}]);";
    } else if (contains(type, "int"))      { ass = "str_2_Int([{VAL}]);";
    } else if (contains(type, "bool"))     { ass = "str_2_Int([{VAL}]);";
    } else if (contains(type, "address"))  { ass = "str_2_Addr([{VAL}]);";
    } else                                 { ass = "[{VAL}];";
    }
    // clang-format on

    if (indexed) {
        replace(ass, "[{VAL}]", "nTops > [{WHICH}] ? topic_2_Str(p->topics[{IDX}]) : \"\"");

    } else if (type == "bytes") {
        replace(ass, "[{VAL}]", "\"0x\" + extract(data, [{WHICH}]*64)");
        which -= (nIndexed + 1);

    } else {
        replace(ass, "[{VAL}]", string_q(type == "address" ? "" : "\"0x\" + ") + "extract(data, [{WHICH}]*64, 64)");
        which -= (nIndexed + 1);
    }

    replace(ass, "[{IDX}]", "++" + uint_2_Str(which) + "++");
    replace(ass, "[{WHICH}]", uint_2_Str(which));
    string_q fmt = "\t\t\ta->[{NAME}] = " + ass + "\n";
    return Format(fmt);
}

//-----------------------------------------------------------------------
static string_q elementaryName(const string_q& in) {
    // clang-format off
    if (startsWith(in, "int["))    return "int256" + in.substr(3);
    if (in == "int")               return "int256";
    if (startsWith(in, "uint["))   return "uint256" + in.substr(4);
    if (in == "uint")              return "uint256";
    if (startsWith(in, "fixed["))  return "fixed128x128" + in.substr(5);
    if (in == "fixed")             return "fixed128x128";
    if (startsWith(in, "ufixed[")) return "ufixed128x128" + in.substr(6);
    if (in == "ufixed")            return "ufixed128x128";
    // clang-format on
    return in;
}

//-----------------------------------------------------------------------
bool CParameter::fromDefinition(const string_q& strIn) {
    string_q str = strIn;
    indexed = contains(str, "indexed");
    str = trim(substitute(str, "indexed", ""));
    type = trim(elementaryName(nextTokenClear(str, ' ')));
    if (contains(type, '['))
        is_flags |= IS_ARRAY;
    name = trim(str);
    if (contains(type, "+")) {
        while (contains(type, "(")) {
            replaceAll(type, "(", "<tuple>");
            replaceAll(type, ")", "</tuple>");
            string_q tuple = snagFieldClear(type, "tuple");
            CStringArray parts;
            explode(parts, tuple, '+');
            size_t cnt = 1;
            for (auto part : parts) {
                CParameter p;
                p.type = nextTokenClear(part, ' ');
                p.name = (part.empty() ? ("val_" + uint_2_Str(cnt++)) : part);
                components.push_back(p);
            }
            type = "tuple" + type;
        }
    }
    return true;
}

//-----------------------------------------------------------------------
bool CParameter::isValid(void) const {
    // TODO(tjayrush): not exhaustive
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
size_t explode(CParameterArray& result, const string& input, char needle) {
    CStringArray strs;
    explode(strs, input, needle);
    for (auto str : strs) {
        CParameter param(str);
        result.push_back(param);
    }
    return result.size();
}

//--------------------------------------------------------------------------------
string_q CParameter::resolveType(void) const {
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
