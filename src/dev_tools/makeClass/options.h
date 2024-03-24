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
#include "utillib.h"
#include "commandoption.h"
#include "classdefinition.h"

#define ERR_CLASSDEFNOTEXIST 1
#define ERR_CONFIGMISSING 2
#define ERR_NEEDONECLASS 3

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
    bool all;
    bool openapi;
    bool protobuf;

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
    CCommandOptionArray endpointArray;
    map<string_q, string_q> hugoAliasMap;

    ostringstream goCallStream, goPkgStream;
    ostringstream goRouteStream;
    ostringstream apiTagStream, apiPathStream;
    ostringstream goStream;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_readmes(void);
    bool handle_datamodel(void);
    bool writeOpenApiFile(void);
    bool handle_protobuf(void);

    string_q getReturnTypes(const CCommandOption& ep, CStringArray& returnTypes);
};

//-------------------------------------------------------------------
extern bool listClasses(const string_q& path, void* data);
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
extern bool isApiRoute(const string_q& route);
extern bool forEveryEnum(APPLYFUNC func, const string_q& enumStr, void* data);
extern string_q type_2_ModelName(const string_q& type, bool raw);
extern string_q getAliases(COptions* opts, const string_q& group, const string_q& route);

//---------------------------------------------------------------------------------------------------
extern void reportOneOption(const string_q& route, const string_q& option, const string_q& codebase);
extern string_q get_corrected_caps(const string_q& capsIn);
