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
// #include "route.h"

// BEG_ERROR_DEFINES
#define ERR_CLASSDEFNOTEXIST 1
#define ERR_CONFIGMISSING 2
#define ERR_CHOOSEONE 3
#define ERR_NOFILTERMATCH 4
#define ERR_NEEDONECLASS 5
// END_ERROR_DEFINES

//-------------------------------------------------------------------
class CCounter {
  public:
    size_t fileCount;
    size_t nVisited;
    size_t nProcessed;
    bool is_counting;
    CStringArray replacements;
    CCounter(void) : fileCount(0), nVisited(0), nProcessed(0), is_counting(true) {
    }
};

typedef enum {
    NONE = 0,
    RUN = (1 << 1),
    EDIT = (1 << 2),
} runmode_t;

//-------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    bool all;
    bool tsx;
    string_q nspace;
    string_q filter;
    bool force;
    bool openapi;
    // END_CODE_DECLARE

    runmode_t mode;
    CClassDefinitionArray classDefs;
    CClassDefinitionArray dataModels;
    CCommandOptionArray optionArray;
    CStringArray positionals;
    CToml classFile;
    CCounter counter;
    timestamp_t lastFormat;
    timestamp_t lastLint;

    ostringstream optionStream, initStream, localStream, autoStream, headerStream, configStream;
    ostringstream notesStream, errorStrStream, errorDefStream, debugStream, goCallStream;
    ostringstream goRouteStream, chifraCmdStream, chifraHelpStream, pairMapStream;
    ostringstream apiTagStream, apiPathStream;
    ostringstream jsLocationStream, jsTemplateStream, jsHotkeyStream, jsRouteStream;

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
        debugStream.str("");
        goCallStream.str("");
        goRouteStream.str("");
        chifraCmdStream.str("");
        chifraHelpStream.str("");
        pairMapStream.str("");
        apiTagStream.str("");
        apiPathStream.str("");
        jsLocationStream.str("");
        jsTemplateStream.str("");
        jsHotkeyStream.str("");
        jsRouteStream.str("");

        optionStream.clear();
        initStream.clear();
        localStream.clear();
        autoStream.clear();
        headerStream.clear();
        configStream.clear();
        notesStream.clear();
        errorStrStream.clear();
        errorDefStream.clear();
        debugStream.clear();
        goCallStream.clear();
        goRouteStream.clear();
        chifraCmdStream.clear();
        chifraHelpStream.clear();
        pairMapStream.clear();
        apiTagStream.clear();
        apiPathStream.clear();
        jsLocationStream.clear();
        jsTemplateStream.clear();
        jsHotkeyStream.clear();
        jsRouteStream.clear();

        positionals.clear();
    }

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_readmes(void);
    bool handle_options(void);
    bool handle_lint(void);
    bool handle_format(void);
    bool handle_generate(CToml& toml, const CClassDefinition& classDef, const string_q& namespc, bool asJs);
    bool handle_datamodel(void);
    bool handle_tsx(void);
    bool handle_tsx_type(const CClassDefinition& classDef);

    void generate_switch(const CCommandOption& option);
    void generate_toggle(const CCommandOption& option);
    void generate_flag(const CCommandOption& option);
    void generate_positional(const CCommandOption& option);
    void generate_deprecated(const CCommandOption& option);

    bool writeCode(const string_q& fn);
    void writeOpenApiFile(void);

    string_q getProductions(const CCommandOption& ep);
};

//-------------------------------------------------------------------
extern bool listClasses(const string_q& path, void* data);
extern bool lintFiles(const string_q& path, void* data);
extern bool formatFiles(const string_q& path, void* data);
extern string_q getCaseGetCode(const CParameterArray& fields);
extern string_q getCaseSetCode(const CParameterArray& fields);
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
extern bool writeIfDifferent(const string_q& path, const string_q& code);
extern bool writeIfDifferent(const string_q& path, const string_q& code, const time_q& now);

//---------------------------------------------------------------------------------------------------
extern const char* STR_YAML_FRONTMATTER;
#define routeCount fileCount
#define cmdCount nVisited
