#pragma once
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
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"
#include "commandoption.h"
#include "classdefinition.h"

// BEG_ERROR_DEFINES
#define ERR_CLASSDEFNOTEXIST 1
#define ERR_CONFIGMISSING 2
#define ERR_NEEDONECLASS 3
// END_ERROR_DEFINES

//-------------------------------------------------------------------
class CCounter {
  public:
    size_t fileCount;
    size_t nVisited;
    size_t nProcessed;
    bool is_counting;
    CCounter(void) : fileCount(0), nVisited(0), nProcessed(0), is_counting(true) {
    }
};

//-------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    bool all;
    bool sdk;
    bool openapi;
    // END_CODE_DECLARE

    CClassDefinitionArray classDefs;
    CClassDefinitionArray dataModels;
    CCommandOptionArray cmdOptionArray;
    CCommandOptionArray routeOptionArray;
    CStringBoolMap toolMap;
    CStringBoolMap cmdExistsMap;
    CStringArray positionals;
    CToml classFile;
    CCounter counter;
    timestamp_t lastFormat;
    timestamp_t lastLint;
    CCommandOptionArray endpointArray;
    map<string_q, string_q> hugoAliasMap;

    ostringstream optionStream, initStream, localStream, autoStream, headerStream, configStream;
    ostringstream notesStream, errorStrStream, errorDefStream, goCallStream, goPkgStream, goConvertStream;
    ostringstream goRouteStream, chifraHelpStream;
    ostringstream apiTagStream, apiPathStream;
    ostringstream goStream;

    void clearStreams(void) {
        optionStream.str("");
        initStream.str("");
        localStream.str("");
        autoStream.str("");
        headerStream.str("");
        configStream.str("");
        notesStream.str("");
        errorStrStream.str("");
        errorDefStream.str("");
        goCallStream.str("");
        goConvertStream.str("");
        goPkgStream.str("");
        goRouteStream.str("");
        chifraHelpStream.str("");
        apiTagStream.str("");
        apiPathStream.str("");
        goStream.str("");

        optionStream.clear();
        initStream.clear();
        localStream.clear();
        autoStream.clear();
        headerStream.clear();
        configStream.clear();
        notesStream.clear();
        errorStrStream.clear();
        errorDefStream.clear();
        goCallStream.clear();
        goConvertStream.clear();
        goPkgStream.clear();
        goRouteStream.clear();
        chifraHelpStream.clear();
        apiTagStream.clear();
        apiPathStream.clear();
        positionals.clear();
        goStream.clear();
    }

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_readmes(void);
    bool handle_options(void);
    bool handle_gocmds(void);
    bool handle_lint(void);
    bool handle_format(void);
    bool handle_generate(CToml& toml, const CClassDefinition& classDef, bool asJs);
    bool handle_datamodel(void);

    bool handle_sdk(void);
    bool handle_sdk_ts(void);
    bool handle_sdk_ts_paths(CStringArray& pathsOut);
    bool handle_sdk_ts_types(CStringArray& typesOut);
    bool handle_sdk_py(void);
    bool handle_sdk_py_paths(CStringArray& pathsOut);
    bool handle_sdk_py_types(CStringArray& typesOut);

    void generate_switch(const CCommandOption& option);
    void generate_toggle(const CCommandOption& option);
    void generate_flag(const CCommandOption& option);
    void generate_positional(const CCommandOption& option);
    void generate_deprecated(const CCommandOption& option);

    bool handle_gocmds_cmd(const CCommandOption& cmd);
    bool handle_gocmds_options(const CCommandOption& cmd);
    bool handle_gocmds_output(const CCommandOption& cmd);
    void verifyGoEnumValidators(void);

    bool writeOpenApiFile(void);

    string_q getReturnTypes(const CCommandOption& ep, CStringArray& returnTypes);
    void verifyDescriptions(void);
};

//-------------------------------------------------------------------
extern bool listClasses(const string_q& path, void* data);
extern bool lintFiles(const string_q& path, void* data);
extern bool formatCppFiles(const string_q& path, void* data);
extern bool formatGoFiles(const string_q& path, void* data);
extern string_q getCaseGetCode(const CMemberArray& fields);
extern string_q getCaseSetCode(const CMemberArray& fields);
extern string_q convertTypes(const string_q& inStr);
extern string_q splitIfTooWide(const string_q& in);
extern void expandTabbys(string_q& strOut);

//------------------------------------------------------------------------------------------------------------
inline bool is_reserved(const string_q& str) {
    CStringArray reserved = {"new", "ret", "do", "or"};
    for (auto r : reserved)
        if (startsWith(str, r))
            return true;
    return false;
}

//------------------------------------------------------------------------------------------------------------
inline string_q short2(const string_q& str) {
    return extract(str, 0, (is_reserved(str) ? 4 : 2));
}

//------------------------------------------------------------------------------------------------------------
inline string_q short3(const string_q& str) {
    return extract(str, 0, (is_reserved(str) ? 4 : 3));
}

//------------------------------------------------------------------------------------------------------------
extern void doReplace(string_q& str, const string_q& type, const string_q& rep, const string_q& spaces);
extern bool writeCodeIn(COptions* opts, const codewrite_t& cw);
extern bool writeCodeOut(COptions* opts, const string_q& fn);
extern bool writeIfDifferent(const string_q& path, const string_q& code);

//---------------------------------------------------------------------------------------------------
extern const char* STR_YAML_FRONTMATTER;
#define routeCount fileCount
#define cmdCount nVisited

//---------------------------------------------------------------------------------------------------
#define makeError(a, b) usage(substitute(usageErrs[(a)], "{0}", (b)))

//---------------------------------------------------------------------------------------------------
extern string_q getPathToSource(const string_q& part);
extern string_q getPathToDocs(const string_q& _part);
extern string_q getDocsPathContent(const string_q& _part);
extern string_q getDocsPathTemplates(const string_q& _part);
extern string_q getDocsPathReadmes(const string_q& _part);
extern string_q getPathToTemplates(const string_q& part);

extern bool parseEndpointsFile(const char* str, void* data);
extern bool parseOptionsFile(const char* str, void* data);
extern bool isChifraRoute(const CCommandOption& cmd, bool depOk);
extern bool isApiRoute(const string_q& route);
extern bool forEveryEnum(APPLYFUNC func, const string_q& enumStr, void* data);
extern string_q type_2_ModelName(const string_q& type, bool raw);
extern string_q getAliases(COptions* opts, const string_q& group, const string_q& route);

//---------------------------------------------------------------------------------------------------
#define sdkPath string_q("/Users/jrush/Development/trueblocks-sdk/")
