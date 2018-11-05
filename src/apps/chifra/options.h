#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    string_q sourceFolder;
    string_q monitorFolder;
    string_q monitorName;
    string_q addrList;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const;
    bool parseArguments(string_q& command);
    void Init(void);
};

//-----------------------------------------------------------------------------
class CQuestion;
typedef bool (*QUESTIONFUNC)(const COptions& options, CQuestion *q);

//-----------------------------------------------------------------------------
class CQuestion {
public:
    string_q question;
    string_q answer;
    QUESTIONFUNC func;
    string_q color;
    bool wantsInput;
    bool nl;
    CQuestion(const string_q& q,bool h, const string_q& c, QUESTIONFUNC f = NULL)
        : question(q), answer(""), func(f), color(c), wantsInput(h), nl(true) {}
    bool getResponse(void);
};

//--------------------------------------------------------------
extern bool createFolders(const COptions& options, CQuestion *q);
extern bool createRebuild(const COptions& options, CQuestion *q);
extern bool createConfig (const COptions& options, CQuestion *q);
extern bool createCache  (const COptions& options, CQuestion *q);
extern bool editMakeLists(const COptions& options, CQuestion *q);

//--------------------------------------------------------------
extern CQuestion questions[];
extern uint64_t  nQuestions;
