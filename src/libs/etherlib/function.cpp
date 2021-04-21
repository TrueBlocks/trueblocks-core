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
#include "function.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CFunction, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextFunctionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextFunctionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CFunction::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["function_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextFunctionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextFunctionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CFunction*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CFunction::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextFunctionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "anonymous") {
                return bool_2_Str(anonymous);
            }
            break;
        case 'c':
            if (fieldName % "constant") {
                return bool_2_Str(constant);
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                return encoding;
            }
            break;
        case 'i':
            if (fieldName % "inputs" || fieldName % "inputsCnt") {
                size_t cnt = inputs.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += inputs[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if (fieldName % "inputs_dict") {
                for (size_t i = 0; i < inputs.size(); i++)
                    ((CFunction*)this)->inputs_dict[inputs[i].name] = inputs[i].value;
                ostringstream os;
                JsonWriter writer;
                writer.writeJson(os, inputs_dict);
                string_q str = os.str();
                while (startsWith(str, '\n') || startsWith(str, ' '))
                    str = trim(trim(str, '\n'), ' ');
                return str;
            }
            break;
        case 'm':
            if (fieldName % "message") {
                return message;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 'o':
            if (fieldName % "outputs" || fieldName % "outputsCnt") {
                size_t cnt = outputs.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += outputs[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if (fieldName % "outputs_dict") {
                for (size_t i = 0; i < outputs.size(); i++)
                    ((CFunction*)this)->outputs_dict[outputs[i].name] = outputs[i].value;
                ostringstream os;
                JsonWriter writer;
                writer.writeJson(os, outputs_dict);
                string_q str = os.str();
                while (startsWith(str, '\n') || startsWith(str, ' '))
                    str = trim(trim(str, '\n'), ' ');
                return str;
            }
            break;
        case 's':
            if (fieldName % "stateMutability") {
                return stateMutability;
            }
            if (fieldName % "signature") {
                return signature;
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
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
bool CFunction::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "signature") {
        signature = getSignature(SIG_CANONICAL);
        return true;
    } else if (fieldName % "name") {
        name = fieldValue;
        return true;
    } else if (fieldName % "inputs") {
        uint64_t cnt = 0;
        CParameter item;
        string_q str = fieldValue;
        while (item.parseJson3(str)) {
            if (item.name.empty())
                item.name = "val_" + uint_2_Str(cnt++);
            inputs.push_back(item);
            item = CParameter();
        }
        return true;
    } else if (fieldName % "outputs") {
        uint64_t cnt = 0;
        CParameter item;
        string_q str = fieldValue;
        while (item.parseJson3(str)) {
            if (item.name.empty())
                item.name = "val_" + uint_2_Str(cnt++);
            outputs.push_back(item);
            item = CParameter();  // reset
        }
        return true;
    } else if (fieldName % "payable" && fieldValue == "true") {
        stateMutability += "payable";
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "anonymous") {
                anonymous = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "constant") {
                constant = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                encoding = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "inputs") {
                CParameter obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    inputs.push_back(obj);
                    obj = CParameter();  // reset
                }
                return true;
            }
            if (fieldName % "inputs_dict") {
                inputs_dict = fieldValue;
                return true;
            }
            break;
        case 'm':
            if (fieldName % "message") {
                message = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "outputs") {
                CParameter obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    outputs.push_back(obj);
                    obj = CParameter();  // reset
                }
                return true;
            }
            if (fieldName % "outputs_dict") {
                outputs_dict = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "stateMutability") {
                stateMutability = fieldValue;
                return true;
            }
            if (fieldName % "signature") {
                signature = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CFunction::finishParse() {
    // EXISTING_CODE
    signature = getSignature(SIG_CANONICAL);
    encoding = encodeItem();
    if (stateMutability.empty())
        stateMutability = "nonpayable";
    // The parameters need to have a name. If not, we provide one
    int cnt = 0;
    for (auto& input : inputs) {
        if (input.name.empty())
            input.name = "val_" + int_2_Str(cnt++);
        input.finishParse();
    }
    cnt = 0;
    for (auto& output : outputs) {
        if (output.name.empty())
            output.name = "out_" + int_2_Str(cnt++);
        output.finishParse();
    }
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CFunction::Serialize(CArchive& archive) {
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
    archive >> type;
    archive >> anonymous;
    archive >> constant;
    archive >> stateMutability;
    archive >> signature;
    archive >> encoding;
    // archive >> message;
    archive >> inputs;
    archive >> outputs;
    // archive >> inputs_dict;
    // archive >> outputs_dict;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CFunction::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << type;
    archive << anonymous;
    archive << constant;
    archive << stateMutability;
    archive << signature;
    archive << encoding;
    // archive << message;
    archive << inputs;
    archive << outputs;
    // archive << inputs_dict;
    // archive << outputs_dict;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CFunctionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CFunctionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CFunction::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CFunction, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CFunction, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CFunction, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CFunction, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CFunction, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CFunction, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "anonymous", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "constant", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "stateMutability", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "signature", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "encoding", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "message", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CFunction, "message");
    ADD_FIELD(CFunction, "inputs", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "outputs", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CFunction, "inputs_dict", T_JSONVAL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CFunction, "inputs_dict");
    ADD_FIELD(CFunction, "outputs_dict", T_JSONVAL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CFunction, "outputs_dict");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CFunction, "schema");
    HIDE_FIELD(CFunction, "deleted");
    HIDE_FIELD(CFunction, "showing");
    HIDE_FIELD(CFunction, "cname");

    builtIns.push_back(_biCFunction);

    // EXISTING_CODE
    ADD_FIELD(CFunction, "input_names", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CFunction, "input_names");
    ADD_FIELD(CFunction, "output_names", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CFunction, "output_names");
    ADD_FIELD(CFunction, "declaration", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CFunction, "declaration");
    HIDE_FIELD(CFunction, "anonymous");
    if (useDict()) {
        HIDE_FIELD(CFunction, "inputs");
        SHOW_FIELD(CFunction, "inputs_dict")
        HIDE_FIELD(CFunction, "outputs");
        SHOW_FIELD(CFunction, "outputs_dict")
    }
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextFunctionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CFunction* fun = reinterpret_cast<const CFunction*>(dataPtr);
    if (fun) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'h':
                if (fieldIn % "hex") {
                    string_q ret = fun->name + "(";
                    for (size_t i = 0; i < fun->inputs.size(); i++) {
                        ret += fun->inputs[i].resolveType();
                        if (i < fun->inputs.size())
                            ret += ",";
                    }
                    ret += ")";
                    replace(ret, ",)", ")");
                    return (ret + "\t" + chr_2_HexStr(ret));
                }
                break;
            case 'd':
                if (fieldIn % "declaration") {
                    return fun->getSignature(SIG_FTYPE | SIG_FNAME | SIG_ITYPE | SIG_INAME | SIG_IINDEXED);
                }
                break;
            case 'i':
                if (fieldIn % "input_names") {
                    string_q ret;
                    for (size_t i = 0; i < fun->inputs.size(); i++) {
                        ret += fun->inputs[i].name;
                        if (i < fun->inputs.size())
                            ret += ",";
                    }
                    return ret;
                }
                break;
            case 'o':
                if (fieldIn % "output_names") {
                    string_q ret;
                    for (size_t i = 0; i < fun->outputs.size(); i++) {
                        ret += fun->outputs[i].name;
                        if (i < fun->outputs.size())
                            ret += ",";
                    }
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, fun);
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
bool CFunction::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CFunction& fun) {
    fun.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CFunction& fun) {
    fun.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CFunction& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CFunction::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "inputs") {
        if (index == NOPOS) {
            CParameter empty;
            ((CFunction*)this)->inputs.push_back(empty);  // NOLINT
            index = inputs.size() - 1;
        }
        if (index < inputs.size())
            return &inputs[index];
    }

    if (fieldName % "outputs") {
        if (index == NOPOS) {
            CParameter empty;
            ((CFunction*)this)->outputs.push_back(empty);  // NOLINT
            index = outputs.size() - 1;
        }
        if (index < outputs.size())
            return &outputs[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_FUNCTION =
    "[{NAME}]\t"
    "[{TYPE}]\t"
    "[{STATEMUTABILITY}]\t"
    "[{SIGNATURE}]\t"
    "[{ENCODING}]\t"
    "[{INPUTS}]\t"
    "[{OUTPUTS}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
string_q CFunction::getSignature(uint64_t parts) const {
    size_t cnt = inputs.size();

    size_t tl = type.length();
    size_t nl = name.length();
    size_t v1 = 35 - tl;
    size_t ll = (nl > v1 ? 0 : v1 - nl);

    ostringstream os;
    os << (parts & SIG_FTYPE ? "\t" + type + " " : "");
    os << (parts & SIG_FNAME ? name : "");
    os << (parts & SIG_FSPACE ? string_q(ll, ' ') : "");
    os << (parts & SIG_FTYPE || parts & SIG_FNAME ? "(" : "");
    for (size_t j = 0; j < cnt; j++) {
        os << (parts & SIG_ITYPE ? inputs[j].resolveType() : "");
        os << (parts & SIG_IINDEXED ? (inputs[j].indexed ? " indexed" : "") : "");
        os << (parts & SIG_INAME ? " " + inputs[j].name : "");
        os << (parts & SIG_ITYPE ? (j < cnt - 1 ? "," : "") : "");
    }
    os << (parts & SIG_FTYPE || parts & SIG_FNAME ? ")" : "");
    if (parts == SIG_ENCODE)
        os << (parts & SIG_ENCODE ? (parts & SIG_FNAME ? " " + encoding : encoding + " ") : "");
    else
        os << (parts & SIG_ENCODE ? " [" + encoding + "]" : "");
    if (verbose && parts != SIG_CANONICAL) {
        os << (anonymous ? " anonymous" : "");
        os << (constant ? " constant" : "");
        os << (contains(substitute(stateMutability, "nonpayable", ""), "payable") ? " payable" : "");
    }

    string_q ret = os.str().c_str();
    replaceAll(ret, " )", ")");
    return trim(ret);
}

//-----------------------------------------------------------------------
bool CFunction::checkTypes(void) const {
    for (auto input : inputs) {
        if (!input.isValid())
            return false;
    }
    return true;
}

//-----------------------------------------------------------------------
string_q CFunction::encodeItem(void) const {
    if (name == "")
        return "";
    // string_q ret = keccak256(substitute(signature, "%", ","));
    string_q ret = keccak256(signature);
    return (type == "event" ? ret : extract(ret, 0, 10));
}

//-------------------------------------------------------------------------
string_q compressInput(const string_q& inputIn) {
    if (inputIn.empty())
        return "";
    string_q input = (startsWith(inputIn, "0x") ? "" : "0x") + inputIn;
    string_q name = input.substr(0, 10);
    replace(input, name, "");
    string_q ret = name + " ( ";
    while (!input.empty()) {
        string_q chunk = input.substr(0, 364);
        replace(input, chunk, "");
        ret += ("stub: " + chunk + ", ");
    }
    ret = trim(trim(ret, ' '), ',');
    ret += " )";
    return ret;
}

//-----------------------------------------------------------------------
const char* STR_COMPRESSED_FMT = "[{NAME}](++INPUTS++);";
const char* STR_COMPRESSED_INPUT_QUOTED = "\"[{VALUE}]\" /*[{NAME}]*/, ";
const char* STR_COMPRESSED_INPUT = "[{VALUE}] /*[{NAME}]*/, ";

//-----------------------------------------------------------------------
string_q CFunction::compressed(const string_q& def) const {
    if (!message.empty())
        return "message:" + message;

    if (name.empty())
        return compressInput(def);

    string_q ret;
    if (isApiMode()) {
        ret = name + " ( ";
        for (auto input : inputs)
            ret += (input.name + ": " + input.value + ", ");
        ret = trim(trim(ret, ' '), ',');
        ret += " )";
        replaceAll(ret, "--tuple--", "");
        return stripWhitespace(ret);
    }

    ostringstream func, inp;
    func << Format(STR_COMPRESSED_FMT) << endl;
    for (auto input : inputs)
        inp << input.Format(substitute(STR_COMPRESSED_INPUT, " /*[{NAME}]*/", (verbose ? " /*[{NAME}]*/" : "")));
    ret = func.str();
    replace(ret, "++INPUTS++", trim(trim(inp.str(), ' '), ','));
    if (ret.empty())
        return compressInput(def);
    replaceAll(ret, "--tuple--", "");
    return stripWhitespace(ret);
}

//-----------------------------------------------------------------------
bool CFunction::isDefault(const CBaseNode* obj) const {
    return operator==(*(const CFunction*)obj);
}
// EXISTING_CODE
}  // namespace qblocks
