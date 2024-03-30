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
#include "utillib.h"
#include "commandoption.h"
#include "classdefinition.h"

//-------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    bool all;

    CClassDefinitionArray classDefs;
    CClassDefinitionArray dataModels;
    CCommandOptionArray cmdOptionArray;
    CCommandOptionArray routeOptionArray;
    CStringBoolMap cmdExistsMap;
    CToml classFile;
    CCommandOptionArray endpointArray;
    map<string_q, string_q> hugoAliasMap;

    COptions(void);
    ~COptions(void);
    bool parseArguments(string_q& command);
    void Init(void);
    bool handle_readmes(void);
    bool handle_datamodel(void);
};

//-------------------------------------------------------------------
extern bool listClasses(const string_q& path, void* data);
extern bool writeCodeIn(COptions* opts, const codewrite_t& cw);
extern bool writeCodeOut(COptions* opts, const string_q& fn);
extern bool writeIfDifferent(const string_q& path, const string_q& code);
extern const char* STR_YAML_FRONTMATTER;
extern string_q getPathToSource(const string_q& part);
extern bool parseEndpointsFile(const char* str, void* data);
extern bool parseOptionsFile(const char* str, void* data);
extern string_q type_2_ModelName(const string_q& type, bool raw);
extern string_q getAliases(COptions* opts, const string_q& group, const string_q& route);
