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
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "etherlib.h"
#include "testcase.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

#define API (1 << 0)
#define CMD (1 << 1)
#define BOTH (API | CMD)

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    // BEG_CODE_DECLARE
    string_q filter;
    bool clean;
    uint64_t skip;
    bool no_quit;
    bool no_post;
    bool report;
    // END_CODE_DECLARE

    int modes = CMD;
    bool ignoreOff = false;
    CStringArray tests;
    bool full_test;
    string_q apiProvider;
    uint64_t nRun = 0;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool doTests(CTestCaseArray& testArray, const string_q& testPath, const string_q& testName, int which);
    bool cleanTest(const string_q& path, const string_q& testName);
};

//-----------------------------------------------------------------------
extern bool saveAndCopy(const string_q& path, void* data);
extern bool replaceFile(const string_q& path, void* data);
extern double verySlow;
extern double tooSlow;
extern double fastEnough;
