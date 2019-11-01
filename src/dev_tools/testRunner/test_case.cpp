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
#include "test_case.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTestCase, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTestcaseChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextTestcaseChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTestCase::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["testcase_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTestcaseChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTestcaseChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTestCase *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTestCase::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "builtin" ) { builtin = str_2_Bool(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "filename" ) { filename = fieldValue; return true; }
            if ( fieldName % "fileName" ) { fileName = fieldValue; return true; }
            break;
        case 'g':
            if ( fieldName % "goldPath" ) { goldPath = fieldValue; return true; }
            break;
        case 'm':
            if ( fieldName % "mode" ) { mode = fieldValue; return true; }
            break;
        case 'o':
            if ( fieldName % "origLine" ) { origLine = fieldValue; return true; }
            if ( fieldName % "onOff" ) { onOff = fieldValue; return true; }
            if ( fieldName % "options" ) { options = fieldValue; return true; }
            if ( fieldName % "optTool" ) { optTool = fieldValue; return true; }
            break;
        case 'p':
            if ( fieldName % "post" ) { post = fieldValue; return true; }
            if ( fieldName % "path" ) { path = fieldValue; return true; }
            break;
        case 'r':
            if ( fieldName % "route" ) { route = fieldValue; return true; }
            break;
        case 's':
            if ( fieldName % "speed" ) { speed = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "tool" ) { tool = fieldValue; return true; }
            break;
        case 'w':
            if ( fieldName % "workPath" ) { workPath = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTestCase::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTestCase::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> origLine;
    archive >> builtin;
    archive >> onOff;
    archive >> mode;
    archive >> speed;
    archive >> route;
    archive >> tool;
    archive >> filename;
    archive >> post;
    archive >> options;
    archive >> optTool;
    archive >> path;
    archive >> goldPath;
    archive >> workPath;
    archive >> fileName;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTestCase::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << origLine;
    archive << builtin;
    archive << onOff;
    archive << mode;
    archive << speed;
    archive << route;
    archive << tool;
    archive << filename;
    archive << post;
    archive << options;
    archive << optTool;
    archive << path;
    archive << goldPath;
    archive << workPath;
    archive << fileName;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTestCaseArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTestCaseArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTestCase::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTestCase, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTestCase, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTestCase, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTestCase, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTestCase, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CTestCase, "origLine", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "builtin", T_BOOL, ++fieldNum);
    ADD_FIELD(CTestCase, "onOff", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "mode", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "speed", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "route", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "tool", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "filename", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "post", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "options", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "optTool", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "path", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "goldPath", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "workPath", T_TEXT, ++fieldNum);
    ADD_FIELD(CTestCase, "fileName", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTestCase, "schema");
    HIDE_FIELD(CTestCase, "deleted");
    HIDE_FIELD(CTestCase, "showing");
    HIDE_FIELD(CTestCase, "cname");

    builtIns.push_back(_biCTestCase);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTestcaseChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CTestCase *tes = reinterpret_cast<const CTestCase *>(dataPtr);
    if (tes) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tes);
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
bool CTestCase::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CTestCase::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextTestcaseChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "builtin" ) return bool_2_Str_t(builtin);
            break;
        case 'f':
            if ( fieldName % "filename" ) return filename;
            if ( fieldName % "fileName" ) return fileName;
            break;
        case 'g':
            if ( fieldName % "goldPath" ) return goldPath;
            break;
        case 'm':
            if ( fieldName % "mode" ) return mode;
            break;
        case 'o':
            if ( fieldName % "origLine" ) return origLine;
            if ( fieldName % "onOff" ) return onOff;
            if ( fieldName % "options" ) return options;
            if ( fieldName % "optTool" ) return optTool;
            break;
        case 'p':
            if ( fieldName % "post" ) return post;
            if ( fieldName % "path" ) return path;
            break;
        case 'r':
            if ( fieldName % "route" ) return route;
            break;
        case 's':
            if ( fieldName % "speed" ) return speed;
            break;
        case 't':
            if ( fieldName % "tool" ) return tool;
            break;
        case 'w':
            if ( fieldName % "workPath" ) return workPath;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTestCase& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TESTCASE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------------------------
CStringArray commands = { "COPYFILE|cp", "RMFILE|rm", "MOVEFILE|mv" };

//-----------------------------------------------------------------------
bool prepareBuiltIn(string_q& options) {
    for (auto cmd : commands) {
        string_q match = nextTokenClear(cmd, '|');
        if (startsWith(options, match)) {
            ostringstream os;
            bool debug = false;
            if (debug)
               os << "pwd ; echo \"" << substitute(options, "\"", "'") << "\" ; ls -l ; ";
            os << options;
            if (debug)
               os << " ; ls -l ; ";
            options = os.str();
            replaceAll(options, match, cmd);
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------
CTestCase::CTestCase(const string_q& line) {
    origLine = line;

    CStringArray parts;
    explode(parts, line, ',');
    onOff    = parts.size() > 0 ? trim(parts[0]) : "";
    mode     = parts.size() > 1 ? trim(parts[1]) : "";
    speed    = parts.size() > 2 ? trim(parts[2]) : "";
    route    = parts.size() > 3 ? trim(parts[3]) : "";
    tool     = parts.size() > 4 ? trim(parts[4]) : "";
    filename = parts.size() > 5 ? trim(parts[5]) : "";
    post     = parts.size() > 6 ? trim(parts[6]) : "";
    options  = parts.size() > 7 ? trim(parts[7]) : "";
    optTool  = parts.size() > 8 ? trim(parts[8]) : "";

    path     = nextTokenClear(tool, '/');
    if (endsWith(path, "lib"))
    path = "libs/" + path;

    fileName = tool + "_" + filename + ".txt";

    replaceAll(post, "n", "");
    replaceAll(post, "y", getGlobalConfig("makeClass")->getConfigStr("settings", "json_pretty_print", "jq"));

    builtin = prepareBuiltIn(options);
    if (!builtin) {
        replaceAll(options, " = ", "="); replaceAll(options, "= ", "=");
        replaceAll(options, " & ", "&"); replaceAll(options, "& ", "&");
        replaceAll(options, " @ ", "@"); replaceAll(options, "@ ", "@");
    }
}

//---------------------------------------------------------------------------------------------
void CTestCase::prepareTest(bool cmdLine) {
    goldPath = "../../../gold/" + path + "/" + tool + "/";
    workPath = substitute(goldPath, "/gold/", "/working/");

    if (!builtin) { // order matters
        if (cmdLine) {
            CStringArray opts = {
                "val",
                "addrs",
                "blocks",
                "classes",
                "dates",
                "transactions",
                "terms",
                "functions",
                "modes",
            };
            options = "&" + options;
            for (auto opt : opts)
                replaceAll(options, "&" + opt + "=", " ");
            replaceAll(options, "%20", " ");
            replaceAll(options, "@", " -");
            replaceAll(options, "&", " --");
            replaceAll(options, "\\*", " \"*\"");
            replaceAll(options, "=", " ");
            if (trim(options) == "--" || startsWith(trim(options), "-- "))
                replace(options, "--", "");

        } else {
            if (tool == "chifra")
                nextTokenClear(options, '&');
            replaceAll(options, "@", "");
            replaceAll(options, " ", "%20");
            goldPath += "api_tests/";
            workPath += "api_tests/";
        }
    }

    string_q removePath = workPath + fileName; // order matters
    if (fileExists(removePath))
        ::remove(removePath.c_str());

    if (!optTool.empty()) // order matters
        tool = optTool;
}
// EXISTING_CODE
}  // namespace qblocks

