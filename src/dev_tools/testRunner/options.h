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
class COptions {
  public:
    uint64_t totalTests = 0;
    uint64_t totalPassed = 0;
    CStringArray fails;
    CStringArray tests;

    COptions(void);
    ~COptions(void);

    void init(void);
    void doTests(vector<CTestCase>& testArray, const string_q& testName, int which);
};

inline COptions::COptions(void) {
}

inline COptions::~COptions(void) {
}

extern string_q getOutputFile(const string& orig, const string_q& goldApiPath);
extern bool cleanTest(const string_q& path, const string_q& testName);
extern void copyBack(const string_q& path, const string_q& tool, const string_q& fileName);
extern string_q linesToString(const CStringArray& lines, char sep = '\n');
extern string_q doCommand(const string_q& cmd, bool readStderr = false);
extern string_q padRight(const string_q& str, size_t len, char p = ' ');
extern string_q padLeft(const string_q& str, size_t len, char p = ' ');
extern string_q getEnvStr(const string_q& name);
extern int copyFile(const string_q& from, const string_q& to);
extern int cleanFolder(const string_q& path, bool recurse = false, bool interactive = false);
extern string_q makeValidName(const string_q& inOut);
extern string_q getCachePath(void);
