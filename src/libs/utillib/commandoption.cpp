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
extern string_q nextCommandoptionChunk(const string_q& fieldIn, const void* dataPtr);
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
            if (fieldName % "go_type") {
                return go_type;
            }
            if (fieldName % "go_flagtype") {
                return go_flagtype;
            }
            break;
        case 'h':
            if (fieldName % "hotKey") {
                return hotKey;
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
        case 'l':
            if (fieldName % "longName") {
                return longName;
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
        case 's':
            if (fieldName % "summary") {
                return summary;
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
    if (fieldName % "description")
        fieldValue = substitute(fieldValue, "&#44;", ",");
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
            if (fieldName % "go_type") {
                go_type = fieldValue;
                return true;
            }
            if (fieldName % "go_flagtype") {
                go_flagtype = fieldValue;
                return true;
            }
            break;
        case 'h':
            if (fieldName % "hotKey") {
                hotKey = fieldValue;
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
        case 'l':
            if (fieldName % "longName") {
                longName = fieldValue;
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
        case 's':
            if (fieldName % "summary") {
                summary = fieldValue;
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
    finishCleanup();
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
    archive >> longName;
    archive >> hotKey;
    archive >> def_val;
    archive >> is_required;
    archive >> is_customizable;
    archive >> is_visible;
    archive >> is_visible_docs;
    archive >> generate;
    archive >> option_type;
    archive >> data_type;
    archive >> real_type;
    archive >> go_type;
    archive >> go_flagtype;
    archive >> summary;
    archive >> description;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCommandOption::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << num;
    archive << group;
    archive << api_group;
    archive << api_route;
    archive << tool;
    archive << longName;
    archive << hotKey;
    archive << def_val;
    archive << is_required;
    archive << is_customizable;
    archive << is_visible;
    archive << is_visible_docs;
    archive << generate;
    archive << option_type;
    archive << data_type;
    archive << real_type;
    archive << go_type;
    archive << go_flagtype;
    archive << summary;
    archive << description;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCommandOption::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CCommandOption copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
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
    ADD_FIELD(CCommandOption, "longName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "hotKey", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "def_val", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_required", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_customizable", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_visible", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "is_visible_docs", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "generate", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "option_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "data_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "real_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "go_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "go_flagtype", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCommandOption, "summary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
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

                    if (com->generate == "config")
                        ret = com->description;
                    else if (com->option_type == "switch")
                        ret += ("|OPT_SWITCH");
                    else if (com->option_type == "toggle")
                        ret += ("|OPT_TOGGLE");
                    else if (com->option_type == "flag")
                        ret += ("|OPT_FLAG");
                    else if (com->option_type == "deprecated")
                        ret += ("|OPT_DEPRECATED");
                    else if (com->option_type == "positional")
                        ret += ("|OPT_POSITIONAL");
                    else if (com->option_type == "note")
                        ret = com->description;
                    else if (com->option_type == "config")
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CCommandOption::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CCommandOption& com) {
    com.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCommandOption& com) {
    com.Serialize(archive);
    return archive;
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
CCommandOption::CCommandOption(const string_q& lineIn) {
    static CStringArray commandFields;
    string_q line = lineIn;
    replaceAny(line, "\n\r", "");
    if (commandFields.empty()) {
        explode(commandFields, line, ',');
        return;
    }
    parseCSV(commandFields, line);
}

//---------------------------------------------------------------------------------------------------
static const CStringArray validOptionTypes = {"switch",     "toggle",      "flag",  "deprecated",
                                              "positional", "description", "error", "note"};

//---------------------------------------------------------------------------------------------------
bool CCommandOption::finishCleanup(void) {
    def_val = substitute(substitute(def_val, "TRUE", "true"), "FALSE", "false");
    description = substitute(description, "&#44;", ",");

    if (!def_val.empty() && (data_type == "<string>" || data_type == "<path>" || contains(data_type, "enum")))
        def_val = "\"" + def_val + "\"";

    description = substitute(description, "[{DEF}]",
                             (option_type == "toggle" ? (def_val == "true" ? "'on'" : "'off'") : def_val));
    if (def_val.empty() && !generate.empty()) {
        if (data_type == "<boolean>") {
            def_val = "false";
        } else if (data_type == "<address>" || data_type == "<string>" || data_type == "<path>" ||
                   contains(data_type, "enum")) {
            def_val = "\"\"";
        } else {
            def_val = "NOPOS";
        }
    }

    if (option_type == "description") {
        swagger_descr = trim(substitute(description, "|", "\n        "));
    } else if (option_type != "note" && option_type != "error" && option_type != "config") {
        swagger_descr = trim(substitute(description, "|", "\n          "));
    }

    if (option_type != "note" && option_type != "error" && generate != "config") {
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
    isAddress = contains(data_type, "address");
    isNote = option_type == "note";
    isConfig = option_type == "config";
    isErr = option_type == "error";
    isConfig = generate == "config";
    isDeprecated = option_type == "deprecated";

    real_type = data_type;
    replaceAny(real_type, "<>", "");
    replace(real_type, "boolean", "bool");
    replace(real_type, "blknum", "blknum_t");
    replace(real_type, "string", "string_q");
    replace(real_type, "uint32", "uint32_t");
    replace(real_type, "uint64", "uint64_t");
    replace(real_type, "address", "address_t");
    if (startsWith(data_type, "enum") || startsWith(data_type, "list<enum"))  // in every case of enum
        real_type = "string_q";

    go_type = real_type;
    replace(go_type, "string_q", "string");
    replace(go_type, "address_t", "string");
    replace(go_type, "uint64_t", "uint64");
    replace(go_type, "uint32_t", "uint32");
    replace(go_type, "blknum_t", "uint64");
    replace(go_type, "double", "float64");
    if (startsWith(data_type, "list"))
        go_type = "[]string";

    go_flagtype = (go_type == "[]string" ? "StringSlice" : toProper(go_type)) + "VarP";

    return true;
}

//---------------------------------------------------------------------------------------------------
void CCommandOption::verifyOptions(CStringArray& warnings) {
    bool valid_kind = false;
    for (auto kind : validOptionTypes) {
        if (kind == option_type) {
            valid_kind = true;
        }
    }
    ostringstream warnstream;
    if (!valid_kind)
        warnstream << "Skipping option kind '" << option_type << "' for option '" << longName << "'|";

    // Check valid data types
    CStringArray validTypes = {
        "<addr>",   "<blknum>", "<pair>",    "<path>", "<range>",  "<string>",
        "<uint32>", "<uint64>", "<boolean>", "<path>", "<double>", "<address>",
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
    if (!valid_type &&
        (option_type == "description" || option_type == "note" || option_type == "error" || option_type == "config") &&
        data_type.empty())
        valid_type = true;
    if (!valid_type && startsWith(data_type, "opt_"))
        valid_type = true;

    if (!valid_type)
        warnstream << "Unknown type '" << data_type << "' for option '" << longName << "'|";
    if (option_type == "description" && !endsWith(description, ".") && !endsWith(description, ":"))
        warnstream << "Description '" << description << "' should end with a period or colon.|";
    if (option_type == "note" && !endsWith(description, ".") && !endsWith(description, ":"))
        warnstream << "Note '" << description << "' should end with a period or colon.|";
    if (option_type == "config") {  // do nothing
        // warnstream << "Note '" << description << "' should end with a period or colon.|";
    }
    if (option_type == "error" && !endsWith(description, ".") && !endsWith(description, ":"))
        warnstream << "Error string '" << description << "' should end with a period or colon.|";
    if ((option_type != "description" && option_type != "note" && option_type != "error" && option_type != "config") &&
        endsWith(description, "."))
        warnstream << "Option '" << description << "' should not end with a period.|";
    if (isReserved(longName))
        warnstream << "Option '" << longName << "' is a reserved word.|";

    explode(warnings, warnstream.str(), '|');
}

//---------------------------------------------------------------------------------------------------
void CCommandOption::verifyHotkey(CStringArray& warnings, map<string, string> existing) {
    if (hotKey.empty() || contains(option_type, "positional") || contains(option_type, "description") ||
        contains(option_type, "note") || contains(option_type, "error")) {
        return;
    }

    ostringstream warnstream;
    if (hotKey == "v")
        warnstream << tool << ":hotKey '" << longName << "-" << hotKey << "' conflicts with --verbose hotKey|";
    if (hotKey == "h")
        warnstream << tool << ":hotKey '" << longName << "-" << hotKey << "' conflicts with --help hotKey|";
    if (hotKey == "x")
        warnstream << tool << ":hotKey '" << longName << "-" << hotKey << "' conflicts with --fmt hotKey|";

    const string_q HOTKEY_WARNING =
        "Hotkey (-[{HOTKEY}]) for tool '[{TOOL}]' at command '[{LONGNAME}]:[{HOTKEY}]' +MSG+|";
    string_q key = tool + ":" + hotKey;
    if (!existing[key].empty()) {
        string_q warn = Format(HOTKEY_WARNING);
        replace(warn, "+MSG+", "conflicts with existing '" + existing[key] + "'");
        warnstream << warn;
    }
    existing[key] = longName + ":" + hotKey;  // store for later to find dups

    bool isUpper = (toLower(hotKey) != hotKey);
    bool isFirst = hotKey == longName.substr(0, 1);
    bool isSecond = hotKey == longName.substr(1, 1);
    bool isContained = contains(longName, hotKey);
    bool isCache = contains(longName, "cache");  // special weird case -- just ignore it
    if (!isUpper && !isFirst && !isSecond && (!verbose && !isContained && !isCache)) {
        string_q warn = Format(HOTKEY_WARNING);
        replace(warn, "+MSG+", "is not first or second character");
        warnstream << warn;
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
    if (longName == "addrs2")
        replaceAll(fmt, "[{LONGNAME}]", "addrs");
    if (longName == "transactions")
        replaceAll(fmt, "[{LONGNAME}]", "transList");
    return Format(fmt);
}

//---------------------------------------------------------------------------------------------------
const char* STR_DEBUG_DISPLAY = "    LOG_TEST(\"[{LONGNAME}]\", [{LONGNAME}], ([{LONGNAME}] == [{DEF_VAL}]));";
const char* STR_DEBUG_DISPLAY_BOOL = "    LOG_TEST_BOOL(\"[{LONGNAME}]\", [{LONGNAME}]);";
const char* STR_DEBUG_DISPLAY_LIST = "    LOG_TEST_LIST(\"[{LONGNAME}]\", [{LONGNAME}], [{LONGNAME}].empty());";

//---------------------------------------------------------------------------------------------------
extern const char* STR_PATH_YAML;
extern const char* STR_PARAM_YAML;

//---------------------------------------------------------------------------------------------------
bool isApiRoute(const string_q& route) {
    if (route == "serve" || route == "explore")
        return false;
    return !route.empty();
}

//---------------------------------------------------------------------------------------------------
bool CCommandOption::isChifraRoute(bool depOk) const {
    if (depOk && option_type == "deprecated")
        return true;
    return (option_type != "deprecated" && option_type != "description" && option_type != "note" &&
            option_type != "config" && option_type != "error");
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toChifraCmd(void) const {
    if (api_route.empty())
        return Format("    // -- [{GROUP}]");
    return Format("    {\"[{API_ROUTE}]\", \"[{TOOL}]\"},");
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toChifraHelp(void) const {
    if (description.empty() && !api_route.empty())
        return "";

    CCommandOption ret = *this;
    replaceAll(ret.description, ".", "");
    ret.description = firstLower(ret.description);
    if (api_route.empty())
        return toUpper(ret.Format("    \"[{GROUP}]|\""));
    return ret.Format("    \"  [{w:14:API_ROUTE}][{DESCRIPTION}]|\"");
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toPairMap(void) const {
    if (is_visible || api_route == "explore") {
        if (!contains(tool, " "))
            return Format("    make_pair(\"[{TOOL}]\", \"chifra [{API_ROUTE}]\"),");
        return Format("    // [{API_ROUTE}]");

    } else {
        if (api_route.empty())
            return Format("    // -- [{GROUP}]");
        return Format("    // [{API_ROUTE}]");
    }

    return "";
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toApiTag(void) const {
    if (isApiRoute(tool) || !is_visible_docs)
        return "";
    const char* STR_TAG_YAML =
        "  - name: [{GROUP}]\n"
        "    description: [{DESCRIPTION}]\n";
    return Format(STR_TAG_YAML);
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toGoCall(void) const {
    if (!isApiRoute(api_route))
        return "";

    string_q goFunc = substitute(group, " ", "") + toProper(api_route);
    ostringstream out;
    out << endl;
    out << "// " << goFunc << " help text todo" << endl;
    out << "func " << goFunc << "(w http.ResponseWriter, r *http.Request) {" << endl;
    if (!tool.empty() && !contains(tool, " ")) {
        out << "\tCallOne(w, r, \"" << tool << "\", \"" << api_route << "\")" << endl;
    } else if (goFunc == "AccountsTags" || goFunc == "AccountsCollections") {
        out << "\tCallOne(w, r, \"ethNames\", \"" << api_route << "\")" << endl;
    } else {
        out << "\tCallOneExtra(w, r, \"chifra\", \"" << api_route << "\", \"" << api_route << "\")" << endl;
    }
    out << "}" << endl;
    return out.str();
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toGoRoute(void) const {
    if (!isApiRoute(api_route))
        return "";

    string_q goFunc = substitute(group, " ", "") + toProper(api_route);
    ostringstream out;
    out << endl;
    out << "\tRoute{" << endl;
    out << "\t\t\"" << goFunc << "\"," << endl;
    out << "\t\t\"GET\"," << endl;
    out << "\t\t\"/" << api_route << "\"," << endl;
    out << "\t\t" << goFunc << "," << endl;
    out << "\t}," << endl;
    return out.str();
}

//--------------------------------------------------------------------------------
string_q clean_positionals(const string_q& progName, const string_q& strIn) {
    if (contains(strIn, "<")) {
        ostringstream os;
        os << (strIn == "list<addr>" ? "<address> [address...]" : "");
        os << (strIn == "list<blknum>" ? "<block> [block...]" : "");
        os << (strIn == "list<tx_id>" ? "<tx_id> [tx_id...]" : "");
        os << (startsWith(strIn, "list<addr> list<topic> list<fourbyte>")
                   ? "<address> [address...] [topics...] [fourbytes...]"
                   : "");
        os << (startsWith(strIn, "list<enum") ? "[mode...]" : "");
        if (contains(toLower(progName), "tokens"))
            os << (strIn == "list<addr> list<blknum>" ? "<address> <address> [address...] [block...]" : "");
        else
            os << (strIn == "list<addr> list<blknum>" ? "<address> [address...] [block...]" : "");
        if (contains(toLower(progName), "when"))
            os << (strIn == "list<string>" ? "< block | date > [ block... | date... ]" : "");
        else
            os << (strIn == "list<string>" ? "<term> [term...]" : "");
        if (os.str().empty()) {
            cerr << "Could not convert " << strIn << " for tool " << progName << endl;
            os << strIn;
        }
        os << endl;
        return " " + os.str();
    }
    string_q strOut = strIn;
    replaceAll(strOut, "addrs2 blocks", "<address> <address> [address...] [block...]");
    replaceAll(strOut, "addrs blocks", "<address> [address...] [block...]");
    replaceAll(strOut, "transactions", "<tx_id> [tx_id...]");
    if (contains(progName, "when"))
        replaceAll(strOut, "blocks", "< block | date > [ block... | date... ]");
    else
        replaceAll(strOut, "blocks", "<block> [block...]");
    replaceAll(strOut, "addrs topics fourbytes", "<address> [address...] [topics] [fourbytes]");
    replaceAll(strOut, "addrs", "<address> [address...]");
    replaceAll(strOut, "files", "<file> [file...]");
    replaceAll(strOut, "terms", "<term> [term...]");
    replaceAll(strOut, "modes", "<mode> [mode...]");
    return trim(strOut);
}

//---------------------------------------------------------------------------------------------------
string_q prepareDescr(const string_q& in) {
    if (in.length() < 75)
        return in;
    return ">\n            " + substitute(in, "\n         ", "");
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::toApiPath(const string_q& inStr) const {
    if (!isApiRoute(api_route))
        return "";

    ostringstream paramStream;
    for (auto param : *(CCommandOptionArray*)params) {
        if (param.longName.empty() || !param.is_visible_docs)
            continue;
        string_q yp = STR_PARAM_YAML;
        replace(yp, "[{NAME}]", param.longName);
        replace(yp, "[{DESCR}]", prepareDescr(param.swagger_descr));
        replace(yp, "[{REQ}]", param.is_required ? "true" : "false");
        replace(yp, "[{SCHEMA}]", param.getSchema());
        if (paramStream.str().empty())
            paramStream << "      parameters:\n";
        paramStream << yp << endl;
    }

    string_q exampleFn = getDocsPathTemplates("api/examples/" + api_route + ".txt");

    ostringstream example;
    if (fileExists(exampleFn)) {
        string_q content = trim(asciiFileToString(exampleFn), '\n');
        if (!content.empty()) {
            if (!contains(content, string_q(18, ' '))) {
                replaceAll(content, "\n", "\n" + string_q(18, ' '));
                content = string_q(18, ' ') + content;
            }
            example << string_q(16, ' ') << "example:" << endl << content << endl;
        }
    }

    string_q content;
    ostringstream properties;
    string_q descr = inStr;
    string_q productions = nextTokenClear(descr, '|');
    const char* STR_PROPERTIES =
        "data:\n"
        "  description: [{DESCR}]\n"
        "  type: array\n"
        "  items:\n"
        "    [{PRODUCTIONS}]";
    content = substitute(substitute(STR_PROPERTIES, "[{PRODUCTIONS}]", productions), "[{DESCR}]", descr);

    if (!content.empty()) {
        if (!contains(content, string_q(18, ' '))) {
            replaceAll(content, "\n", "\n" + string_q(18, ' '));
            content = string_q(18, ' ') + content;
        }
        properties << string_q(16, ' ') << "properties:" << endl << content << endl;

    } else {
        if (!fileExists(exampleFn))
            properties << string_q(16, ' ') << "items:\n                  $ref: \"#/components/schemas/response\"\n";
    }

    string_q grp = substitute(substitute(group, "ChainData", "Chain Data"), "ChainState", "Chain State");

    string_q ret = STR_PATH_YAML;
    replaceAll(ret, "[{TAGS}]", grp);
    replaceAll(ret, "[{PROPERTIES}]", properties.str());
    replaceAll(ret, "[{EXAMPLE}]", example.str());
    replaceAll(ret, "[{PATH}]", api_route);
    replaceAll(ret, "[{PARAMS}]", paramStream.str());
    replaceAll(ret, "[{SUMMARY}]", summary);
    replaceAll(ret, "[{DESCR}]", description);
    replaceAll(ret, "[{ID}]", toLower(substitute(grp, " ", "") + "-" + api_route));
    return ret;
}

//---------------------------------------------------------------------------------------------------
bool visitEnumItem(string_q& item, void* data) {
    ostringstream* osp = (ostringstream*)data;
    if (isNumeral(item)) {
        *osp << substitute("~- \"[{VAL}]\"\n", "[{VAL}]", item);
    } else {
        *osp << substitute("~- [{VAL}]\n", "[{VAL}]", item);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
string_q CCommandOption::getSchema(void) const {
    string_q lead = "            ";

    if (contains(data_type, "list")) {
        if (contains(data_type, "enum")) {
            ostringstream os;
            forEveryEnum(visitEnumItem, data_type, &os);
            string_q str_array_enum =
                "~type: array\n"
                "~items:\n"
                "~  type: string\n"
                "~  enum:\n" +
                substitute(substitute(trim(os.str(), '\n'), "~", "+    "), "+", "~");
            return substitute(str_array_enum, "~", lead);
        }

        string_q ret;
        ret += lead + "type: array\n";
        ret += lead + "items:\n";
        ret += lead + "  type: string\n";
        string_q type = substitute(substitute(data_type, "list<", ""), ">", "");
        replace(type, "addr", "address_t");
        if (!endsWith(type, "_t"))
            type += "_t";
        ret += lead + "  format: " + type;
        return ret;
    }

    if (contains(data_type, "boolean")) {
        return lead + "type: " + "boolean";

    } else if (contains(data_type, "uint") || contains(data_type, "double")) {
        return lead + "type: " + "number";

    } else if (contains(data_type, "enum")) {
        ostringstream os;
        forEveryEnum(visitEnumItem, data_type, &os);
        string_q str_array_enum = lead + "type: string\n" + lead + "enum:\n";
        str_array_enum += substitute(substitute(trim(os.str(), '\n'), "~", "+  "), "+", "~");
        return substitute(str_array_enum, "~", lead);
    }

    return lead + "type: " + "string";
}

//---------------------------------------------------------------------------------------------------
bool forEveryEnum(APPLYFUNC func, const string_q& enumStr, void* data) {
    string_q es = substitute(substitute(enumStr, "list<", ""), ">", "");
    string_q cleaned = substitute(substitute(substitute(es, "*", ""), "enum[", ""), "]", "");
    CStringArray items;
    explode(items, cleaned, '|');
    for (auto item : items) {
        if (func) {
            if (!(*func)(item, data))
                return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
const char* STR_PATH_YAML =
    "  /[{PATH}]:\n"
    "    get:\n"
    "      tags:\n"
    "        - [{TAGS}]\n"
    "      summary: [{SUMMARY}]\n"
    "      description: [{DESCR}]\n"
    "      operationId: [{ID}]\n"
    "[{PARAMS}]"
    "      responses:\n"
    "        \"200\":\n"
    "          description: returns the requested data\n"
    "          content:\n"
    "            application/json:\n"
    "              schema:\n"
    "[{PROPERTIES}][{EXAMPLE}]        \"400\":\n"
    "          description: bad input parameter\n";

//---------------------------------------------------------------------------------------------------
const char* STR_PARAM_YAML =
    "        - name: [{NAME}]\n"
    "          description: [{DESCR}]\n"
    "          required: [{REQ}]\n"
    "          style: form\n"
    "          in: query\n"
    "          explode: true\n"
    "          schema:\n"
    "[{SCHEMA}]";
// EXISTING_CODE
}  // namespace qblocks
