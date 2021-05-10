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
#include "commandoption.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCommandOption, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextCommandoptionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextCommandoptionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CCommandOption::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["commandoption_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextCommandoptionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCommandoptionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCommandOption*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CCommandOption::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextCommandoptionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "api_group") {
                return api_group;
            }
            if (fieldName % "api_route") {
                return api_route;
            }
            break;
        case 'c':
            if (fieldName % "command") {
                return command;
            }
            break;
        case 'd':
            if (fieldName % "def_val") {
                return def_val;
            }
            if (fieldName % "data_type") {
                return data_type;
            }
            if (fieldName % "description") {
                return description;
            }
            break;
        case 'g':
            if (fieldName % "group") {
                return group;
            }
            if (fieldName % "generate") {
                return generate;
            }
            break;
        case 'h':
            if (fieldName % "hotkey") {
                return hotkey;
            }
            break;
        case 'i':
            if (fieldName % "is_required") {
                return bool_2_Str(is_required);
            }
            if (fieldName % "is_customizable") {
                return bool_2_Str(is_customizable);
            }
            if (fieldName % "is_visible") {
                return bool_2_Str(is_visible);
            }
            if (fieldName % "is_visible_docs") {
                return bool_2_Str(is_visible_docs);
            }
            break;
        case 'n':
            if (fieldName % "num") {
                return num;
            }
            break;
        case 'o':
            if (fieldName % "option_type") {
                return option_type;
            }
            break;
        case 'r':
            if (fieldName % "real_type") {
                return real_type;
            }
            break;
        case 't':
            if (fieldName % "tool") {
                return tool;
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
bool CCommandOption::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "api_group") {
                api_group = fieldValue;
                return true;
            }
            if (fieldName % "api_route") {
                api_route = fieldValue;
                return true;
            }
            break;
        case 'c':
            if (fieldName % "command") {
                command = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "def_val") {
                def_val = fieldValue;
                return true;
            }
            if (fieldName % "data_type") {
                data_type = fieldValue;
                return true;
            }
            if (fieldName % "description") {
                description = fieldValue;
                return true;
            }
            break;
        case 'g':
            if (fieldName % "group") {
                group = fieldValue;
                return true;
            }
            if (fieldName % "generate") {
                generate = fieldValue;
                return true;
            }
            break;
        case 'h':
            if (fieldName % "hotkey") {
                hotkey = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "is_required") {
                is_required = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_customizable") {
                is_customizable = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_visible") {
                is_visible = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_visible_docs") {
                is_visible_docs = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "num") {
                num = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "option_type") {
                option_type = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "real_type") {
                real_type = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "tool") {
                tool = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCommandOption::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCommandOption::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> num;
    archive >> group;
    archive >> api_group;
    archive >> api_route;
    archive >> tool;
    archive >> command;
    archive >> hotkey;
    archive >> def_val;
    archive >> is_required;
    archive >> is_customizable;
    archive >> is_visible;
    archive >> is_visible_docs;
    archive >> generate;
    archive >> option_type;
    archive >> data_type;
    archive >> real_type;
    archive >> description;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCommandOption::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << num;
    archive << group;
    archive << api_group;
    archive << api_route;
    archive << tool;
    archive << command;
    archive << hotkey;
    archive << def_val;
    archive << is_required;
    archive << is_customizable;
    archive << is_visible;
    archive << is_visible_docs;
    archive << generate;
    archive << option_type;
    archive << data_type;
    archive << real_type;
    archive << description;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCommandOptionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CCommandOptionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCommandOption::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CCommandOption, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CCommandOption, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCommandOption, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CCommandOption, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CCommandOption, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CCommandOption, "num", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "group", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "api_group", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "api_route", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "tool", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "command", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "hotkey", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "def_val", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_required", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_customizable", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_visible", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_visible_docs", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "generate", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "option_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "data_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "real_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "description", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCommandOption, "schema");
    HIDE_FIELD(CCommandOption, "deleted");
    HIDE_FIELD(CCommandOption, "showing");
    HIDE_FIELD(CCommandOption, "cname");

    builtIns.push_back(_biCCommandOption);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCommandoptionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CCommandOption* com = reinterpret_cast<const CCommandOption*>(dataPtr);
    if (com) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if (fieldIn % "datatype") {
                    if (com->option_type == "switch" || com->option_type == "toggle")
                        return "";
                    return (startsWith(com->data_type, "opt_") ? "\"+" + com->data_type + "+\"" : com->data_type);
                }
                break;
            case 'o':
                if (fieldIn % "opts") {
                    string_q ret;
                    if (com->is_required)
                        ret += ("|OPT_REQUIRED");

                    if (!(com->is_visible))
                        ret += ("|OPT_HIDDEN");

                    if (com->option_type == "switch")
                        ret += ("|OPT_SWITCH");
                    else if (com->option_type == "toggle")
                        ret += ("|OPT_TOGGLE");
                    else if (com->option_type == "flag")
                        ret += ("|OPT_FLAG");
                    else if (startsWith(com->option_type, "deprecated"))
                        ret += ("|OPT_DEPRECATED");
                    else if (com->option_type == "positional")
                        ret += ("|OPT_POSITIONAL");
                    else if (com->option_type == "note")
                        ret = com->description;
                    else if (com->option_type == "error")
                        ret = com->description;
                    else
                        ret += ("|OPT_DESCRIPTION");
                    return substitute(trim(ret, '|'), "|", " | ");
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, com);
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
bool CCommandOption::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCommandOption& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_COMMANDOPTION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------------------------------
CCommandOption::CCommandOption(const string_q& line) {
    CStringArray parts;
    explode(parts, line, ',');
    if (parts.size() > 0)
        num = parts[0];
    if (parts.size() > 1)
        group = parts[1];
    if (parts.size() > 2)
        api_group = parts[2];
    if (parts.size() > 3)
        api_route = parts[3];
    if (parts.size() > 4)
        tool = parts[4];
    if (parts.size() > 5)
        command = parts[5];
    if (parts.size() > 6)
        hotkey = parts[6];
    if (parts.size() > 7)
        def_val = substitute(substitute(parts[7], "TRUE", "true"), "FALSE", "false");
    if (parts.size() > 8)
        is_required = str_2_Bool(parts[8]);
    if (parts.size() > 9)
        is_customizable = str_2_Bool(parts[9]);
    if (parts.size() > 10)
        is_visible = str_2_Bool(parts[10]);
    if (parts.size() > 11)
        is_visible_docs = str_2_Bool(parts[11]);
    if (parts.size() > 12)
        generate = parts[12];
    if (parts.size() > 13)
        option_type = parts[13];
    if (parts.size() > 14)
        data_type = parts[14];
    if (parts.size() > 15)
        description = substitute(parts[15], "&#44;", ",");

    if (!def_val.empty() && (data_type == "<string>" || data_type == "<path>" || contains(data_type, "enum")))
        def_val = "\"" + def_val + "\"";

    description = substitute(description, "[{DEF}]",
                             (option_type == "toggle" ? (def_val == "true" ? "'on'" : "'off'") : def_val));
    if (def_val.empty() && !generate.empty()) {
        if (data_type == "<boolean>") {
            def_val = "false";
        } else if (data_type == "<string>" || data_type == "<path>" || contains(data_type, "enum")) {
            def_val = "\"\"";
        } else {
            def_val = "NOPOS";
        }
    }
    if (option_type == "description") {
        swagger_descr = trim(substitute(description, "|", "\n        "));
    } else if (option_type != "note" && option_type != "error") {
        swagger_descr = trim(substitute(description, "|", "\n          "));
    }
    if (option_type != "note" && option_type != "error") {
        description = trim(substitute(description, "|", " "));
    }

    isList = contains(data_type, "list<");
    isEnumList = contains(data_type, "list<enum");
    isStringList = contains(data_type, "list<string");
    isAddressList = contains(data_type, "list<addr");
    isTopicList = contains(data_type, "list<topic");
    isEnum = contains(data_type, "enum") && !isEnumList;
    isBool = contains(data_type, "boolean");
    isBlockNum = contains(data_type, "blknum");
    isUint32 = contains(data_type, "uint32");
    isUint64 = contains(data_type, "uint64");
    isDouble = contains(data_type, "double");
    isNote = option_type == "note";
    isError = option_type == "error";

    real_type = substituteAny(substitute(data_type, "boolean", "bool"), "<>", "");
    if (contains(data_type, "enum"))
        real_type = "string";
    replace(real_type, "blknum", "blknum_t");
    replace(real_type, "string", "string_q");
    replace(real_type, "uint32", "uint32_t");
    replace(real_type, "uint64", "uint64_t");
}

//---------------------------------------------------------------------------------------------------
void CCommandOption::verifyOptions(CStringArray& warnings) {
    // Check valid option kinds
    CStringArray validKinds = {
        "switch", "toggle", "flag", "deprecated", "positional", "description", "error", "note",
    };
    bool valid_kind = false;
    for (auto kind : validKinds) {
        if (kind == option_type) {
            valid_kind = true;
        }
    }
    ostringstream warnstream;
    if (!valid_kind)
        warnstream << "Skipping option kind '" << option_type << "' for option '" << command << "'|";

    // Check valid data types
    CStringArray validTypes = {
        "<addr>",   "<blknum>", "<pair>",    "<path>", "<range>",  "<string>",
        "<uint32>", "<uint64>", "<boolean>", "<path>", "<double>",
    };

    bool valid_type = false;
    for (auto type : validTypes) {
        if (type == data_type) {
            valid_type = true;
        }
    }
    if (!valid_type) {
        if (startsWith(data_type, "enum"))
            valid_type = true;
        if (startsWith(data_type, "list"))
            valid_type = true;
    }
    if (!valid_type && (option_type == "description" || option_type == "note" || option_type == "error") &&
        data_type.empty())
        valid_type = true;
    if (!valid_type && startsWith(data_type, "opt_"))
        valid_type = true;

    if (!valid_type)
        warnstream << "Unknown type '" << data_type << "' for option '" << command << "'|";
    if (option_type == "description" && !endsWith(description, ".") && !endsWith(description, ":"))
        warnstream << "Description '" << description << "' should end with a period or colon.|";
    if (option_type == "note" && !endsWith(description, ".") && !endsWith(description, ":"))
        warnstream << "Note '" << description << "' should end with a period or colon.|";
    if (option_type == "error" && !endsWith(description, ".") && !endsWith(description, ":"))
        warnstream << "Error string '" << description << "' should end with a period or colon.|";
    if ((option_type != "description" && option_type != "note" && option_type != "error") && endsWith(description, "."))
        warnstream << "Option '" << description << "' should not end with a period.|";
    if (isReserved(command))
        warnstream << "Option '" << command << "' is a reserved word.|";

    explode(warnings, warnstream.str(), '|');
}

//---------------------------------------------------------------------------------------------------
void CCommandOption::verifyHotkey(CStringArray& warnings) {
    if (hotkey.empty() || contains(option_type, "positional") || contains(option_type, "description") ||
        contains(option_type, "note") || !contains(option_type, "error")) {
        return;
    }

    ostringstream warnstream;
    if (hotkey == "v")
        warnstream << tool << ":hotkey '" << command << "-" << hotkey << "' conflicts with --verbose hotkey|";
    if (hotkey == "h")
        warnstream << tool << ":hotkey '" << command << "-" << hotkey << "' conflicts with --help hotkey|";
    if (hotkey == "x")
        warnstream << tool << ":hotkey '" << command << "-" << hotkey << "' conflicts with --fmt hotkey|";

    static map<string, string> shortCmds;
    if (!shortCmds[hotkey].empty())
        warnstream << "Hotkey '" << command << "-" << hotkey << "' conflicts with existing '" << shortCmds[hotkey]
                   << "'|";
    shortCmds[hotkey] = command + "-" + hotkey;  // store for later to find dups

    bool isUpper = (toLower(hotkey) != hotkey);
    bool isFirst = hotkey == command.substr(0, 1);
    bool isSecond = hotkey == command.substr(1, 1);
    bool isContained = contains(command, hotkey);
    if (!isUpper && !isFirst && !isSecond && (!verbose && !isContained)) {
        warnstream << "Hotkey '" << hotkey << "' ";
        warnstream << "of command '" << command << "' is not first or second character|";
    }

    explode(warnings, warnstream.str(), '|');
}

//---------------------------------------------------------------------------------------------------
extern const char* STR_DEBUG_DISPLAY;
extern const char* STR_DEBUG_DISPLAY_BOOL;
extern const char* STR_DEBUG_DISPLAY_LIST;
//---------------------------------------------------------------------------------------------------
string_q CCommandOption::debugCode(void) const {
    string_q fmt = isBool ? STR_DEBUG_DISPLAY_BOOL : isList ? STR_DEBUG_DISPLAY_LIST : STR_DEBUG_DISPLAY;
    if (command == "addrs2")
        replaceAll(fmt, "[{COMMAND}]", "addrs");
    if (command == "transactions")
        replaceAll(fmt, "[{COMMAND}]", "transList");
    return Format(fmt);
}

//---------------------------------------------------------------------------------------------------
const char* STR_DEBUG_DISPLAY = "    LOG_TEST(\"[{COMMAND}]\", [{COMMAND}], ([{COMMAND}] == [{DEF_VAL}]));";
const char* STR_DEBUG_DISPLAY_BOOL = "    LOG_TEST_BOOL(\"[{COMMAND}]\", [{COMMAND}]);";
const char* STR_DEBUG_DISPLAY_LIST = "    LOG_TEST_LIST(\"[{COMMAND}]\", [{COMMAND}], [{COMMAND}].empty());";
// EXISTING_CODE
}  // namespace qblocks
