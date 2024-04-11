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
#include "testcase.h"

#define API (1 << 0)
#define CMD (1 << 1)
#define BOTH (API | CMD)

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    uint64_t totalTests = 0;
    uint64_t totalPassed = 0;
    CStringArray fails;
    CStringArray tests;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    void doTests(CTestCaseArray& testArray, const string_q& testPath, const string_q& testName, int which);
    bool cleanTest(const string_q& path, const string_q& testName);
};

//-----------------------------------------------------------------------
extern bool saveAndCopy(const string_q& path, void* data);
extern bool replaceFile(const string_q& path, void* data);
