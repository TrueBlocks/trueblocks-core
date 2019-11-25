#pragma once
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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"
#include "commandoption.h"
#include "classdefinition.h"

// BEG_ERROR_DEFINES
#define ERR_CLASSDEFNOTEXIST 1
#define ERR_CONFIGMISSING 2
#define ERR_EMPTYJSFILE 3
#define ERR_CHOOSEONE 4
#define ERR_NOFILTERMATCH 5
#define ERR_NEEDONECLASS 6
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
    string_q nspace;
    string_q filter;
    bool test;
    // END_CODE_DECLARE

    runmode_t mode;
    CClassDefinitionArray classDefs;
    CToml classFile;
    ostringstream warnings;
    CCounter counter;
    timestamp_t lastFormat;
    timestamp_t lastLint;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command);
    void Init(void);

    bool handle_options(void);
    bool handle_lint(void);
    bool handle_format(void);
    bool handle_json_export(const string_q& cl);
    bool handle_generate(CToml& toml, const CClassDefinition& classDef, const string_q& ns);

    bool check_option(const CCommandOption& option);
    bool writeCode(const string_q& fn, const string_q& code, const string_q& opt = "", const string_q& local = "",
                   const string_q& init = "", const string_q& notes = "", const string_q& errors = "");
};

//-------------------------------------------------------------------
extern bool listClasses(const string_q& path, void* data);
extern bool lintFiles(const string_q& path, void* data);
extern bool formatFiles(const string_q& path, void* data);
extern void updateTemplates(void);
extern string_q getCaseGetCode(const CParameterArray& fields);
extern string_q getCaseSetCode(const CParameterArray& fields);
extern string_q convertTypes(const string_q& inStr);
extern string_q splitIfTooWide(const string_q& in);

//------------------------------------------------------------------------------------------------------------
inline bool is_reserved(const string_q& str) {
    CStringArray reserved = { "new", "ret", "do", "or" };
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
extern const char* STR_COMMENT_LINE;
extern const char* STR_OPERATOR_DECL;
extern const char* STR_OPERATOR_IMPL;
extern const char* STR_GETVALUE1;
extern const char* STR_GETVALUE2;
extern const char* STR_GETOBJ_CODE;
extern const char* STR_GETOBJ_CODE_FIELD;
extern const char* STR_GETSTR_CODE;
extern const char* STR_GETSTR_CODE_FIELD;
extern const char* STR_GETOBJ_HEAD;
extern const char* STR_GETSTR_HEAD;
extern const char* STR_UPGRADE_CODE;
extern const char* STR_SORT_COMMENT_1;
extern const char* STR_SORT_COMMENT_2;
extern const char* STR_EQUAL_COMMENT_1;
extern const char* STR_EQUAL_COMMENT_2;
extern const char* STR_PRTREADFMT;
extern const char* STR_READFMT;
extern const char* STR_PTRWRITEFMT;
extern const char* STR_WRITEFMT;
extern const char* STR_UNKOWNTYPE;
extern const char* STR_CHILD_OBJS;
